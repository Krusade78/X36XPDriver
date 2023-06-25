/*++

Copyright (c) 2005 Alfredo Costalago
Module Name:

    hidgame.c

Abstract: Human Interface Device (HID) Gameport driver

Environment:

    Kernel mode


--*/

#include <wdm.h>
#include <hidport.h>
#include "hidgame.h"
#include "registro.h"
#include "i386/timing.h"
#include "ioctl.h"
#include "pnp.h"

#ifdef ALLOC_PRAGMA
    #pragma alloc_text( INIT, DriverEntry )
    #pragma alloc_text( PAGE, HGM_CreateClose)
    #pragma alloc_text( PAGE, HGM_AddDevice)
    #pragma alloc_text( PAGE, HGM_Unload)
    #pragma alloc_text( PAGE, HGM_SystemControl)
#endif /* ALLOC_PRAGMA */

HIDGAME_GLOBAL Global;

/*****************************************************************************
 *
 *  @func   NTSTATUS | HGM_DriverInit |
 *
 *          Perform global initialization.
 *          <nl>This is called from DriverEntry.  Try to initialize a CPU 
 *          specific timer but if it fails set up default
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   STATUS_UNSUCCESSFUL | not success
 *
 *****************************************************************************/
NTSTATUS HGM_DriverInit()
{
    NTSTATUS ntStatus = STATUS_SUCCESS;

    if( !HGM_CPUCounterInit() )
    {
        LARGE_INTEGER QPCFrequency;

        KeQueryPerformanceCounter( &QPCFrequency );

        if( ( QPCFrequency.HighPart == 0 )
         && ( QPCFrequency.LowPart <= 10000 ) )
        {
            ntStatus = STATUS_UNSUCCESSFUL;
        }
        else
        {
            Global.CounterScale = CALCULATE_SCALE( QPCFrequency.QuadPart );
            Global.ReadCounter = (COUNTER_FUNCTION)&KeQueryPerformanceCounter;
        }
    }

    return ntStatus;
}

/*****************************************************************************
 *
 *  @func   NTSTATUS  | DriverEntry |
 *
 *          Installable driver initialization entry point.
 *          <nl>This entry point is called directly by the I/O system.
 *
 *  @parm   IN PDRIVER_OBJECT | DriverObject |
 *
 *          Pointer to the driver object
 *
 *  @parm   IN PUNICODE_STRING | RegistryPath |
 *
 *          Pointer to a unicode string representing the path,
 *          to driver-specific key in the registry.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   ???            | returned HidRegisterMinidriver()
 *
 *****************************************************************************/
NTSTATUS DriverEntry
    (
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{
    NTSTATUS                        ntStatus;
    HID_MINIDRIVER_REGISTRATION     hidMinidriverRegistration;

    ntStatus = HGM_DriverInit();

    if( NT_SUCCESS(ntStatus) )
    {
        DriverObject->MajorFunction[IRP_MJ_CREATE]    =
            DriverObject->MajorFunction[IRP_MJ_CLOSE] = HGM_CreateClose;
        DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] =
            HGM_InternalIoctl;
        DriverObject->MajorFunction[IRP_MJ_PNP]   = HGM_PnP;
        DriverObject->MajorFunction[IRP_MJ_POWER] = HGM_Power;
        DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = HGM_SystemControl;
        DriverObject->DriverUnload                = HGM_Unload;
        DriverObject->DriverExtension->AddDevice  = HGM_AddDevice;

        /*
         * Register  with HID.SYS module
         */
        RtlZeroMemory(&hidMinidriverRegistration, sizeof(hidMinidriverRegistration));

        hidMinidriverRegistration.Revision            = HID_REVISION;
        hidMinidriverRegistration.DriverObject        = DriverObject;
        hidMinidriverRegistration.RegistryPath        = RegistryPath;
        hidMinidriverRegistration.DeviceExtensionSize = sizeof(DEVICE_EXTENSION);
		hidMinidriverRegistration.DevicesArePolled    = TRUE;


        ntStatus = HidRegisterMinidriver(&hidMinidriverRegistration);

        if( NT_SUCCESS(ntStatus) )
        {
            /*
             *  Protect the list with a Mutex
             */
            ExInitializeFastMutex (&Global.Mutex);

            /*
             *  Initialize the device list head
             */
            InitializeListHead(&Global.DeviceListHead);

            /*
             *  Initialize gameport access spinlock
             */
            KeInitializeSpinLock(&Global.SpinLock);
        }
    }

    return ntStatus;
} /* DriverEntry */

