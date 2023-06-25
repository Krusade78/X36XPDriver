#include <windows.h>
#include <winioctl.h>
#include <stdlib.h>
#include "raton.h"
#include "resource.h"
#include "idioma.h"

#define IOCTL_USR_TICKRATON	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0107, METHOD_BUFFERED, FILE_READ_ACCESS)


#define Enable(id,b) EnableWindow(GetDlgItem(dlg2,id),b);

CRaton::CRaton()
{
	dlg1=NULL;
	dlg2=NULL;
	sref=-1;
}

void CRaton::IniciarDlg1(HWND hWnd)
{
	dlg1=hWnd;
}

void CRaton::IniciarDlg2(HWND hWnd)
{
	dlg2=hWnd;
	slRefresco.Attach(dlg2,IDC_SensiDes);
	if(sref!=-1) {
		slRefresco.SetRange(1,100);
		ActualizarDlg2();
	}
}

void CRaton::Cambiar()
{
	HKEY key;
	DWORD ret=1;
	UCHAR buf;
	if(ERROR_SUCCESS==RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado",0,KEY_READ,&key))
	{
		if(ERROR_SUCCESS!=RegQueryValueEx(key,"TickRaton",0,NULL,(BYTE*)&buf,&ret))
		{
			buf=70;
		}
		RegCloseKey(key);
	}

	if(buf!=sref) {
		sref=buf;
		ActualizarDlg1();
		ActualizarDlg2();
	}
}

void CRaton::ActualizarDlg1()
{
	char n[5]; n[0]=' ';
	_itoa(sref,&n[1],10);
	SetDlgItemText(dlg1,IDS_REF,n);
}

void CRaton::ActualizarDlg2()
{
	if(dlg2!=NULL)
	{
		char n[4];
		slRefresco.SetPos(sref);
		SetDlgItemText(dlg2,IDC_SensiEd,_itoa(sref,n,10));
	}
}

void CRaton::OnCambiar()
{
	HKEY key;
	DWORD ret;
	BYTE pausa=(UCHAR)slRefresco.GetPos();
	if(ERROR_SUCCESS==RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado",0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&ret))
	{
		if(ERROR_SUCCESS!=RegSetValueEx(key,"TickRaton",0,REG_BINARY,&pausa,1))
		{
			Traduce::Msg(dlg2,LG_CPL_ERROR0,"CRaton::OnCambiar[0]",MB_ICONWARNING);
			RegCloseKey(key);
			return;
		}
		RegCloseKey(key);
	}
//======================= LLamada minidriver =============================
	HANDLE driver=CreateFile(
			"\\\\.\\X36HidInterface",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if(driver==INVALID_HANDLE_VALUE) {
		Traduce::Msg(NULL,LG_ERROR1,"CRaton::OnCambiar[1]",MB_ICONERROR);
		return;
	}
	if(!DeviceIoControl(driver,IOCTL_USR_TICKRATON,NULL,0,NULL,0,&ret,NULL)) {
		CloseHandle(driver);
		Traduce::Msg(NULL,LG_ERROR2,"CRaton::OnCambiar[2]",MB_ICONERROR);
		return;
	}
	CloseHandle(driver);
//===========================================================

	Traduce::Msg(dlg2,LG_CPL_OK0,"OK",MB_ICONINFORMATION);
}

void CRaton::OnScroll()
{
	char n[4];
	SetDlgItemText(dlg2,IDC_SensiEd,_itoa(slRefresco.GetPos(),n,10));
}