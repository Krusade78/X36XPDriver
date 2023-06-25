#pragma once
#include "rich.h"

// Cuadro de diálogo de DTexto

class CDTexto : public CDialog
{
	DECLARE_DYNAMIC(CDTexto)

public:
	CDTexto(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~CDTexto();
	virtual BOOL Create(CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMdfCambiar();
	afx_msg void OnJdfCambiar();
	bool EstaModificado();
	void Cargar(CString ruta);
	bool Guardar();
	void ComandoMdf(char* comando);
	void ComandoJdf(char* comando);
	bool EsNuevo();
	void SetRutaPerfil(CString ruta);
	char* GetRutaPerfil();
	void Copiar();
	void Pegar();
	void Cortar();
	void Deshacer();
	void Rehacer();
// Datos del cuadro de diálogo
	enum { IDD = IDD_TEXTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX o DDV

	DECLARE_MESSAGE_MAP()
private:
	CWnd* padre;
	CTabCtrl tab;
	CRich mdfedit;
	CRich jdfedit;
	CString n_rutaPerfil;
	bool saltar;

	void CargarMacros();
};
