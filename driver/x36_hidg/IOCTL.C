/*++

Copyright (c) 2005 Alfredo Costalago

Module Name:

    ioctl.c

Abstract: Contains routines to support HIDCLASS internal
          ioctl queries for game devices.

Environment:

    Kernel mode


--*/

#include <wdm.h>
#include <hidport.h>
#include "hidgame.h"
#include "pnp.h"
#include "hidjoy.h"
#include "poll.h"
#define _IOCTLC_
#include "ioctl.h"

#ifdef ALLOC_PRAGMA
    #pragma alloc_text (PAGE, HGM_GetDeviceDescriptor)
    #pragma alloc_text (PAGE, HGM_GetReportDescriptor)
    #pragma alloc_text (PAGE, HGM_GetAttributes      )
#endif



/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_InternalIoctl |
 *
 *          Process the Control IRPs sent to this device.
 *          <nl>This function cannot be pageable because reads/writes
 *          can be made at dispatch-level
 *
 *  @parm   IN PDRIVER_OBJECT | DeviceObject |
 *
 *          Pointer to the driver object
 *
 *  @parm   IN PIRP | Irp |
 *
 *          Pointer to an I/O Request Packet.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   STATUS_NOT_SUPPORT | Irp function not supported
 *  @rvalue   ???            | ???
 *
 *****************************************************************************/
NTSTATUS HGM_InternalIoctl
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    NTSTATUS            ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   DeviceExtension;
    PIO_STACK_LOCATION  IrpStack;

    /*
     *  Get a pointer to the current location in the Irp
     */

    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    /*
     *  Get a pointer to the device extension
     */

    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION (DeviceObject);


    ntStatus = HGM_IncRequestCount( DeviceExtension );
    if (!NT_SUCCESS (ntStatus))
    {
        /*
         *  Someone sent us another plug and play IRP after removed
         */

        Irp->IoStatus.Information = 0;
        Irp->IoStatus.Status = ntStatus;
    } else
    {
        switch(IrpStack->Parameters.DeviceIoControl.IoControlCode)
        {
            case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
                ntStatus = HGM_GetDeviceDescriptor(DeviceObject, Irp);
                break;

            case IOCTL_HID_GET_REPORT_DESCRIPTOR:
                ntStatus = HGM_GetReportDescriptor(DeviceObject, Irp);
                break;

			case IOCTL_HID_READ_REPORT:
                ntStatus = HGM_ReadReport(DeviceObject, Irp);
                break;

            case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
                ntStatus = HGM_GetAttributes(DeviceObject, Irp);
                break;

            default:
                ntStatus = STATUS_NOT_SUPPORTED;
                break;
        }


        /*
         * Set real return status in Irp
         */
        Irp->IoStatus.Status = ntStatus;

        HGM_DecRequestCount( DeviceExtension );
    }


    if(ntStatus != STATUS_PENDING)
    {
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        /*
         * NOTE: Real return status set in Irp->IoStatus.Status
         */
        ntStatus = STATUS_SUCCESS;
    } else
    {
        /*
         * No reason why there should be a status pending
         */
        IoMarkIrpPending( Irp );
    }

    return ntStatus;
} /* HGM_InternalIoctl */



/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_GetDeviceDescriptor |
 *
 *          Respond to HIDCLASS IOCTL_HID_GET_DEVICE_DESCRIPTOR
 *          by returning a device descriptor
 *
 *  @parm   IN PDRIVER_OBJECT | DeviceObject |
 *
 *          Pointer to the driver object
 *
 *  @parm   IN PIRP | Irp |
 *
 *          Pointer to an I/O Request Packet.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   STATUS_BUFFER_TOO_SMALL |  need more memory
 *
 *****************************************************************************/
