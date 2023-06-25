// DlgBarra.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Editor.h"
#include "DlgBarra.h"
#include "DTexto.h"
#include "VTexto.h"


// CDlgBarra

IMPLEMENT_DYNAMIC(CDlgBarra, CDialogBar)
CDlgBarra::CDlgBarra(){}

int CDlgBarra::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	CWnd* bt=ChildWindowFromPoint(point,CWP_SKIPTRANSPARENT);
	if(bt!=NULL && bt!=(CWnd*)this){
		((CVTexto*)this->GetParent())->SendMessage(WM_COMMAND,bt->GetDlgCtrlID()+1000);
	} else {
		((CVTexto*)this->GetParent())->SendMessage(WM_COMMAND,2000);
	}

	return CDialogBar::OnToolHitTest(point,pTI);
}
