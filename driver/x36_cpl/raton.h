#pragma once
#include "sliderctrl.h"

class CRaton
{
public:
	CRaton();
	void IniciarDlg1(HWND hWnd);
	void IniciarDlg2(HWND hWnd);
	void Cambiar();
	void OnCambiar();
	void OnScroll();
private:
	HWND dlg1,dlg2;
	CSliderCtrl slRefresco;
	short sref;

	void ActualizarDlg1();
	void ActualizarDlg2();
};