// DTexto.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Editor.h"
#include "DTexto.h"
#include "VTexto.h"
#include "idioma.h"
#include "principal.h"


// Cuadro de diálogo de DTexto

IMPLEMENT_DYNAMIC(CDTexto, CDialog)
CDTexto::CDTexto(CWnd* pParent /*=NULL*/)
	: CDialog(CDTexto::IDD, pParent)
{
	this->padre=pParent;
	n_rutaPerfil.Empty();
	saltar=false;
}

CDTexto::~CDTexto(){}

BOOL CDTexto::Create(CWnd* pParentWnd)
{
	this->padre=pParentWnd;
	n_rutaPerfil.Empty();

	return CDialog::Create(CDTexto::IDD, pParentWnd);
}

void CDTexto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, tab);
	DDX_Control(pDX, IDC_RICHEDIT21, mdfedit);
	DDX_Control(pDX, IDC_RICHEDIT22, jdfedit);
}


BEGIN_MESSAGE_MAP(CDTexto, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_EN_CHANGE(IDC_RICHEDIT21, OnMdfCambiar)
	ON_EN_CHANGE(IDC_RICHEDIT22, OnJdfCambiar)
END_MESSAGE_MAP()


// Controladores de mensajes de DTexto

BOOL CDTexto::OnInitDialog()
{
	CDialog::OnInitDialog();

	mdfedit.Iniciar();
	mdfedit.SetBackgroundColor(FALSE,RGB(220,220,255));
	mdfedit.SetEventMask(mdfedit.GetEventMask()|ENM_CHANGE);
	jdfedit.Iniciar();
	jdfedit.SetBackgroundColor(FALSE,RGB(220,235,220));
	jdfedit.SetEventMask(mdfedit.GetEventMask()|ENM_CHANGE);
	tab.InsertItem(0,Traduce::Txt(103));
	tab.InsertItem(1,Traduce::Txt(104));

	return FALSE;
}

void CDTexto::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(tab.m_hWnd!=NULL) {
		tab.SetWindowPos(&CWnd::wndBottom,0,0,cx,cy,SWP_NOOWNERZORDER);
		mdfedit.MoveWindow(5,24,cx-10,cy-23);
		jdfedit.MoveWindow(5,24,cx-10,cy-23);
	}
}

void CDTexto::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch(tab.GetCurSel())
	{
	case 0:
		jdfedit.ShowWindow(SW_HIDE);
		mdfedit.ShowWindow(SW_SHOW);
		((CVTexto*)padre)->VerMdf();
		mdfedit.SetFocus();
		break;
	case 1:
		mdfedit.ShowWindow(SW_HIDE);
		jdfedit.ShowWindow(SW_SHOW);
		CargarMacros();
		((CVTexto*)padre)->VerJdf();
		jdfedit.SetFocus();
	}
	*pResult = 0;	*pResult = 0;
}

void CDTexto::Cargar(CString ruta)
{
	CStdioFile f;
	CString ruta2=ruta.GetBuffer(0);
	CString linea;
	ruta2.Delete(ruta2.GetLength()-3,3);
	ruta2+="mdf";

	if(f.Open(ruta.GetBuffer(0),CFile::modeRead)==0) {
		Traduce::Msg(padre->m_hWnd,107,105,MB_ICONWARNING);
		return;
	}

	while(f.ReadString(linea))
	{
		jdfedit.ReplaceSel(linea.GetBuffer(0));
		saltar=true;
		jdfedit.ReplaceSel("\r");
		saltar=false;
	}
	f.Close();
	jdfedit.SetSel(jdfedit.GetTextLength()-2,jdfedit.GetTextLength()-1);
	jdfedit.ReplaceSel("");
	jdfedit.SetModify(FALSE);
	jdfedit.SetSel(0,0);

	if(f.Open(ruta2.GetBuffer(0),CFile::modeRead)==0) {
		Traduce::Msg(padre->m_hWnd,106,105,MB_ICONWARNING);
		jdfedit.Clear();
		jdfedit.SetModify(FALSE);
		return;
	}

	mdfedit.ShowWindow(SW_HIDE);
	while(f.ReadString(linea))
	{
		mdfedit.ReplaceSel(linea.GetBuffer(0));
		saltar=true;
		mdfedit.ReplaceSel("\r");
		saltar=false;
	}
	f.Close();
	mdfedit.SetSel(mdfedit.GetTextLength()-2,mdfedit.GetTextLength()-1);
	mdfedit.ReplaceSel("");
	mdfedit.ShowWindow(SW_SHOW);

	SetRutaPerfil(ruta);
	mdfedit.SetModify(FALSE);
	mdfedit.SetSel(0,0);
	mdfedit.SetFocus();

	CargarMacros();
}