/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_CreateClose |
 *
 *          Process the create and close IRPs sent to this device.
 *
 *  @parm   IN PDEVICE_OBJECT | DeviceObject |
 *
 *          Pointer to the device object
 *
 *  @parm   IN PIRP | Irp |
 *
 *          Pointer to an I/O Request Packet.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   STATUS_INVALID_PARAMETER  | Irp not handled
 *
 *****************************************************************************/
NTSTATUS HGM_CreateClose
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION   IrpStack;
    NTSTATUS             ntStatus = STATUS_SUCCESS;

    PAGED_CODE ();

    /*
     * Get a pointer to the current location in the Irp.
     */
    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    switch(IrpStack->MajorFunction)
    {
        case IRP_MJ_CREATE:
            Irp->IoStatus.Information = 0;
            break;

        case IRP_MJ_CLOSE:
            Irp->IoStatus.Information = 0;
            break;

        default:
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
    }

    /*
     * Save Status for return and complete Irp
     */

    Irp->IoStatus.Status = ntStatus;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return ntStatus;
} /* HGM_CreateClose */


/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_AddDevice |
 *
 *          Called by hidclass, allows us to initialize our device extensions.
 *
 *  @parm   IN PDRIVER_OBJECT | DriverObject |
 *
 *          Pointer to the driver object
 *
 *  @parm   IN PDEVICE_OBJECT | FunctionalDeviceObject |
 *
 *          Pointer to a functional device object created by hidclass.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *
 *****************************************************************************/
