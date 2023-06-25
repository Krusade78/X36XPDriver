#include <wdm.h>
#include "hidfilter.h"

#define _ACCIONES_
#include "acciones.h"
#undef _ACCIONES_

/*************************************************************
					FUNCIONES DE ENTRADA
**************************************************************/

BOOLEAN EncolarAcciones(PITFDEVICE_EXTENSION idevExt,PCOLA eventos,UINT16 accion,UCHAR boton)
{
	UCHAR idx;
	KIRQL irql;
	
	KeAcquireSpinLock(&idevExt->slComandos,&irql);
		if(idevExt->nComandos==0 || idevExt->Comandos==NULL) {
			KeReleaseSpinLock(&idevExt->slComandos,irql);
			ColaBorrar(eventos);
			return FALSE;
		}
		for(idx=0;idx<idevExt->Comandos[accion-1].tam;idx++) {
			PVOID evt=ExAllocatePoolWithTag(NonPagedPool, 
					                        sizeof(UCHAR)*2, // Tamaño de un evento
							                (ULONG)'vepV'
					                        );
			if(evt!=NULL) {
				RtlCopyMemory(evt,&(idevExt->Comandos[accion-1].datos[idx]),sizeof(UCHAR)*2);
				if(*((PUCHAR)evt)==9 || *((PUCHAR)evt)==10) ((PUCHAR)evt)[1]=boton;
				if(!ColaPush(eventos,evt)) {
					KeReleaseSpinLock(&idevExt->slComandos,irql);
					ExFreePool(evt);
					ColaBorrar(eventos);
					return FALSE;
				}
			} else {
				KeReleaseSpinLock(&idevExt->slComandos,irql);
				ColaBorrar(eventos);
				return FALSE;
			}
		}
	KeReleaseSpinLock(&idevExt->slComandos,irql);

	return TRUE;
}

VOID Accionar
(
	PDEVICE_EXTENSION devExt,
	UINT16 accionId,
	UCHAR boton
	)
{
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION)devExt->itfObj->DeviceExtension;
	
	if(accionId!=0) {
		PCOLA eventos=ColaCrear();
		if(eventos!=NULL) {
			if(EncolarAcciones(idevExt,eventos,accionId,boton)) {
				KIRQL irql;
				KeAcquireSpinLock(&devExt->slListaAcciones,&irql);
					if(!ColaPush(&devExt->ListaAcciones,eventos)) {
						ColaBorrar(eventos);
					} else {
						KeSetEvent(&devExt->evAccion,0,FALSE);
					}
				KeReleaseSpinLock(&devExt->slListaAcciones,irql);
			}
		}
	}
}

VOID AccionPulsarBoton
(	PDEVICE_EXTENSION devExt,
	UCHAR idx)
{
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION)devExt->itfObj->DeviceExtension;
	UINT16 accionId;
	KIRQL irql;

	KeAcquireSpinLock(&idevExt->slPosicion,&irql);

	if((idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Estado&0xf)>0) {
		accionId=idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Indices[idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Estado>>4];
		idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Estado+=16;
		if((idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Estado>>4)>(idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Estado&0xf)) idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Estado&=0xf;
	} else {
		accionId=idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][1][idx].Indices[0];
	}

	KeReleaseSpinLock(&idevExt->slPosicion,irql);

	Accionar(devExt,accionId,idx);
}

VOID AccionSoltarBoton
(	PDEVICE_EXTENSION devExt,
	UCHAR idx)
{
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION)devExt->itfObj->DeviceExtension;
	UINT16 accionId;
	KIRQL irql;

	KeAcquireSpinLock(&idevExt->slPosicion,&irql);

	if((idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Estado&0xf)>0) {
		accionId=idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Indices[idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Estado>>4];
		idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Estado+=16;
		if((idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Estado>>4)>(idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Estado&0xf)) idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Estado&=0xf;
	} else {
		accionId=idevExt->MapaBotones[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][0][idx].Indices[0];
	}

	KeReleaseSpinLock(&idevExt->slPosicion,irql);

	Accionar(devExt,accionId,idx);
}

VOID AccionEje
(	PDEVICE_EXTENSION devExt,
	UCHAR idx,
	UCHAR cambios)
{
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION)devExt->itfObj->DeviceExtension;
	UINT16 accionID;
	KIRQL irql;

	KeAcquireSpinLock(&idevExt->slPosicion,&irql);
		accionID=idevExt->MapaEjes[(UCHAR)idevExt->Pinkie][idevExt->Modos&0xf][idevExt->Modos>>4][idx].Indices[cambios];
	KeReleaseSpinLock(&idevExt->slPosicion,irql);

	Accionar(devExt,accionID,0);
}

