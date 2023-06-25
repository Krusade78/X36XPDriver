// ProfileEditorTxt.h: archivo de encabezado principal para la aplicación ProfileEditorTxt
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Símbolos principales


// CProfileEditorTxtApp:
// Consulte la sección ProfileEditorTxt.cpp para obtener información sobre la implementación de esta clase
//

class CEditor : public CWinApp
{
public:
	CEditor();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnCompilar();
	afx_msg void OnLanzar();
	bool OnFileSave2();

	DECLARE_MESSAGE_MAP()
protected:
	HACCEL m_hMDIAccel;
	HMENU m_hMenu1,m_hMenu2;
private:
	CString defPlantilla;
	bool OnFileSaveAs2();
};

extern CEditor theApp;
