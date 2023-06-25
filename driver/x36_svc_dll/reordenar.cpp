/*--
Copyright (c) 2005 Alfredo Costalago

Module Name:

    Reordenar.cpp

Funcion : Funciones DirectX Calibrado (clase)

--*/


#include <windows.h>
#include <dinput.h>
#include "reordenar.h"

//#pragma comment(lib,"dxguid.lib")
//#pragma comment(lib,"dinput8.lib")

LPDIRECTINPUT8  g_pDI=NULL;
LPDIRECTINPUTDEVICE8 g_pJoystick=NULL; 
BOOL CALLBACK DIEnumDevicesCallback(
  LPCDIDEVICEINSTANCE lpddi,  
  LPVOID pvRef  
);

//-------------------------------------------------//
//-------------------------------------------------//

CReorden::CReorden(/*HINSTANCE hInst*/)
{
//	this->hInst=hInst;
	this->hInst=GetModuleHandle(NULL);
	for(UCHAR i=0;i<4;i++)
	{
		datosEje[i].Margen=0;
		datosEje[i].n=0;
		datosEje[i].i=0;
		datosEje[i].d=1000;
		datosEje[i].c=500;
	}
}

CReorden::~CReorden()
{
	if(g_pJoystick!=NULL) {
		g_pJoystick->Release();
		g_pJoystick=NULL;
	}
	if(g_pDI!=NULL) {
		g_pDI->Release();
		g_pDI=NULL;
		CoUninitialize();
	}

}

bool CReorden::Recalibrar(UCHAR* ejes)
{
	if(LeerRegistro()) {
		if(!IniciarDX()) { return false; }
		for(UCHAR i=0;i<((nEjes==6)?4:nEjes);i++)
			if(!Comprobar(i)) { return false; }
		if(!GuardarDatos(ejes)) { return false;	}
	}

	return true;
}

/*************************************************
******************** Métodos *********************
**************************************************/

bool CReorden::LeerRegistro()
{
	HKEY key;
	DWORD tipo,tam=sizeof(CALIBRADO);
	LONG res;

	if(ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado",0,KEY_READ,&key))
		return false;

	res=RegQueryValueEx(key,"Eje1",0,&tipo,(BYTE*)&datosEje[0],&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) goto mal;
	res=RegQueryValueEx(key,"Eje2",0,&tipo,(BYTE*)&datosEje[1],&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) goto mal;
	res=RegQueryValueEx(key,"Eje3",0,&tipo,(BYTE*)&datosEje[2],&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) goto mal;
	res=RegQueryValueEx(key,"Eje4",0,&tipo,(BYTE*)&datosEje[3],&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) goto mal;

	RegCloseKey(key);

	return true;
mal:
	RegCloseKey(key);
	return false;
}

/**********************************************************
						DirectX
***********************************************************/

bool CReorden::IniciarDX()
{
	HRESULT         hr;

	hr = CoInitialize(NULL);
	if(FAILED(hr)) return false;

	hr = DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID*)&g_pDI,NULL);
	if(FAILED(hr)) {
		CoUninitialize();
		return false;
	}

	hr =g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, DIEnumDevicesCallback,
		NULL, DIEDFL_ATTACHEDONLY);
	if(FAILED(hr)) return false;

    if( NULL == g_pJoystick ) return false;

	DIDEVCAPS devCaps;
	devCaps.dwSize=sizeof(DIDEVCAPS);
	if( FAILED( g_pJoystick->GetCapabilities(&devCaps) ) ) return false;

	nEjes=(UCHAR)devCaps.dwAxes;

	return true;
}

BOOL CALLBACK DIEnumDevicesCallback(
  LPCDIDEVICEINSTANCE lpddi,  
  LPVOID pvRef  
)
{
	if(strcmp(lpddi->tszProductName,"Saitek X36F + X35T")==0 || strcmp(lpddi->tszProductName,"Saitek X36F")==0)
	{
		g_pDI->CreateDevice(lpddi->guidInstance,&g_pJoystick, NULL);
		return DIENUM_STOP;
	} else {
		return DIENUM_CONTINUE;
	}
}


//-----------------------------------------------------
//-----------------------------------------------------


bool CReorden::Comprobar(UCHAR eje)
{
	//Comprobar valores
	if((datosEje[eje].d>20000) || (datosEje[eje].i>20000) || (datosEje[eje].c>20000))
		return false;
	if(datosEje[eje].d<datosEje[eje].i) {
		return false;
	} else {
		ULONG ln=(ULONG)(datosEje[eje].n/2);
		if((datosEje[eje].d-datosEje[eje].i)<ln)
			return false;
	}

	LONG l1=(datosEje[eje].d-(datosEje[eje].c+datosEje[eje].n));
	LONG l2=((datosEje[eje].c-datosEje[eje].n)-datosEje[eje].i);
	if(l1<=l2) {
		datosEje[eje].d=l1;
		datosEje[eje].i=-l1;
	} else {
		datosEje[eje].d=l2;
		datosEje[eje].i=-l2;
	}

	return true;
}


//-------------------------------------------------//
//-------------------------------------------------//


bool CReorden::GuardarDatos(UCHAR* ejes)
{
	struct {
			DIPROPHEADER diph;
			LONG data[3];
	} dipdw;
	dipdw.diph.dwSize       = sizeof(dipdw); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwHow        = DIPH_BYID; 
	dipdw.data[1]=0;

	// Eje x
	dipdw.diph.dwObj=2;
	dipdw.data[0]=datosEje[ejes[0]].i;
	dipdw.data[2]=datosEje[ejes[0]].d;
	if( FAILED(g_pJoystick->SetProperty( DIPROP_CALIBRATION, &dipdw.diph ) ) ) 
		return false;
	// Eje y
	dipdw.diph.dwObj=0x102;
	dipdw.data[0]=datosEje[ejes[1]].i;
	dipdw.data[2]=datosEje[ejes[1]].d;	
	if( FAILED(g_pJoystick->SetProperty( DIPROP_CALIBRATION, &dipdw.diph ) ) ) 
		return false;

	if(nEjes>2)	{
		// Eje z
		dipdw.diph.dwObj=0x202;
		dipdw.data[0]=datosEje[ejes[2]].i;
		dipdw.data[2]=datosEje[ejes[2]].d;	
		if( FAILED(g_pJoystick->SetProperty( DIPROP_CALIBRATION, &dipdw.diph ) ) ) 
			return false;
	}
	if(nEjes>3) {
		// Eje r
		dipdw.diph.dwObj=0x502;
		dipdw.data[0]=datosEje[ejes[3]].i;
		dipdw.data[2]=datosEje[ejes[3]].d;	
		if( FAILED(g_pJoystick->SetProperty( DIPROP_CALIBRATION, &dipdw.diph ) ) ) 
			return false;
	}
	if(nEjes>4) {
		// Rotatorio 1
		dipdw.diph.dwObj=0x602;
		dipdw.data[0]=-127;
		dipdw.data[2]=127;	
		if( FAILED(g_pJoystick->SetProperty( DIPROP_CALIBRATION, &dipdw.diph ) ) ) 
			return false;
		// Rotatorio 2
		dipdw.diph.dwObj=0x702;
		dipdw.data[0]=-127;
		dipdw.data[2]=127;	
		if( FAILED(g_pJoystick->SetProperty( DIPROP_CALIBRATION, &dipdw.diph ) ) ) 
			return false;
	}

	return true;
}