/*************************************************************
					FUNCIONES DE SALIDA
**************************************************************/

BOOLEAN ProcesarEventoRaton(PDEVICE_EXTENSION devExt,PCOLA cola, BOOLEAN* siguiente)
{
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION)devExt->itfObj->DeviceExtension;
	BOOLEAN soltar;
	PNODO nodo=cola->principio,nodoA=NULL;
	struct {
		UCHAR tipo;
		UCHAR dato;
	} evento;

	if(ColaEstaVacia(cola)) {*siguiente=TRUE; return TRUE;}
volver:
	RtlCopyMemory(&evento,(PUCHAR)nodo->Datos,sizeof(UCHAR)*2);
	soltar=((evento.tipo>>5)==1) ? TRUE : FALSE;
	evento.tipo&=0x1f;

	if(evento.tipo==0 || evento.tipo==8 || evento.tipo>11) {
		if(nodo->link==NULL) {
			*siguiente=TRUE;
			return FALSE;
		} else {
			nodoA=nodo;
			nodo=nodo->link;
			goto volver;
		}
	}

	switch(evento.tipo) {
		case 1:
			if(!soltar)	devExt->stRaton[0]|=1; else devExt->stRaton[0]&=254; 
			break;
		case 2:
			if(!soltar)	devExt->stRaton[0]|=2; else devExt->stRaton[0]&=253; 
			break;
		case 3:
			if(!soltar)	devExt->stRaton[0]|=4; else devExt->stRaton[0]&=251; 
			break;
		case 4: //Eje -x
			if(!soltar)	devExt->stRaton[1]|=-evento.dato; else devExt->stRaton[1]=0; 
			break;
		case 5: //Eje x
			if(!soltar)	devExt->stRaton[1]|=evento.dato; else devExt->stRaton[1]=0; 
			break;
		case 6: //Eje -y
			if(!soltar)	devExt->stRaton[2]|=-evento.dato; else devExt->stRaton[2]=0; 
			break;
		case 7: //Eje y
			if(!soltar)	devExt->stRaton[2]|=evento.dato; else devExt->stRaton[2]=0; 
			break;
		case 9: // Autorepeat hold
			if(((idevExt->ultBotones[evento.dato/8]>>(evento.dato%8))&1)==0) {
				if(ColaBorrarNodo(cola,nodo,nodoA)) {
					*siguiente=TRUE;
					return TRUE;
				} else {
					nodo=cola->principio;
					goto volver;
				}
			} else {
				*siguiente=TRUE;
				return FALSE;
			}
			break;
		case 10:
		case 11:
			*siguiente=TRUE;
			return FALSE;
			break;
	}

	*siguiente=FALSE;
	return ColaBorrarNodo(cola,nodo,nodoA);
}

