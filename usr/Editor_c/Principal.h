// MainFrm.h: interfaz de la clase CMainFrame
//


#pragma once
class CPrincipal : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CPrincipal)
public:
	CPrincipal();
	virtual ~CPrincipal();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	void SetSobreescribir(bool sb);
	void SetPosicion(int lin,int col);
	void SetAyuda(LPCSTR texto);

protected:
	afx_msg BOOL Hints( UINT id, NMHDR* pTTTStruct, LRESULT *pResult );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateUI2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUI3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUI4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUI5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUI6(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	bool		sobreescribir;
	int			linea,columna;
};


