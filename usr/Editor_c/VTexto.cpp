// VTexto.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Editor.h"
#include "DTexto.h"
#include "VTexto.h"
#include "idioma.h"
#include "principal.h"
#include ".\vtexto.h"



// CVTexto

IMPLEMENT_DYNCREATE(CVTexto, CMDIChildWnd)

CVTexto::CVTexto(){}
CVTexto::~CVTexto(){}

BEGIN_MESSAGE_MAP(CVTexto, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDB_TP,IDB_R21,ComMdf)
	ON_COMMAND_RANGE(IDB_JMACROS,IDB_JB,ComJdf)
	ON_COMMAND_RANGE(5000,5100,OnPlantilla)
	ON_COMMAND_RANGE(2000,2039,OnNotifyComMdf)
	ON_COMMAND_RANGE(2042,2084,OnNotifyComJdf)
	ON_WM_CLOSE()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
END_MESSAGE_MAP()


// Controladores de mensajes de CVTexto

int CVTexto::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!barra1.Create(this, IDD_MACROPROP,CBRS_ALIGN_LEFT|CBRS_TOOLTIPS,AFX_IDW_DIALOGBAR))
	{
		TRACE0("No se pudo crear el control dialogbar\n");
		return -1;		// No se pudo crear
	}
	barra1.EnableToolTips();

	if (!barra2.Create(this, IDD_JOYPROP,CBRS_ALIGN_LEFT|CBRS_TOOLTIPS,AFX_IDW_DIALOGBAR))
	{
		TRACE0("No se pudo crear el control dialogbar\n");
		return -1;		// No se pudo crear
	}

	TraducirBarra1();
	TraducirBarra2();

	if (!texto.Create(this))
	{
		TRACE0("No se pudo crear el control dialogbar\n");
		return -1;		// No se pudo crear
	}

	if (!rebar.Create(this) ||
		!rebar.AddBar(&barra1) || !rebar.AddBar(&barra2))
	{
		TRACE0("No se pudo crear el control rebar\n");
		return -1;      // No se pudo crear
	}
	rebar.GetReBarCtrl().ShowBand(1,FALSE);
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_RS))->SetBuddy(barra1.GetDlgItem(IDC_R));
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_RS))->SetRange(0,255);
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_RS))->SetPos(0);
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_REPS))->SetBuddy(barra1.GetDlgItem(IDC_REP));
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_REPS))->SetRange(1,100);
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_REPS))->SetPos(1);
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_PS))->SetBuddy(barra1.GetDlgItem(IDC_P));
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_PS))->SetRange(1,250);
	((CSpinButtonCtrl*)barra1.GetDlgItem(IDC_PS))->SetPos(1);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JST))->SetBuddy(barra2.GetDlgItem(IDC_JT));
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JST))->SetRange(1,16);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JST))->SetPos(1);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX1))->SetBuddy(barra2.GetDlgItem(IDC_JDX1));
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX1))->SetRange(1,40);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX1))->SetPos(1);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX2))->SetBuddy(barra2.GetDlgItem(IDC_JDX2));
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX2))->SetRange(1,4);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX2))->SetPos(1);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX3))->SetBuddy(barra2.GetDlgItem(IDC_JDX3));
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX3))->SetRange(1,8);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSDX3))->SetPos(1);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE1))->SetBuddy(barra2.GetDlgItem(IDC_JE1));
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE1))->SetRange(0,16);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE1))->SetPos(0);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE2))->SetBuddy(barra2.GetDlgItem(IDC_JE2));
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE2))->SetRange(1,15);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE2))->SetPos(1);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE3))->SetBuddy(barra2.GetDlgItem(IDC_JE3));
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE3))->SetRange(1,255);
	((CSpinButtonCtrl*)barra2.GetDlgItem(IDC_JSE3))->SetPos(1);

	texto.MoveWindow(158,0,0,0);
	texto.ShowWindow(SW_SHOW);
	this->MDIMaximize();

	return 0;
}

BOOL CVTexto::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MAXIMIZEBOX;

	return TRUE;
}

void CVTexto::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	texto.MoveWindow(158,0,cx-158,cy);
}

//============================================================
//============================================================

