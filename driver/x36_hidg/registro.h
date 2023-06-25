VOID HGM_CargarCalibracion
    (
    IN      PDEVICE_OBJECT       DeviceObject
    );

#ifdef _REGISTRO_

typedef struct _ST_CALIBRADO {
	ULONG i;
	ULONG c;
	ULONG d;
	UCHAR n;
	UCHAR Margen;
}CALIBRADO,*PCALIBRADO;

NTSTATUS
QueryValueKey (
    IN  HANDLE  Handle,
    IN  PWCHAR  ValueNameString,
    OUT PVOID   Data,
    IN  ULONG   DataLength
    );

#endif