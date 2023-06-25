#include <windows.h>
#include "dinput.h"
#include "dinputd.h"
#include "raton.h"
#include "dinfo.h"
#include "resource.h"
#include "idioma.h"

#define Texto(id,texto) SendMessage(GetDlgItem(hWnd,id),WM_SETTEXT,0,(LPARAM)texto);
#define Color(id,color) SetWindowLongPtr(GetDlgItem(hWnd,id),-21,color);
#define Boton(id,estado) SetWindowLongPtr(GetDlgItem(hWnd,id),-21,estado);

CDInfo::CDInfo(	HWND hWnd,IDirectInputDevice8* pd8, IDirectInputJoyConfig8* pjc, CRaton* raton)
{
	this->pd8 = pd8;
	this->pjc = pjc;
	this->hWnd = hWnd;
	this->raton = raton;
	CargarParteEstatica();
}

CDInfo::~CDInfo(){}

void CDInfo::CargarParteEstatica()
{
	DIJOYCONFIG auxjc;
	DIJOYTYPEINFO auxjti;
	UINT idx=0;

	// Coger nombre

	auxjc.dwSize=sizeof(DIJOYCONFIG);
	auxjti.dwSize=sizeof(DIJOYTYPEINFO);
	while(pjc->GetConfig(idx,&auxjc,DIJC_REGHWCONFIGTYPE)==DI_OK)
	{
		if(wcscmp(auxjc.wszType,L"VID_06A3&PID_3635")==0 || wcscmp(auxjc.wszType,L"VID_06A3&PID_3600")==0)
		{
			pjc->GetTypeInfo(auxjc.wszType,&auxjti,DITC_DISPLAYNAME);
			wcscpy(auxjc.wszCallout,L" ");
			wcscat(auxjc.wszCallout,auxjti.wszDisplayName);
			SendMessageW(GetDlgItem(hWnd,IDS_JOY),WM_SETTEXT,0,(LPARAM)auxjc.wszCallout);
			if(GetWindowTextLength(GetDlgItem(hWnd,IDS_JOY))>14) ActivarX35();
			break;
		}
		idx++;
	}

	// Coger otros datos del servicio

	Texto(IDS_CAL,"");
	Texto(IDS_SER," ...");

}

void CDInfo::ActivarX35()
{
	EnableWindow(GetDlgItem(hWnd,IDS_SZ),TRUE);
	EnableWindow(GetDlgItem(hWnd,IDS_SR),TRUE);
	EnableWindow(GetDlgItem(hWnd,IDS_SR1),TRUE);
	EnableWindow(GetDlgItem(hWnd,IDS_SR2),TRUE);
}

void CDInfo::Actualizar()
{
	InterrogarServicio();

	if(pd8->Acquire()!=DI_OK) return;

	DIJOYSTATE2 estado;
	char n[10];
	pd8->Poll();
	pd8->GetDeviceState(sizeof(DIJOYSTATE2),&estado);

	// X36-F

	Texto(IDS_X,_itoa(estado.lX,n,10));
	Texto(IDS_Y,_itoa(estado.lY,n,10));
	Texto(IDS_P1,PuntosC(estado.rgdwPOV[0],n));
	Texto(IDS_P2,PuntosC(estado.rgdwPOV[1],n));
	for(char i=0;i<22;i++) {
		Boton(i+1,(estado.rgbButtons[i]&128)/128);
		Texto(i+1,_itoa(i+1,n,10));
	}

	// X35-T

	if(GetWindowTextLength(GetDlgItem(hWnd,IDS_JOY))>14) {
		Texto(IDS_Z,_itoa(estado.lZ,n,10));
		Texto(IDS_R,_itoa(estado.lRz,n,10));
		Texto(IDS_R1,_itoa(estado.rglSlider[0],n,10));
		Texto(IDS_R2,_itoa(estado.rglSlider[1],n,10));
		Texto(IDS_P3,PuntosC(estado.rgdwPOV[2],n));
		Texto(IDS_P4,PuntosC(estado.rgdwPOV[3],n));
		for(char i=22;i<36;i++) {
			Boton(i+1,(estado.rgbButtons[i]&128)/128);
			Texto(i+1,_itoa(i+1,n,10));
		}
	}
	pd8->Unacquire();
}

