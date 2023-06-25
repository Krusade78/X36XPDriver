#include <windows.h>
#include "idioma.h"

char TextoTraducido[1024];

void Traduceid(short ID)
{
	typedef void (*FUNC)(short,char*);

	HMODULE dll=LoadLibrary("x36_lng.dll");
	if(dll==NULL) return;

	FUNC Traducir=(FUNC)GetProcAddress(dll,"TraduceGlobal");
	if(Traducir!=NULL) {
		Traducir(ID,TextoTraducido);
	}

	FreeLibrary(dll);
}

HRESULT Traduce::Msg(HWND hWnd,short ID1, short ID2,UINT tipo)
{
	char textoaux[1024];
	Traduceid(ID1);
	strcpy(textoaux,TextoTraducido);
	Traduceid(ID2);
	return MessageBox(hWnd,textoaux,TextoTraducido,tipo);
}

HRESULT Traduce::Msg(HWND hWnd,short ID1, const char* tit,UINT tipo)
{
	Traduceid(ID1);
	return MessageBox(hWnd,TextoTraducido,tit,tipo);
}