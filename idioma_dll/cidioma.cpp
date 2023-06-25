#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Algunos constructores CString serán explícitos
#define _AFX_ALL_WARNINGS
#include "../../inc/mfc42/afxwin.h"         // Componentes principales y estándar de MFC
#include "cidioma.h"


void CIdioma::Traducir(bool ndrv,short ID,char* texto)
{
	char nombre[11];
	char ruta[1024];
	ZeroMemory(ruta,1024);
	CogerNombreIdioma(nombre);
	if(!BuscarArchivo(ndrv,nombre,ruta)) {texto=NULL; return;}
	if(!ExtraerCadena(ruta,ID,texto)) texto=NULL;
}

void CIdioma::CogerNombreIdioma(char* nombre)
{
	LANGID lid=GetUserDefaultLangID();
	switch(lid&0xFF)	{
		case 0x0c:
				strcpy(nombre,"french.h");
				break;
		case 0x10:
				strcpy(nombre,"italian.h");
				break;
		case 0x0a:
				strcpy(nombre,"spanish.h");
				break;
		case 0x07:
				strcpy(nombre,"german.h");
				break;
		default:
				strcpy(nombre,"english.h");
	}

}

bool CIdioma::BuscarArchivo(bool ndrv,char *idioma, char *ruta)
{
	HANDLE arch;
	bool pordefecto=false;

	if(ndrv) { // Buscar en el directorio actual
da:
		arch=CreateFile(idioma,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if(arch!=INVALID_HANDLE_VALUE) {
			CloseHandle(arch);
			if(GetCurrentDirectory(1013,ruta)==0) return false;
			strcat(ruta,"\\");
			strcat(ruta,idioma);
			return true;
		} else {
			if(!pordefecto) {
				strcpy(idioma,"english.h");
				pordefecto=true;
				goto da;
			} else {
				return false;
			}
		}
	} else { // Buscar en el directorio de sistema
ds:
		if(GetSystemDirectory(ruta,1004)==0) return false;
		strcat(ruta,"\\x36_");
		strcat(ruta,idioma);
		arch=CreateFile(ruta,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if(arch!=INVALID_HANDLE_VALUE) {
			CloseHandle(arch);
			return true;
		} else {
			if(!pordefecto) {
				strcpy(idioma,"english.h");
				pordefecto=true;
				goto ds;
			} else {
				return false;
			}
		}
	}
}

bool CIdioma::ExtraerCadena(char *ruta, short ID, char* texto)
{
//	if (!AfxWinInit(GetModuleHandle(NULL), NULL, NULL, 0)) return false;

	CStdioFile arch;
	CString linea;
	short lID;

	if(!arch.Open(ruta,CFile::modeRead)) return false;

	while(true) {
		try {
			if(!arch.ReadString(linea)) goto mal;
		} catch (CException* e) {goto mal;}
		if(linea.GetAt(0)=='#') {
			ZeroMemory(texto,1024);
			sscanf(linea.Left(linea.FindOneOf("=")).GetBuffer(0),"#%u=",&lID);
			linea.Delete(0,linea.FindOneOf("=")+1);
			linea.Replace("\\n","\n");
			strcpy(texto,linea.GetBuffer(0));
			if(lID==ID) break;
		}
	}
	arch.Close();
	return true;

mal:
	arch.Close();
	return false;
}