BOOLEAN ProcesarEventoTeclado(PDEVICE_EXTENSION devExt, PCOLA cola, BOOLEAN* siguiente)
{
	PITFDEVICE_EXTENSION idevExt = (PITFDEVICE_EXTENSION)devExt->itfObj->DeviceExtension;
	BOOLEAN soltar,repeat=FALSE;
	PNODO nodo=cola->principio,nodoA=NULL;
	struct {
		UCHAR tipo;
		UCHAR dato;
	} evento;

	if(ColaEstaVacia(cola)) {*siguiente=TRUE; return TRUE;}
volver:
	RtlCopyMemory(&evento,(PUCHAR)nodo->Datos,sizeof(UCHAR)*2);
	soltar=((evento.tipo>>5)==1) ? TRUE : FALSE;
	evento.tipo&=0x1f;

	if(evento.tipo>0 && evento.tipo<8) {
		if(nodo->link==NULL) {
			*siguiente=TRUE;
			return FALSE;
		} else {
			nodoA=nodo;
			nodo=nodo->link;
			goto volver;
		}
	}

	switch(evento.tipo) {
		case 0:	//Tecla
			if(!soltar) devExt->stTeclado[evento.dato/8]|=1<<(evento.dato%8); else devExt->stTeclado[evento.dato/8]&=~(1<<(evento.dato%8));
			break;
		case 8: // Delay
			{
				PDELAY_CONTEXT ctx = ExAllocatePoolWithTag(NonPagedPool, 
                                sizeof(DELAY_CONTEXT), 
                                (ULONG)'npHV'
                                );
				if(ctx==NULL) {
					return FALSE;
				} else {
					LONG timeout=(-10*1000*100*evento.dato);
					ctx->DeviceExtension=devExt;
					KeInitializeDpc(&ctx->DelayTimerDpc,
				            DpcRoutineDelay,
					        (PVOID)ctx
						    );
					KeInitializeTimer(&ctx->DelayTimer);
					ctx->NodoFin=cola->fin;
					ctx->NodoIni=nodo->link;
					ColaBorrarNodo(cola,nodo,nodoA);
					if(nodoA==NULL) {
						cola->principio=cola->fin=NULL;
					} else {
						nodoA->link=NULL;
						cola->fin=nodoA;
					}
			        KeSetTimer(&ctx->DelayTimer,
					       RtlConvertLongToLargeInteger(timeout),
						   &ctx->DelayTimerDpc
						);
					*siguiente=TRUE;
					return ColaEstaVacia(cola);
				}
			}
			break;
		case 9: // Autorepeat hold
			if(((idevExt->ultBotones[evento.dato/8]>>(evento.dato%8))&1)==0) {
				if(ColaBorrarNodo(cola,nodo,nodoA)) {
					*siguiente=TRUE;
					return TRUE;
				} else {
					nodo=cola->principio;
					goto volver;
				}
			} else {
				*siguiente=TRUE;
				return FALSE;
			}
			break;
		case 10: // Autorepeat
			if(((idevExt->ultBotones[evento.dato/8]>>(evento.dato%8))&1)==0) {
				PNODO nodos=nodo->link;
				while(*((PUCHAR)nodos->Datos)!=27)
				{
					ColaBorrarNodo(cola,nodos,nodo);
					nodos=nodo->link;
				}
				ColaBorrarNodo(cola,nodos,nodo);
				if(!ColaBorrarNodo(cola,nodo,nodoA)) {
					nodo=cola->principio;
					goto volver;
				} else {
					*siguiente=TRUE;
					return TRUE;
				}
			} else {
				PNODO nodos=nodo->link;
				PNODO pos=NULL;
				UCHAR idx=0;
				while(*((PUCHAR)nodos->Datos)!=27) //tipo soltar 10
				{
					PNODO nodon = (PNODO)ExAllocatePoolWithTag(NonPagedPool, 
																sizeof(NODO), 
																(ULONG)'npHV'
																);
					if(nodon!=NULL) {
						PVOID datos=ExAllocatePoolWithTag(NonPagedPool, 
																sizeof(UCHAR)*2, 
																(ULONG)'npHV'
																);
						if(datos!=NULL) {
							RtlCopyMemory(datos,nodos->Datos,2);
							nodon->Datos=datos;
							if(pos==NULL) {
								nodon->link=cola->principio;
								cola->principio=nodon;
							} else {
								nodon->link=pos->link;
								pos->link=nodon;
							}
							pos=nodon;
							idx++;
						} else {
							ExFreePool((PVOID)nodon);
						}
					}

					nodos=nodos->link;
				}
				if(idx!=0) {
					PNODO nodon = (PNODO)ExAllocatePoolWithTag(NonPagedPool, 
														sizeof(NODO), 
														(ULONG)'npHV'
														);
					if(nodon!=NULL) {
						PVOID datos=ExAllocatePoolWithTag(NonPagedPool, 
																sizeof(UCHAR)*2, 
																(ULONG)'npHV'
																);
						if(datos!=NULL) {
							*((PUCHAR)datos)=15;
							*((PUCHAR)datos+1)=0;
							nodon->Datos=datos;
							nodon->link=cola->principio;
							cola->principio=nodon;
						} else {
							ExFreePool((PVOID)nodon);
						}
					}

					nodo=cola->principio;
					goto volver;
				}
			}
			break;
		case 11: // Autorepeat
			if(evento.dato!=0) {
				PNODO nodos=nodo->link;
				PNODO pos=NULL;
				UCHAR idx=0;
				*((PUCHAR)nodo->Datos+1)-=1;
				while(*((PUCHAR)nodos->Datos)!=27)
				{
					PNODO nodon = (PNODO)ExAllocatePoolWithTag(NonPagedPool, 
																sizeof(NODO), 
																(ULONG)'npHV'
																);
					if(nodon!=NULL) {
						PVOID datos=ExAllocatePoolWithTag(NonPagedPool, 
																sizeof(UCHAR)*2, 
																(ULONG)'npHV'
																);
						if(datos!=NULL) {
							RtlCopyMemory(datos,nodos->Datos,2);
							nodon->Datos=datos;
							if(pos==NULL) {
								nodon->link=cola->principio;
								cola->principio=nodon;
							} else {
								nodon->link=pos->link;
								pos->link=nodon;
							}
							pos=nodon;
							idx++;
						} else {
							ExFreePool((PVOID)nodon);
						}
					}

					nodos=nodos->link;
				}
				if(idx!=0) {
					PNODO nodon = (PNODO)ExAllocatePoolWithTag(NonPagedPool, 
														sizeof(NODO), 
														(ULONG)'npHV'
														);
					if(nodon!=NULL) {
						PVOID datos=ExAllocatePoolWithTag(NonPagedPool, 
																sizeof(UCHAR)*2, 
																(ULONG)'npHV'
																);
						if(datos!=NULL) {
							*((PUCHAR)datos)=15;
							*((PUCHAR)datos+1)=0;
							nodon->Datos=datos;
							nodon->link=cola->principio;
							cola->principio=nodon;
						} else {
							ExFreePool((PVOID)nodon);
						}
					}

					nodo=cola->principio;
					goto volver;
				}
			}
			break;
		case 12: //Cambio modo
			idevExt->Modos=(idevExt->Modos&0xf0)|evento.dato;
			break;
		case 13: //Cambio modo Aux
			idevExt->Modos=(idevExt->Modos&0xf)|(evento.dato<<4);
			break;
		case 14: //Cambio modo Pinkie
			idevExt->Pinkie=(evento.dato==1) ? TRUE:FALSE;
			break;
		case 15:
			if(nodo->link!=NULL) {
				if(*((PUCHAR)(nodo->link->Datos))==10 || *((PUCHAR)(nodo->link->Datos))==11) {
					ColaBorrarNodo(cola,nodo,nodoA);
					nodo=cola->principio;
					repeat=FALSE;
				} else {
					nodoA=nodo;
					nodo=nodo->link;
					repeat=TRUE;
				}
				goto volver;
			}
			break;
		case 16: // Botón DX
			{
				KIRQL irql;
				KeAcquireSpinLock(&devExt->slGameData,&irql);
					if(!soltar) devExt->stBotonesDX[(evento.dato-1)/8]|=1<<((evento.dato-1)%8); else devExt->stBotonesDX[(evento.dato-1)/8]&=~(1<<((evento.dato-1)%8));
				KeReleaseSpinLock(&devExt->slGameData,irql);
			}
			break;
		case 17: // Seta DX
			{
				KIRQL irql;
				KeAcquireSpinLock(&devExt->slGameData,&irql);
					if(!soltar) devExt->stSetasDX[(evento.dato/10)-1]=(evento.dato%10); else devExt->stSetasDX[(evento.dato/10)-1]=0;
				KeReleaseSpinLock(&devExt->slGameData,irql);
			}
			break;
	}

	*siguiente=repeat;
	return ColaBorrarNodo(cola,nodo,nodoA);
}

