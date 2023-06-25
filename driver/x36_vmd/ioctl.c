#include <wdm.h>
#include <hidport.h>
#include "vhidmini.h"

#define _IOCTL_
#include "ioctl.h"
#undef _IOCTL_

#ifdef ALLOC_PRAGMA
	#pragma alloc_text( PAGE, GetHidDescriptor)
	#pragma alloc_text( PAGE, GetReportDescriptor)
	#pragma alloc_text( PAGE, GetDeviceAttributes)
#endif

NTSTATUS 
InternalIoctl
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
/*++

Routine Description:

    Handles all the internal ioctls

Arguments:

    DeviceObject - Pointer to the device object.

    Irp - Pointer to the request packet.

Return Value:

    NT Status code

--*/
{
    NTSTATUS            ntStatus = STATUS_SUCCESS;
    PIO_STACK_LOCATION  IrpStack;

    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    switch(IrpStack->Parameters.DeviceIoControl.IoControlCode)
    {
    case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
        ntStatus = GetHidDescriptor(DeviceObject, Irp);
        break;
    case IOCTL_HID_GET_REPORT_DESCRIPTOR:
        ntStatus = GetReportDescriptor(DeviceObject, Irp);
        break;
    case IOCTL_HID_READ_REPORT:
	    IoSkipCurrentIrpStackLocation (Irp);
        ntStatus = IoCallDriver(GET_NEXT_DEVICE_OBJECT(DeviceObject), Irp);
        return ntStatus;
        
    case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
        ntStatus = GetDeviceAttributes(DeviceObject, Irp);
        break;
 /*   case IOCTL_HID_SET_FEATURE:
        ntStatus = GetSetFeature(DeviceObject, Irp);                
        break;
    case IOCTL_HID_GET_FEATURE:
        ntStatus = GetSetFeature(DeviceObject, Irp);
        break;
*/    default:
        ntStatus = STATUS_NOT_SUPPORTED;
        break;
    }
    //
    //Set real return status in Irp
    //
    Irp->IoStatus.Status = ntStatus;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    ntStatus = STATUS_SUCCESS;

    return ntStatus;
} 

NTSTATUS 
GetHidDescriptor(
    IN PDEVICE_OBJECT DeviceObject, 
    IN PIRP Irp
    )
/*++

Routine Description:

    Finds the HID descriptor and copies it into the buffer provided by the Irp.

Arguments:

    DeviceObject - pointer to a device object.

    Irp - Pointer to Interrupt Request Packet.

Return Value:

    NT status code.

--*/
{
    NTSTATUS            ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   deviceInfo;
    PIO_STACK_LOCATION  IrpStack;
	PHID_DESCRIPTOR pHidDescriptor; 

    PAGED_CODE ();

	IrpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceInfo = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);


    if(IrpStack->Parameters.DeviceIoControl.OutputBufferLength<sizeof(HID_DESCRIPTOR)) {
        return STATUS_BUFFER_TOO_SMALL;
    }

	pHidDescriptor = (PHID_DESCRIPTOR)Irp->UserBuffer;
    RtlZeroMemory( pHidDescriptor, sizeof(*pHidDescriptor) );

    pHidDescriptor->bLength                         = sizeof(*pHidDescriptor);
    pHidDescriptor->bDescriptorType                 = 0x21;
    pHidDescriptor->bcdHID                          = 0x0100;
    pHidDescriptor->bCountry                        = 0; /*not localized*/
    pHidDescriptor->bNumDescriptors                 = 1;
    pHidDescriptor->DescriptorList[0].bReportType   = 0x22 ;
    pHidDescriptor->DescriptorList[0].wReportLength = sizeof(DefaultReportDescriptor);

    Irp->IoStatus.Information = sizeof(*pHidDescriptor);

    return ntStatus;
}



NTSTATUS 
GetReportDescriptor(
    IN PDEVICE_OBJECT DeviceObject, 
    IN PIRP Irp
    )
/*++

Routine Description:

    Finds the Report descriptor and copies it into the buffer provided by the 
    Irp.

Arguments:

    DeviceObject - pointer to a device object.

    Irp - Pointer to Interrupt Request Packet.

Return Value:

    NT status code.

--*/
{
    NTSTATUS            ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   deviceInfo;
    PIO_STACK_LOCATION  IrpStack;

	PAGED_CODE ();

    IrpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceInfo = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

    if(IrpStack->Parameters.DeviceIoControl.OutputBufferLength<sizeof(DefaultReportDescriptor)) {
        return STATUS_BUFFER_TOO_SMALL;
    }

    RtlCopyMemory((PUCHAR) Irp->UserBuffer,
                (PUCHAR) DefaultReportDescriptor, 
                sizeof(DefaultReportDescriptor)
                );

    Irp->IoStatus.Information = sizeof(DefaultReportDescriptor);

    return ntStatus;    
}


NTSTATUS 
GetDeviceAttributes(
    IN PDEVICE_OBJECT DeviceObject, 
    IN PIRP Irp)
