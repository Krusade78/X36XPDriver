// X36NuevoDlg.cpp: Dialogo para crear un mapa nuevo
// Implementación

#include <windows.h>
#include <winioctl.h>
#include <dinput.h>
#include "Calibraje.h"
#include "resource.h"
#include "idioma.h"

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#define SVC_RECALIBRAR 1
#define SVC_DESCALIBRAR 2

#define IOCTL_USR_CALIBRAR	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0105, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_USR_DESCALIBRAR	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0106, METHOD_BUFFERED, FILE_READ_ACCESS)


/*************************************************************
					Diálogo
**************************************************************/
CCalibraje* ccal = NULL;

INT_PTR CALLBACK X36CALIBRAJE_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
		TraducirDlgCalibraje(hWnd);
		return ccal->OnInitDialog(hWnd);
        break;
	case WM_COMMAND:
		switch(wParam)
		{
			case ID6Botonx: ccal->OnBnClickedBx(); break;
			case ID6Botony: ccal->OnBnClickedBy(); break;
			case ID6Botonz: ccal->OnBnClickedBz(); break;
			case ID6Botonr: ccal->OnBnClickedBr(); break;
			case ID6Borrar: ccal->OnBnClickedBorrar(); break;
			case IDCANCEL:
				ccal->OnCancel();
				EndDialog(hWnd,0);
				return TRUE;
			case IDOK:
				if(ccal->OnOK()) {
					EndDialog(hWnd,0);
					return TRUE;
				}
				break;
		}
		break;
	case WM_TIMER:
		ccal->OnTimer();
		break;
	case WM_CLOSE:
		ccal->OnCancel();
		EndDialog(hWnd,0);
		return TRUE;
    }

    return FALSE;
} //*** end X36CALIBRAJE_DlgProc()

//===================== Taduccion ===============================
void TraducirDlgCalibraje(HWND hWnd)
{
	Traduce::Dlg(GetDlgItem(hWnd,IDOK),LG_CPL_C_GUI0);
	Traduce::Dlg(GetDlgItem(hWnd,IDCANCEL),LG_CPL_C_GUI1);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Borrar),LG_CPL_C_GUI2);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Botonx),LG_CPL_C_GUI3);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Botony),LG_CPL_C_GUI3);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Botonz),LG_CPL_C_GUI3);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Botonr),LG_CPL_C_GUI3);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sejex),LG_CPL_C_GUI4);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sejey),LG_CPL_C_GUI5);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sejez),LG_CPL_C_GUI6);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sejer),LG_CPL_C_GUI7);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Snulax),LG_CPL_C_GUI8);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Santivx),LG_CPL_C_GUI9);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sprealx),LG_CPL_C_GUI10);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Spfiltx),LG_CPL_C_GUI11);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slminx),LG_CPL_C_GUI12);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Scentx),LG_CPL_C_GUI13);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slmaxx),LG_CPL_C_GUI14);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Snulay),LG_CPL_C_GUI8);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Santivy),LG_CPL_C_GUI9);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sprealy),LG_CPL_C_GUI10);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Spfilty),LG_CPL_C_GUI11);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slminy),LG_CPL_C_GUI12);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Scenty),LG_CPL_C_GUI13);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slmaxy),LG_CPL_C_GUI14);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Snulaz),LG_CPL_C_GUI8);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Santivz),LG_CPL_C_GUI9);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sprealz),LG_CPL_C_GUI10);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Spfiltz),LG_CPL_C_GUI11);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slminz),LG_CPL_C_GUI12);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Scentz),LG_CPL_C_GUI13);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slmaxz),LG_CPL_C_GUI14);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Snular),LG_CPL_C_GUI8);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Santivr),LG_CPL_C_GUI9);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Sprealr),LG_CPL_C_GUI10);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Spfiltr),LG_CPL_C_GUI11);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slminr),LG_CPL_C_GUI12);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Scentr),LG_CPL_C_GUI13);
	Traduce::Dlg(GetDlgItem(hWnd,ID6Slmaxr),LG_CPL_C_GUI14);
	Traduce::Dlg(hWnd,LG_CPL_C_GUI15);
}
//---------------------------------------------------------------

