#pragma once

class Traduce
{
public:
	static HRESULT Msg(HWND hWnd,short ID1, short ID2,UINT tipo);
	static void Dlg(HWND hWnd,short ID);
	static HRESULT Msg(HWND hWnd,short ID1, const char* tit,UINT tipo);
};

/* IDs DrvInst */

	#define LG_UNINSTALL	0
	#define LG_REBOOTCAP	1
	#define LG_REBOOT		2
	#define LG_INSTCAP		3
	#define LG_INST_GB		4
	#define LG_INST0		5
	#define LG_INST1		6
	#define LG_INST2		7
	#define LG_GAMEPORT		8
	#define LG_ADVERT		9
	#define LG_SEL			10
	#define LG_TIMEOUT		11
	#define LG_INTERFAZ		12
	#define LG_ENUSO		13