// Instalar.cpp: Instalación del X36 Driver
//


#include <windows.h>
#include <setupapi.h>
#include "newdev.h"
#include "instalar.h"
#include "desinstalar.h"
#include "joyConfig.h"
#include "resource.h"
#include "idioma.h"

//#pragma comment(lib,"setupapi.lib")
//#pragma comment(lib,"newdev.lib")
//#pragma comment(lib,"dxguid.lib")
//#pragma comment(lib,"dinput8.lib")

CInstalar::CInstalar(HWND hWnd,HINSTANCE hInst)//,bool manual)
{
	this->hWnd = hWnd;
	this->hInst = hInst;
	//this->manual = manual;
	fase = 0;
	s_interface = -1;
	s_teclado = -1;
	s_joystick = -1;
//	s_rudder = 0;
	fin = false;
}

CInstalar::~CInstalar(void){}

bool CInstalar::Fin() {return fin;}

void CInstalar::OnCancel()
{
	//if(fase==2) {
	//	CDesinstalar des(hWnd,hInst);
	//	des.VHID();
	//}
}

bool CInstalar::OnOk(LRESULT sel)
{
	ShowWindow(hWnd,SW_HIDE);
	switch(fase)
	{
		case 0:
		{
			fase=1;
			s_teclado = sel;
			char ret=PrepararDlgInterfaces();
			if(ret==0) return false;
			if(ret>1) {
				ShowWindow(hWnd,SW_SHOW);
			} else {
				s_interface = 0;
				fase=2;
				if(!PrepararDlgJoysticks()) return false;
			}

			break;
		}
		case 1:
		{
//			char tipo[256];
			fase=2;
			s_interface = sel;
			if(!PrepararDlgJoysticks()) return false;
/*			if(BST_CHECKED==SendMessage(GetDlgItem(hWnd,IDrudder),BM_GETCHECK,0,0)) {
				s_rudder=1;
			} else {
				s_rudder=0;
			}
			SendMessage(GetDlgItem(hWnd,IDlista),LB_GETTEXT,(WPARAM)sel,(LPARAM)tipo);
			if(_stricmp(tipo,"Saitek X36F")==0 || _stricmp(tipo,"Saitek X36F + X35T")==0) {
				char ret=PrepararDlgTeclados();
				if(ret==0) return false;
				if(ret>1) {
					ShowWindow(hWnd,SW_SHOW);
				} else {
					s_teclado = 0;
					fin=true;
					return InstalarArchivos();
				}
*///			} else {
//				fin=true;
//				return InstalarArchivos();
//			}
			break;
		}
		case 2:
		{
			s_joystick = sel;
			fin=true;
			return InstalarArchivos();
			break;
		}
		case 10:
		{
			s_teclado = sel;
			fin=true;
			return InstalarArchivos();
			break;
		}
	}

	return true;
}


bool CInstalar::Iniciar(void)
{
	// Traducir
	Traduce::Dlg(hWnd,LG_INSTCAP);		//común
	Traduce::Dlg(GetDlgItem(hWnd,IDtxd),LG_INST_GB); //común

	char sel;
//	if(!manual) {
		sel=PrepararDlgTeclados();
		if(sel==0) return false;
		if(sel>1) {
			ShowWindow(hWnd,SW_SHOW);
		} else {
			fase=1;
			s_teclado=0;

			sel=PrepararDlgInterfaces();
			if(sel==0) return false;
			if(sel>1) {
				ShowWindow(hWnd,SW_SHOW);
			} else {
				s_interface=0;
				fase=2;
				if(!PrepararDlgJoysticks()) return false;
			}
		}
/*	} else {
		sel=PrepararDlgTeclados();
		if(sel==0) return false;
		if(sel>1) {
			fase=10;
			ShowWindow(hWnd,SW_SHOW);
		} else {
			s_teclado=0;
			fin=true;
			return InstalarArchivos();
		}
	}
*/
	return true;
}


// Instalación

bool CInstalar::InstalarArchivos()
{
	CDesinstalar des(hWnd,hInst);
	if(!FiltroTeclado()) {
		return false;
	}
	if(!VHID()) {
		des.FiltroTeclado();
		des.VHID();
		des.Base();
		return false;
	}

//	if(!manual) {
		if(!Joystick()) {
			des.FiltroTeclado();
			des.Joystick();
			des.VHID();
			des.Base();
			return false;
		}
//	}

	return true;
}


bool CInstalar::VHID()
{
	GUID guidHid={0x745a17a0,0x74d3,0x11d0,{0xb6,0xfe,0x00,0xa0,0xc9,0x0f,0x57,0xda}};
	const char hardwareId[]="VHID\\X36VirtualMinidriver\0";
	HDEVINFO di=SetupDiCreateDeviceInfoList(&guidHid,NULL);
	if(di==INVALID_HANDLE_VALUE)
	{
		Error("VHID[0]");
		return false;
	}

	SP_DEVINFO_DATA dev;
	dev.cbSize=sizeof(SP_DEVINFO_DATA);
	if(!SetupDiCreateDeviceInfo(di,"VHID",&guidHid,NULL,NULL,DICD_GENERATE_ID,&dev))
	{
		Error("VHID[1]");
		SetupDiDestroyDeviceInfoList(di);
		return false;
	}
	if(!SetupDiSetDeviceRegistryProperty(di,&dev,SPDRP_HARDWAREID,(BYTE*)hardwareId,27)) {
		Error("VHID[2]");
		SetupDiDestroyDeviceInfoList(di);
		return false;
	}
	if (!SetupDiCallClassInstaller(DIF_REGISTERDEVICE,di,&dev))
    {
		Error("VHID[3]");
		SetupDiDestroyDeviceInfoList(di);
		return false;
	}

	BOOL boot;
	if(!UpdateDriverForPlugAndPlayDevices(NULL,hardwareId,"x36_hid.inf",INSTALLFLAG_FORCE,&boot))
	{
		Error("VHID[4]");
		SetupDiDestroyDeviceInfoList(di);
		return false;
	}

	SetupDiDestroyDeviceInfoList(di);

	return true;
}

