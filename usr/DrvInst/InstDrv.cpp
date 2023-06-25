// InstDrv.cpp: Instalación del X36 Driver
//

#include <windows.h>
#include <setupapi.h>
#include "desinstalar.h"
#include "instalar.h"
#include "resource.h"
#include "idioma.h"


INT_PTR CALLBACK InstDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK UnInstDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
void Reiniciar();
CInstalar* inst = NULL;
CDesinstalar* uninst = NULL;
HINSTANCE hInst;
//bool manual=false;

int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	INT_PTR res=0;
	hInst=hInstance;
	if(lstrcmpi(lpCmdLine,"/i")==0 ) {
		res=DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,InstDlgProc);
//		if(res==0) Reiniciar();
	}/* else if(lstrcmpi(lpCmdLine,"/im")==0 ) {
//		manual=true;
		res=DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,InstDlgProc);
	}*/ else {
		res=DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,UnInstDlgProc);
//		if(res==IDOK) Reiniciar();
	}
	if(inst != NULL) {delete inst; inst = NULL;}
	if(uninst != NULL) {delete uninst; uninst = NULL;}

	return (int)res;
}

void Reiniciar()
{
//	int r=SetupPromptReboot(NULL,NULL,FALSE);
/*	int r=TraduceMsg(NULL,LG_REBOOT,LG_REBOOTCAP,MB_YESNO|MB_ICONINFORMATION);
	if(r==IDYES) {
		HANDLE hToken; 
		TOKEN_PRIVILEGES tkp; 
		if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) return; 
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0); 

		ExitWindowsEx(EWX_REBOOT|EWX_FORCEIFHUNG,0);
	}*/
}

INT_PTR CALLBACK InstDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			inst = new CInstalar(hWnd,hInst);//,manual);
			if(!inst->Iniciar()) {
				EndDialog(hWnd,1);
			} else {
				if(inst->Fin()) EndDialog(hWnd,0);
			}
			return TRUE;
		}
		case WM_COMMAND:
		{
			if(wParam==IDOK)
			{
				LRESULT sel=SendMessage(GetDlgItem(hWnd,IDlista),LB_GETCURSEL,0,0);
				if(sel==-1) {
					Traduce::Msg(hWnd,LG_SEL,LG_ADVERT,MB_ICONWARNING);
				} else {
					if(!inst->OnOk(sel)) {
						EndDialog(hWnd,1);
					} else {
						if(inst->Fin()) EndDialog(hWnd,0);
					}
					return TRUE;
				}
			}
			return FALSE;
		}
		case WM_CLOSE:
		{
			inst->OnCancel();
			EndDialog(hWnd,1);
			return TRUE;
		}
	}
	return FALSE;
}

INT_PTR CALLBACK UnInstDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			uninst = new CDesinstalar(hWnd,hInst);
			if(!uninst->Iniciar()) {
				EndDialog(hWnd,1);
			} else {
				EndDialog(hWnd,0);
			}
			return TRUE;
		}
/*		case WM_COMMAND:
		{
			if(wParam==IDOK)
			{
				LRESULT sel=SendMessage(GetDlgItem(hWnd,IDlista),LB_GETCURSEL,0,0);
				if(sel==-1) {
					MessageBox(hWnd,Traduce(LG_SEL),Traduce(LG_ADVERT),MB_ICONWARNING);
				} else {
					ShowWindow(hWnd,SW_HIDE);
					if(!uninst->Base((DWORD)sel)) {
						EndDialog(hWnd,1);
					} else {
						if(!uninst->Joystick()) {
							EndDialog(hWnd,1);
						} else {
							EndDialog(hWnd,0);
						}
					}
					return TRUE;
				}
			}
			return FALSE;
		}
		case WM_CLOSE:
			EndDialog(hWnd,1);
			return TRUE;*/
	}
	return FALSE;
}


