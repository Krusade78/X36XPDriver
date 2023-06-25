// X36Map.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "LauncherDlg.h"

//********** Definicion de la Clase Principal

class CLauncherApp : public CWinApp
{
public:
	CLauncherApp();
	virtual BOOL InitInstance();
};

//******* Objeto de Aplicación

extern CLauncherApp theApp;
CLauncherApp theApp;

// Implementación de la clase principal

CLauncherApp::CLauncherApp(){}

BOOL CLauncherApp::InitInstance()
{
	CWinApp::InitInstance();
	CLauncherDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return TRUE;
}