#include <gameport.h>
#include "cola.h"

typedef struct _DEVICE_EXTENSION
{
	ULONG id;
    //
    // The top of the stack before this filter was added.  AKA the location
    // to which all IRPS should be directed.
    //
    PDEVICE_OBJECT  TopOfStack;

	PDEVICE_OBJECT itfObj;

	UCHAR	stRaton[3];
	UCHAR	stTeclado[29];
	UCHAR	stBotonesDX[5];
	UCHAR	stSetasDX[4];
	UINT16	DPCsActivos;
	KEVENT  evAccion;
	KSPIN_LOCK slGameData;
	KSPIN_LOCK slListaAcciones;
	KSPIN_LOCK slDPCsActivos;
	COLA ListaAcciones;
	UCHAR TickRaton;
	UCHAR TickRatonTimer;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

typedef struct _ST_COMANDO
{
	UCHAR tam;
	UINT16 *datos;
} COMANDO, *PCOMANDO;

typedef struct _ITFDEVICE_EXTENSION
{
	ULONG id;
	PDEVICE_EXTENSION vDevExt;

	struct {
		UCHAR Estado;	// 4 bit idc 4 bit npos
		UINT16 Indices[16];
	}MapaBotones[2][3][3][2][36];
	struct {
		BOOLEAN Incremental;
		UCHAR Bandas[15];
		UINT16 Indices[16];
	}MapaEjes[2][3][3][4];

	BOOLEAN Pinkie;
	UCHAR Modos; // 4 bits modo, 4 bits modo aux
	UCHAR	stEjesDX[2];
	UCHAR	stRotatoriosDX[2];
	UCHAR ultBotones[5];
	UCHAR posVieja[2][3][3][4];
	KSPIN_LOCK slPosicion;

	COMANDO *Comandos;
	UINT16 nComandos;
	KSPIN_LOCK slComandos;

	KSPIN_LOCK slEstado;
	UCHAR Estado;
	PVOID Configuracion;

	//HIDGAME_INPUT_DATA HidGameData;

}ITFDEVICE_EXTENSION,*PITFDEVICE_EXTENSION;

#ifdef _HIDFILTER_

NTSTATUS DriverEntry
    (
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    );

//DRIVER_ADD_DEVICE HF_AddDevice;
NTSTATUS
HF_AddDevice(
    IN PDRIVER_OBJECT   Driver,
    IN PDEVICE_OBJECT   PDO
    );

VOID
IniciarExtensiones(
	PDEVICE_EXTENSION devExt
	);

//DRIVER_UNLOAD HF_Unload;
VOID
HF_Unload(
   IN PDRIVER_OBJECT Driver
   );

//DRIVER_DISPATCH HF_DispatchPassThrough;
NTSTATUS
HF_DispatchPassThrough(
        IN PDEVICE_OBJECT DeviceObject,
        IN PIRP Irp
        );

NTSTATUS
HF_InternIoCtl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );
NTSTATUS
HF_InternIoCtlA(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
HF_PnP(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
HF_Power(
    IN PDEVICE_OBJECT    DeviceObject,
    IN PIRP              Irp
    );

#endif