void IniciarDlgCalibraje(IDirectInputDevice8* g_pd8, HINSTANCE hInst, HWND hWnd)
{
//======================= Descalibrar =============================
	DWORD ret;
	HANDLE driver=CreateFile(
			"\\\\.\\X36HidInterface",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if(driver==INVALID_HANDLE_VALUE) {
		Traduce::Msg(NULL,LG_ERROR1,"CCalibraje::IniciarDlg[0]",MB_ICONERROR);
		return;
	}
	if(!DeviceIoControl(driver,IOCTL_USR_DESCALIBRAR,NULL,0,NULL,0,&ret,NULL)) {
		CloseHandle(driver);
		Traduce::Msg(NULL,LG_ERROR2,"CCalibraje::IniciarDlg[1]",MB_ICONERROR);
		return;
	}
	CloseHandle(driver);
//===========================================================

	ccal = new CCalibraje(g_pd8);
	DialogBox(hInst,MAKEINTRESOURCE(IDD_X36PROP_PAGE2_1),hWnd,X36CALIBRAJE_DlgProc);
	delete ccal; ccal=NULL;
}



// Constructor de la Clase

CCalibraje::CCalibraje(IDirectInputDevice8* pd8)
{
	g_pJoystick = pd8;
}
CCalibraje::~CCalibraje()
{
	if(g_pJoystick!=NULL) { g_pJoystick->Unacquire(); g_pJoystick=NULL;}
}

//-------------------------------------------------//
//-------------------------------------------------//


//--------------> Inicialización

BOOL CCalibraje::OnInitDialog(HWND hWnd)
{
	this->hWnd = hWnd;
	reglaxr.Attach(hWnd,ID6Reglaxr);
	reglaxv.Attach(hWnd,ID6Reglaxv);
	reglayr.Attach(hWnd,ID6Reglayr);
	reglayv.Attach(hWnd,ID6Reglayv);
	reglazr.Attach(hWnd,ID6Reglazr);
	reglazv.Attach(hWnd,ID6Reglazv);
	reglarr.Attach(hWnd,ID6Reglarr);
	reglarv.Attach(hWnd,ID6Reglarv);
	OnBnClickedBorrar();
	LeerRegistro();
	ZeroMemory(jitter,sizeof(jitter));
	EjesPosAnt[0]=200;
	EjesPosAnt[1]=200;
	EjesPosAnt[2]=200;
	EjesPosAnt[3]=200;
	if(IniciarDX()) {
		ActualizarX();
		ActualizarY();
		ActualizarZ();
		ActualizarR();
		SetTimer(hWnd,1,100,NULL);
		return FALSE;
	} else {
		OnCancel();
		return TRUE;
	}
}

//-------------------------------------------------//
//-------------------------------------------------//


/*************************************************
******************** Métodos *********************
**************************************************/


void CCalibraje::LeerRegistro()
{
	HKEY key;
	DWORD tam=sizeof(CALIBRADO);
	LONG res;
	char st[6];
	CALIBRADO cal;

	if(ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado",0,KEY_READ,&key))
		return;

	res=RegQueryValueEx(key,"Eje1",0,NULL,(BYTE*)&cal,&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) {
		ZeroMemory(&cal,sizeof(CALIBRADO));
	} 
		SetDlgItemText(hWnd,ID6Escalax1,_itoa(cal.Margen,st,10));
		SetDlgItemText(hWnd,ID6ZNulax1,_itoa(cal.n,st,10));
		SetDlgItemText(hWnd,ID6ZNulax0,_itoa(cal.i,st,10));
		SetDlgItemText(hWnd,ID6Centrox,_itoa(cal.c,st,10));
		SetDlgItemText(hWnd,ID6ZNulax2,_itoa(cal.d,st,10));

	res=RegQueryValueEx(key,"Eje2",0,NULL,(BYTE*)&cal,&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) {
		ZeroMemory(&cal,sizeof(CALIBRADO));
	}
		SetDlgItemText(hWnd,ID6Escalay1,_itoa(cal.Margen,st,10));
		SetDlgItemText(hWnd,ID6ZNulay1,_itoa(cal.n,st,10));
		SetDlgItemText(hWnd,ID6ZNulay0,_itoa(cal.i,st,10));
		SetDlgItemText(hWnd,ID6Centroy,_itoa(cal.c,st,10));
		SetDlgItemText(hWnd,ID6ZNulay2,_itoa(cal.d,st,10));

	res=RegQueryValueEx(key,"Eje3",0,NULL,(BYTE*)&cal,&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) {
		ZeroMemory(&cal,sizeof(CALIBRADO));
	}
		SetDlgItemText(hWnd,ID6Escalaz1,_itoa(cal.Margen,st,10));
		SetDlgItemText(hWnd,ID6ZNulaz1,_itoa(cal.n,st,10));
		SetDlgItemText(hWnd,ID6ZNulaz0,_itoa(cal.i,st,10));
		SetDlgItemText(hWnd,ID6Centroz,_itoa(cal.c,st,10));
		SetDlgItemText(hWnd,ID6ZNulaz2,_itoa(cal.d,st,10));

	res=RegQueryValueEx(key,"Eje4",0,NULL,(BYTE*)&cal,&tam);
	if(ERROR_SUCCESS!=res || tam!=sizeof(CALIBRADO)) {
		ZeroMemory(&cal,sizeof(CALIBRADO));
	}
		SetDlgItemText(hWnd,ID6Escalar1,_itoa(cal.Margen,st,10));
		SetDlgItemText(hWnd,ID6ZNular1,_itoa(cal.n,st,10));
		SetDlgItemText(hWnd,ID6ZNular0,_itoa(cal.i,st,10));
		SetDlgItemText(hWnd,ID6Centror,_itoa(cal.c,st,10));
		SetDlgItemText(hWnd,ID6ZNular2,_itoa(cal.d,st,10));

	RegCloseKey(key);
}

//-----------------------------------------------------

bool CCalibraje::GuardarRegistro()
{
	HKEY key;
	char num[5]={0,0,0,0,0};
	DWORD dato=0;
	CALIBRADO cal;

	if(ERROR_SUCCESS!=RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\Calibrado",0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dato))
		goto error;

	GetDlgItemText(hWnd,ID6Escalax1,num,5);	dato=atoi(num);
	cal.Margen=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNulax1,num,5);	dato=atoi(num);
	cal.n=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNulax0,num,5);	dato=atoi(num);
	cal.i=dato;
	GetDlgItemText(hWnd,ID6Centrox,num,5);	dato=atoi(num);
	cal.c=dato;
	GetDlgItemText(hWnd,ID6ZNulax2,num,5);	dato=atoi(num);
	cal.d=dato;
	if(ERROR_SUCCESS!=RegSetValueEx(key,"Eje1",0,REG_BINARY,(BYTE*)&cal,sizeof(CALIBRADO))) {
		goto error2;
	}

	GetDlgItemText(hWnd,ID6Escalay1,num,5);	dato=atoi(num);
	cal.Margen=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNulay1,num,5);	dato=atoi(num);
	cal.n=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNulay0,num,5);	dato=atoi(num);
	cal.i=dato;
	GetDlgItemText(hWnd,ID6Centroy,num,5);	dato=atoi(num);
	cal.c=dato;
	GetDlgItemText(hWnd,ID6ZNulay2,num,5);	dato=atoi(num);
	cal.d=dato;
	if(ERROR_SUCCESS!=RegSetValueEx(key,"Eje2",0,REG_BINARY,(BYTE*)&cal,sizeof(CALIBRADO))) {
		goto error2;
	}

	GetDlgItemText(hWnd,ID6Escalaz1,num,5);	dato=atoi(num);
	cal.Margen=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNulaz1,num,5);	dato=atoi(num);
	cal.n=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNulaz0,num,5);	dato=atoi(num);
	cal.i=dato;
	GetDlgItemText(hWnd,ID6Centroz,num,5);	dato=atoi(num);
	cal.c=dato;
	GetDlgItemText(hWnd,ID6ZNulaz2,num,5);	dato=atoi(num);
	cal.d=dato;
	if(ERROR_SUCCESS!=RegSetValueEx(key,"Eje3",0,REG_BINARY,(BYTE*)&cal,sizeof(CALIBRADO))) {
		goto error2;
	}

	GetDlgItemText(hWnd,ID6Escalar1,num,5);	dato=atoi(num);
	cal.Margen=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNular1,num,5);	dato=atoi(num);
	cal.n=(UCHAR)dato;
	GetDlgItemText(hWnd,ID6ZNular0,num,5);	dato=atoi(num);
	cal.i=dato;
	GetDlgItemText(hWnd,ID6Centror,num,5);	dato=atoi(num);
	cal.c=dato;
	GetDlgItemText(hWnd,ID6ZNular2,num,5);	dato=atoi(num);
	cal.d=dato;
	if(ERROR_SUCCESS!=RegSetValueEx(key,"Eje4",0,REG_BINARY,(BYTE*)&cal,sizeof(CALIBRADO))) {
		goto error2;
	}
	RegCloseKey(key);

	return true;

