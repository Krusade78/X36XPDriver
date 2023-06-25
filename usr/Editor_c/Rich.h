#pragma once
#include "tom.h"

// CRich

class CRich : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRich)

public:
	CRich();
	virtual ~CRich();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void GetTextRange(int a, int b,CString& st);
	void FormatNumero(int i, int j);
	void DesactivarUndo();
	void ActivarUndo();
	void Iniciar();
protected:
	DECLARE_MESSAGE_MAP()
private:
	bool sobreescribir;
	ITextDocument* td;
	IUnknown* ro;
	long GetPos();
};


