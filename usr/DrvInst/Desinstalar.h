#pragma once

class CDesinstalar
{
public:
	CDesinstalar(HWND hWnd,HINSTANCE hInst);
	~CDesinstalar(void);
	bool Iniciar();
	bool VHID();
	bool Joystick();
	bool Joystick(DWORD idJoy);
	bool FiltroTeclado();
	bool Base();
//	bool Fin();
private:
//	bool fin;
	HWND hWnd;
	HINSTANCE hInst;
	void Error(char* tit);
};