NTSTATUS HGM_GetDeviceDescriptor
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PHID_DESCRIPTOR pHidDescriptor;        /* Hid descriptor for this device */
    USHORT   cbReport;
    UCHAR               rgGameReport[MAXBYTES_GAME_REPORT] ;
    NTSTATUS            ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   DeviceExtension;
    PIO_STACK_LOCATION  IrpStack;

    PAGED_CODE ();

    /*
     * Get a pointer to the current location in the Irp
     */

    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    /*
     * Get a pointer to the device extension
     */

    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION (DeviceObject);


    if( IrpStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(HID_DESCRIPTOR)  )
    {

        ntStatus = STATUS_BUFFER_TOO_SMALL;
    } else
    {
        /*
         * Generate the report
         */
        ntStatus =  HGM_GenerateReport(DeviceObject, rgGameReport, &cbReport);

        if( NT_SUCCESS(ntStatus) )
        {
            /*
             *  Get a pointer to the HID_DESCRIPTOR
             *  HIDCLASS is trusted to pass a valid pointer.
             */
            pHidDescriptor =  (PHID_DESCRIPTOR) Irp->UserBuffer;

            RtlZeroMemory( pHidDescriptor, sizeof(*pHidDescriptor) );
            /*
             * Copy device descriptor to HIDCLASS buffer
             */
            pHidDescriptor->bLength                         = sizeof(*pHidDescriptor);
            pHidDescriptor->bDescriptorType                 = HID_HID_DESCRIPTOR_TYPE;
            pHidDescriptor->bcdHID                          = HID_REVISION;
            pHidDescriptor->bCountry                        = 0; /*not localized*/
            pHidDescriptor->bNumDescriptors                 = 1;
            pHidDescriptor->DescriptorList[0].bReportType   = HID_REPORT_DESCRIPTOR_TYPE ;
            pHidDescriptor->DescriptorList[0].wReportLength = cbReport;

            /*
             * Report how many bytes were copied
             */
            Irp->IoStatus.Information = sizeof(*pHidDescriptor);
        } else
        {
            Irp->IoStatus.Information = 0x0;
        }
    }

    return ntStatus;
} /* HGM_GetDeviceDescriptor */


/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_GetReportDescriptor |
 *
 *          Respond to HIDCLASS IOCTL_HID_GET_REPORT_DESCRIPTOR
 *          by returning appropriate the report descriptor
 *
 *  @parm   IN PDRIVER_OBJECT | DeviceObject |
 *
 *          Pointer to the driver object
 *
 *  @parm   IN PIRP | Irp |
 *
 *          Pointer to an I/O Request Packet.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   ???            | ???
 *
 *****************************************************************************/
NTSTATUS HGM_GetReportDescriptor
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PDEVICE_EXTENSION     DeviceExtension;
    PIO_STACK_LOCATION    IrpStack;
    NTSTATUS              ntStatus;
    UCHAR                 rgGameReport[MAXBYTES_GAME_REPORT] ;
    USHORT                cbReport;

    PAGED_CODE ();

    /*
     * Get a pointer to the current location in the Irp
     */

    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    /*
     * Get a pointer to the device extension
     */

    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION (DeviceObject);


    /*
     * Generate the report
     */
    ntStatus =  HGM_GenerateReport(DeviceObject, rgGameReport, &cbReport);

    if( NT_SUCCESS(ntStatus) )
    {
        if( cbReport >  (USHORT) IrpStack->Parameters.DeviceIoControl.OutputBufferLength )
        {
            ntStatus = STATUS_BUFFER_TOO_SMALL;

        } else
        {
            RtlCopyMemory( Irp->UserBuffer, rgGameReport, cbReport );
            /*
             * Report how many bytes were copied
             */
            Irp->IoStatus.Information = cbReport;
            ntStatus = STATUS_SUCCESS;
        }
    }

    return ntStatus;
} /* HGM_GetReportDescriptor */


/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_ReadReport |
 *
 *          Poll the gameport, remap the axis and button data and package
 *          into the defined HID report field.
 *          <nl>This routine cannot be pageable as HID can make reads at 
 *          dispatch-level.
 *
 *  @parm   IN PDRIVER_OBJECT | DeviceObject |
 *
 *          Pointer to the driver object
 *
 *  @parm   IN PIRP | Irp |
 *
 *          Pointer to an I/O Request Packet.
 *
 *  @rvalue   STATUS_SUCCESS  | success
 *  @rvalue   STATUS_DEVICE_NOT_CONNECTED | Device Failed to Quiesce 
 *                                          ( not connected )
 *  @rvalue   STATUS_TIMEOUT  | Could not determine exact transition time for 
 *                              one or more axis but not a failure.
 *
 *****************************************************************************/