bool CDTexto::Guardar()
{
	CString ruta1=n_rutaPerfil.GetBuffer(0);
	CString ruta2=n_rutaPerfil.GetBuffer(0);
	CFile f;
	int c=0;
	UCHAR buf[4096];
	ruta1+=".jdf";
	ruta2+=".mdf";

	if(f.Open(ruta1.GetBuffer(0),CFile::modeWrite|CFile::modeCreate)==0)
	{
		Traduce::Msg(padre->m_hWnd,107,105,MB_ICONWARNING);
		return false;
	}
	for(int i=0;i<jdfedit.GetLineCount();i++) {
		c=jdfedit.GetLine(i,(LPTSTR)buf,4095);
		if(i==(jdfedit.GetLineCount()-1)) {buf[c-1]=0; c--;}
		if(buf[c-1]=='\r') {buf[c]='\n', buf[c+1]=0; c++;}
		f.Write(buf,c);
	}
	f.Close();
	jdfedit.SetModify(FALSE);

	if(f.Open(ruta2.GetBuffer(0),CFile::modeWrite|CFile::modeCreate)==0)
	{
		Traduce::Msg(padre->m_hWnd,106,105,MB_ICONWARNING);
		return false;
	}
	for(int i=0;i<mdfedit.GetLineCount();i++) {
		c=mdfedit.GetLine(i,(LPTSTR)buf,4095);
		if(i==(mdfedit.GetLineCount()-1)) {buf[c-1]=0; c--;}
		if(buf[c-1]=='\r') {buf[c]='\n', buf[c+1]=0; c++;}
		f.Write(buf,c);
	}
	f.Close();
	mdfedit.SetModify(FALSE);

	return true;
}

bool CDTexto::EstaModificado()
{
	return (mdfedit.GetModify() || jdfedit.GetModify());
}

bool CDTexto::EsNuevo()
{
	if(n_rutaPerfil.IsEmpty()) return true; else return false;
}

void CDTexto::SetRutaPerfil(CString ruta)
{
	n_rutaPerfil=ruta.GetBuffer(0);
	n_rutaPerfil.Delete(n_rutaPerfil.GetLength()-4,4);
}
char* CDTexto::GetRutaPerfil()
{
	return n_rutaPerfil.GetBuffer(0);
}

void CDTexto::ComandoMdf(char* comando)
{
	mdfedit.ReplaceSel(comando,TRUE);
	mdfedit.SetFocus();
}
void CDTexto::ComandoJdf(char* comando)
{
	jdfedit.ReplaceSel(comando,TRUE);
	jdfedit.SetFocus();
}

