/*--
Copyright (c) 2005 Alfredo Costalago

Module Name:

    x36.c

--*/

NTSTATUS LowerCompletionRoutineOut(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  Irp,
    IN PVOID  Context
    )
{
	IoFreeIrp(Irp);
	ExFreePool(Context);
	return STATUS_MORE_PROCESSING_REQUIRED;
}

VOID LLamarDispositivoVirtual
    (
    PX36DEVICE_EXTENSION x36devExt
    )
{
	PIRP pIrpOut;
	PVOID bufOut;
	LARGE_INTEGER	offset;
	IO_STATUS_BLOCK IoStatus;
	PDEVICE_EXTENSION devExt=x36devExt->kDevExt;

	if(devExt->vfo!=NULL)
	{
		bufOut = ExAllocatePoolWithTag(NonPagedPool, 
                                       sizeof(HIDGAME_INPUT_DATA), 
                                       (ULONG)'f63X'
                                       );
		if(bufOut!=NULL) {
			RtlCopyMemory(bufOut,&x36devExt->hidData,sizeof(HIDGAME_INPUT_DATA));
			offset.HighPart=0;
			offset.LowPart=0;
			pIrpOut =   IoBuildAsynchronousFsdRequest(IRP_MJ_WRITE,
													devExt->vdo,
													bufOut,
													sizeof(HIDGAME_INPUT_DATA),
													&offset,
													&IoStatus);
			if(pIrpOut!=NULL) {
				IoSetCompletionRoutine(pIrpOut,
									LowerCompletionRoutineOut,
									bufOut,
									TRUE,
									TRUE,
									TRUE);
				IoCallDriver(devExt->vdo,pIrpOut);
			}
		}
	}
}

void CambiarEstado(PX36DEVICE_EXTENSION devExt,UCHAR ID,BOOLEAN dato)
{
	if(dato)
	{
		devExt->hidData.Botones[ID/8]|=1<<(ID%8);
	} else {
		devExt->hidData.Botones[ID/8]&=255-(1<<(ID%8));
	}
	LLamarDispositivoVirtual(devExt);
}


