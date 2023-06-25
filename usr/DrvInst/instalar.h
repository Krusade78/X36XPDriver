#pragma once

class CInstalar
{
public:
	CInstalar(HWND hWnd,HINSTANCE hInst);//,bool manual);
	~CInstalar(void);
	bool Iniciar(void);
	bool OnOk(LRESULT sel);
	void OnCancel();
	bool Fin();
private:
	//bool manual;
	HWND hWnd;
	HINSTANCE hInst;
	BYTE fase;
	bool fin;
	LRESULT s_interface,s_teclado,s_joystick;
//	BYTE s_rudder;
	bool InstalarArchivos();
	bool VHID();
	bool FiltroTeclado();
	bool Joystick(/*DWORD* idJoy*/);
	char PrepararDlgTeclados();
	char PrepararDlgInterfaces();
	bool PrepararDlgJoysticks();
	void Error(char* tit);
	void Error(char* tit,HRESULT res);
};
