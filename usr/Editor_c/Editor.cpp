// ProfileEditorTxt.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "Editor.h"
#include "Principal.h"
#include "dtexto.h"
#include "vtexto.h"
#include "vgrafica.h"
#include "idioma.h"
#include <afxdlgs.h>



// CProfileEditorTxtApp

BEGIN_MESSAGE_MAP(CEditor, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_COMPILAR, OnCompilar)	
	ON_COMMAND(ID_LANZAR, OnLanzar)	
END_MESSAGE_MAP()


// Construcción de CProfileEditorTxtApp

CEditor::CEditor(){ defPlantilla.Empty();}


// El único objeto CProfileEditorTxtApp

CEditor theApp;

// Inicialización de CProfileEditorTxtApp

BOOL CEditor::InitInstance()
{
	CWinApp::InitInstance();

	AfxInitRichEdit();

	CPrincipal* pMainFrame = new CPrincipal;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_hMDIAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_CHILDFRAME));
	m_hMenu1 = ::LoadMenu(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MENUTEXT));
	m_hMenu2 = ::LoadMenu(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MENUGRAF));

	//pMainFrame->GetMenu()->DestroyMenu();
	//pMainFrame->SetMenu(NULL);
	m_pMainWnd = pMainFrame;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	Traduce::Dlg(pMainFrame->CreateNewChild(RUNTIME_CLASS(CVGrafica), IDR_CHILDFRAME,m_hMenu2,m_hMDIAccel)->m_hWnd,89);
	Traduce::Dlg(pMainFrame->CreateNewChild(RUNTIME_CLASS(CVTexto), IDR_CHILDFRAME,m_hMenu1,m_hMDIAccel)->m_hWnd,89);
	::SetMenu(pMainFrame->m_hWnd,m_hMenu1);
	Traduce::Dlg(pMainFrame->m_hWnd,90);
	((CVTexto*)pMainFrame->MDIGetActive())->SetPlantilla(defPlantilla);

	return TRUE;
}

int CEditor::ExitInstance() 
{
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);
	if (m_hMenu1 != NULL)
		FreeResource(m_hMenu1);
	if (m_hMenu2 != NULL)
		FreeResource(m_hMenu2);
	return CWinApp::ExitInstance();
}

void CEditor::OnFileNew()
{
	CPrincipal* pFrame = STATIC_DOWNCAST(CPrincipal, m_pMainWnd);
	CVTexto* texto;
	if(pFrame->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVGrafica)) {
		CWnd* ch=pFrame->MDIGetActive()->GetWindow(GW_HWNDNEXT);
		do {
			if(ch->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
				texto=(CVTexto*)ch;
				break;
			}
			ch=ch->GetWindow(GW_HWNDNEXT);
		} while(ch);
		if(ch==NULL) return;
	} else {
		texto=(CVTexto*)pFrame->MDIGetActive();
	}

	if(texto->GetDlg()->EstaModificado()) {
		int r=Traduce::Msg(m_pMainWnd->m_hWnd,13,14,MB_YESNOCANCEL|MB_ICONWARNING);
		if(r==IDCANCEL) return; else {
			if(r==IDYES) {if(!OnFileSave2()) return;}
		}
	}
	texto->GetPlantilla(defPlantilla);
	pFrame->MDIGetActive()->DestroyWindow();
	pFrame->MDIGetActive()->DestroyWindow();
	Traduce::Dlg(pFrame->CreateNewChild(RUNTIME_CLASS(CVGrafica), IDR_CHILDFRAME,m_hMenu2,m_hMDIAccel)->m_hWnd,89);
	Traduce::Dlg(pFrame->CreateNewChild(RUNTIME_CLASS(CVTexto), IDR_CHILDFRAME,m_hMenu1,m_hMDIAccel)->m_hWnd,89);
	::SetMenu(pFrame->m_hWnd,m_hMenu1);
	Traduce::Dlg(pFrame->m_hWnd,90);
	((CVTexto*)pFrame->MDIGetActive())->SetPlantilla(defPlantilla);
}