void TraducirBoton(PX36DEVICE_EXTENSION devExt,USHORT MakeCode)
{
	switch (MakeCode)
	{
//****************************** X36 F
//------------ Gatillo
		case 13:
            CambiarEstado(devExt,0,TRUE);
			break;
		case 77:
            CambiarEstado(devExt,0,FALSE);
			break;
//------------ Lanzar
		case 4:
            CambiarEstado(devExt,1,TRUE);
			break;
		case 68:
            CambiarEstado(devExt,1,FALSE);
			break;

//------------ Boton a
		case 2:
            CambiarEstado(devExt,2,TRUE);
			break;
		case 66:
            CambiarEstado(devExt,2,FALSE);
			break;
//----------- Boton b
		case 3:
            CambiarEstado(devExt,3,TRUE);
			break;
		case 67:
            CambiarEstado(devExt,3,FALSE);
			break;
//------------ Boton c
		case 1:
            CambiarEstado(devExt,4,TRUE);
			break;
		case 65:
            CambiarEstado(devExt,4,FALSE);
			break;

// ------------ Pinkie
		case 14:
            CambiarEstado(devExt,5,TRUE);
			break;
		case 78:
            CambiarEstado(devExt,5,FALSE);
			break;
//************ Seta 1
		case 5:
			devExt->stSeta1=devExt->stSeta1|1;
			if((devExt->stSeta1&4)) {
                CambiarEstado(devExt,8,FALSE);
                CambiarEstado(devExt,10,TRUE);
			} else {
				if((devExt->stSeta1&8)) {
                    CambiarEstado(devExt,9,FALSE);
                    CambiarEstado(devExt,11,TRUE);
				} else {
                    CambiarEstado(devExt,6,TRUE);
				}
			}
			break;
		case 69:
			devExt->stSeta1=devExt->stSeta1&254;
			if((devExt->stSeta1&4)) {
                CambiarEstado(devExt,10,FALSE);
                CambiarEstado(devExt,8,TRUE);
			} else {
				if((devExt->stSeta1&8)) {
                    CambiarEstado(devExt,11,FALSE);
                    CambiarEstado(devExt,9,TRUE);
				} else {
                    CambiarEstado(devExt,6,FALSE);
				}
			}
			break;
		case 7:
			devExt->stSeta1=devExt->stSeta1|2;
			if((devExt->stSeta1&4)) {
                CambiarEstado(devExt,8,FALSE);
                CambiarEstado(devExt,12,TRUE);
			} else {
				if((devExt->stSeta1&8)) {
                    CambiarEstado(devExt,9,FALSE);
                    CambiarEstado(devExt,13,TRUE);
				} else {
                    CambiarEstado(devExt,7,TRUE);
				}
			}
			break;
		case 71:
			devExt->stSeta1=devExt->stSeta1&253;
			if((devExt->stSeta1&4)) {
                CambiarEstado(devExt,12,FALSE);
                CambiarEstado(devExt,8,TRUE);
			} else {
				if((devExt->stSeta1&8)) {
                    CambiarEstado(devExt,13,FALSE);
                    CambiarEstado(devExt,9,TRUE);
				} else {
                    CambiarEstado(devExt,7,FALSE);
				}
			}
			break;
		case 8:
			devExt->stSeta1=devExt->stSeta1|4;
			if((devExt->stSeta1&1)) {
                CambiarEstado(devExt,6,FALSE);
                CambiarEstado(devExt,10,TRUE);
			} else {
				if((devExt->stSeta1&2)) {
                    CambiarEstado(devExt,7,FALSE);
                    CambiarEstado(devExt,12,TRUE);
				} else {
                    CambiarEstado(devExt,8,TRUE);
				}
			}
			break;
		case 72:
			devExt->stSeta1=devExt->stSeta1&251;
			if((devExt->stSeta1&1)) {
                CambiarEstado(devExt,10,FALSE);
                CambiarEstado(devExt,6,TRUE);
			} else {
				if((devExt->stSeta1&2)) {
                    CambiarEstado(devExt,12,FALSE);
                    CambiarEstado(devExt,7,TRUE);
				} else {
                    CambiarEstado(devExt,8,FALSE);
				}
			}
			break;
		case 6:
			devExt->stSeta1=devExt->stSeta1|8;
			if((devExt->stSeta1&1)) {
                CambiarEstado(devExt,6,FALSE);
                CambiarEstado(devExt,11,TRUE);
			} else {
				if((devExt->stSeta1&2)) {
                    CambiarEstado(devExt,7,FALSE);
                    CambiarEstado(devExt,13,TRUE);
				} else {
                    CambiarEstado(devExt,9,TRUE);
				}
			}
			break;
		case 70:
			devExt->stSeta1=devExt->stSeta1&247;
			if((devExt->stSeta1&1)) {
                CambiarEstado(devExt,11,FALSE);
                CambiarEstado(devExt,6,TRUE);
			} else {
				if((devExt->stSeta1&2)) {
                    CambiarEstado(devExt,13,FALSE);
                    CambiarEstado(devExt,7,TRUE);
				} else {
                    CambiarEstado(devExt,9,FALSE);
				}
			}
			break;
//----------------
//********** Seta 2
		case 9:
			devExt->stSeta2=devExt->stSeta2|16;
			if((devExt->stSeta2&64)) {
                CambiarEstado(devExt,16,FALSE);
                CambiarEstado(devExt,18,TRUE);
			} else {
				if((devExt->stSeta2&128)) {
                    CambiarEstado(devExt,17,FALSE);
                    CambiarEstado(devExt,19,TRUE);
				} else {
                    CambiarEstado(devExt,14,TRUE);
				}
			}
			break;
		case 73:
			devExt->stSeta2=devExt->stSeta2&239;
			if((devExt->stSeta2&64)) {
                CambiarEstado(devExt,18,FALSE);
                CambiarEstado(devExt,16,TRUE);
			} else {
				if((devExt->stSeta2&128)) {
                    CambiarEstado(devExt,19,FALSE);
                    CambiarEstado(devExt,17,TRUE);
				} else {
                    CambiarEstado(devExt,14,FALSE);
				}
			}
			break;
		case 11:
			devExt->stSeta2=devExt->stSeta2|32;
			if((devExt->stSeta2&64)) {
                CambiarEstado(devExt,16,FALSE);
                CambiarEstado(devExt,20,TRUE);
			} else {
				if((devExt->stSeta2&128)) {
                    CambiarEstado(devExt,17,FALSE);
                    CambiarEstado(devExt,21,TRUE);
				} else {
                    CambiarEstado(devExt,15,TRUE);
				}
			}
			break;
		case 75:
			devExt->stSeta2=devExt->stSeta2&223;
			if((devExt->stSeta2&64)) {
                CambiarEstado(devExt,20,FALSE);
                CambiarEstado(devExt,16,TRUE);
			} else {
				if((devExt->stSeta2&128)) {
                    CambiarEstado(devExt,21,FALSE);
                    CambiarEstado(devExt,17,TRUE);
				} else {
                    CambiarEstado(devExt,15,FALSE);
				}
			}
			break;
		case 12:
			devExt->stSeta2=devExt->stSeta2|64;
			if((devExt->stSeta2&16)) {
                CambiarEstado(devExt,14,FALSE);
                CambiarEstado(devExt,18,TRUE);
			} else {
				if((devExt->stSeta2&32)) {
                    CambiarEstado(devExt,15,FALSE);
                    CambiarEstado(devExt,20,TRUE);
				} else {
                    CambiarEstado(devExt,16,TRUE);
				}
			}
			break;
		case 76:
			devExt->stSeta2=devExt->stSeta2&191;
			if((devExt->stSeta2&16)) {
                CambiarEstado(devExt,18,FALSE);
                CambiarEstado(devExt,14,TRUE);
			} else {
				if((devExt->stSeta2&32)) {
                    CambiarEstado(devExt,20,FALSE);
                    CambiarEstado(devExt,15,TRUE);
				} else {
                    CambiarEstado(devExt,16,FALSE);
				}
			}
			break;
		case 10:
			devExt->stSeta2=devExt->stSeta2|128;
			if((devExt->stSeta2&16)) {
                CambiarEstado(devExt,14,FALSE);
                CambiarEstado(devExt,19,TRUE);
			} else {
				if((devExt->stSeta2&32)) {
                    CambiarEstado(devExt,15,FALSE);
                    CambiarEstado(devExt,21,TRUE);
				} else {
                    CambiarEstado(devExt,17,TRUE);
				}
			}
			break;
		case 74:
			devExt->stSeta2=devExt->stSeta2&127;
			if((devExt->stSeta2&16)) {
                CambiarEstado(devExt,19,FALSE);
                CambiarEstado(devExt,14,TRUE);
			} else {
				if((devExt->stSeta2&32)) {
                    CambiarEstado(devExt,21,FALSE);
                    CambiarEstado(devExt,15,TRUE);
				} else {
                    CambiarEstado(devExt,17,FALSE);
				}
			}
			break;
//----------------
//******************** X35 ********************
//------------ Boton d
		case 15:
            CambiarEstado(devExt,22,TRUE);
			break;
		case 79:
            CambiarEstado(devExt,22,FALSE);
			break;
//----------- Boton Raton
		case 16:
            CambiarEstado(devExt,23,TRUE);
			break;
		case 80:
            CambiarEstado(devExt,23,FALSE);
			break;
//----------- Modo Izquierda
		case 32:
            CambiarEstado(devExt,24,TRUE);
			break;
		case 96:
            CambiarEstado(devExt,24,FALSE);
			break;
//----------- Modo Derecha
		case 31:
            CambiarEstado(devExt,25,TRUE);
			break;
		case 95:
            CambiarEstado(devExt,25,FALSE);
			break;
//----------- Aux Izquierda
		case 30:
            CambiarEstado(devExt,26,TRUE);
			break;
		case 94:
            CambiarEstado(devExt,26,FALSE);
			break;
//----------- Aux Derecha
		case 29:
            CambiarEstado(devExt,27,TRUE);
			break;
		case 93:
            CambiarEstado(devExt,27,FALSE);
			break;
//*********** Seta Raton
		case 21:
            CambiarEstado(devExt,28,TRUE);
			break;
		case 85:
            CambiarEstado(devExt,28,FALSE);
			break;
		case 23:
            CambiarEstado(devExt,29,TRUE);
			break;
		case 87:
            CambiarEstado(devExt,29,FALSE);
			break;
		case 24:
            CambiarEstado(devExt,30,TRUE);
			break;
		case 88:
            CambiarEstado(devExt,30,FALSE);
			break;
		case 22:
            CambiarEstado(devExt,31,TRUE);
			break;
		case 86:
            CambiarEstado(devExt,31,FALSE);
			break;
//-------------
//********** Seta 3
		case 17:
            CambiarEstado(devExt,32,TRUE);
			break;
		case 81:
            CambiarEstado(devExt,32,FALSE);
			break;
		case 19:
            CambiarEstado(devExt,33,TRUE);
			break;
		case 83:
            CambiarEstado(devExt,33,FALSE);
			break;
		case 20:
            CambiarEstado(devExt,34,TRUE);
			break;
		case 84:
            CambiarEstado(devExt,34,FALSE);
			break;
		case 18:
            CambiarEstado(devExt,35,TRUE);
			break;
		case 82:
            CambiarEstado(devExt,35,FALSE);
//------------
	}
}


void TraducirGiratorio(PX36DEVICE_EXTENSION devExt,USHORT MakeCode)
{
	UCHAR girNuevo=((UCHAR)devExt->gPosicion-48)+(((UCHAR)MakeCode-48)*16);

	if(devExt->gEje<2) {
		devExt->hidData.Ejes[devExt->gEje]=girNuevo;
	} else {
		//devExt->hidData.Rotatorios[devExt->gEje-2]=girNuevo;
		if(girNuevo==127 || girNuevo==128) {
			devExt->hidData.Rotatorios[devExt->gEje-2]=0;
		} else {
			devExt->hidData.Rotatorios[devExt->gEje-2]=girNuevo-((girNuevo>128)?128:127);
		}
	}

	LLamarDispositivoVirtual(devExt);
}
