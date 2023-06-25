#pragma once

class CjoyConfig
{
public:
	CjoyConfig();

	bool EnumerarGameports(HWND hWnd,DWORD* idx);
	bool EnumerarJoysticks(HWND hWnd);
	bool Instalar(DWORD s_int,DWORD s_joy);
	bool Desinstalar();
private:
	bool GuardarRegistro(BYTE* ruta,BYTE* datos,DWORD tam);
	bool BorrarRegistro();
	bool LeerRegistro(char** interfaz,PVOID* puerto);
};