void CDTexto::CargarMacros()
{
	CString nombre;
	((CComboBox*)((CVTexto*)padre)->barra2.GetDlgItem(IDC_JMACROS))->ResetContent();
	for(int i=0;i<(mdfedit.GetLineCount());i++) {
		if(i==(mdfedit.GetLineCount()-1)) {
			mdfedit.GetTextRange(mdfedit.LineIndex(i),mdfedit.GetTextLength(),nombre);
		} else {
			mdfedit.GetTextRange(mdfedit.LineIndex(i),mdfedit.LineIndex(i+1),nombre);
		}
		if(nombre.FindOneOf("=")!=-1 && (nombre.Find("//")==-1 || nombre.Find("//")>=nombre.FindOneOf("="))) {
			nombre.Delete(nombre.FindOneOf("="),nombre.GetLength()-nombre.FindOneOf("="));
			((CComboBox*)((CVTexto*)padre)->barra2.GetDlgItem(IDC_JMACROS))->AddString(nombre.GetBuffer(0));
		}
	}
	if(((CComboBox*)((CVTexto*)padre)->barra2.GetDlgItem(IDC_JMACROS))->GetCount()>0) {
		((CComboBox*)((CVTexto*)padre)->barra2.GetDlgItem(IDC_JMACROS))->SetCurSel(0);
	}
}

void CDTexto::OnMdfCambiar()
{
	if(saltar) return;

	CString c;
	CHARFORMAT cf;
	long ichar=mdfedit.LineIndex();
	long fchar=ichar+mdfedit.LineLength(mdfedit.LineIndex());
	CHARRANGE p;
	bool nombrem=false;

	mdfedit.DesactivarUndo();

	//GetTextRange(&mdfedit,ichar,fchar,c);
	//mdfedit.GetTextRange(ichar,fchar,c);
	mdfedit.GetSel(p);
	mdfedit.SetSel(ichar,fchar);
	mdfedit.GetDefaultCharFormat(cf);
	mdfedit.SetSelectionCharFormat(cf);
	//mdfedit.ReplaceSel(c.GetBuffer(0));
	//mdfedit.SetSel(p);

	for(long i=ichar;i<fchar;i++) {
		mdfedit.GetTextRange(i,i+2,c);
		if(c.Compare("//")==0) {
			cf.dwMask = CFM_COLOR|CFM_ITALIC;
			cf.dwEffects=CFE_ITALIC;
			cf.crTextColor = RGB(100,100,100);
			//mdfedit.GetSel(p);
			//GetTextRange(&mdfedit,i,fchar,c);
			//mdfedit.GetTextRange(i,fchar,c);
			mdfedit.SetSel(i,fchar);
			mdfedit.SetSelectionCharFormat(cf);
			//mdfedit.ReplaceSel(c.GetBuffer(0));
			//mdfedit.SetSel(p);
			break;
		}
		mdfedit.GetTextRange(i,i+1,c);
		if(c.Compare("=")==0 && !nombrem) {
				nombrem=true;
				cf.dwMask = CFM_BOLD;
				cf.dwEffects=CFE_BOLD;
				//mdfedit.GetSel(p);
				//GetTextRange(&mdfedit,ichar,i,c);
				//mdfedit.GetTextRange(ichar,i,c);
				mdfedit.SetSel(ichar,i);
				mdfedit.SetSelectionCharFormat(cf);
//				mdfedit.SetWordCharFormat(cf);
//				mdfedit.ReplaceSel(c.GetBuffer(0));
				//mdfedit.SetSel(p);
				continue;
		}
		mdfedit.GetTextRange(i,i+1,c);
		if(c.GetAt(0)>47 && c.GetAt(0)<58 && nombrem)
		{
			mdfedit.FormatNumero(i,i+1);
		}
		if(nombrem) {
			mdfedit.GetTextRange(i,i+4,c);
			if(c.CompareNoCase("keyu")==0 || c.CompareNoCase("keyd")==0 ||
				c.CompareNoCase("ms1u")==0 || c.CompareNoCase("ms1d")==0 ||
				c.CompareNoCase("ms2u")==0 || c.CompareNoCase("ms2d")==0 ||
				c.CompareNoCase("ms3u")==0 || c.CompareNoCase("ms3d")==0 ||
				c.CompareNoCase("msxl")==0 || c.CompareNoCase("msxr")==0 ||
				c.CompareNoCase("msyu")==0 || c.CompareNoCase("msyd")==0 ||
				c.CompareNoCase("hold")==0 || c.CompareNoCase("repf")==0 ||
				c.CompareNoCase("repi")==0 || c.CompareNoCase("repn")==0 ||
				c.CompareNoCase("dlay")==0 ||
				c.CompareNoCase("setm")==0 || c.CompareNoCase("setk")==0 || c.CompareNoCase("setx")==0) {
					i+=3;
					cf.dwMask = CFM_COLOR|CFM_BOLD;
					cf.dwEffects=CFE_BOLD;
					cf.crTextColor = RGB(0,0,255);
					//mdfedit.GetSel(p);
					//GetTextRange(&mdfedit,i-3,i+1,c);
					//mdfedit.GetTextRange(i-3,i+1,c);
					mdfedit.SetSel(i-3,i+1);
					mdfedit.SetSelectionCharFormat(cf);
//					mdfedit.ReplaceSel(c.GetBuffer(0));
					//mdfedit.SetSel(p);
			}
		}
	}

	mdfedit.SetSel(p);
	mdfedit.ActivarUndo();
}

