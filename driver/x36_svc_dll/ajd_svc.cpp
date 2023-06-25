/*--
Copyright (c) 2005 Alfredo Costalago

Module Name:

    AJD-Comp.cpp

Funcion : Libreria de compilacion y carga
--*/

#include <windows.h>
#include "lanzador.h"
#include "joyconfig.h"
#include "reordenar.h"
#include "idioma.h"


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

__declspec(dllexport) bool CompileAndLaunch(char* mdf, char* jdf)
{
	bool ok;
	CLanzador l(mdf,jdf);
	ok=l.Compilar();
	if(ok) ok=l.Cargar();
	return ok; 
}

__declspec(dllexport) bool Compile(char* mdf, char* jdf)
{
	CLanzador l(mdf,jdf);
	return l.Compilar(); 
}

__declspec(dllexport) bool CargarDatos(char* mdf, char* jdf,BYTE* mapaB,BYTE* mapaE,BYTE* conf,BOOLEAN* mapaAD)
{
	bool ok;
	CLanzador l(mdf,jdf);
	ok=l.Compilar();
	if(ok) l.CargarE(mapaB,mapaE,conf,mapaAD);
	return ok; 
}

__declspec(dllexport) bool ComprobarJoysticks(void)
{
	CjoyConfig joyc;
	return joyc.Comprobar(); 
}

__declspec(dllexport) bool Recalibrate()
{
	UCHAR ejesaux[4]={0,1,2,3};
	CReorden ord;
	if(!ord.Recalibrar(ejesaux)) {
		Traduce::Msg(NULL,LG_C_ERROR5,"Recalibrar",MB_ICONERROR);
		return false;
	}
	return true;
}