error2:
	RegCloseKey(key);
error:
	Traduce::Msg(hWnd,LG_CPL_ERROR0,"CCalibraje::GuardarRegistro",MB_ICONWARNING);
	return false;
}


/**********************************************************
						DirectX
***********************************************************/

bool CCalibraje::IniciarDX()
{
	HRESULT         hr; 
	DIDEVCAPS devCaps;

	devCaps.dwSize=sizeof(DIDEVCAPS);
	if( FAILED( g_pJoystick->GetCapabilities(&devCaps) ) )
	{
        Traduce::Msg(hWnd,LG_CPL_ERROR1,"CCalibraje::IniciarDX",MB_ICONSTOP);
		return false;
	}
	nEjes=(UCHAR)devCaps.dwAxes;

	return true;
}



//-----------------------------------------------------
//-----------------------------------------------------

void CCalibraje::ReglasVirtuales(ULONG Axis[])
{
	LONG ancho1,ancho2,LAxis;
	CSliderCtrl* reglas[4]={&reglaxv,&reglayv,&reglazv,&reglarv};
	const int dlg[4]={ID6xvval,ID6yvval,ID6zvval,ID6rvval};
	char n[6];

	for(UCHAR Idx=0; Idx<((nEjes==6)?4:nEjes); Idx++)
	{
		// Antivibraciones
		if (Axis[Idx]>jitter[Idx].PosElegida)
		{
		    if (Axis[Idx]>(jitter[Idx].PosElegida+Limite[Idx].Margen)) {
				jitter[Idx].PosElegida=Axis[Idx];
			} else {
				if(jitter[Idx].PosRepetida<3) {
					Axis[Idx]=jitter[Idx].PosElegida;
				} else {
					jitter[Idx].PosRepetida=0;
					jitter[Idx].PosElegida=Axis[Idx];
				}
			}
		} else {
		    if (Axis[Idx]<(jitter[Idx].PosElegida-Limite[Idx].Margen)) {
				jitter[Idx].PosElegida=Axis[Idx];
			} else {
				if(jitter[Idx].PosRepetida<3) {
					Axis[Idx]=jitter[Idx].PosElegida;
				} else {
					jitter[Idx].PosRepetida=0;
					jitter[Idx].PosElegida=Axis[Idx];
				}
			}
		}
		//-------------------
		LAxis=Axis[Idx];
		// Calibrado
		if((Axis[Idx]>=(Limite[Idx].c-Limite[Idx].n)) && (Axis[Idx]<=(Limite[Idx].c+Limite[Idx].n)))
		{
			LAxis=0;
		} else {
            if(Axis[Idx]<Limite[Idx].i)
			{
				LAxis=Axis[Idx]=Limite[Idx].i;
			} else {
				if(Axis[Idx]>Limite[Idx].d)
				{
					LAxis=Axis[Idx]=Limite[Idx].d;
				}
			}

            ancho1=(Limite[Idx].c-Limite[Idx].n)-Limite[Idx].i;
            ancho2=Limite[Idx].d-(Limite[Idx].c+Limite[Idx].n);
			if(Axis[Idx]<Limite[Idx].c)
			{
				LAxis=0-((Limite[Idx].c-Limite[Idx].n)-LAxis);
				if(ancho1>ancho2)
				{
					LAxis=(LAxis*ancho2)/ancho1;
				}
			} else {
				LAxis=LAxis-(Limite[Idx].c+Limite[Idx].n);
				if(ancho2>ancho1)
				{
					LAxis=(LAxis*ancho1)/ancho2;
				}
			}
		}

		reglas[Idx]->SetPos(LAxis);
		SetDlgItemText(hWnd,dlg[Idx],_itoa(LAxis,n,10));
	}
}