bool CInstalar::FiltroTeclado()
{
	CONST GUID kbdc={0x4D36E96B,0xe325,0x11CE,{0xbF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18}};

	HDEVINFO di=SetupDiGetClassDevs(&kbdc,NULL,NULL,DIGCF_PRESENT);
	if(di==INVALID_HANDLE_VALUE)
	{
		Error("FiltroTeclado[0]");
		return false;
	}

	SP_DEVINFO_DATA dev;
	dev.cbSize=sizeof(SP_DEVINFO_DATA);
	if(SetupDiEnumDeviceInfo(di,(DWORD)s_teclado,&dev)) {
		if(!SetupDiSetDeviceRegistryProperty(di,&dev,SPDRP_UPPERFILTERS,(BYTE*)"x36KbFilter\0",13)) {
			Error("FiltroTeclado[1]");
			SetupDiDestroyDeviceInfoList(di);
			return false;
		}
	} else {
		Error("FiltroTeclado[2]");
		SetupDiDestroyDeviceInfoList(di);
		return false;
	}

	SetupDiDestroyDeviceInfoList(di);

	return true;
}

bool CInstalar::Joystick(/*DWORD* idJoy*/)
{
	CjoyConfig jc;
	return jc.Instalar((DWORD)s_interface,(DWORD)s_joystick);
}


// Prepara dialógos

char CInstalar::PrepararDlgTeclados()
{
	Traduce::Dlg(GetDlgItem(hWnd,IDtxt),LG_INST0);
	SendMessage(GetDlgItem(hWnd,IDlista),LB_RESETCONTENT,0,0);
	ShowWindow(GetDlgItem(hWnd,IDrudder),SW_HIDE);

	CONST GUID kbdc={0x4D36E96B,0xe325,0x11CE,{0xbF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18}};
	SP_DEVINFO_DATA dev;
	DWORD idx=0;

	HDEVINFO di=SetupDiGetClassDevs(&kbdc,NULL,NULL,DIGCF_PRESENT);
	if(di==INVALID_HANDLE_VALUE)
	{
		Error("PrepararDlgTeclados[0]");
		return 0;
	}
			
	dev.cbSize=sizeof(SP_DEVINFO_DATA);
	while(SetupDiEnumDeviceInfo(di,idx,&dev)) {
		idx++;
		DWORD tam=0;
		BYTE* desc=NULL;
		SetupDiGetDeviceRegistryProperty(di,&dev,SPDRP_DEVICEDESC,NULL,NULL,0,&tam);
		desc=new BYTE[tam];
		if(!SetupDiGetDeviceRegistryProperty(di,&dev,SPDRP_DEVICEDESC,NULL,desc,tam,NULL))
		{
			Error("PrepararDlgTeclados[1]");
			delete []desc; desc = NULL;
			SetupDiDestroyDeviceInfoList(di);
			return 0;
		}

		SendMessage(GetDlgItem(hWnd,IDlista),LB_ADDSTRING,0,(LPARAM)desc);

		delete []desc; desc = NULL;
	}
	SetupDiDestroyDeviceInfoList(di);

	return (char)idx;
}


char CInstalar::PrepararDlgInterfaces()
{
	Traduce::Dlg(GetDlgItem(hWnd,IDtxt),LG_INST2);
	SendMessage(GetDlgItem(hWnd,IDlista),LB_RESETCONTENT,0,0);

	DWORD i=0;
	CjoyConfig jc;
	if(!jc.EnumerarGameports(GetDlgItem(hWnd,IDlista),&i)) {
		return 0;
	}

	if(i==0) Traduce::Msg(NULL,LG_GAMEPORT,LG_ADVERT,MB_ICONWARNING);

	return (char)i;
}

bool CInstalar::PrepararDlgJoysticks()
{
	Traduce::Dlg(GetDlgItem(hWnd,IDtxt),LG_INST1);
//	TraduceDlg(GetDlgItem(hWnd,IDrudder),LG_RUDDER);
//	ShowWindow(GetDlgItem(hWnd,IDrudder),SW_SHOW);
	SendMessage(GetDlgItem(hWnd,IDlista),LB_RESETCONTENT,0,0);

	CjoyConfig jc;
	if(!jc.EnumerarJoysticks(GetDlgItem(hWnd,IDlista))) {
		return false;
	}

	ShowWindow(hWnd,SW_SHOW);

	return true;
}


// Errores

void CInstalar::Error(char* tit)
{
	LPCTSTR msg;
	FormatMessage( 
	    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &msg,
		0, NULL );
	MessageBox(NULL,msg,tit,MB_ICONERROR);
}
void CInstalar::Error(char* tit, HRESULT res)
{
	LPCTSTR msg;
	FormatMessage( 
	    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		res,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &msg,
		0, NULL );
	MessageBox(NULL,msg,tit,MB_ICONERROR);
}
