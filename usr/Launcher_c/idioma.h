#pragma once

class Traduce
{
public:
	static HRESULT Msg(HWND hWnd,short ID1, short ID2,UINT tipo);
	static void Dlg(HWND hWnd,short ID);
};