//-----------------------------------------------------

bool CCalibraje::Comprobar(
							  LPCSTR eje,
							  UCHAR e,
							  int izq,
							  int cen,
							  int der,
							  int nul,
							  int vib,
							  int rmi,
							  int rma,
							  CSliderCtrl* regla)
{
	struct {LONG i; LONG c; LONG d;}Limite_aux;
	INT16 ZonaNula_aux;
	INT16 Antiv_aux;

	//Leer valores
	char s[6];
	GetDlgItemText(hWnd,izq,s,6);
	Limite_aux.i=atoi(s);
	GetDlgItemText(hWnd,der,s,6);
	Limite_aux.d=atoi(s);
	GetDlgItemText(hWnd,cen,s,6);
	Limite_aux.c=atoi(s);
	GetDlgItemText(hWnd,nul,s,6);
	ZonaNula_aux=(INT16)atoi(s);
	GetDlgItemText(hWnd,vib,s,6);
	Antiv_aux=(INT16)atoi(s);

	//Comprobar valores
	if(Antiv_aux<0 || Antiv_aux>255)
	{
		Traduce::Msg(hWnd,LG_CPL_ERROR2,eje,MB_ICONWARNING);
		return false;
	}
	if((Limite_aux.d<0 || Limite_aux.d>20000) || (Limite_aux.i<0 || Limite_aux.d>20000) || (Limite_aux.c<0 || Limite_aux.c>20000))
	{
		Traduce::Msg(hWnd,LG_CPL_ERROR3,eje,MB_ICONWARNING);
		return false;
	}
	if(Limite_aux.d<Limite_aux.i)
	{
		Traduce::Msg(hWnd,LG_CPL_ERROR4,eje,MB_ICONWARNING);
		return false;
	}
	if((ZonaNula_aux/2)>(Limite_aux.d-Limite_aux.i))
	{
		Traduce::Msg(hWnd,LG_CPL_ERROR5,eje,MB_ICONWARNING);
		return false;
	}

	LONG l1=(Limite_aux.d-(Limite_aux.c+ZonaNula_aux));
	LONG l2=((Limite_aux.c-ZonaNula_aux)-Limite_aux.i);
	if(l1<=l2) {
		regla->SetRange(-l1,l1);
		SetDlgItemText(hWnd,rmi,_itoa(-l1,s,10));
		SetDlgItemText(hWnd,rma,_itoa(l1,s,10));
	} else {
		regla->SetRange(-l2,l2);
		SetDlgItemText(hWnd,rmi,_itoa(-l2,s,10));
		SetDlgItemText(hWnd,rma,_itoa(l2,s,10));
	}
	Limite[e].i=Limite_aux.i;
	Limite[e].c=Limite_aux.c;
	Limite[e].d=Limite_aux.d;
	Limite[e].n=(UCHAR)ZonaNula_aux;
	Limite[e].Margen=(UCHAR)Antiv_aux;

	return true;
}