void CDTexto::OnJdfCambiar()
{
	if(saltar) return;
	CString c;
	CHARFORMAT cf;
	long ichar=jdfedit.LineIndex();
	long fchar=ichar+jdfedit.LineLength(jdfedit.LineIndex());
	CHARRANGE p;
	bool nombrem=false;
	
	jdfedit.DesactivarUndo();

	//jdfedit.GetTextRange(ichar,fchar,c);
	jdfedit.GetSel(p);
	jdfedit.SetSel(ichar,fchar);
	jdfedit.GetDefaultCharFormat(cf);
	jdfedit.SetSelectionCharFormat(cf);
	//jdfedit.ReplaceSel(c.GetBuffer(0));
	//jdfedit.SetSel(p);

	for(long i=ichar;i<fchar;i++) {
		jdfedit.GetTextRange(i,i+2,c);
		if(c.Compare("//")==0) {
			cf.dwMask = CFM_COLOR|CFM_ITALIC;
			cf.dwEffects=CFE_ITALIC;
			cf.crTextColor = RGB(100,100,100);
			//jdfedit.GetSel(p);
			//jdfedit.GetTextRange(i,fchar,c);
			jdfedit.SetSel(i,fchar);
			jdfedit.SetSelectionCharFormat(cf);
			//jdfedit.ReplaceSel(c.GetBuffer(0));
			//jdfedit.SetSel(p);
			break;
		} else if(c.CompareNoCase("/p")==0 || c.CompareNoCase("/r")==0) {
			cf.dwMask = CFM_COLOR|CFM_BOLD;
			cf.dwEffects=CFE_BOLD;
			cf.crTextColor = RGB(0,0,255);
			//jdfedit.GetSel(p);
			//GetTextRange(&jdfedit,i,i+2,c);
			jdfedit.SetSel(i,i+2);
			jdfedit.SetSelectionCharFormat(cf);
			//jdfedit.ReplaceSel(c.GetBuffer(0));
			//jdfedit.SetSel(p);
			i+=2;
			continue;
		} else if(c.CompareNoCase("/t")==0 || c.CompareNoCase("/b")==0) {
			cf.dwMask = CFM_COLOR|CFM_BOLD;
			cf.dwEffects=CFE_BOLD;
			cf.crTextColor = RGB(0,0,255);
			//jdfedit.GetSel(p);
			//GetTextRange(&jdfedit,i,i+2,c);
			jdfedit.SetSel(i,i+2);
			jdfedit.SetSelectionCharFormat(cf);
			//jdfedit.ReplaceSel(c.GetBuffer(0));
			//jdfedit.SetSel(p);
			i+=2;
			jdfedit.GetTextRange(i,i+1,c);
			if(c.GetAt(0)>47 && c.GetAt(0)<58)
			{
				jdfedit.FormatNumero(i,i+1);
				i+=1;
				jdfedit.GetTextRange(i,i+1,c);
				if(c.GetAt(0)>47 && c.GetAt(0)<58)
				{
					jdfedit.FormatNumero(i,i+1);
					i+=1;
				}
			}
			continue;
		}

		jdfedit.GetTextRange(i,i+3,c);
		if(c.CompareNoCase("/k0")==0 || c.CompareNoCase("/k1")==0 ||
				c.CompareNoCase("/m1")==0 || c.CompareNoCase("/m2")==0 ||
				c.CompareNoCase("/m3")==0 || c.CompareNoCase("/x1")==0 ||
				c.CompareNoCase("/x2")==0 || c.CompareNoCase("/x3")==0) {
					cf.dwMask = CFM_COLOR|CFM_BOLD;
					cf.dwEffects=CFE_BOLD;
					cf.crTextColor = RGB(0,0,255);
					//jdfedit.GetSel(p);
					//GetTextRange(&jdfedit,i,i+2,c);
					jdfedit.SetSel(i,i+2);
					jdfedit.SetSelectionCharFormat(cf);
					//jdfedit.ReplaceSel(c.GetBuffer(0));
					//jdfedit.SetSel(p);
					jdfedit.FormatNumero(i+2,i+3);
					i+=3;
					continue;
		}

		jdfedit.GetTextRange(i,i+4,c);
		if(c.CompareNoCase("/inc")==0 || c.CompareNoCase("/dec")==0 || c.CompareNoCase("/dxb")==0) {
			cf.dwMask = CFM_COLOR|CFM_BOLD;
			cf.dwEffects=CFE_BOLD;
			cf.crTextColor = RGB(0,0,255);
			//jdfedit.GetSel(p);
			//GetTextRange(&jdfedit,i,i+4,c);
			jdfedit.SetSel(i,i+4);
			jdfedit.SetSelectionCharFormat(cf);
			//jdfedit.ReplaceSel(c.GetBuffer(0));
			//jdfedit.SetSel(p);
			i+=4;
			jdfedit.GetTextRange(i,i+1,c);
			if(c.GetAt(0)>47 && c.GetAt(0)<58)
			{
				jdfedit.FormatNumero(i,i+1);
				i+=1;
				jdfedit.GetTextRange(i,i+1,c);
				if(c.GetAt(0)>47 && c.GetAt(0)<58)
				{
					jdfedit.FormatNumero(i,i+1);
					i+=1;
				}
			}
			continue;
		} else if(c.CompareNoCase("/dxp")==0){
			cf.dwMask = CFM_COLOR|CFM_BOLD;
			cf.dwEffects=CFE_BOLD;
			cf.crTextColor = RGB(0,0,255);
			//jdfedit.GetSel(p);
			//GetTextRange(&jdfedit,i,i+4,c);
			jdfedit.SetSel(i,i+4);
			jdfedit.SetSelectionCharFormat(cf);
			//jdfedit.ReplaceSel(c.GetBuffer(0));
			//jdfedit.SetSel(p);
			i+=4;
			jdfedit.GetTextRange(i,i+2,c);
			if(c.GetAt(0)>47 && c.GetAt(0)<58 && c.GetAt(1)>47 && c.GetAt(1)<58)
			{
				jdfedit.FormatNumero(i,i+2);
				i+=2;
			}
			continue;
		} else if(c.CompareNoCase("/as1")==0 || c.CompareNoCase("/as2")==0 || c.CompareNoCase("/asd")==0 ||
			c.CompareNoCase("/asx")==0 ||c.CompareNoCase("/asy")==0 ||c.CompareNoCase("/asz")==0 ||
			c.CompareNoCase("/asr")==0 ) {
				cf.dwMask = CFM_COLOR|CFM_BOLD;
				cf.dwEffects=CFE_BOLD;
				cf.crTextColor = RGB(0,0,255);
				//jdfedit.GetSel(p);
				//GetTextRange(&jdfedit,i,i+3,c);
				jdfedit.SetSel(i,i+3);
				jdfedit.SetSelectionCharFormat(cf);
				//jdfedit.ReplaceSel(c.GetBuffer(0));
				//jdfedit.SetSel(p);
				jdfedit.FormatNumero(i+3,i+4);
				i+=4;
				continue;
		}
	}

	jdfedit.GetTextRange(ichar,ichar+3,c);
    if(c.CompareNoCase("btn")==0) {
		jdfedit.GetTextRange(ichar+3,ichar+6,c);
		if(c.GetAt(0)>47 && c.GetAt(0)<58) {
			if(c.GetAt(1)==32) {
				cf.dwMask = CFM_BOLD;
				cf.dwEffects=CFE_BOLD;
				//jdfedit.GetSel(p);
				//GetTextRange(&jdfedit,ichar,ichar+4,c);
				jdfedit.SetSel(ichar,ichar+4);
				jdfedit.SetSelectionCharFormat(cf);
				//jdfedit.ReplaceSel(c.GetBuffer(0));
				//jdfedit.SetSel(p);
			} else if(c.GetAt(1)>47 && c.GetAt(1)<58) {
				if(c.GetAt(2)==32) {
					cf.dwMask = CFM_BOLD;
					cf.dwEffects=CFE_BOLD;
					//jdfedit.GetSel(p);
					//GetTextRange(&jdfedit,ichar,ichar+5,c);
					jdfedit.SetSel(ichar,ichar+5);
					jdfedit.SetSelectionCharFormat(cf);
					//jdfedit.ReplaceSel(c.GetBuffer(0));
					//jdfedit.SetSel(p);
				}
			}
		}
	}
	jdfedit.GetTextRange(ichar,ichar+5,c);
	if(c.CompareNoCase("rot1 ")==0 || c.CompareNoCase("rot2 ")==0) {
		cf.dwMask = CFM_BOLD;
		cf.dwEffects=CFE_BOLD;
		//jdfedit.GetSel(p);
		//GetTextRange(&jdfedit,ichar,ichar+4,c);
		jdfedit.SetSel(ichar,ichar+4);
		jdfedit.SetSelectionCharFormat(cf);
		//jdfedit.ReplaceSel(c.GetBuffer(0));
		//jdfedit.SetSel(p);
	}
	jdfedit.GetTextRange(ichar,ichar+4,c);
	if(c.CompareNoCase("axis")==0) {
		jdfedit.GetTextRange(ichar+4,ichar+6,c);
		if(c.GetAt(0)>47 && c.GetAt(0)<58) {
			if(c.GetAt(1)==32) {
				cf.dwMask = CFM_BOLD;
				cf.dwEffects=CFE_BOLD;
				//jdfedit.GetSel(p);
				//GetTextRange(&jdfedit,ichar,ichar+5,c);
				jdfedit.SetSel(ichar,ichar+5);
				jdfedit.SetSelectionCharFormat(cf);
				//jdfedit.ReplaceSel(c.GetBuffer(0));
				//jdfedit.SetSel(p);
			}
		}
	}

	jdfedit.SetSel(p);
	jdfedit.ActivarUndo();
}

void CDTexto::Copiar()
{
	if(tab.GetCurSel()==0)
		mdfedit.Copy();
	else
		jdfedit.Copy();
}

void CDTexto::Cortar()
{
	if(tab.GetCurSel()==0)
		mdfedit.Cut();
	else
		jdfedit.Cut();
}

void CDTexto::Pegar()
{
	if(tab.GetCurSel()==0)
		mdfedit.Paste();
	else
		jdfedit.Paste();
}

void CDTexto::Deshacer()
{
	if(tab.GetCurSel()==0) {
		if(mdfedit.CanUndo() )
			mdfedit.Undo();
	} else {
		if(jdfedit.CanUndo() )
			jdfedit.Undo();
	}
}

void CDTexto::Rehacer()
{

	if(tab.GetCurSel()==0) {
		if(mdfedit.SendMessage(EM_CANREDO))
			mdfedit.SendMessage(EM_REDO);
	} else {
		if(jdfedit.SendMessage(EM_CANREDO))
			jdfedit.SendMessage(EM_REDO);
	}
}