NTSTATUS
HF_IoTickRaton(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
	);

VOID LeerTickRaton(
	IN OUT UCHAR* Tick
	);

#ifdef _RATON_

NTSTATUS
QueryValueKey (
    IN  HANDLE  Handle,
    IN  PWCHAR  ValueNameString,
    OUT PVOID   Data,
    IN  ULONG   DataLength
    );

//VOID
//GuardarRegistro (ULONG val);

#endif