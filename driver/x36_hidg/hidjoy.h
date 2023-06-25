/*
 *  hidjoy.c
 */

// Funciones públicas

#define MAXBYTES_GAME_REPORT    ( 256 )

NTSTATUS HGM_InitAnalog
    (
    IN PDEVICE_OBJECT         DeviceObject
    );

NTSTATUS HGM_GenerateReport
    (
    IN PDEVICE_OBJECT   DeviceObject,
    OUT UCHAR           rgGameReport[MAXBYTES_GAME_REPORT],
    OUT PUSHORT         pCbReport
    );

VOID HGM_Game2HID
    (
    IN PDEVICE_OBJECT			DeviceObject,
	IN OUT PUHIDGAME_INPUT_DATA	pHIDData
    );

// Funciones privadas

#ifdef _HIDJOYC_

NTSTATUS HGM_JoystickConfig
    (
    IN PDEVICE_OBJECT DeviceObject
    );

NTSTATUS HGM_MapAxesFromDevExt
    (
    IN OUT PDEVICE_EXTENSION DeviceExtension 
    );

VOID HGM_CogerDatosX36
    (
    IN      PDEVICE_OBJECT       DeviceObject,
    IN  OUT PHIDGAME_INPUT_DATA    pHIDData
    );

VOID HGM_ComprobarEstado
	(
    IN      PDEVICE_OBJECT       DeviceObject
	);

VOID HGM_CargarConfiguracion
    (
    IN      PDEVICE_OBJECT       DeviceObject
    );

VOID HGM_Filtrar
    (
    IN      PDEVICE_OBJECT       DeviceObject,
    IN  OUT PHIDGAME_INPUT_DATA    pHIDData
    );

#endif