// VTexto.cpp: archivo de implementación
//

#include "stdafx.h"
#include "VGrafica.h"


// CVTexto

IMPLEMENT_DYNCREATE(CVGrafica, CMDIChildWnd)

CVGrafica::CVGrafica(){}
CVGrafica::~CVGrafica(){}

BEGIN_MESSAGE_MAP(CVGrafica, CMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// Controladores de mensajes de CVTexto

int CVGrafica::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->MDIMaximize();

	return 0;
}

BOOL CVGrafica::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MAXIMIZEBOX;

	return TRUE;
}

//============================================================
//============================================================