NTSTATUS HGM_AddDevice
    (
    IN PDRIVER_OBJECT DriverObject,
    IN PDEVICE_OBJECT FunctionalDeviceObject
    )
{
    NTSTATUS                ntStatus = STATUS_SUCCESS;
    PDEVICE_OBJECT          DeviceObject;
    PDEVICE_EXTENSION       DeviceExtension;
	UCHAR idx;

    PAGED_CODE ();

    DeviceObject = FunctionalDeviceObject;

    /*
     * Initialize the device extension.
     */
    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION (DeviceObject);
	//DeviceExtension->xfo=NULL;
	//DeviceExtension->xdo=NULL;
	DeviceExtension->vfo=NULL;
	DeviceExtension->vdo=NULL;
	DeviceExtension->EjesAntiguos[0]=0;
	DeviceExtension->EjesAntiguos[1]=1;
	DeviceExtension->EjesAntiguos[2]=2;
	DeviceExtension->EjesAntiguos[3]=3;
	HGM_CargarCalibracion(DeviceObject);
	DeviceExtension->nuevaConfiguracion=FALSE;
	//DeviceExtension->Configuracion.MapaEjes[0]=0;
	//DeviceExtension->Configuracion.MapaEjes[1]=1;
	//DeviceExtension->Configuracion.MapaEjes[2]=2;
	//DeviceExtension->Configuracion.MapaEjes[3]=3;
	DeviceExtension->Configuracion.MapaRotatorios=9;
	//for(idx=0;idx<40;idx++) {
	//	DeviceExtension->Configuracion.MapaBotones[0][idx]=idx+1;
	//}
	RtlZeroMemory(&DeviceExtension->hidBuffer,sizeof(HIDGAME_INPUT_DATA));
	/*RtlZeroMemory(DeviceExtension->Configuracion.MapaBotones[1],40);
	DeviceExtension->Configuracion.MapaBotones[1][6]=0|(4*1);
	DeviceExtension->Configuracion.MapaBotones[1][9]=0|(4*3);
	DeviceExtension->Configuracion.MapaBotones[1][7]=0|(4*5);
	DeviceExtension->Configuracion.MapaBotones[1][8]=0|(4*7);
	DeviceExtension->Configuracion.MapaBotones[1][11]=0|(4*2);
	DeviceExtension->Configuracion.MapaBotones[1][13]=0|(4*4);
	DeviceExtension->Configuracion.MapaBotones[1][12]=0|(4*6);
	DeviceExtension->Configuracion.MapaBotones[1][10]=0|(4*8);
	DeviceExtension->Configuracion.MapaBotones[1][14]=1|(4*1);
	DeviceExtension->Configuracion.MapaBotones[1][17]=1|(4*3);
	DeviceExtension->Configuracion.MapaBotones[1][15]=1|(4*5);
	DeviceExtension->Configuracion.MapaBotones[1][16]=1|(4*7);
	DeviceExtension->Configuracion.MapaBotones[1][19]=1|(4*2);
	DeviceExtension->Configuracion.MapaBotones[1][21]=1|(4*4);
	DeviceExtension->Configuracion.MapaBotones[1][20]=1|(4*6);
	DeviceExtension->Configuracion.MapaBotones[1][18]=1|(4*8);
	DeviceExtension->Configuracion.MapaBotones[1][32]=2|(4*1);
	DeviceExtension->Configuracion.MapaBotones[1][35]=2|(4*2);
	DeviceExtension->Configuracion.MapaBotones[1][33]=2|(4*3);
	DeviceExtension->Configuracion.MapaBotones[1][34]=2|(4*4);
	DeviceExtension->Configuracion.MapaBotones[1][28]=3|(4*1);
	DeviceExtension->Configuracion.MapaBotones[1][31]=3|(4*2);
	DeviceExtension->Configuracion.MapaBotones[1][29]=3|(4*3);
	DeviceExtension->Configuracion.MapaBotones[1][30]=3|(4*4);
	for(idx=10;idx<18;idx++) {
		DeviceExtension->Configuracion.MapaBotones[1][idx]=((idx-10)/4)|(4*((idx+idx-19)-(8*((idx-10)/4))));
	}
    */DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    /*
     * Initialize the list
     */
    InitializeListHead(&DeviceExtension->Link);

    /*
     *  Acquire mutex before modifying the Global Linked list of devices
     */
    ExAcquireFastMutex (&Global.Mutex);

    /*
     * Add this device to the linked list of devices
     */
    InsertTailList(&Global.DeviceListHead, &DeviceExtension->Link);

    /*
     *  Release the mutex
     */
    ExReleaseFastMutex (&Global.Mutex);

    /*
     * Initialize the remove lock 
     */
    DeviceExtension->RequestCount = 1;
    KeInitializeEvent(&DeviceExtension->RemoveEvent,
                      SynchronizationEvent,
                      FALSE);

    return ntStatus;
} /* HGM_AddDevice */


/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_SystemControl |
 *
 *          Process the WMI IRPs sent to this device.
 *
 *  @parm   IN PDEVICE_OBJECT | DeviceObject |
 *
 *          Pointer to the device object
 *
 *  @parm   IN PIRP | Irp |
 *
 *          Pointer to an I/O Request Packet.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   STATUS_INVALID_PARAMETER  | Irp not handled
 *
 *****************************************************************************/
NTSTATUS HGM_SystemControl
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PAGED_CODE ();

    IoSkipCurrentIrpStackLocation(Irp);

    return IoCallDriver(GET_NEXT_DEVICE_OBJECT(DeviceObject), Irp);
} /* HGM_SystemControl */

/*****************************************************************************
 *
 *  @func   void  | HGM_Unload |
 *
 *          Free all the allocated resources, etc.
 *
 *  @parm   IN PDRIVER_OBJECT | DeviceObject |
 *
 *          Pointer to the driver object
 *
 *
 *****************************************************************************/
VOID HGM_Unload
    (
    IN PDRIVER_OBJECT DriverObject
    )
{
    PAGED_CODE();

    /*
     * All the device objects should be gone
     */

    ASSERT ( NULL == DriverObject->DeviceObject);

    return;
} /* HGM_Unload */