void CVTexto::BuscarPlantillas()
{
	CFileFind busca;
	short i=5000;
	short sel=-1;
	popup.CreatePopupMenu();
	BOOL va = busca.FindFile("*.kbp");
	while(va) {
		va = busca.FindNextFile();
		popup.AppendMenu(MF_UNCHECKED|MF_STRING,i,busca.GetFileName().GetBuffer(0));
		if(defPlantilla.IsEmpty()) {
			if(busca.GetFileName().CompareNoCase("english-us.kbp")==0) {
				sel=i;
			}
		} else {
			if(busca.GetFileName().CompareNoCase(defPlantilla.GetBuffer(0))==0) {
				sel=i;
			}
		}
		i++;
	}
	if(popup.GetMenuItemCount()!=0) {
		GetParentFrame()->GetMenu()->ModifyMenu(3,MF_BYPOSITION|MF_POPUP,(UINT_PTR)popup.m_hMenu);
		if(sel!=-1)CargarPlantilla(sel);
	} else {
		popup.DestroyMenu();
	}
	DrawMenuBar();
}

void CVTexto::CargarPlantilla(short i)
{
	CStdioFile f;
	CString linea;
	CString nombre;
	for(short j=5000;j<(short)(5000+GetParentFrame()->GetMenu()->GetSubMenu(3)->GetMenuItemCount());j++) {
		GetParentFrame()->GetMenu()->CheckMenuItem(j,MF_BYCOMMAND|MF_UNCHECKED);
	}
	GetParentFrame()->GetMenu()->GetMenuString(i,nombre,MF_BYCOMMAND);
	if(f.Open(nombre.GetBuffer(0),CFile::modeRead)==0) {
		Traduce::Msg(GetParentFrame()->m_hWnd,35,36,MB_ICONWARNING);
		return;
	}
	((CComboBox*)barra1.GetDlgItem(IDC_CODIGOS))->ResetContent();
	while(f.ReadString(linea)) {
		((CComboBox*)barra1.GetDlgItem(IDC_CODIGOS))->AddString(linea.GetBuffer(0));
	}
	f.Close();
	((CComboBox*)barra1.GetDlgItem(IDC_CODIGOS))->SetCurSel(0);
	GetParentFrame()->GetMenu()->CheckMenuItem(i,MF_BYCOMMAND|MF_CHECKED);
	defPlantilla=nombre.GetBuffer(0);
}

void CVTexto::OnPlantilla(UINT nID)
{
	CargarPlantilla((short)nID);
}

void CVTexto::SetPlantilla(CString nombre)
{
	defPlantilla=nombre.GetBuffer(0);
	BuscarPlantillas();
	TraducirMenu();
}
void CVTexto::GetPlantilla(CString& nombre)
{
	nombre=defPlantilla.GetBuffer(0);
}

CDTexto* CVTexto::GetDlg()
{
	return &texto;
}

void CVTexto::VerMdf()
{
	rebar.GetReBarCtrl().ShowBand(1,FALSE);
	rebar.GetReBarCtrl().ShowBand(0);
}
void CVTexto::VerJdf()
{
	rebar.GetReBarCtrl().ShowBand(0,FALSE);
	rebar.GetReBarCtrl().ShowBand(1);
}

