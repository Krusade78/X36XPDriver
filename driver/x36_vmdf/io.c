#include <wdm.h>
#include "hidfilter.h"
#include "acciones.h"

#define _IO_
#include "io.h"
#undef _IO_


NTSTATUS
HF_IoDescalibrar(
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

	KeAcquireSpinLock(&devExt->slEstado,&irql);
		devExt->Estado|=4;
	KeReleaseSpinLock(&devExt->slEstado,irql);

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS
HF_IoRecalibrar(
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

	KeAcquireSpinLock(&devExt->slEstado,&irql);
		devExt->Estado|=2;
	KeReleaseSpinLock(&devExt->slEstado,irql);

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}


NTSTATUS
HF_IoEscribirConfiguracion(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);

	if(irpStack->Parameters.DeviceIoControl.InputBufferLength==sizeof(CONF)) {
		KIRQL irql;
		KeAcquireSpinLock(&devExt->slEstado,&irql);
		if(devExt->Configuracion!=NULL) {
				ExFreePoolWithTag(devExt->Configuracion,(ULONG)'ppHV');
				devExt->Configuracion=NULL;
		}
		devExt->Configuracion = ExAllocatePoolWithTag(NonPagedPool,sizeof(CONF),(ULONG)'ppHV');
		if(devExt->Configuracion!=NULL) {
			RtlCopyMemory(devExt->Configuracion,Irp->AssociatedIrp.SystemBuffer,sizeof(CONF));
			devExt->Estado|=1;
			KeReleaseSpinLock(&devExt->slEstado,irql);
			Irp->IoStatus.Information = sizeof(CONF);
		} else {
			KeReleaseSpinLock(&devExt->slEstado,irql);
			status=STATUS_INSUFFICIENT_RESOURCES;
			Irp->IoStatus.Information = 0;
		}
	} else {
		status=STATUS_BUFFER_TOO_SMALL;
		Irp->IoStatus.Information = 0;
	}

	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS
HF_IoLeer(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);

	if(irpStack->Parameters.DeviceIoControl.OutputBufferLength==sizeof(HIDGAME_INPUT_DATA)) {
		KIRQL irql;
		HIDGAME_INPUT_DATA gd;
		KeAcquireSpinLock(&devExt->vDevExt->slGameData,&irql);
			//RtlCopyMemory(gd.Ejes,devExt->stEjesDX,sizeof(gd.Ejes));
			RtlCopyMemory(gd.Rotatorios,devExt->stRotatoriosDX,sizeof(gd.Rotatorios));
			RtlCopyMemory(gd.Botones,devExt->vDevExt->stBotonesDX,sizeof(gd.Botones));
			RtlCopyMemory(gd.Setas,devExt->vDevExt->stSetasDX,sizeof(gd.Setas));
		KeReleaseSpinLock(&devExt->vDevExt->slGameData,irql);
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,&gd,sizeof(HIDGAME_INPUT_DATA));
		Irp->IoStatus.Information = sizeof(HIDGAME_INPUT_DATA);
	} else {
		status=STATUS_BUFFER_TOO_SMALL;
		Irp->IoStatus.Information = 0;
	}

	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS
HF_IoLeerEstado(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
	if(irpStack->Parameters.DeviceIoControl.OutputBufferLength==1) {
		KIRQL irql;
		KeAcquireSpinLock(&devExt->slEstado,&irql);
			*((PCHAR)Irp->AssociatedIrp.SystemBuffer)=devExt->Estado;
			devExt->Estado=0;//&=1;
		KeReleaseSpinLock(&devExt->slEstado,irql);
		Irp->IoStatus.Information = 1;
	} else {
		status=STATUS_BUFFER_TOO_SMALL;
		Irp->IoStatus.Information = 0;
	}

	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS
HF_IoLeerConf(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);

	if(irpStack->Parameters.DeviceIoControl.OutputBufferLength==sizeof(CONF)) {
		KIRQL irql;
		KeAcquireSpinLock(&devExt->slEstado,&irql);
		if(devExt->Configuracion!=NULL) {
				RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,devExt->Configuracion,sizeof(CONF));
				ExFreePoolWithTag(devExt->Configuracion,(ULONG)'ppHV');
				devExt->Configuracion=NULL;
				//devExt->Estado&=6;
			KeReleaseSpinLock(&devExt->slEstado,irql);
			Irp->IoStatus.Information = sizeof(CONF);
		} else {
			KeReleaseSpinLock(&devExt->slEstado,irql);
			Irp->IoStatus.Information = 0;
		}
	} else {
		status=STATUS_BUFFER_TOO_SMALL;
		Irp->IoStatus.Information = 0;
	}

	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

