/*--
Copyright (c) 2005 Alfredo Costalago

Module Name:

    servicio.c
--*/

#include <windows.h>
#include <winioctl.h>
#include "servicio.h"
#include "idioma.h"


#define IOCTL_X36_JOYINICIAR	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0101, METHOD_BUFFERED, FILE_ANY_ACCESS)

CServicio::CServicio(){}

/****************************************************
			FUNCIONES DE INICIO
*****************************************************/

BOOL CServicio::IniciarServicio()
{
	if(!IniciarJoysticks()) return FALSE;
	IniciarX36();

	return TRUE;
}

BOOL CServicio::IniciarJoysticks()
{
	typedef bool (*FUNC)(void);
	bool ok;
	HMODULE dll=LoadLibrary("x36_svc.dll");
	if(dll==NULL) return FALSE;

	FUNC Comprobar=(FUNC)GetProcAddress(dll,"ComprobarJoysticks");
	if(Comprobar==NULL) {
		ok=false;
	} else {
		ok=Comprobar();
	}

	FreeLibrary(dll);
	return (BOOL)ok;
}

void CServicio::IniciarX36()		// Error 1
{
	DWORD ret;

	HANDLE driver=CreateFile(
			"\\\\.\\X36KbInterface",
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if(driver==INVALID_HANDLE_VALUE) {
		Traduce::Msg(NULL,LG_ERROR1,"[X36-Service][1.1]",MB_SERVICE_NOTIFICATION|MB_ICONWARNING);
		return;
	}
	if(!DeviceIoControl(driver,IOCTL_X36_JOYINICIAR,NULL,0,NULL,0,&ret,NULL))
		Traduce::Msg(NULL,LG_ERROR2,"[X36-Service][1.2]",MB_SERVICE_NOTIFICATION|MB_ICONWARNING);
	CloseHandle(driver);
}