void CVTexto::TraducirBarra1()
{
	Traduce::Dlg(barra1.GetDlgItem(IDS_TECLAS)->m_hWnd,37);
	Traduce::Dlg(barra1.GetDlgItem(IDS_RATON)->m_hWnd,38);
	Traduce::Dlg(barra1.GetDlgItem(IDS_REPETIR)->m_hWnd,39);
	Traduce::Dlg(barra1.GetDlgItem(IDS_PAUSA)->m_hWnd,40);
	Traduce::Dlg(barra1.GetDlgItem(IDS_MODOS)->m_hWnd,41);
}
void CVTexto::TraducirBarra2()
{
	Traduce::Dlg(barra2.GetDlgItem(IDS_JELEMENTOS)->m_hWnd,42);
	Traduce::Dlg(barra2.GetDlgItem(IDS_JMACROS)->m_hWnd,43);
	Traduce::Dlg(barra2.GetDlgItem(IDS_JMODISC)->m_hWnd,44);
	Traduce::Dlg(barra2.GetDlgItem(IDS_JMODISB)->m_hWnd,45);
	Traduce::Dlg(barra2.GetDlgItem(IDS_JMODISE)->m_hWnd,46);

	CComboBox* el=(CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS);
	for(short i=47;i<83;i++)
		el->AddString(Traduce::Txt(i));
	el->AddString("-------------------------");
		el->AddString(Traduce::Txt(83));
		el->AddString(Traduce::Txt(84));
		el->AddString(Traduce::Txt(85));
		el->AddString(Traduce::Txt(86));
	el->AddString("-------------------------");
		el->AddString(Traduce::Txt(87));
		el->AddString(Traduce::Txt(88));
	el->SetCurSel(0);
}
void CVTexto::TraducirMenu()
{
	CString aux;
	GetParentFrame()->GetMenu()->ModifyMenu(0,MF_BYPOSITION,0,Traduce::Txt(17));
	aux=Traduce::Txt(18);
	aux+="\tCtrl+N";
	GetParentFrame()->GetMenu()->ModifyMenu(ID_FILE_NEW,MF_BYCOMMAND,ID_FILE_NEW,aux.GetBuffer(0));
	aux=Traduce::Txt(19);
	aux+="\tCtrl+O";
	GetParentFrame()->GetMenu()->ModifyMenu(ID_FILE_OPEN,MF_BYCOMMAND,ID_FILE_OPEN,aux.GetBuffer(0));
	aux=Traduce::Txt(20);
	aux+="\tCtrl+S";
	GetParentFrame()->GetMenu()->ModifyMenu(ID_FILE_SAVE,MF_BYCOMMAND,ID_FILE_SAVE,aux.GetBuffer(0));
	GetParentFrame()->GetMenu()->ModifyMenu(ID_FILE_SAVE_AS,MF_BYCOMMAND,ID_FILE_SAVE_AS,Traduce::Txt(21));
	GetParentFrame()->GetMenu()->ModifyMenu(ID_APP_EXIT,MF_BYCOMMAND,ID_APP_EXIT,Traduce::Txt(22));

	GetParentFrame()->GetMenu()->ModifyMenu(1,MF_BYPOSITION,0,Traduce::Txt(23));
	aux=Traduce::Txt(24);
	aux+="\tCtrl+Z";
	GetParentFrame()->GetMenu()->ModifyMenu(ID_EDIT_UNDO,MF_BYCOMMAND,ID_EDIT_UNDO,aux.GetBuffer(0));
	GetParentFrame()->GetMenu()->ModifyMenu(ID_EDIT_REDO,MF_BYCOMMAND,ID_EDIT_REDO,Traduce::Txt(25));
	aux=Traduce::Txt(26);
	aux+="\tCtrl+X";
	GetParentFrame()->GetMenu()->ModifyMenu(ID_EDIT_CUT,MF_BYCOMMAND,ID_EDIT_CUT,aux.GetBuffer(0));
	aux=Traduce::Txt(27);
	aux+="\tCtrl+C";
	GetParentFrame()->GetMenu()->ModifyMenu(ID_EDIT_COPY,MF_BYCOMMAND,ID_EDIT_COPY,aux.GetBuffer(0));
	aux=Traduce::Txt(28);
	aux+="\tCtrl+V";
	GetParentFrame()->GetMenu()->ModifyMenu(ID_EDIT_PASTE,MF_BYCOMMAND,ID_EDIT_PASTE,aux.GetBuffer(0));

	GetParentFrame()->GetMenu()->ModifyMenu(2,MF_BYPOSITION,0,Traduce::Txt(29));
	GetParentFrame()->GetMenu()->ModifyMenu(IDM_TEXTO,MF_BYCOMMAND,IDM_TEXTO,Traduce::Txt(30));
	GetParentFrame()->GetMenu()->ModifyMenu(IDM_GRAFICO,MF_BYCOMMAND,IDM_GRAFICO,Traduce::Txt(31));

	GetParentFrame()->GetMenu()->ModifyMenu(3,MF_BYPOSITION,0,Traduce::Txt(32));

	GetParentFrame()->GetMenu()->ModifyMenu(4,MF_BYPOSITION,0,Traduce::Txt(33));
	GetParentFrame()->GetMenu()->ModifyMenu(ID_APP_ABOUT,MF_BYCOMMAND,ID_APP_ABOUT,Traduce::Txt(34));
}