/*++

Routine Description:

    Fill in the given struct _HID_DEVICE_ATTRIBUTES

Arguments:

    DeviceObject - pointer to a device object.

    Irp - Pointer to Interrupt Request Packet.

Return Value:

    NT status code.

--*/
{
    NTSTATUS                 ntStatus = STATUS_SUCCESS;
    PIO_STACK_LOCATION       irpStack;
    PHID_DEVICE_ATTRIBUTES   deviceAttributes;

    PAGED_CODE ();

    irpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceAttributes = (PHID_DEVICE_ATTRIBUTES) Irp->UserBuffer;

    if( irpStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof (HID_DEVICE_ATTRIBUTES)   )
    {
        return STATUS_BUFFER_TOO_SMALL;
    }

    RtlZeroMemory( deviceAttributes, sizeof(HID_DEVICE_ATTRIBUTES));
    Irp->IoStatus.Information = sizeof (HID_DEVICE_ATTRIBUTES);

    deviceAttributes->Size = sizeof (HID_DEVICE_ATTRIBUTES);
    deviceAttributes->VendorID = 0x06a3;
    deviceAttributes->ProductID = 0x3635;
    deviceAttributes->VersionNumber = 0x0101;

    return ntStatus;
}

/*
NTSTATUS
GetSetFeature(
    IN PDEVICE_OBJECT DeviceObject, 
    IN PIRP Irp
    )
/*++

Routine Description:

    Handles Ioctls for Get and Set feature for all the collection. 
    For control collection (custom defined collection) it handles 
    the user-defined control codes for sideband communication 
    
Arguments:

    DeviceObject - pointer to a device object.

    Irp - Pointer to Interrupt Request Packet.

Return Value:

    NT status code.

--*//*
{
    NTSTATUS            ntStatus = STATUS_SUCCESS;
    PIO_STACK_LOCATION  IrpStack;
    PHID_XFER_PACKET    transferPacket = NULL;

    IrpStack = IoGetCurrentIrpStackLocation(Irp);

    transferPacket = (PHID_XFER_PACKET)Irp->UserBuffer;

    if(!transferPacket->reportBufferLen){
        ntStatus = STATUS_BUFFER_TOO_SMALL;
        return ntStatus;
    }

    switch(IrpStack->Parameters.DeviceIoControl.IoControlCode) 
    {
    case IOCTL_HID_GET_FEATURE:
        //
        // IOCTL_HID_GET_FEATURE is being used to send user-defined 
        // HIDMINI_CONTROL_CODE_Xxxx request to a custom collection
        // defined especially for this purpose
        // 

        if(CONTROL_COLLECTION_REPORT_ID == transferPacket->reportId){
            //
            //This is a special HIDMINI_CONTROL_CODE_Xxxx Type request
            //

            ntStatus = HandleControlRequests(transferPacket);
            break;
        }
        //
        // If collection ID is not for control collection then handle
        // this request just as you would for a regular collection.
        //
        // fall thru
        //
    case IOCTL_HID_SET_FEATURE:
        //
        // not handled, fall thru
        //
    default:
    
        ntStatus = STATUS_NOT_SUPPORTED;
        break;            
    }

    return ntStatus;
    
}*/

/*
NTSTATUS
HandleControlRequests(
    PHID_XFER_PACKET TransferPacket
    )
/*++

Routine Description:

    Handles HIDMINI_CONTROLCODE_Xxxx type requests

Arguments:

    TransferPacket - pointer to HID_XFER_PACKET.

Return Value:

    NT status code.

--*/
/*
{
    NTSTATUS                   ntStatus = STATUS_SUCCESS;
    PHIDMINI_CONTROL_INFO      controlInfo = NULL;
    PHID_DEVICE_ATTRIBUTES     deviceAttributes = NULL;

    if(TransferPacket->reportBufferLen < sizeof(HIDMINI_CONTROL_INFO))
    {
        ntStatus = STATUS_BUFFER_TOO_SMALL;
        return ntStatus;
    }

    controlInfo = (PHIDMINI_CONTROL_INFO)TransferPacket->reportBuffer;

    switch(controlInfo->ControlCode)
    {
    case HIDMINI_CONTROL_CODE_GET_ATTRIBUTES:

        DebugPrint(("Control Code HIDMINI_CONTROL_CODE_GET_ATTRIBUTES\n"));

        if(TransferPacket->reportBufferLen 
                         < (sizeof(HID_DEVICE_ATTRIBUTES) 
                         + sizeof(HIDMINI_CONTROL_INFO))) 
        {
            ntStatus = STATUS_BUFFER_TOO_SMALL;
            return ntStatus;
        }

        deviceAttributes = (PHID_DEVICE_ATTRIBUTES) controlInfo->ControlBuffer;

        deviceAttributes->Size = sizeof (HID_DEVICE_ATTRIBUTES);
        deviceAttributes->VendorID = HIDMINI_VID;
        deviceAttributes->ProductID = HIDMINI_PID;
        deviceAttributes->VersionNumber = HIDMINI_VERSION;

        break;

    case HIDMINI_CONTROL_CODE_DUMMY1:

        DebugPrint(("Control Code HIDMINI_CONTROL_CODE_DUMMY1\n"));
        ntStatus = STATUS_NOT_SUPPORTED;
        break;
     
    case HIDMINI_CONTROL_CODE_DUMMY2:
        
        DebugPrint(("Control Code HIDMINI_CONTROL_CODE_DUMMY2\n"));
        ntStatus = STATUS_NOT_SUPPORTED;
        break;
        
    default:

        DebugPrint(("Unknown control Code\n"));
        ntStatus = STATUS_NOT_SUPPORTED;
        break;
    }

    return ntStatus;         
}
*/