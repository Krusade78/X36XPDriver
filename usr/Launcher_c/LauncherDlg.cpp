// X36MapDlg.cpp: Dialogo principal de programa
// Implementación

#include "stdafx.h"
#include "resource.h"
#include "LauncherDlg.h"
#include "idioma.h"

// Constructor de la Clase

CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/):
	CDialog(IDD_LAUNCHER_DIALOG, pParent){}


/*************************************************
*************** Control de Mensajes **************
**************************************************/

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, listaMapas);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
	ON_BN_CLICKED(IDC_boton1, OnBnClickedDefecto)
	ON_BN_CLICKED(IDC_boton2, OnBnClickedCargar)
	ON_BN_CLICKED(IDC_boton3, OnBnClickedActualizar)
	ON_BN_CLICKED(IDC_boton4, OnBnClickedEditar)
END_MESSAGE_MAP()

//-------------------------------------------------//
//-------------------------------------------------//


//--------------> Inicialización

BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);	// Establecer icono grande
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);	// Establecer icono pequeño

	Traduce::Dlg(this->m_hWnd,0);
	Traduce::Dlg(GetDlgItem(IDC_Texto1)->m_hWnd,1);
	Traduce::Dlg(GetDlgItem(IDC_boton1)->m_hWnd,2);
	Traduce::Dlg(GetDlgItem(IDC_boton2)->m_hWnd,3);
	Traduce::Dlg(GetDlgItem(IDC_boton3)->m_hWnd,4);
	Traduce::Dlg(GetDlgItem(IDC_boton4)->m_hWnd,5);
	Traduce::Dlg(GetDlgItem(IDOK)->m_hWnd,6);

	CargarListaMapas();

	return TRUE;
}

//-------------------------------------------------//
//-------------------------------------------------//


/*************************************************
******************** Métodos *********************
**************************************************/

void CLauncherDlg::CargarListaMapas()
{
	listaMapas.ResetContent();

	CFileFind busca;
	BOOL va = busca.FindFile("*.jdf");
	while (va) {
		va = busca.FindNextFile();
		listaMapas.AddString(busca.GetFileTitle().GetBuffer(0));
	}
}

//-------------------------------------------------//

void CLauncherDlg::CargarPerfilDefecto()
{
	typedef bool (*FUNC)(char*,char*);
	CStdioFile f;

	if(!f.Open("defaultTempMdf.tmp",CFile::modeCreate|CFile::modeWrite))
	{
		Traduce::Msg(this->m_hWnd,11,0,MB_ICONWARNING);
		return;
	}
	f.WriteString("// Null mdf");
	f.Flush();
	f.Close();

	if(!f.Open("defaultTempJdf.tmp",CFile::modeCreate|CFile::modeWrite))
	{
		Traduce::Msg(this->m_hWnd,12,0,MB_ICONWARNING);
		f.Remove("defaultTempMdf.tmp");
		return;
	}

	try {
		f.WriteString("BTN1 /DXB1\n");
		f.WriteString("BTN2 /DXB2\n");
		f.WriteString("BTN3 /DXB3\n");
		f.WriteString("BTN4 /DXB4\n");
		f.WriteString("BTN5 /DXB5\n");
		f.WriteString("BTN6 /DXB6\n");
		f.WriteString("BTN7 /DXP11\n");
		f.WriteString("BTN8 /DXP15\n");
		f.WriteString("BTN9 /DXP17\n");
		f.WriteString("BTN10 /DXP13\n");
		f.WriteString("BTN11 /DXP18\n");
		f.WriteString("BTN12 /DXP12\n");
		f.WriteString("BTN13 /DXP16\n");
		f.WriteString("BTN14 /DXP14\n");
		f.WriteString("BTN15 /DXP21\n");
		f.WriteString("BTN16 /DXP25\n");
		f.WriteString("BTN17 /DXP27\n");
		f.WriteString("BTN18 /DXP23\n");
		f.WriteString("BTN19 /DXP28\n");
		f.WriteString("BTN20 /DXP22\n");
		f.WriteString("BTN21 /DXP26\n");
		f.WriteString("BTN22 /DXP24\n");
		f.WriteString("BTN23 /DXB7\n");
		f.WriteString("BTN24 /DXB8\n");
		f.WriteString("BTN29 /DXP31\n");
		f.WriteString("BTN30 /DXP33\n");
		f.WriteString("BTN31 /DXP34\n");
		f.WriteString("BTN32 /DXP32\n");
		f.WriteString("BTN33 /DXP41\n");
		f.WriteString("BTN34 /DXP43\n");
		f.WriteString("BTN35 /DXP44\n");
		f.WriteString("BTN36 /DXP42\n");
	//	f.WriteString("AXIS1 /ASX\n");
	//	f.WriteString("AXIS2 /ASY\n");
	//	f.WriteString("AXIS3 /ASZ\n");
	//	f.WriteString("AXIS4 /ASR\n");
	//	f.WriteString("ROT1 /AS1\n");
	//	f.WriteString("ROT2 /AS2\n");
		f.Flush();
	} catch (CException* e) {
		Traduce::Msg(this->m_hWnd,12,0,MB_ICONWARNING);
	}
	f.Close();

	HMODULE dll=LoadLibrary("x36_svc.dll");
	if(dll==NULL) {
		Traduce::Msg(this->m_hWnd,9,0,MB_ICONWARNING);
		goto fin;
	}

	FUNC Compilar=(FUNC)GetProcAddress(dll,"CompileAndLaunch");
	if(Compilar!=NULL) {
		if(Compilar("defaultTempMdf.tmp","defaultTempJdf.tmp")) {
			Traduce::Msg(this->m_hWnd,10,0,MB_ICONINFORMATION);
		}
	}
	FreeLibrary(dll);

fin:
	f.Remove("defaultTempMdf.tmp");
	f.Remove("defaultTempJdf.tmp");
}

