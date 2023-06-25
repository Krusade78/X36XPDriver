// X36MapDlg.h: Dialogo principal de programa
// Definición

#pragma once

class CLauncherDlg : public CDialog
{
public:
	CLauncherDlg(CWnd* pParent = NULL);	// Constructor

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedActualizar();
	afx_msg void OnBnClickedCargar();
	afx_msg void OnBnClickedDefecto();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedEditar();

	DECLARE_MESSAGE_MAP()

private:
	CListBox listaMapas;
	void CargarPerfilDefecto();
	void CargarListaMapas();
	void CargarPerfil();
};
