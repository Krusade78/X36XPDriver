/*--
Copyright (c) 2005 Alfredo Costalago

Module Name:

    Reordenar.h

Funcion : Funciones DirectX (clase)

--*/

#pragma once

class CReorden
{
public:
	CReorden(/*HINSTANCE hInst*/);
	virtual ~CReorden();
	bool Recalibrar(UCHAR* ejes);
private:
	typedef struct _ST_CALIBRADO {
		ULONG i;
		ULONG c;
		ULONG d;
		UCHAR n;
		UCHAR Margen;
	}CALIBRADO,*PCALIBRADO;

	HINSTANCE hInst;
	CALIBRADO datosEje[4];
	UCHAR nEjes;
	bool LeerRegistro();
	bool Comprobar(UCHAR eje);
	bool IniciarDX();
	bool GuardarDatos(UCHAR* ejes);
};
