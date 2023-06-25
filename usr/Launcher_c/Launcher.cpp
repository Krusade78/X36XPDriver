// X36Map.cpp : define los comportamientos de las clases para la aplicaci�n.
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

//******* Objeto de Aplicaci�n

extern CLauncherApp theApp;
CLauncherApp theApp;

// Implementaci�n de la clase principal

CLauncherApp::CLauncherApp(){}

BOOL CLauncherApp::InitInstance()
{
	CWinApp::InitInstance();
	CLauncherDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return TRUE;
}