char* CDInfo::PuntosC(DWORD gr, char *st)
{
	switch(gr)
	{
		case -1:
			strcpy(st,"");
			break;
		case 0:
			strcpy(st,Traduce::Txt(LG_CPL_TXT4));
			break;
		case 4500:
			strcpy(st,Traduce::Txt(LG_CPL_TXT5));
			break;
		case 9000:
			strcpy(st,Traduce::Txt(LG_CPL_TXT6));
			break;
		case 13500:
			strcpy(st,Traduce::Txt(LG_CPL_TXT7));
			break;
		case 18000:
			strcpy(st,Traduce::Txt(LG_CPL_TXT8));
			break;
		case 22500:
			strcpy(st,Traduce::Txt(LG_CPL_TXT9));
			break;
		case 27000:
			strcpy(st,Traduce::Txt(LG_CPL_TXT10));
			break;
		case 31500:
			strcpy(st,Traduce::Txt(LG_CPL_TXT11));
			break;
	}
	return st;
}

void CDInfo::InterrogarServicio()
{
	// Calibrado
	{
		HKEY key;
		LONG res;

		if(ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado",0,KEY_READ,&key)) {
			Color(IDS_CAL,RGB(200,140,0));
			Texto(IDS_CAL,Traduce::Txt(LG_CPL_TXT16));
			goto serv;
		}

		res=RegQueryValueEx(key,"Eje1",0,NULL,NULL,0);
		if(ERROR_SUCCESS!=res) {
			Color(IDS_CAL,RGB(200,140,0));
			Texto(IDS_CAL,Traduce::Txt(LG_CPL_TXT17));
			RegCloseKey(key);
			goto serv;
		} 
		res=RegQueryValueEx(key,"Eje2",0,NULL,NULL,0);
		if(ERROR_SUCCESS!=res) {
			Color(IDS_CAL,RGB(200,140,0));
			Texto(IDS_CAL,Traduce::Txt(LG_CPL_TXT18));
			RegCloseKey(key);
			goto serv;
		}
		res=RegQueryValueEx(key,"Eje3",0,NULL,NULL,0);
		if(ERROR_SUCCESS!=res) {
			Color(IDS_CAL,RGB(200,140,0));
			Texto(IDS_CAL,Traduce::Txt(LG_CPL_TXT19));
			RegCloseKey(key);
			goto serv;
		}
		res=RegQueryValueEx(key,"Eje4",0,NULL,NULL,0);
		if(ERROR_SUCCESS!=res) {
			Color(IDS_CAL,RGB(200,140,0));
			Texto(IDS_CAL,Traduce::Txt(LG_CPL_TXT20));
			RegCloseKey(key);
			goto serv;
		}

		RegCloseKey(key);

		Color(IDS_CAL,RGB(0,160,0));
		Texto(IDS_CAL,Traduce::Txt(LG_CPL_TXT21));
	}
serv:
	// Servicio
	SC_HANDLE scHandle=OpenSCManager(NULL,NULL,GENERIC_READ);
	if (NULL == scHandle) {
		Color(IDS_SER,RGB(200,0,0));
		Texto(IDS_SER,Traduce::Txt(LG_CPL_TXT12));
	} else {
		SC_HANDLE serv=OpenService(scHandle,"x36HidUserService",SERVICE_QUERY_STATUS );
		if (NULL == scHandle) {
			Color(IDS_SER,RGB(200,0,0));
			Texto(IDS_SER,Traduce::Txt(LG_CPL_TXT13));
		} else {
			DWORD ret;
			SERVICE_STATUS_PROCESS buff;
			if(!QueryServiceStatusEx(serv,SC_STATUS_PROCESS_INFO,(BYTE*)&buff,sizeof(SERVICE_STATUS_PROCESS),&ret))
			{
				Color(IDS_SER,RGB(200,140,0));
				Texto(IDS_SER,Traduce::Txt(LG_CPL_TXT13));
			} else {
				if(buff.dwCurrentState==SERVICE_RUNNING) {
					Color(IDS_SER,RGB(0,160,0));
					Texto(IDS_SER,Traduce::Txt(LG_CPL_TXT14));
				} else {
					Color(IDS_SER,RGB(200,140,0));
					Texto(IDS_SER,Traduce::Txt(LG_CPL_TXT15));
				}
			}
			CloseHandle(serv);
		}
		CloseHandle(scHandle);
	}

	raton->Cambiar();

}