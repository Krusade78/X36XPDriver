VOID AccionPulsarBoton
(	PDEVICE_EXTENSION devExt,
	UCHAR idx);

VOID AccionSoltarBoton
(	PDEVICE_EXTENSION devExt,
	UCHAR idx);

VOID AccionEje
(	PDEVICE_EXTENSION devExt,
	UCHAR idx,
	UCHAR cambios);

VOID ProcesarAccionRaton(PDEVICE_EXTENSION devExt);
VOID ProcesarAccionTeclado(PDEVICE_EXTENSION devExt);

#ifdef _ACCIONES_

typedef struct _DELAY_CONTEXT{
    
    KDPC			DelayTimerDpc;
    KTIMER			DelayTimer;
    PNODO			NodoIni;
    PNODO			NodoFin;
	PDEVICE_EXTENSION DeviceExtension;
  
} DELAY_CONTEXT, *PDELAY_CONTEXT;

VOID 
DpcRoutineDelay(   
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    );

#endif