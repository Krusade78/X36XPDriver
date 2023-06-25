typedef struct _HIDGAME_INPUT_DATA
{
    UCHAR   Ejes[2];
    UCHAR   Setas[4];
    UCHAR   Rotatorios[2];
	UCHAR	Botones[5];
} HIDGAME_INPUT_DATA, *PHIDGAME_INPUT_DATA;

NTSTATUS
HF_IoDescalibrar(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
	);

NTSTATUS
HF_IoRecalibrar(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
HF_IoEscribirConfiguracion
(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
HF_IoLeer(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
HF_IoLeerEstado(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
HF_IoLeerConf(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
	);

NTSTATUS
HF_IoLeer(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

HF_IoWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );


#ifdef _IO_

typedef struct _ST_CONF
{
	//UCHAR MapaBotones[2][40]; // [0][] boton [1][] seta
	UCHAR MapaEjes[4];
	UCHAR MapaRotatorios;	//bit 1 y 2 rot 1, bit 3 y 4 rot 2
} CONF,*PCONF;

VOID 
ProcesarWrite(   
    PDEVICE_EXTENSION devExt,
	PHIDGAME_INPUT_DATA hidGameData
    );

UCHAR TraducirGiratorio(
	PITFDEVICE_EXTENSION idevExt,
	UCHAR eje,
	UCHAR nueva
	);

#endif