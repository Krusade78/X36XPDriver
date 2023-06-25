#include <wdm.h>
#include "hidfilter.h"

#include "mapa.h"


NTSTATUS
HF_IoEscribirMapa(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);

	if(irpStack->Parameters.DeviceIoControl.InputBufferLength==(sizeof(devExt->MapaBotones)+sizeof(devExt->MapaEjes))) {
		KIRQL irql;
		KeAcquireSpinLock(&devExt->slPosicion,&irql);
			RtlCopyMemory(devExt->MapaBotones,Irp->AssociatedIrp.SystemBuffer,sizeof(devExt->MapaBotones));
			RtlCopyMemory(devExt->MapaEjes,(PUCHAR)Irp->AssociatedIrp.SystemBuffer+sizeof(devExt->MapaBotones),sizeof(devExt->MapaEjes));
		KeReleaseSpinLock(&devExt->slPosicion,irql);
		Irp->IoStatus.Information = irpStack->Parameters.DeviceIoControl.InputBufferLength;
	} else {
		status=STATUS_BUFFER_TOO_SMALL;
		Irp->IoStatus.Information = 0;
	}

	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS
HF_IoEscribirComandos(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    PIO_STACK_LOCATION              irpStack;
    PITFDEVICE_EXTENSION            devExt;
    NTSTATUS                        status = STATUS_SUCCESS;
	UINT16							npos;
	UINT32							idxc=0;
	PUCHAR							bufIn;
	KIRQL							irql;

    devExt = (PITFDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);

	LimpiarMemoria(devExt->vDevExt);

	RtlCopyMemory(&npos,Irp->AssociatedIrp.SystemBuffer,2);
	if(npos>=(PAGE_SIZE/sizeof(COMANDO))) {
		status=STATUS_INSUFFICIENT_RESOURCES;
		Irp->IoStatus.Information = 2;
	} else if(npos==0) {
		Irp->IoStatus.Information = 2;
	} else {

		bufIn=(PUCHAR)Irp->AssociatedIrp.SystemBuffer+2;

		KeAcquireSpinLock(&devExt->slComandos,&irql);

		devExt->Comandos = (PCOMANDO)ExAllocatePoolWithTag(NonPagedPool,
															sizeof(COMANDO)*npos,
															(ULONG)'ocpV');
		if(devExt->Comandos==NULL) {
			KeReleaseSpinLock(&devExt->slComandos,irql);
			status=STATUS_INSUFFICIENT_RESOURCES;
			Irp->IoStatus.Information = 2;
		} else {
			while(devExt->nComandos!=npos)
			{
				if(irpStack->Parameters.DeviceIoControl.InputBufferLength<(idxc+3)) {
					status=STATUS_BUFFER_TOO_SMALL;
					goto mal;
				}
				devExt->Comandos[devExt->nComandos].tam=bufIn[idxc]; idxc++;
				if(irpStack->Parameters.DeviceIoControl.InputBufferLength<(idxc+2+(devExt->Comandos[devExt->nComandos].tam*sizeof(UINT16)))) {
					status=STATUS_BUFFER_TOO_SMALL;
					goto mal;
				}
				devExt->Comandos[devExt->nComandos].datos= (PUINT16)ExAllocatePoolWithTag(NonPagedPool,
																sizeof(UINT16)*devExt->Comandos[devExt->nComandos].tam,
																(ULONG)'ocpV');
				if(devExt->Comandos[devExt->nComandos].datos==NULL) {
					status=STATUS_INSUFFICIENT_RESOURCES;
					goto mal;
				} else {
					RtlCopyMemory((PUCHAR)devExt->Comandos[devExt->nComandos].datos,&bufIn[idxc],sizeof(UINT16)*devExt->Comandos[devExt->nComandos].tam);
					idxc+=sizeof(UINT16)*devExt->Comandos[devExt->nComandos].tam;
					devExt->nComandos++;
				}
			}

			KeReleaseSpinLock(&devExt->slComandos,irql);
			goto fin;
mal:
			KeReleaseSpinLock(&devExt->slComandos,irql);
			LimpiarMemoria((PVOID)devExt->vDevExt);
fin:
			Irp->IoStatus.Information = idxc+2;
		}

	}
	Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

VOID LimpiarMemoria(PVOID contexto)
{
	PDEVICE_EXTENSION devExt=(PDEVICE_EXTENSION)contexto;
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION) devExt->itfObj->DeviceExtension;
	KIRQL irql1,irql2,irql3;
	UINT16 idx;

	KeAcquireSpinLock(&idevExt->slComandos,&irql1);
	KeAcquireSpinLock(&devExt->slListaAcciones,&irql2);

	KeAcquireSpinLock(&idevExt->slPosicion,&irql3);
		RtlZeroMemory(idevExt->MapaBotones,sizeof(idevExt->MapaBotones));
		RtlZeroMemory(idevExt->MapaEjes,sizeof(idevExt->MapaEjes));
		RtlZeroMemory(idevExt->posVieja,sizeof(idevExt->posVieja));
	KeReleaseSpinLock(&idevExt->slPosicion,irql3);

		if(idevExt->Comandos!=NULL) {
			for(idx=0;idx<idevExt->nComandos;idx++) {
				if(idevExt->Comandos[idx].datos!=NULL) ExFreePoolWithTag((PVOID)idevExt->Comandos[idx].datos,(ULONG)'ocpV');
			}
			ExFreePoolWithTag((PVOID)idevExt->Comandos,(ULONG)'ocpV');
			idevExt->Comandos=NULL;
			idevExt->nComandos=0;
		}

		if(!ColaEstaVacia(&devExt->ListaAcciones)) {
			PNODO siguiente = devExt->ListaAcciones.principio;
			do {
				ColaBorrar((PCOLA)siguiente->Datos);
				siguiente->Datos=NULL;
				siguiente=siguiente->link;
			} while(!ColaBorrarNodo(&devExt->ListaAcciones,devExt->ListaAcciones.principio,NULL));
		}
		KeClearEvent(&devExt->evAccion);

		RtlZeroMemory(devExt->stTeclado,sizeof(devExt->stTeclado));
		RtlZeroMemory(devExt->stRaton,sizeof(devExt->stRaton));
		idevExt->Pinkie=FALSE;
		idevExt->Modos=0;

	KeReleaseSpinLock(&devExt->slListaAcciones,irql2);
	KeReleaseSpinLock(&idevExt->slComandos,irql1);

	if(idevExt->Configuracion!=NULL) {
		KeAcquireSpinLock(&idevExt->slEstado,&irql1);
			ExFreePoolWithTag(idevExt->Configuracion,(ULONG)'ppHV');
			idevExt->Configuracion=NULL;
			idevExt->Estado&=6;
		KeReleaseSpinLock(&idevExt->slEstado,irql1);
	}
}