VOID ProcesarAccion(PDEVICE_EXTENSION devExt,BOOLEAN raton)
{
	if(!ColaEstaVacia(&devExt->ListaAcciones)) {
		PNODO nodo=devExt->ListaAcciones.principio;
		PNODO nodoA=NULL;
		BOOLEAN finEvt,sig=FALSE;
		do {
			if(raton) {
				finEvt=ProcesarEventoRaton(devExt,(PCOLA)nodo->Datos,&sig);
			} else {
				finEvt=ProcesarEventoTeclado(devExt,(PCOLA)nodo->Datos,&sig);
			}
			if(finEvt) { // Fin eventos
				PNODO nodos=nodo->link;
				ColaBorrar((PCOLA)nodo->Datos);
				nodo->Datos=NULL;
				ColaBorrarNodo(&devExt->ListaAcciones,nodo,nodoA);
				nodo=nodos;
			} else {	// Quedan eventos
				nodoA=nodo;
				nodo=nodo->link;
			}
		} while(sig && nodo!=NULL);

		if(ColaEstaVacia(&devExt->ListaAcciones)) {
			KeClearEvent(&devExt->evAccion);
		}
	}
}

VOID ProcesarAccionRaton(PDEVICE_EXTENSION devExt)
{
	ProcesarAccion(devExt,TRUE);
}

VOID ProcesarAccionTeclado(PDEVICE_EXTENSION devExt)
{
	ProcesarAccion(devExt,FALSE);
}


VOID 
DpcRoutineDelay(   
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    )
{
	PDELAY_CONTEXT ctx=(PDELAY_CONTEXT)DeferredContext;
	PDEVICE_EXTENSION devExt=ctx->DeviceExtension;
	PNODO nodoini=ctx->NodoIni,nodofin=ctx->NodoFin;
	PCOLA eventos;
	ExFreePool(DeferredContext);

	eventos=ColaCrear();
	if(eventos!=NULL) {
		eventos->principio=nodoini;
		eventos->fin=nodofin;
		KeAcquireSpinLockAtDpcLevel(&devExt->slListaAcciones);
		if(!ColaPush(&devExt->ListaAcciones,eventos)) {
			KeReleaseSpinLockFromDpcLevel(&devExt->slListaAcciones);
			ColaBorrar(eventos);
		} else {
			KeReleaseSpinLockFromDpcLevel(&devExt->slListaAcciones);
		}
	}
}