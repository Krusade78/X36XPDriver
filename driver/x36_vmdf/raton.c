#include <wdm.h>
#include "hidfilter.h"

#define _RATON_
#include "raton.h"
#undef _RATON_

NTSTATUS
HF_IoTickRaton(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
	)
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;
	KIRQL irql;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);

	if(KeGetCurrentIrql()==PASSIVE_LEVEL) {
		LeerTickRaton(&devExt->vDevExt->TickRaton);
	}

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}

VOID LeerTickRaton(
	IN OUT UCHAR* tick
	)
{
    NTSTATUS        sta;
	UNICODE_STRING  keyst;
	OBJECT_ATTRIBUTES oa;
	HANDLE key;

	PAGED_CODE();

	RtlInitUnicodeString(&keyst, L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado");
	InitializeObjectAttributes (&oa,
		                        &keyst,
			                    OBJ_CASE_INSENSITIVE,
				                NULL,
					            (PSECURITY_DESCRIPTOR) NULL);
	sta = ZwOpenKey (&key, KEY_READ, &oa);
	if(NT_SUCCESS(sta)) {
		UCHAR t;
        sta=QueryValueKey(key,L"TickRaton",&t,1);
		if(NT_SUCCESS(sta)) {
			if(t>0 && t<101) *tick=t;
		}

		ZwClose(key);
	}
}

NTSTATUS
QueryValueKey (
    IN  HANDLE  Handle,
    IN  PWCHAR  ValueNameString,
    OUT PVOID   Data,
    IN  ULONG   DataLength
    )
{
    NTSTATUS        status;
    UNICODE_STRING  valueName;
    ULONG           length;
    PKEY_VALUE_FULL_INFORMATION fullInfo;

    PAGED_CODE();

    RtlInitUnicodeString (&valueName, ValueNameString);

    length = sizeof (KEY_VALUE_FULL_INFORMATION)
           + valueName.MaximumLength
           + DataLength;

    fullInfo = ExAllocatePoolWithTag (PagedPool, length, (ULONG)'geRR');

    if (fullInfo) {
        status = ZwQueryValueKey (Handle,
                                  &valueName,
                                  KeyValueFullInformation,
                                  fullInfo,
                                  length,
                                  &length);

        if (NT_SUCCESS (status)) {
            ASSERT (DataLength == fullInfo->DataLength);
            RtlCopyMemory (Data,
                           ((PUCHAR) fullInfo) + fullInfo->DataOffset,
                           fullInfo->DataLength);
        }

        ExFreePoolWithTag (fullInfo, (ULONG)'geRR');
    } else {
        status = STATUS_NO_MEMORY;
    }

    return status;
}

/*
VOID
GuardarRegistro (ULONG val)
{
    NTSTATUS        sta;
	UNICODE_STRING  keyst;
	UNICODE_STRING  value;
	OBJECT_ATTRIBUTES oa;
	HANDLE key;

    RtlInitUnicodeString(&keyst, L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM\\VID_06A3&PID_3635");
    RtlInitUnicodeString(&value, L"dato");
    InitializeObjectAttributes (&oa,
                                &keyst,
                                OBJ_CASE_INSENSITIVE,
                                NULL,
                                (PSECURITY_DESCRIPTOR) NULL);
	sta = ZwOpenKey (&key, KEY_WRITE, &oa);
	if(!NT_SUCCESS(sta)) { return; }

    ZwSetValueKey(key,&value,0,REG_DWORD,&val,4);

	ZwClose(key);
}
*/