//==============================================================
//				COMANDOS MDF
//==============================================================

void CVTexto::ComMdf(UINT nID)
{
	CString cmd;
	cmd.Empty();
	switch(nID)
	{
	case IDB_TP:
		if(((CComboBox*)barra1.GetDlgItem(IDC_CODIGOS))->GetCurSel()!=-1) {
			cmd.Format("KEYD %u ",((CComboBox*)barra1.GetDlgItem(IDC_CODIGOS))->GetCurSel());
		}
		break;
	case IDB_TS:
		if(((CComboBox*)barra1.GetDlgItem(IDC_CODIGOS))->GetCurSel()!=-1) {
			cmd.Format("KEYU %u ",((CComboBox*)barra1.GetDlgItem(IDC_CODIGOS))->GetCurSel());
		}
		break;
	case IDB_R10:
		cmd="MS1U ";
		break;
	case IDB_R11:
		cmd="MS1D ";
		break;
	case IDB_R20:
		cmd="MS2U ";
		break;
	case IDB_R21:
		cmd="MS2D ";
		break;
	case IDB_R30:
		cmd="MS3U ";
		break;
	case IDB_R31:
		cmd="MS3D ";
		break;
	case IDB_RL:
		barra1.GetDlgItem(IDC_R)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>255 || n<0) {
				Traduce::Msg(this->m_hWnd,92,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				cmd.Insert(0,"MSXL "); cmd+=" ";
			}
		}
		break;
	case IDB_RR:
		barra1.GetDlgItem(IDC_R)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>255 || n<0) {
				Traduce::Msg(this->m_hWnd,92,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				cmd.Insert(0,"MSXR "); cmd+=" ";
			}
		}
		break;
	case IDB_RU:
		barra1.GetDlgItem(IDC_R)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>255 || n<0) {
				Traduce::Msg(this->m_hWnd,92,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				cmd.Insert(0,"MSYU "); cmd+=" ";
			}
		}
		break;
	case IDB_RD:
		barra1.GetDlgItem(IDC_R)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>255 || n<0) {
				Traduce::Msg(this->m_hWnd,92,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				cmd.Insert(0,"MSYD "); cmd+=" ";
			}
		}
		break;
	case IDB_RH:
		cmd="HOLD ";
		break;
	case IDB_RI:
		cmd="REPI ";
		break;
	case IDB_RF:
		cmd="REPF ";
		break;
	case IDB_RN:
		barra1.GetDlgItem(IDC_REP)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>100 || n<1) {
				Traduce::Msg(this->m_hWnd,93,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				cmd.Insert(0,"REPN "); cmd+=" ";
			}
		}
		break;
	case IDB_P:
		barra1.GetDlgItem(IDC_P)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>250 || n<1) {
				Traduce::Msg(this->m_hWnd,94,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				cmd.Insert(0,"DLAY "); cmd+=" ";
			}
		}
		break;
	case IDB_MP0:
		cmd="SETK 0 ";
		break;
	case IDB_MP1:
		cmd="SETK 1 ";
		break;
	case IDB_M10:
		cmd="SETM 1 ";
		break;
	case IDB_M20:
		cmd="SETM 2 ";
		break;
	case IDB_M30:
		cmd="SETM 3 ";
		break;
	case IDB_M11:
		cmd="SETX 1 ";
		break;
	case IDB_M21:
		cmd="SETX 2 ";
		break;
	case IDB_M22:
		cmd="SETX 3 ";
		break;
	default: break;
	}

	if(!cmd.IsEmpty()) texto.ComandoMdf(cmd.GetBuffer(0));
}

