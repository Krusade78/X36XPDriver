// MainFrm.cpp: implementación de la clase CMainFrame
//

#include "stdafx.h"
#include "resource.h"
#include "idioma.h"
#include "dtexto.h"
#include "vtexto.h"
#include ".\principal.h"
#include "editor.h"
#include "vgrafica.h"

static UINT indicadores[] =
{
	ID_SEPARATOR,
	ID_INDICADOR_LIN,
	ID_INDICADOR_CAR,
	ID_INDICADOR_SOB,
	ID_INDICADOR_MAY,
	ID_INDICADOR_NUM
};

// CMainFrame

IMPLEMENT_DYNAMIC(CPrincipal, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CPrincipal, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_INDICADOR_LIN, OnUpdateUI2)
	ON_UPDATE_COMMAND_UI(ID_INDICADOR_CAR, OnUpdateUI3)
	ON_UPDATE_COMMAND_UI(ID_INDICADOR_SOB, OnUpdateUI4)
	ON_UPDATE_COMMAND_UI(ID_INDICADOR_MAY, OnUpdateUI5)
	ON_UPDATE_COMMAND_UI(ID_INDICADOR_NUM, OnUpdateUI6)
	ON_WM_CLOSE()
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, Hints )
END_MESSAGE_MAP()

BOOL CPrincipal::Hints( UINT id, NMHDR* pTTTStruct, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;

	switch(pTTTStruct->idFrom)
	{
		case ID_COMPILAR:
			pTTT->lpszText=(LPSTR)Traduce::Txt(163);
			return TRUE;
		case ID_LANZAR:
			pTTT->lpszText=(LPSTR)Traduce::Txt(165);
			return TRUE;
	}
	return FALSE;
}
// Construcción o destrucción de CMainFrame

CPrincipal::CPrincipal(){sobreescribir=false;}
CPrincipal::~CPrincipal(){}

int CPrincipal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | /*CBRS_TOOLTIPS | CBRS_FLYBY |*/ CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("No se pudo crear la barra de herramientas\n");
		return -1;      // No se pudo crear
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicadores, sizeof(indicadores)/sizeof(UINT)))
	{
		TRACE0("No se pudo crear la barra de estado\n");
		return -1;      // No se pudo crear
	}

	m_wndToolBar.EnableToolTips();
	this->MoveWindow(0,0,700,528);
	this->CenterWindow();

	return 0;
}

BOOL CPrincipal::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
void CPrincipal::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	Traduce::Dlg(this->m_hWnd,90);
}

// Barra de estado

void CPrincipal::SetAyuda(LPCSTR texto)
{
	CWnd* a=this->MDIGetActive();
	if(a!=NULL) {
		if(this->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
			m_wndStatusBar.SetWindowText(texto);
		}
	}
}

void CPrincipal::SetPosicion(int lin,int col)
{
	linea=lin;
	columna=col;
}
void CPrincipal::OnUpdateUI2(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
	CString lin;
	lin.Format("%u",linea);
	lin.Insert(0,Traduce::Txt(108));
	pCmdUI->SetText(lin.GetBuffer(0));
}

void CPrincipal::OnUpdateUI3(CCmdUI *pCmdUI)
{
	CString col;
	col.Format("%u",columna);
	col.Insert(0,Traduce::Txt(109));
	pCmdUI->SetText(col.GetBuffer(0));
}

void CPrincipal::SetSobreescribir(bool sb)
{
	sobreescribir=sb;
}
void CPrincipal::OnUpdateUI4(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
	if(sobreescribir) {
		pCmdUI->SetText(Traduce::Txt(110));
	} else {
		pCmdUI->SetText("");
	}
}

void CPrincipal::OnUpdateUI5(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
	if((GetKeyState(VK_CAPITAL)&1)==1) {
		pCmdUI->SetText(Traduce::Txt(111));
	} else {
		pCmdUI->SetText("");
	}
}

void CPrincipal::OnUpdateUI6(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
	if((GetKeyState(VK_NUMLOCK)&1)==1) {
		pCmdUI->SetText(Traduce::Txt(112));
	} else {
		pCmdUI->SetText("");
	}
}

void CPrincipal::OnClose()
{
	CVTexto* texto;
	if(this->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVGrafica)) {
		CWnd* ch=this->MDIGetActive()->GetWindow(GW_HWNDNEXT);
		do {
			if(ch->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
				texto=(CVTexto*)ch;
				break;
			}
			ch=ch->GetWindow(GW_HWNDNEXT);
		} while(ch);
		if(ch==NULL) return;
	} else {
		texto=(CVTexto*)this->MDIGetActive();
	}

	if(texto->GetDlg()->EstaModificado()) {
		int r=Traduce::Msg(this->m_hWnd,13,14,MB_YESNOCANCEL|MB_ICONWARNING);
		if(r==IDCANCEL) return; else {
			if(r==IDYES) {if(!theApp.OnFileSave2()) return;}
		}
	}

	CMDIFrameWnd::OnClose();
}