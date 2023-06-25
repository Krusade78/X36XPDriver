// X36NuevoDlg.h: Dialogo para crear un mapa nuevo
// Definición

#pragma once
#include "sliderctrl.h"

void TraducirDlgCalibraje(HWND hWnd);
void IniciarDlgCalibraje(IDirectInputDevice8* g_pd8, HINSTANCE hInst, HWND hWnd);


class CCalibraje
{
public:
	CCalibraje(IDirectInputDevice8* g_pd8);   // Constructor
	virtual ~CCalibraje();
	BOOL OnInitDialog(HWND hWnd);
	bool OnOK();
	void OnCancel();
	void OnTimer();
	void OnBnClickedBx();
	void OnBnClickedBy();
	void OnBnClickedBz();
	void OnBnClickedBr();
	void OnBnClickedBorrar();

private:
	typedef struct _ST_CALIBRADO {
		ULONG i;
		ULONG c;
		ULONG d;
		UCHAR n;
		UCHAR Margen;
	}CALIBRADO,*PCALIBRADO;

	HWND hWnd;
	IDirectInputDevice8* g_pJoystick;
	CALIBRADO Limite[4];
	struct {
		ULONG PosElegida;
		UCHAR PosRepetida;
	} jitter[4];
	ULONG EjesPosAnt[4];
	UCHAR nEjes;
	CSliderCtrl reglaxr,reglaxv;
	CSliderCtrl reglayr,reglayv;
	CSliderCtrl reglazr,reglazv;
	CSliderCtrl reglarr,reglarv;

	void LeerRegistro();
	bool IniciarDX();
	bool GuardarRegistro();
	void ReglasVirtuales(ULONG Axis[]);
	bool Comprobar(
							  LPCSTR eje,
							  UCHAR e,
							  int izq,
							  int cen,
							  int der,
							  int nul,
							  int vib,
							  int rmi,
							  int rma,
							  CSliderCtrl* regla);
	bool ActualizarX();
	bool ActualizarY();
	bool ActualizarZ();
	bool ActualizarR();
	void EnviarCalibrar();
};