void CVTexto::OnNotifyComMdf(UINT ID)
{
	switch(ID-1000) {
		case 1000:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(" ");
			break;
		case IDB_TP:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(113));
			break;
		case IDB_TS:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(114));
			break;
		case IDB_R10:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(115));
			break;
		case IDB_R11:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(116));
			break;
		case IDB_R20:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(117));
			break;
		case IDB_R21:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(118));
			break;
		case IDB_R30:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(119));
			break;
		case IDB_R31:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(120));
			break;
		case IDB_RL:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(121));
			break;
		case IDB_RR:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(122));
			break;
		case IDB_RU:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(123));
			break;
		case IDB_RD:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(124));
			break;
		case IDB_RH:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(125));
			break;
		case IDB_RI:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(126));
			break;
		case IDB_RF:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(127));
			break;
		case IDB_RN:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(128));
			break;
		case IDB_P:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(129));
			break;
		case IDB_MP0:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(130));
			break;
		case IDB_MP1:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(131));
			break;
		case IDB_M10:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(132));
			break;
		case IDB_M20:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(133));
			break;
		case IDB_M30:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(134));
			break;
		case IDB_M11:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(135));
			break;
		case IDB_M21:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(136));
			break;
		case IDB_M22:
			((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(137));
			break;
		default:break;
	}
}

//==============================================================
//				COMANDOS JDF
//==============================================================

