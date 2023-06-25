#pragma once

class Traduce
{
public:
	static HRESULT Msg(HWND hWnd,short ID1, const char* tit,UINT tipo);
};

/* IDs ajd_service */

	#define LG_ERROR1		13
	#define LG_ERROR2		14