void CEditor::OnFileOpen()
{
	CPrincipal* pFrame = STATIC_DOWNCAST(CPrincipal, m_pMainWnd);
	CVTexto* texto;
	if(pFrame->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVGrafica)) {
		CWnd* ch=pFrame->MDIGetActive()->GetWindow(GW_HWNDNEXT);
		do {
			if(ch->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
				texto=(CVTexto*)ch;
				break;
			}
			ch=ch->GetWindow(GW_HWNDNEXT);
		} while(ch);
		if(ch==NULL) return;
	} else {
		texto=(CVTexto*)pFrame->MDIGetActive();
	}

	if(texto->GetDlg()->EstaModificado()) {
		int r=Traduce::Msg(m_pMainWnd->m_hWnd,13,14,MB_YESNOCANCEL|MB_ICONWARNING);
		if(r==IDCANCEL) return; else {
			if(r==IDYES) {if(!OnFileSave2()) return;}
		}
	}
	CFileFind dir; dir.FindFile(); dir.FindNextFile();
	CFileDialog fd(TRUE,0,0,OFN_FILEMUSTEXIST| OFN_HIDEREADONLY,Traduce::Txt(16),this->m_pMainWnd);
	if(fd.DoModal()==IDOK) {
		texto->GetPlantilla(defPlantilla);
		pFrame->MDIGetActive()->DestroyWindow();
		pFrame->MDIGetActive()->DestroyWindow();
		SetCurrentDirectory(dir.GetRoot().GetBuffer(0));
		Traduce::Dlg(pFrame->CreateNewChild(RUNTIME_CLASS(CVGrafica), IDR_CHILDFRAME,m_hMenu2,m_hMDIAccel)->m_hWnd,89);
		Traduce::Dlg(pFrame->CreateNewChild(RUNTIME_CLASS(CVTexto), IDR_CHILDFRAME,m_hMenu1,m_hMDIAccel)->m_hWnd,89);
		::SetMenu(pFrame->m_hWnd,m_hMenu1);
		Traduce::Dlg(pFrame->m_hWnd,90);
		((CVTexto*)pFrame->MDIGetActive())->SetPlantilla(defPlantilla);
		texto=(CVTexto*)pFrame->MDIGetActive();
		texto->SetWindowText(fd.GetFileTitle().GetBuffer(0));
		texto->GetDlg()->Cargar(fd.GetPathName());
	}
	SetCurrentDirectory(dir.GetRoot().GetBuffer(0));
}

void CEditor::OnFileSave() { OnFileSave2();}
bool CEditor::OnFileSave2()
{
	CPrincipal* pFrame = STATIC_DOWNCAST(CPrincipal, m_pMainWnd);
	CVTexto* texto;
	if(pFrame->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVGrafica)) {
		CWnd* ch=pFrame->MDIGetActive()->GetWindow(GW_HWNDNEXT);
		do {
			if(ch->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
				texto=(CVTexto*)ch;
				break;
			}
			ch=ch->GetWindow(GW_HWNDNEXT);
		} while(ch);
		if(ch==NULL) return false;
	} else {
		texto=(CVTexto*)pFrame->MDIGetActive();
	}

	if(texto->GetDlg()->EsNuevo()) {
		return OnFileSaveAs2();
	} else {
		return texto->GetDlg()->Guardar();
	}
}
void CEditor::OnFileSaveAs() { OnFileSaveAs2();}
bool CEditor::OnFileSaveAs2()
{
	CPrincipal* pFrame = STATIC_DOWNCAST(CPrincipal, m_pMainWnd);
	CVTexto* texto;
	if(pFrame->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVGrafica)) {
		CWnd* ch=pFrame->MDIGetActive()->GetWindow(GW_HWNDNEXT);
		do {
			if(ch->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
				texto=(CVTexto*)ch;
				break;
			}
			ch=ch->GetWindow(GW_HWNDNEXT);
		} while(ch);
		if(ch==NULL) return false;
	} else {
		texto=(CVTexto*)pFrame->MDIGetActive();
	}

	CFileFind dir; dir.FindFile(); dir.FindNextFile();
	CString aux=Traduce::Txt(16);
	CFileDialog fd(FALSE,"jdf",Traduce::Txt(15),OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,aux.GetBuffer(0),this->m_pMainWnd);
	if(fd.DoModal()==IDOK) {
		SetCurrentDirectory(dir.GetRoot().GetBuffer(0));
		texto->GetDlg()->SetRutaPerfil(fd.GetPathName());
		SetCurrentDirectory(dir.GetRoot().GetBuffer(0));
		if(texto->GetDlg()->Guardar()) {
			texto->SetWindowText(fd.GetFileTitle().GetBuffer(0));
			return true;
		} else {
			return false;
		}
	} else {
		SetCurrentDirectory(dir.GetRoot().GetBuffer(0));
		return false;
	}
}