HF_IoWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    Irp->IoStatus.Information = 0;

	if(irpStack->Parameters.Write.Length==sizeof(HIDGAME_INPUT_DATA))
	{
		HIDGAME_INPUT_DATA hidGameData;
		KIRQL irql;
		RtlCopyMemory(&hidGameData,Irp->AssociatedIrp.SystemBuffer,sizeof(HIDGAME_INPUT_DATA));
	    Irp->IoStatus.Information = sizeof(HIDGAME_INPUT_DATA);
		Irp->IoStatus.Status = status;
	    IoCompleteRequest(Irp, IO_NO_INCREMENT);

//		KeAcquireSpinLock(&devExt->slGameData,&irql);
//			RtlCopyMemory(&devExt->HidGameData,&hidGameData,sizeof(HIDGAME_INPUT_DATA));
//		KeReleaseSpinLock(&devExt->slGameData,irql);

		ProcesarWrite(devExt->vDevExt,&hidGameData);
	} else {
		status=STATUS_INVALID_BUFFER_SIZE;
		Irp->IoStatus.Status = status;
	    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	}

	return status;
}


VOID 
ProcesarWrite(   
    PDEVICE_EXTENSION devExt,
	PHIDGAME_INPUT_DATA hidGameData
    )
{
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION)devExt->itfObj->DeviceExtension;
	UCHAR ubotones[5];
	UCHAR cambios;
	UCHAR idx;
	KIRQL irql;

	RtlCopyMemory(ubotones,idevExt->ultBotones,sizeof(ubotones));
	RtlCopyMemory(idevExt->ultBotones,hidGameData->Botones,sizeof(ubotones));
	KeAcquireSpinLock(&devExt->slGameData,&irql);
		RtlCopyMemory(idevExt->stEjesDX,hidGameData->Ejes,2);
		RtlCopyMemory(idevExt->stRotatoriosDX,hidGameData->Rotatorios,2);
	KeReleaseSpinLock(&devExt->slGameData,irql);

	// Botones

	for(idx=0;idx<5;idx++) {
		cambios=hidGameData->Botones[idx]^ubotones[idx];//;(!hidGameData->Botones[idx] | !idevExt->gdUltimo.Botones[idx])&(hidGameData->Botones[idx]|idevExt->gdUltimo.Botones[idx]);
		if(cambios!=0) {
			UCHAR exp;
			for(exp=0;exp<8;exp++) {
				if((cambios>>exp)&1) { // Si ha cambiado
					if((hidGameData->Botones[idx]>>exp)&1) {
						AccionPulsarBoton(devExt,(idx*8)+exp);
					} else {
						AccionSoltarBoton(devExt,(idx*8)+exp);
					}
				}
			}
		}
	}

	// Ejes

	for(idx=0;idx<2;idx++) {
		KeAcquireSpinLock(&idevExt->slPosicion,&irql);
			cambios=TraducirGiratorio(idevExt,idx,(UCHAR)hidGameData->Ejes[idx]);
		KeReleaseSpinLock(&idevExt->slPosicion,irql);
		if(cambios!=0) {
			AccionEje(devExt,idx,cambios-1);
		}
	}
	for(idx=0;idx<2;idx++) {
		KeAcquireSpinLock(&idevExt->slPosicion,&irql);
			if(hidGameData->Rotatorios[idx]==0) {
				hidGameData->Rotatorios[idx]=127;
			} else {
				hidGameData->Rotatorios[idx]=hidGameData->Rotatorios[idx]+((hidGameData->Rotatorios[idx]<0)?127:128);
			}
			cambios=TraducirGiratorio(idevExt,idx+2,hidGameData->Rotatorios[idx]);
		KeReleaseSpinLock(&idevExt->slPosicion,irql);
		if(cambios!=0) {
			AccionEje(devExt,idx+2,cambios-1);
		}
	}
}

UCHAR TraducirGiratorio(
	PITFDEVICE_EXTENSION idevExt,
	UCHAR eje,
	UCHAR nueva
	)
{
    BOOLEAN incremental=idevExt->MapaEjes[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje].Incremental;
	UCHAR idn=0;
	UCHAR vieja=idevExt->posVieja[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje];

	if (incremental) {
		if (nueva>vieja) {
			UCHAR posiciones=(UCHAR)idevExt->MapaEjes[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje].Indices[3];
			if(vieja<(255-posiciones)) {
				if (nueva>(vieja+posiciones)) {
					idevExt->posVieja[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje]=nueva;
					idn=2;
				}
			}
		} else {
			UCHAR posiciones=(UCHAR)idevExt->MapaEjes[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje].Indices[2];
			if(vieja>posiciones) {
				if (nueva<(vieja-posiciones)) {
					idevExt->posVieja[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje]=nueva;
					idn=1;
				}
			}
		}
	} else {
		UINT16 posActual=1000;
		UCHAR posant=0;
		UCHAR idc;
		nueva=255-nueva;
		for(idc=0; idc<15; idc++) {
			if(idevExt->MapaEjes[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje].Bandas[idc]==0) {
				break;
			} else {
				if(nueva>=posant && nueva<idevExt->MapaEjes[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje].Bandas[idc]) {
					posActual=idc;
					break;
				}
				posant=idevExt->MapaEjes[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje].Bandas[idc];
			}
		}
		if(posActual==1000) {
			if(nueva>=posant && nueva<256) posActual=idc;
		}
		if(posActual!=1000 && posActual!=vieja) {
			idevExt->posVieja[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][eje]=(UCHAR)posActual;
			idn=posActual+1;
		}
	}

	return idn;
}
