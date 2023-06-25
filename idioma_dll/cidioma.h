#pragma once

class CIdioma
{
public:
	CIdioma(){}
	void Traducir(bool ndrv,short ID,char* texto);
private:
	void CogerNombreIdioma(char* nombre);
	bool BuscarArchivo(bool ndrv,char* idioma, char* ruta);
	bool ExtraerCadena(char* ruta, short ID, char* texto);
};