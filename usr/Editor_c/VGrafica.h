#pragma once

// Marco de CVTexto

class CVGrafica : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CVGrafica)
protected:
	CVGrafica();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CVGrafica();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnClose(){}
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