void CVTexto::ComJdf(UINT nID)
{
	CString cmd;
	cmd.Empty();
	switch(nID)
	{
	case IDB_JELEMENTOS:
		if(((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()!=-1) {
			if(((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()<36) {
				cmd.Format("BTN%u ",((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()+1);
			} else if(((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()<41 && ((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()>36) {
				cmd.Format("AXIS%u ",((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()-36);
			} else if(((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()>41){
				cmd.Format("ROT%u ",((CComboBox*)barra2.GetDlgItem(IDC_JELEMENTOS))->GetCurSel()-41);
			}
		}
		break;
	case IDB_JMACROS:
		if(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel()!=-1) {
			((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetLBText(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel(),cmd);
			cmd+=" ";
		} else {
			cmd.Empty();
		}
		break;
	case IDB_JK0:
		cmd="/K0 ";
		break;
	case IDB_JK1:
		cmd="/K1 ";
		break;
	case IDB_JM1:
		cmd="/M1 ";
		break;
	case IDB_JM2:
		cmd="/M2 ";
		break;
	case IDB_JM3:
		cmd="/M3 ";
		break;
	case IDB_JX1:
		cmd="/X1 ";
		break;
	case IDB_JX2:
		cmd="/X2 ";
		break;
	case IDB_JX3:
		cmd="/X3 ";
		break;
	case IDB_JT:
		barra2.GetDlgItem(IDC_JT)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>16 || n<1) {
				Traduce::Msg(this->m_hWnd,95,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				if(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel()!=-1) {
					CString aux;
					cmd.Insert(0,"/T");
					cmd+=" ";
					((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetLBText(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel(),aux);
					cmd+=aux.GetBuffer(0); cmd+=" ";
				} else {
					cmd.Empty();
				}
			}
		}
		break;
	case IDB_JP:
		cmd="/P ";
		break;
	case IDB_JR:
		cmd="/R ";
		break;
	case IDB_JDX1:
		barra2.GetDlgItem(IDC_JDX1)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>40 || n<1) {
				Traduce::Msg(this->m_hWnd,97,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				cmd.Insert(0,"/DXB"); cmd+=" ";
			}
		}
		break;
	case IDB_JDX2:
		barra2.GetDlgItem(IDC_JDX2)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			CString aux;
			barra2.GetDlgItem(IDC_JDX3)->GetWindowText(aux);
			int n=atoi(cmd.GetBuffer(0));
			int	m=atoi(aux.GetBuffer(0));
			if(n>4 || n<1) {
				Traduce::Msg(this->m_hWnd,98,91,MB_ICONWARNING);
				MessageBox("1-4");
				cmd.Empty();
			} else {
				if(n<3 && (m<1 || m>8)) {
					Traduce::Msg(this->m_hWnd,99,91,MB_ICONWARNING);
					cmd.Empty();
				} else if(n>2 && (m<1 || m>3)) {
					Traduce::Msg(this->m_hWnd,100,91,MB_ICONWARNING);
					cmd.Empty();
				} else {
					cmd.Insert(0,"/DXP");
					cmd+=aux.GetBuffer(0); cmd+=" ";
				}
			}
		}
		break;
	case IDB_JAS1:
		cmd="/AS1 ";
		break;
	case IDB_JAS2:
		cmd="/AS2 ";
		break;
	case IDB_JASX:
		cmd="/ASx ";
		break;
	case IDB_JASY:
		cmd="/ASy ";
		break;
	case IDB_JASR:
		cmd="/ASr ";
		break;
	case IDB_JASZ:
		cmd="/ASz ";
		break;
	case IDB_JASD:
		cmd="/ASD ";
		break;
	case IDB_JINC:
		barra2.GetDlgItem(IDC_JE1)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>16 || n<0) {
				Traduce::Msg(this->m_hWnd,101,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				if(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel()!=-1) {
					CString aux;
					cmd.Insert(0,"/INC");
					cmd+=" ";
					((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetLBText(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel(),aux);
					cmd+=aux.GetBuffer(0); cmd+=" ";
				} else {
					cmd.Empty();
				}
			}
		}
		break;
	case IDB_JDEC:
		barra2.GetDlgItem(IDC_JE1)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			int n=atoi(cmd.GetBuffer(0));
			if(n>16 || n<0) {
				Traduce::Msg(this->m_hWnd,101,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				if(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel()!=-1) {
					CString aux;
					cmd.Insert(0,"/DEC");
					cmd+=" ";
					((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetLBText(((CComboBox*)barra2.GetDlgItem(IDC_JMACROS))->GetCurSel(),aux);
					cmd+=aux.GetBuffer(0); cmd+=" ";
				} else {
					cmd.Empty();
				}
			}
		}
		break;
	case IDB_JB:
		barra2.GetDlgItem(IDC_JE2)->GetWindowText(cmd);
		if(!cmd.IsEmpty()) {
			CString aux;
			barra2.GetDlgItem(IDC_JE3)->GetWindowText(aux);
			int n=atoi(cmd.GetBuffer(0));
			if(n>15 || n<1) {
				Traduce::Msg(this->m_hWnd,96,91,MB_ICONWARNING);
				cmd.Empty();
			} else {
				n=atoi(aux.GetBuffer(0));
				if(n<1 || n>255) {
					Traduce::Msg(this->m_hWnd,102,91,MB_ICONWARNING);
					cmd.Empty();
				} else {
					cmd.Insert(0,"/B"); cmd+=" #";
					cmd+=aux.GetBuffer(0); cmd+=" ";
				}
			}
		}
		break;
	default: break;
	}

	if(!cmd.IsEmpty()) texto.ComandoJdf(cmd.GetBuffer(0));
}

void CVTexto::OnNotifyComJdf(UINT ID)
{
	switch(ID-1000)
	{
	case IDB_JELEMENTOS:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(138));
		break;
	case IDB_JK0:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(139));
		break;
	case IDB_JK1:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(140));
		break;
	case IDB_JM1:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(141));
		break;
	case IDB_JM2:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(142));
		break;
	case IDB_JM3:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(143));
		break;
	case IDB_JX1:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(144));
		break;
	case IDB_JX2:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(145));
		break;
	case IDB_JX3:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(146));
		break;
	case IDB_JT:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(147));
		break;
	case IDB_JP:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(148));
		break;
	case IDB_JR:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(149));
		break;
	case IDB_JDX1:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(150));
		break;
	case IDB_JDX2:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(151));
		break;
	case IDB_JAS1:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(152));
		break;
	case IDB_JAS2:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(153));
		break;
	case IDB_JASX:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(154));
		break;
	case IDB_JASY:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(155));
		break;
	case IDB_JASR:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(156));
		break;
	case IDB_JASZ:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(157));
		break;
	case IDB_JASD:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(158));
		break;
	case IDB_JINC:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(159));
		break;
	case IDB_JDEC:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(160));
		break;
	case IDB_JB:
		((CPrincipal*)this->GetParentFrame())->SetAyuda(Traduce::Txt(161));
		break;
	default: break;
	}
}

void CVTexto::OnEditCut(){	texto.Cortar();}
void CVTexto::OnEditCopy(){	texto.Copiar();}
void CVTexto::OnEditPaste(){texto.Pegar();}
void CVTexto::OnEditUndo(){	texto.Deshacer();}
void CVTexto::OnEditRedo(){	texto.Rehacer();}