// Rich.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Rich.h"
#include "principal.h"
#include ".\rich.h"


#define DEFINE_GUIDXXX(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
EXTERN_C const GUID CDECL name \
= { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } }

DEFINE_GUIDXXX(IID_ITextDocument,0x8CC497C0,0xA1DF,0x11CE,0x80,0x98,
0x00,0xAA,0x00,0x47,0xBE,0x5D);

// CRich

IMPLEMENT_DYNAMIC(CRich, CRichEditCtrl)
CRich::CRich()
{
	sobreescribir=false;

}
CRich::~CRich()
{
	td->Release();
	ro->Release();
}

BEGIN_MESSAGE_MAP(CRich, CRichEditCtrl)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CRich::Iniciar()
{
	SendMessage(EM_GETOLEINTERFACE,0,(LPARAM)&ro);
	ro->QueryInterface(IID_ITextDocument,(void**)&td);
}

long CRich::GetPos()
{
	long a,b;
	this->GetSel(a,b);
	return b;
}

// Controladores de mensajes de CRich

void CRich::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar==9) {
		this->ReplaceSel("\t",TRUE);
	} else {
		CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CRich::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar==VK_INSERT) {
		sobreescribir=!sobreescribir;
		((CPrincipal*)this->GetParent()->GetParent()->GetParentFrame())->SetSobreescribir(sobreescribir);
	}
	long pos=this->GetPos();
	((CPrincipal*)this->GetParent()->GetParent()->GetParentFrame())->SetPosicion(LineFromChar(pos),pos-LineIndex(LineFromChar(pos)));
}

void CRich::OnSetFocus(CWnd* pOldWnd)
{
	CRichEditCtrl::OnSetFocus(pOldWnd);
	long pos=this->GetPos();
	((CPrincipal*)this->GetParent()->GetParent()->GetParentFrame())->SetSobreescribir(sobreescribir);
	((CPrincipal*)this->GetParent()->GetParent()->GetParentFrame())->SetPosicion(LineFromChar(pos),pos-LineIndex(LineFromChar(pos)));
}

void CRich::OnLButtonUp(UINT nFlags, CPoint point)
{
	long pos=this->GetPos();
	((CPrincipal*)this->GetParent()->GetParent()->GetParentFrame())->SetSobreescribir(sobreescribir);
	((CPrincipal*)this->GetParent()->GetParent()->GetParentFrame())->SetPosicion(LineFromChar(pos),pos-LineIndex(LineFromChar(pos)));

	CRichEditCtrl::OnLButtonUp(nFlags, point);
}

void CRich::GetTextRange(int a, int b,CString& st)
{
	CHARRANGE rg;
		rg.cpMin=a;
		rg.cpMax=b;
	TEXTRANGE r;
		r.chrg=rg;
		r.lpstrText=st.GetBuffer(b-a);
	
	this->SendMessage(EM_GETTEXTRANGE,0,(LPARAM)&r);
	st.ReleaseBuffer();
}

void CRich::FormatNumero(int i, int j)
{
	//CString c;
	CHARFORMAT cf;
	CHARRANGE p;

	cf.dwMask = CFM_COLOR; cf.dwEffects=0;
	cf.crTextColor = RGB(255,0,0);
	this->GetSel(p);
	//GetTextRange(r,i,j,c);
	this->SetSel(i,j);
	this->SetSelectionCharFormat(cf);
	//r->ReplaceSel(c.GetBuffer(0));
	this->SetSel(p);
}

void CRich::DesactivarUndo()
{
	td->Undo(tomSuspend,NULL);
	//this->LockWindowUpdate();
	this->HideSelection(TRUE,TRUE);
}

void CRich::ActivarUndo()
{
	td->Undo(tomResume,NULL);
	//this->UnlockWindowUpdate();
	this->HideSelection(FALSE,TRUE);
}
