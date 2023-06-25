/*
 *  pnp.c
 */

// Funciones publicas

NTSTATUS HGM_PnP
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS HGM_Power
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP pIrp
    );

NTSTATUS HGM_IncRequestCount
    (
    PDEVICE_EXTENSION DeviceExtension
    );

VOID HGM_DecRequestCount
    (
    PDEVICE_EXTENSION DeviceExtension
    );

// Funciones privadas

#ifdef _PNPC_

VOID HGM_RemoveDevice
    (
    PDEVICE_EXTENSION DeviceExtension
    );

NTSTATUS HGM_InitDevice
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS HGM_GetResources
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS HGM_PnPComplete
    (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Context
    );

#endif