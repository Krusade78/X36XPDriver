/*--
Copyright (c) 2005 Alfredo Costalago
--*/

#include <windows.h>
#include "cidioma.h"


__declspec(dllexport) void TraduceGlobal(short ID,char* texto);
__declspec(dllexport) void TraduceLocal(short ID,char* texto);
void Traduce(short ID,char* texto,bool ndrv);


BOOL WINAPI DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

__declspec(dllexport) void TraduceGlobal(short ID,char* texto)
{
	Traduce(ID,texto,false);
}
__declspec(dllexport) void TraduceLocal(short ID,char* texto)
{
	Traduce(ID,texto,true);
}

void Traduce(short ID,char* texto,bool ndrv)
{
	char buf[1024];
	ZeroMemory(buf,1024);
	CIdioma lng;
	lng.Traducir(ndrv,ID,buf);
	CopyMemory(texto,buf,1024);
}
