#pragma once

class CDInfo
{
public:
	CDInfo(	HWND hWnd, IDirectInputDevice8* pd8, IDirectInputJoyConfig8* pjc,CRaton* raton);
	virtual ~CDInfo();
	void CargarParteEstatica();
	void Actualizar();
private:
	IDirectInputDevice8*    pd8;
    IDirectInputJoyConfig8* pjc;
	HWND hWnd;
	LPVOID pEstado;
	CRaton* raton;

	char* PuntosC(DWORD gr,char* st);
	void InterrogarServicio();
	void ActivarX35();
};