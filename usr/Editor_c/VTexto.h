#pragma once
#include "DlgBarra.h"

// Marco de CVTexto

class CVTexto : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CVTexto)
protected:
	CVTexto();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CVTexto();

	DECLARE_MESSAGE_MAP()
public:
	CDlgBarra  barra2;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnClose(){}
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	CDTexto* GetDlg();
	void VerMdf();
	void VerJdf();
	void SetPlantilla(CString nombre);
	void GetPlantilla(CString& nombre);

private:
	CDlgBarra  barra1;
	CReBar		rebar;
	CDTexto		texto;
	CMenu		popup;
	CString		defPlantilla;

	void TraducirBarra1();
	void TraducirBarra2();
	void TraducirMenu();
	void BuscarPlantillas();
	void CargarPlantilla(short i);
	void OnPlantilla(UINT nID);
	void ComMdf(UINT nID);
	void ComJdf(UINT nID);
	void OnNotifyComMdf(UINT ID);
	void OnNotifyComJdf(UINT ID);
};