bool CCalibraje::ActualizarX()
{
	char txt[1024];
	strcpy(txt,Traduce::Txt(LG_CPL_TXT0));
	return Comprobar(txt,0,
						ID6ZNulax0,
						ID6Centrox,
						ID6ZNulax2,
						ID6ZNulax1,
						ID6Escalax1,
						ID6xvmin,
						ID6xvmax,
						&reglaxv);
}
bool CCalibraje::ActualizarY()
{
	char txt[1024];
	strcpy(txt,Traduce::Txt(LG_CPL_TXT1));
	return Comprobar(txt,1,
						ID6ZNulay0,
						ID6Centroy,
						ID6ZNulay2,
						ID6ZNulay1,
						ID6Escalay1,
						ID6yvmin,
						ID6yvmax,
						&reglayv);
}
bool CCalibraje::ActualizarZ()
{
	char txt[1024];
	strcpy(txt,Traduce::Txt(LG_CPL_TXT2));
	return Comprobar(txt,2,
						ID6ZNulaz0,
						ID6Centroz,
						ID6ZNulaz2,
						ID6ZNulaz1,
						ID6Escalaz1,
						ID6zvmin,
						ID6zvmax,
						&reglazv);
}
bool CCalibraje::ActualizarR()
{
	char txt[1024];
	strcpy(txt,Traduce::Txt(LG_CPL_TXT3));
	return Comprobar(txt,3,
						ID6ZNular0,
						ID6Centror,
						ID6ZNular2,
						ID6ZNular1,
						ID6Escalar1,
						ID6rvmin,
						ID6rvmax,
						&reglarv);
}