void CEditor::OnCompilar()
{
	CPrincipal* pFrame = STATIC_DOWNCAST(CPrincipal, m_pMainWnd);
	CVTexto* texto;
	if(pFrame->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVGrafica)) {
		CWnd* ch=pFrame->MDIGetActive()->GetWindow(GW_HWNDNEXT);
		do {
			if(ch->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
				texto=(CVTexto*)ch;
				break;
			}
			ch=ch->GetWindow(GW_HWNDNEXT);
		} while(ch);
		if(ch==NULL) return;
	} else {
		texto=(CVTexto*)pFrame->MDIGetActive();
	}

	if(texto->GetDlg()->EstaModificado()) {
		int r=Traduce::Msg(m_pMainWnd->m_hWnd,162,163,MB_OKCANCEL|MB_ICONINFORMATION);
		if(r==IDCANCEL) return; else {
				if(!OnFileSave2()) return;
		}
	}
	{
		typedef bool (*FUNC)(char*,char*);
		HMODULE dll=LoadLibrary("x36_svc.dll");
		if(dll==NULL) {
			Traduce::Msg(m_pMainWnd->m_hWnd,8,163,MB_ICONWARNING);
			return;
		}

		FUNC Compilar=(FUNC)GetProcAddress(dll,"Compile");
		if(Compilar!=NULL) {
			CString mdf=texto->GetDlg()->GetRutaPerfil();
			CString jdf=mdf.GetBuffer(0);
			mdf+=".mdf";
			jdf+=".jdf";
			if(Compilar(mdf.GetBuffer(0),jdf.GetBuffer(0))) {
				Traduce::Msg(m_pMainWnd->m_hWnd,164,163,MB_ICONINFORMATION);
			}
		}
		FreeLibrary(dll);
	}
}
void CEditor::OnLanzar()
{
	CPrincipal* pFrame = STATIC_DOWNCAST(CPrincipal, m_pMainWnd);
	CVTexto* texto;
	if(pFrame->MDIGetActive()->GetRuntimeClass()==RUNTIME_CLASS(CVGrafica)) {
		CWnd* ch=pFrame->MDIGetActive()->GetWindow(GW_HWNDNEXT);
		do {
			if(ch->GetRuntimeClass()==RUNTIME_CLASS(CVTexto)) {
				texto=(CVTexto*)ch;
				break;
			}
			ch=ch->GetWindow(GW_HWNDNEXT);
		} while(ch);
		if(ch==NULL) return;
	} else {
		texto=(CVTexto*)pFrame->MDIGetActive();
	}

	if(texto->GetDlg()->EstaModificado()) {
		int r=Traduce::Msg(m_pMainWnd->m_hWnd,162,0,MB_OKCANCEL|MB_ICONINFORMATION);
		if(r==IDCANCEL) return; else {
				if(!OnFileSave2()) return;
		}
	}
	{
		typedef bool (*FUNC)(char*,char*);
		HMODULE dll=LoadLibrary("x36_svc.dll");
		if(dll==NULL) {
			Traduce::Msg(m_pMainWnd->m_hWnd,8,0,MB_ICONWARNING);
			return;
		}

		FUNC Compilar=(FUNC)GetProcAddress(dll,"CompileAndLaunch");
		if(Compilar!=NULL) {
			CString mdf=texto->GetDlg()->GetRutaPerfil();
			CString jdf=mdf.GetBuffer(0);
			mdf+=".mdf";
			jdf+=".jdf";
			if(Compilar(mdf.GetBuffer(0),jdf.GetBuffer(0))) {
				Traduce::Msg(m_pMainWnd->m_hWnd,9,0,MB_ICONINFORMATION);
			}
		}
		FreeLibrary(dll);
	}
}

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD){}


void CEditor::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}