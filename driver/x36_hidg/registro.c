#include <wdm.h>
#include <hidport.h>
#include "hidgame.h"

#define _REGISTRO_
#include "registro.h"
#undef _REGISTRO_

#ifdef ALLOC_PRAGMA
    #pragma alloc_text( PAGE, HGM_CargarCalibracion)
    #pragma alloc_text( PAGE, QueryValueKey)
#endif /* ALLOC_PRAGMA */

VOID HGM_CargarCalibracion
    (
    IN      PDEVICE_OBJECT       DeviceObject
    )
{
	PDEVICE_EXTENSION devExt = GET_MINIDRIVER_DEVICE_EXTENSION(DeviceObject);

	PAGED_CODE();

    if(KeGetCurrentIrql()==PASSIVE_LEVEL)
	{
	    NTSTATUS        sta;
		UNICODE_STRING  keyst;
		OBJECT_ATTRIBUTES oa;
		HANDLE key;
		UCHAR idx;

		RtlZeroMemory(devExt->limites,sizeof(devExt->limites));
		RtlZeroMemory(devExt->jitter,sizeof(devExt->jitter));

		RtlInitUnicodeString(&keyst, L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado");
		InitializeObjectAttributes (&oa,
			                        &keyst,
				                    OBJ_CASE_INSENSITIVE,
					                NULL,
						            (PSECURITY_DESCRIPTOR) NULL);
		sta = ZwOpenKey (&key, KEY_READ, &oa);
		if(NT_SUCCESS(sta)) {
			CALIBRADO conf;
	        sta=QueryValueKey(key,L"Eje1",&conf,sizeof(CALIBRADO));
			if(NT_SUCCESS(sta)) {
				devExt->limites[0].c=conf.c;
				devExt->limites[0].i=conf.i;
				devExt->limites[0].d=conf.d;
				devExt->limites[0].n=conf.n;
				devExt->jitter[0].Margen=conf.Margen;
				devExt->limites[0].cal=TRUE;
				devExt->jitter[0].antiv=TRUE;
			}

	        sta=QueryValueKey(key,L"Eje2",&conf,sizeof(CALIBRADO));
			if(NT_SUCCESS(sta)) {
				devExt->limites[1].c=conf.c;
				devExt->limites[1].i=conf.i;
				devExt->limites[1].d=conf.d;
				devExt->limites[1].n=conf.n;
				devExt->jitter[1].Margen=conf.Margen;
				devExt->limites[1].cal=TRUE;
				devExt->jitter[1].antiv=TRUE;
			}

	        sta=QueryValueKey(key,L"Eje3",&conf,sizeof(CALIBRADO));
			if(NT_SUCCESS(sta)) {
				devExt->limites[2].c=conf.c;
				devExt->limites[2].i=conf.i;
				devExt->limites[2].d=conf.d;
				devExt->limites[2].n=conf.n;
				devExt->jitter[2].Margen=conf.Margen;
				devExt->limites[2].cal=TRUE;
				devExt->jitter[2].antiv=TRUE;
			}

	        sta=QueryValueKey(key,L"Eje4",&conf,sizeof(CALIBRADO));
			if(NT_SUCCESS(sta)) {
				devExt->limites[3].c=conf.c;
				devExt->limites[3].i=conf.i;
				devExt->limites[3].d=conf.d;
				devExt->limites[3].n=conf.n;
				devExt->jitter[3].Margen=conf.Margen;
				devExt->limites[3].cal=TRUE;
				devExt->jitter[3].antiv=TRUE;
			}

			ZwClose(key);
		}

		for(idx=0;idx<4;idx++) {
			devExt->Configuracion.MapaEjes[idx]=devExt->EjesAntiguos[idx];
		}
		devExt->nuevaCalibracion=FALSE;
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

    fullInfo = ExAllocatePool (PagedPool, length);

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

        ExFreePool (fullInfo);
    } else {
        status = STATUS_NO_MEMORY;
    }

    return status;
}