void CCalibraje::EnviarCalibrar()
{
	typedef bool (*FUNC)(void);
	HMODULE dll=LoadLibrary("x36_svc.dll");
	if(dll==NULL) {
		Traduce::Msg(NULL,LG_CPL_ERROR6,"CCalibraje::EnviarCalibrar[1]",MB_ICONERROR);
		goto labcal;
	}
	FUNC Recalibrar=(FUNC)GetProcAddress(dll,"Recalibrate");
	if(Comprobar!=NULL) {
		if(!Recalibrar()) {
			Traduce::Msg(NULL,LG_CPL_ERROR7,"CCalibraje::EnviarCalibrar[2]",MB_ICONERROR);
		}
	}
	FreeLibrary(dll);
labcal:
	//======================= Calibrar =============================
	DWORD ret;
	HANDLE driver=CreateFile(
			"\\\\.\\X36HidInterface",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if(driver==INVALID_HANDLE_VALUE) {
		Traduce::Msg(NULL,LG_ERROR1,"CCalibraje::EnviarCalibrar[3]",MB_ICONERROR);
		return;
	}
	if(!DeviceIoControl(driver,IOCTL_USR_CALIBRAR,NULL,0,NULL,0,&ret,NULL)) {
		CloseHandle(driver);
		Traduce::Msg(NULL,LG_ERROR2,"CCalibraje::EnviarCalibrar[4]",MB_ICONERROR);
		return;
	}
	CloseHandle(driver);
//===========================================================

}
//-------------------------------------------------//
//-------------------------------------------------//


/*************************************************
******************* Eventos **********************
**************************************************/

bool CCalibraje::OnOK()
{
	if(!ActualizarX())  {
		Traduce::Msg(hWnd,LG_CPL_ERROR8,"CCalibraje::OnOK[1]",MB_ICONWARNING);
		return false;
	}
	if(!ActualizarY())  {
		Traduce::Msg(hWnd,LG_CPL_ERROR9,"CCalibraje::OnOK[2]",MB_ICONWARNING);
		return false;
	}
	if(nEjes>2) {
		if(!ActualizarZ()){
			Traduce::Msg(hWnd,LG_CPL_ERROR10,"CCalibraje::OnOK[3]",MB_ICONWARNING);
			return false;
		}
	}
	if(nEjes>3) {
		if(!ActualizarR()){
			Traduce::Msg(hWnd,LG_CPL_ERROR11,"CCalibraje::OnOK[4]",MB_ICONWARNING);
			return false;
		}
	}

	if(!GuardarRegistro()) {
		return false;
	}

	KillTimer(hWnd,1);
	EnviarCalibrar();
	return true;
}

void CCalibraje::OnCancel()
{
	KillTimer(hWnd,1);
	EnviarCalibrar();
}

void CCalibraje::OnBnClickedBorrar()
{
	reglaxr.SetRange(65535,0);
	reglayr.SetRange(65535,0);
	reglazr.SetRange(65535,0);
	reglarr.SetRange(65535,0);
	SetDlgItemText(hWnd,ID6xrmin,"0");
	SetDlgItemText(hWnd,ID6yrmin,"0");
	SetDlgItemText(hWnd,ID6zrmin,"0");
	SetDlgItemText(hWnd,ID6rrmin,"0");
	SetDlgItemText(hWnd,ID6xrmax,"0");
	SetDlgItemText(hWnd,ID6yrmax,"0");
	SetDlgItemText(hWnd,ID6zrmax,"0");
	SetDlgItemText(hWnd,ID6rrmax,"0");
}

//-------------------------------------------------//

void CCalibraje::OnTimer()
{
	if( FAILED( g_pJoystick->Poll() ) )
	{
		g_pJoystick->Acquire();
	} else {
		DIJOYSTATE2 js;
		if(!FAILED(g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )
		{
			char n[6];

			// Eje x
			if(js.lX>reglaxr.GetRangeMax())
			{
				reglaxr.SetRangeMax(js.lX);
				SetDlgItemText(hWnd,ID6xrmax,_itoa(js.lX,n,10));
			}
			if(js.lX<reglaxr.GetRangeMin())
			{
				reglaxr.SetRangeMin(js.lX);
				SetDlgItemText(hWnd,ID6xrmin,_itoa(js.lX,n,10));
			}
			SetDlgItemText(hWnd,ID6xrval,_itoa(js.lX,n,10));
			reglaxr.SetPos(js.lX);

			// Eje y
			if(js.lY>reglayr.GetRangeMax())
			{
				reglayr.SetRangeMax(js.lY);
				SetDlgItemText(hWnd,ID6yrmax,_itoa(js.lY,n,10));
			}
			if(js.lY<reglayr.GetRangeMin())
			{
				reglayr.SetRangeMin(js.lY);
				SetDlgItemText(hWnd,ID6yrmin,_itoa(js.lY,n,10));
			}
			SetDlgItemText(hWnd,ID6yrval,_itoa(js.lY,n,10));
			reglayr.SetPos(js.lY);

			// Eje z
			if(js.lZ>reglazr.GetRangeMax())
			{
				reglazr.SetRangeMax(js.lZ);
				SetDlgItemText(hWnd,ID6zrmax,_itoa(js.lZ,n,10));
			}
			if(js.lZ<reglazr.GetRangeMin())
			{
				reglazr.SetRangeMin(js.lZ);
				SetDlgItemText(hWnd,ID6zrmin,_itoa(js.lZ,n,10));
			}
			SetDlgItemText(hWnd,ID6zrval,_itoa(js.lZ,n,10));
			reglazr.SetPos(js.lZ);

			// Eje r
			if(js.lRz>reglarr.GetRangeMax())
			{
				reglarr.SetRangeMax(js.lRz);
				SetDlgItemText(hWnd,ID6rrmax,_itoa(js.lRz,n,10));
			}
			if(js.lRz<reglarr.GetRangeMin())
			{
				reglarr.SetRangeMin(js.lRz);
				SetDlgItemText(hWnd,ID6rrmin,_itoa(js.lRz,n,10));
			}
			SetDlgItemText(hWnd,ID6rrval,_itoa(js.lRz,n,10));
			reglarr.SetPos(js.lRz);

			// Virtual
			ULONG eje[4]={js.lX,js.lY,js.lZ,js.lRz};
			for(char i=0;i<4;i++) {
				if(EjesPosAnt[i]==eje[i]) {
					jitter[i].PosRepetida++;
				} else {
					jitter[i].PosRepetida=0;
				}
				EjesPosAnt[i]=eje[i];
			}
			ReglasVirtuales(eje);
		}
	}
}

//-------------------------------------------------//

void CCalibraje::OnBnClickedBx()
{
	ActualizarX();
}

void CCalibraje::OnBnClickedBy()
{
	ActualizarY();
}

void CCalibraje::OnBnClickedBz()
{
	ActualizarZ();
}

void CCalibraje::OnBnClickedBr()
{
	ActualizarR();
}