void CLauncherDlg::CargarPerfil()
{
	typedef bool (*FUNC)(char*,char*);
	CFile f;
	CString mdf,jdf;

	listaMapas.GetText(listaMapas.GetCurSel(),mdf);
	mdf+=".mdf";
	listaMapas.GetText(listaMapas.GetCurSel(),jdf);
	jdf+=".jdf";

	if(f.Open(jdf,CFile::modeRead)) {
		jdf=f.GetFilePath().GetBuffer(0);
		f.Close();
	} else {
		Traduce::Msg(this->m_hWnd,7,0,MB_ICONWARNING);
		return;
	}
	if(f.Open(mdf,CFile::modeRead)) {
		mdf=f.GetFilePath().GetBuffer(0);
		f.Close();
	} else {
		Traduce::Msg(this->m_hWnd,8,0,MB_ICONWARNING);
		return;
	}

	HMODULE dll=LoadLibrary("x36_svc.dll");
	if(dll==NULL) {
		Traduce::Msg(this->m_hWnd,9,0,MB_ICONWARNING);
		return;
	}

	FUNC Compilar=(FUNC)GetProcAddress(dll,"CompileAndLaunch");
	if(Compilar!=NULL) {
		if(Compilar(mdf.GetBuffer(0),jdf.GetBuffer(0))) {
			Traduce::Msg(this->m_hWnd,10,0,MB_ICONINFORMATION);
		}
	}
	FreeLibrary(dll);
}

//-------------------------------------------------//
//-------------------------------------------------//


/*************************************************
******************* Eventos **********************
**************************************************/


void CLauncherDlg::OnBnClickedActualizar()
{
	CargarListaMapas();
}

void CLauncherDlg::OnBnClickedCargar()
{
	if(listaMapas.GetCurSel()!=-1)
		CargarPerfil();
}

void CLauncherDlg::OnBnClickedDefecto()
{
	CargarPerfilDefecto();
}

void CLauncherDlg::OnBnClickedEditar()
{
	if(listaMapas.GetCurSel()!=-1) {
		CString ruta;
		listaMapas.GetText(listaMapas.GetCurSel(),ruta);
		if((UINT64)ShellExecuteA(NULL,"open","editor.exe",ruta.GetBuffer(0),".",SW_SHOWNORMAL)>32) {
			this->OnOK();
		}
	}
}