NTSTATUS HGM_ReadReport
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    NTSTATUS            ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   DeviceExtension;
    PIO_STACK_LOCATION  IrpStack;

    /*
     * Get a pointer to the device extension.
     */

    DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION (DeviceObject);

    /*
     * Get Stack location.
     */

    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    /*
     * First check the size of the output buffer (there is no input buffer)
     */

    if( IrpStack->Parameters.DeviceIoControl.OutputBufferLength <  sizeof(HIDGAME_INPUT_DATA))
    {
        ntStatus = STATUS_BUFFER_TOO_SMALL;
    }

    if( DeviceExtension->fStarted == FALSE )
    {
        ntStatus = STATUS_DEVICE_NOT_READY ;
    }

    /*
     *  All the checking done so do device specific polling
     */
    if( NT_SUCCESS(ntStatus) )
    {
        ntStatus = HGM_UpdateLatestPollData( DeviceExtension );
    }

    /*
     *  If all's well, translate device specific data to HID report
     */
    if( NT_SUCCESS(ntStatus) )
    {
		HGM_Game2HID(DeviceObject,(PHIDGAME_INPUT_DATA)Irp->UserBuffer);
		Irp->IoStatus.Information = sizeof(HIDGAME_INPUT_DATA);
    } 
    else
    {
        Irp->IoStatus.Information = 0x0;
    }

    Irp->IoStatus.Status = ntStatus;

    return ntStatus;
} /* HGM_ReadReport */


/*****************************************************************************
 *
 *  @func   NTSTATUS  | HGM_GetAttributes |
 *
 *          Respond to IOCTL_HID_GET_ATTRIBUTES, by filling
 *          the HID_DEVICE_ATTRIBUTES struct
 *
 *  @parm   IN PDRIVER_OBJECT | DeviceObject |
 *
 *          Pointer to the driver object
 *
 *  @parm   IN PIRP | Irp |
 *
 *          Pointer to an I/O Request Packet.
 *
 *  @rvalue   STATUS_SUCCESS | success
 *  @rvalue   ???            | ???
 *
 *****************************************************************************/
NTSTATUS HGM_GetAttributes
    (
    PDEVICE_OBJECT  DeviceObject,
    PIRP            Irp
    )
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PIO_STACK_LOCATION  IrpStack;

    PAGED_CODE();

    /*
     * Get a pointer to the current location in the Irp
     */

    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    if( IrpStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof (HID_DEVICE_ATTRIBUTES)   )
    {
        ntStatus = STATUS_BUFFER_TOO_SMALL;

    } else
    {
        PDEVICE_EXTENSION       DeviceExtension;
        PHID_DEVICE_ATTRIBUTES  DeviceAttributes;
        POEMDATA    OemData;

        /*
         * Get a pointer to the device extension
         */
        DeviceExtension = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);
        DeviceAttributes = (PHID_DEVICE_ATTRIBUTES) Irp->UserBuffer;


        OemData = &DeviceExtension->HidGameOemData.OemData[0];
        if( DeviceExtension->fSiblingFound)
        {
            OemData = &DeviceExtension->HidGameOemData.OemData[1];
        }

        RtlZeroMemory( DeviceAttributes, sizeof(*DeviceAttributes));

        /*
         * Report how many bytes were copied
         */

        Irp->IoStatus.Information   = sizeof(*DeviceAttributes);

        DeviceAttributes->Size          = sizeof (*DeviceAttributes);
        DeviceAttributes->VendorID      = OemData->VID;
        DeviceAttributes->ProductID     = OemData->PID;
        DeviceAttributes->VersionNumber = HIDGAME_VERSION_NUMBER;

    }

    return ntStatus;
} /* HGM_GetAttributes */
