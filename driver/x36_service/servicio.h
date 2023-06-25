/*--
Copyright (c) 2005 Alfredo Costalago

Module Name:

    serviciox36.h
--*/

class CServicio
{
public:
	CServicio();

	BOOL IniciarServicio();
	void IniciarX36();
private:
	BOOL IniciarJoysticks();
};