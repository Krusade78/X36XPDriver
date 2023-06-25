#include <windows.h>
#include <winioctl.h>
#include <stdlib.h>
#include "reordenar.h"
#include "idioma.h"

#include "lanzador.h"

#define IOCTL_USR_NUEVACONF	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0102, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_USR_COMANDOS	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0103, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_USR_MAPA		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0104, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_X36_JOYINICIAR	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0101, METHOD_BUFFERED, FILE_ANY_ACCESS)

CLanzador::CLanzador(char *mdf,char* jdf)
{
	this->mdf=mdf;
	this->jdf=jdf;
	iniComandos=NULL;
	finComandos=NULL;
	ZeroMemory(&conf,sizeof(CONF));
	conf.MapaEjes[0]=0;
	conf.MapaEjes[1]=1;
	conf.MapaEjes[2]=2;
	conf.MapaEjes[3]=3;
	mapaBotones=(MBOTONES*)malloc(sizeof(MBOTONES)*2*3*3*2*36);
	if(mapaBotones!=NULL) ZeroMemory(mapaBotones,sizeof(MBOTONES)*2*3*3*2*36);
	mapaEjes=(MEJES*)malloc(sizeof(MEJES)*2*3*3*4);
	if(mapaEjes!=NULL) ZeroMemory(mapaEjes,sizeof(MEJES)*2*3*3*4);
	mapaToggles=(bool*)malloc(sizeof(bool)*2*3*3*4);
	if(mapaToggles!=NULL) ZeroMemory(mapaToggles,sizeof(bool)*2*3*3*4);
	acelPed=0;
}

CLanzador::~CLanzador()
{
	free(mapaBotones); mapaBotones=NULL;
	free(mapaEjes); mapaEjes=NULL;
	free(mapaToggles); mapaToggles=NULL;
	LimpiarMemoria();
}

void CLanzador::LimpiarMemoria()
{
	if(finComandos!=NULL) {
		PNODO nodo=iniComandos,n;
		while(nodo!=NULL) {
			n=nodo; nodo=n->link;
			free(n->acciones); n->acciones=NULL;
			free(n); n=NULL;
		}
		iniComandos=finComandos=NULL;
	}
}

/*******************************************************
				Método públicos
********************************************************/

bool CLanzador::Compilar()
{
	DWORD ret;
	INT64 posa=0;
	LONG high32,low32;
	bool eol;
	UINT32 nlinea=0;
	HANDLE arch;

	{
		char linea[4096];

		arch=CreateFile(mdf,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if(arch==INVALID_HANDLE_VALUE) {
			Traduce::Msg(NULL,LG_C_ERROR0,"CLanzador::Compilar",MB_ICONERROR);
			return false;
		}

		do {
			eol=false;
			if(!ReadFile(arch,linea,4096,&ret,0)) {
				Traduce::Msg(NULL,LG_C_ERROR1,"CLanzador::Compilar",MB_ICONERROR);
				goto mal1;
			}
			if(ret==0) break;
			for(UINT16 i=0;i<ret-1;i++) {
				if(linea[i]=='\r' && linea[i+1]=='\n') {
					nlinea++;
					eol=true;
					if(i==0 || (linea[0]=='/' && linea[1]=='/'))
					{} else {
						if(!ProcesarLineaM(linea,i)) goto mal2;
					}
					posa+=i+2;
					break;
				} 
			}
			if(!eol) {
				if(linea[0]=='/' && linea[1]=='/')
				{} else {
					if(!ProcesarLineaM(linea,(UINT16)ret)) goto mal2;
				}
				posa+=ret;
			}
			high32=(DWORD)(posa>>32);
			low32=(DWORD)(posa&0xffffffff);
			SetFilePointer(arch,low32,&high32,FILE_BEGIN);
		} while(true);

		CloseHandle(arch);
	}

	nlinea=0;
	{
		char linea[1024];
		posa=0;
		UCHAR btn=255,pinkie=0,modo=0,amodo=0,pulsar=1;

		arch=CreateFile(jdf,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if(arch==INVALID_HANDLE_VALUE) {
			Traduce::Msg(NULL,LG_C_ERROR100,"CLanzador::Compilar",MB_ICONERROR);
			return false;
		}

		do {
			eol=false;
			if(!ReadFile(arch,linea,1024,&ret,0)) {
				Traduce::Msg(NULL,LG_C_ERROR101,"CLanzador::Compilar",MB_ICONERROR);
				goto mal1;
			}
			if(ret==0) break;
			for(UINT16 i=0;i<ret-1;i++) {
				if(linea[i]=='\r' && linea[i+1]=='\n') {
					nlinea++;
					eol=true;
					if(i==0 || (linea[0]=='/' && linea[1]=='/'))
					{} else {
						if(!ProcesarLineaJ(linea,i,&btn,&pinkie,&modo,&amodo,&pulsar)) goto mal2;
					}
					posa+=i+2;
					break;
				}
			}
			if(!eol) {
				if(linea[0]=='/' && linea[1]=='/')
				{} else {
					if(!ProcesarLineaJ(linea,(UINT16)ret,&btn,&pinkie,&modo,&amodo,&pulsar)) goto mal2;
				}
				posa+=ret;
			}
			high32=(DWORD)(posa>>32);
			low32=(DWORD)(posa&0xffffffff);
			SetFilePointer(arch,low32,&high32,FILE_BEGIN);
		} while(true);

		CloseHandle(arch);

		if(!ComprobarBandas()) goto mal4;
	}

	return true;

mal4:
	LimpiarMemoria();
	return false;
/*mal3:
	CloseHandle(arch);
	LimpiarMemoria();
	{
		char tit[29]; char nlst[6];
		_itoa(nlinea,nlst,10);
		strcpy(tit,"CLanzador::Compilar (ln:");
		strcat(tit,nlst); strcat(tit,")");
		Traduce::Msg(NULL,LG_C_ERROR3,tit,MB_ICONWARNING);
	}
	return false;*/
mal2:
	CloseHandle(arch);
	LimpiarMemoria();
	{
		char tit[29]; char nlst[]={0,0,0,0,0,0};
		_itoa(nlinea,nlst,10);
		strcpy(tit,"CLanzador::Compilar (ln:");
		strcat(tit,nlst); strcat(tit,")");
		Traduce::Msg(NULL,LG_C_ERROR2,tit,MB_ICONINFORMATION);
	}
	return false;
mal1:
	CloseHandle(arch);
	LimpiarMemoria();
	return false;
}

bool CLanzador::Cargar()
{
	{
		CReorden ord;
		if(!ord.Recalibrar(conf.MapaEjes)) {
			Traduce::Msg(NULL,LG_C_ERROR5,"CLanzador::Cargar",MB_ICONERROR);
			return false;
		}
	}
	DWORD ret;
	HANDLE driver=CreateFile(
			"\\\\.\\X36HidInterface",
			GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if(driver==INVALID_HANDLE_VALUE) {
		Traduce::Msg(NULL,LG_ERROR1,"CLanzador::Cargar",MB_ICONERROR);
		return false;
	}

	{
		UINT32 tam=2;
		UINT16 idx=0;
		PNODO nodo=iniComandos;
		while(nodo!=NULL) {
			tam+=(nodo->acciones[0]*2)+1;
			nodo=nodo->link;
		}
		BYTE* bufferAux=(BYTE*)malloc(tam);
		if(bufferAux!=NULL) {
			tam=2;
			nodo=iniComandos;
			while(nodo!=NULL) {
				CopyMemory(&bufferAux[tam],nodo->acciones,1+(nodo->acciones[0]*2));
				tam+=(nodo->acciones[0]*2)+1;
				idx++;
				nodo=nodo->link;
			}
			*((UINT16*)bufferAux)=idx;
			LimpiarMemoria();

			if(!DeviceIoControl(driver,IOCTL_USR_COMANDOS,bufferAux,tam,NULL,0,&ret,NULL)) {
				free(bufferAux); bufferAux=NULL;
				CloseHandle(driver);
				Traduce::Msg(NULL,LG_ERROR2,"CLanzador::Cargar[0]",MB_ICONERROR);
				return false;
			}
			free(bufferAux); bufferAux=NULL;
		}
	}
	{
		BYTE* bufferAux=(BYTE*)malloc((sizeof(MBOTONES)*2*3*3*2*36)+(sizeof(MEJES)*2*3*3*4));
		if(bufferAux!=NULL) {
			CopyMemory(bufferAux,mapaBotones,sizeof(MBOTONES)*2*3*3*2*36);
			CopyMemory(bufferAux+(sizeof(MBOTONES)*2*3*3*2*36),mapaEjes,sizeof(MEJES)*2*3*3*4);
			if(!DeviceIoControl(driver,IOCTL_USR_MAPA,bufferAux,(sizeof(MBOTONES)*2*3*3*2*36)+(sizeof(MEJES)*2*3*3*4),0,0,&ret,NULL)) {
				free(bufferAux); bufferAux=NULL;
				CloseHandle(driver);
				Traduce::Msg(NULL,LG_ERROR2,"CLanzador::Cargar[1]",MB_ICONERROR);
				return false;
			}
			free(bufferAux); bufferAux=NULL;
		}
	}

	if(!DeviceIoControl(driver,IOCTL_USR_NUEVACONF,&conf,sizeof(CONF),NULL,0,&ret,NULL)) {
		CloseHandle(driver);
			Traduce::Msg(NULL,LG_ERROR2,"CLanzador::Cargar[2]",MB_ICONERROR);
		return false;
	}

	CloseHandle(driver);

	// Iniciar X36

	driver=CreateFile(
			"\\\\.\\X36KbInterface",
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	if(driver==INVALID_HANDLE_VALUE) {
		Traduce::Msg(NULL,LG_ERROR1,"CLanzador::Cargar[3]",MB_ICONWARNING);
		return false;
	}
	{
		UCHAR bufferAux=acelPed;
		if(!DeviceIoControl(driver,IOCTL_X36_JOYINICIAR,&bufferAux,1,&bufferAux,1,&ret,NULL)) {
			Traduce::Msg(NULL,LG_ERROR2,"CLanzador::Cargar[4]",MB_ICONWARNING);
		} else {
			if(bufferAux!=acelPed)
				Traduce::Msg(NULL,LG_X36INIT,"Saitek X36-F",MB_ICONINFORMATION);
		}
	}

	CloseHandle(driver);

	return true;
}

void CLanzador::CargarE(BYTE* mapaB, BYTE* mapaE,BYTE* confD,BOOLEAN* mapaAD)
{
	LimpiarMemoria();
	CopyMemory(mapaB,mapaBotones,(sizeof(MBOTONES)*2*3*3*2*36));
	CopyMemory(mapaE,mapaEjes,sizeof(MEJES)*2*3*3*4);
	CopyMemory(confD,&conf,sizeof(CONF));
	switch(acelPed) {
		case 0:
			mapaAD[0]=mapaAD[1]=TRUE;
			break;
		case 1:
			mapaAD[0]=FALSE; mapaAD[1]=TRUE;
			break;
		case 2:
			mapaAD[1]=FALSE; mapaAD[0]=TRUE;
			break;
		case 3:
			mapaAD[0]=mapaAD[1]=FALSE;
	}
}

/*******************************************************
				Método privados
********************************************************/

bool CLanzador::ProcesarLineaM(char* linea,UINT16 tam)
{
	char nombre[33];
	UCHAR acciones[513];
	UINT16 inicio=0;
	char comando[5]={0,0,0,0,0};
	char dato[4]={0,0,0,0};
	char posc=0;
	char posd=0;
	bool tcom=true,holds=false;
	UCHAR reps=0;

	ZeroMemory(nombre,33);
	ZeroMemory(acciones,513);

	// Comprobar nombre de macro
	for(UINT16 i=0;i<tam;i++)
	{
		if(linea[i]=='=') {
			inicio=i+1;
			break;
		} else {
			if(linea[i]==' ') {
				Traduce::Msg(NULL,LG_C_ERROR6,LG_C_CM,MB_ICONWARNING);
				return false;
			}
			if(i!=32) {
				nombre[i]=linea[i];
			} else {
				Traduce::Msg(NULL,LG_C_ERROR7,LG_C_CM,MB_ICONWARNING);
				return false;
			}
		}
	}
	// borrar espacios, tabs y comentarios
	for(UINT16 i=inicio;i<tam;i++)
	{
		if(linea[i]==' ' || linea[i]=='\t') {
			for(UINT16 k=i;k<(tam-1);k++) linea[k]=linea[k+1];
			i--;
			tam--;
		} else {
			if(i<(tam-1)) if(linea[i]=='/' && linea[i+1]=='/') {
				tam=i;
			}
		}
	}
	for(UINT16 i=inicio;i<tam;i++)
	{
		if(tcom) {
			comando[posc]=linea[i];
			if(posc==3) {
				posc=0;
				tcom=false;
				if((i+1)==tam) if(!ProcesarComandoM(comando,dato,acciones,&reps,&holds)) return false;
			} else {
				posc++;
			}
		} else {
			if(linea[i]<48 || linea[i]>57) {
				if(!ProcesarComandoM(comando,dato,acciones,&reps,&holds)) return false;
				tcom=true;
				posd=0;
				ZeroMemory(dato,3);
				i--;
			} else {
				dato[posd]=linea[i];
				if(posd==2 || (i+1)==tam) {
					if(!ProcesarComandoM(comando,dato,acciones,&reps,&holds)) return false;
					tcom=true;
					posd=0;
					ZeroMemory(dato,3);
				} else {
					posd++;
				}
			}
		}
	}

	if(reps!=0) {
		Traduce::Msg(NULL,LG_C_ERROR8,LG_C_CM,MB_ICONWARNING);
		return false;
	}

	// Comprobar nombre
	if(nombre[0]>47 && nombre[0]<58) {
		Traduce::Msg(NULL,LG_C_ERROR9,LG_C_CM,MB_ICONWARNING);
		return false;
	}
	
	{
		PNODO nodo=iniComandos;
		while(nodo!=NULL) {
			if(_strcmpi(nodo->nombre,nombre)==0) {
				Traduce::Msg(NULL,LG_C_ERROR10,LG_C_CM,MB_ICONWARNING);
				return false;
			} else {
				nodo=nodo->link;
			}
		}
	}
	
	// Añadir comando
	if(acciones[0]==0) {
		Traduce::Msg(NULL,LG_C_ERROR11,LG_C_CM,MB_ICONWARNING);
		return false;
	}

	PNODO nodo=(PNODO)malloc(sizeof(NODO));
	if(nodo!=NULL) {
		CopyMemory(nodo->nombre,nombre,33);
		nodo->acciones=(UCHAR*)malloc((acciones[0]*2)+1);
		if(nodo->acciones==NULL) {
			free(nodo); nodo=NULL;
		} else {
			CopyMemory(nodo->acciones,acciones,(acciones[0]*2)+1);
			nodo->link=NULL;
			if(finComandos==NULL) {
				iniComandos=finComandos=nodo;
			} else {
				finComandos->link=nodo;
				finComandos=nodo;
			}
		}
	}

	return true;
}

bool CLanzador::ProcesarLineaJ(char* linea, UINT16 tam,UCHAR* btn,UCHAR* pinkie,UCHAR* modo,UCHAR* amodo,UCHAR* pulsar)
{
	char buffer[33];
	UINT16 inipos=0;

	ZeroMemory(buffer,33);

	// Borrar espacios y comentarios
	for(UINT16 i=0;i<tam;i++)
	{
		if(linea[i]==' ' || linea[i]=='\t') {
			for(UINT16 k=i;k<(tam-1);k++) linea[k]=linea[k+1];
			i--;
			tam--;
		} else {
			if(i<(tam-1)) if(linea[i]=='/' && linea[i+1]=='/') {
				tam=i;
			}
		}
	}
	if(tam==0) return true;

	if(linea[0]!='/') {
		if(tam<4) {
			Traduce::Msg(NULL,LG_C_ERROR12,LG_C_CJ,MB_ICONWARNING);
			return false;
		}
		CopyMemory(buffer,linea,3);
		if(_strcmpi(buffer,"BTN")==0) {
			UCHAR boton;
			buffer[0]=linea[3];
			buffer[2]=0;
			if(tam>4) {
				if(linea[4]<48 || linea[4]>57) {
					buffer[1]=0;
					inipos=4;
				} else {
					buffer[1]=linea[4];
					inipos=5;
				}
			} else {
				buffer[1]=0;
				inipos=4;
			}
			boton=(UCHAR)atoi(buffer);
			if(boton<1 || boton>36) {
				Traduce::Msg(NULL,LG_C_ERROR13,LG_C_CJ,MB_ICONWARNING);
				return false;
			} else {
				*btn=boton;
				*pinkie=0; *modo=0;	*amodo=0; *pulsar=1;
			}
		} else if(_strcmpi(buffer,"ROT")==0) {
			if(linea[3]!='1' && linea[3]!='2') {
				Traduce::Msg(NULL,LG_C_ERROR14,LG_C_CJ,MB_ICONWARNING);
				return false;
			} else {
				buffer[0]=linea[3]; buffer[1]=0;
				*btn=((UCHAR)atoi(buffer))+68;
				*pinkie=0; *modo=0;	*amodo=0; *pulsar=1;
				inipos=4;
			}
		} else {
			buffer[3]=linea[3];
			if(_strcmpi(buffer,"AXIS")==0) {
				if(tam<5) {
					Traduce::Msg(NULL,LG_C_ERROR15,LG_C_CJ,MB_ICONWARNING);
					return false;
				}
				if(linea[4]!='1' && linea[4]!='2' && linea[4]!='3' && linea[4]!='4') {
					Traduce::Msg(NULL,LG_C_ERROR16,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					buffer[0]=linea[4]; buffer[1]=0;
					*btn=((UCHAR)atoi(buffer))+64;
					*pinkie=0; *modo=0;	*amodo=0; *pulsar=1;
					inipos=5;
				}
			} else {
				Traduce::Msg(NULL,LG_C_ERROR12,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
		}
	}


	bool macro=true;
	char comando[6]={0,0,0,0,0,0};
	char posn=0,posc=0;
	char tipo=0;

	ZeroMemory(buffer,33);
	for(UINT16 i=inipos;i<tam;i++) {
		if(linea[i]=='/') {
			macro=false;
		}
		if(macro) {
			if(posn==32) {
				Traduce::Msg(NULL,LG_C_ERROR7,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			buffer[posn]=linea[i];
			posn++;
			if((i+1)==tam) if(!ProcesarMacro(buffer,*btn,*pinkie,*modo,*amodo,*pulsar,tipo)) return false;
		} else {
			if(posn!=0) { //hay una macro sin procesar
				if(!ProcesarMacro(buffer,*btn,*pinkie,*modo,*amodo,*pulsar,tipo)) return false;
				tipo=0;
				ZeroMemory(buffer,33);
				posn=0;
			}
			char sig=0;
			comando[posc]=linea[i]; posc++;
			if((i+1)<tam) sig=linea[i+1];
			if(!ProcesarComandoJ(comando,sig,*btn,pinkie,modo,amodo,pulsar,&tipo,&macro)) return false;
			if(macro) {
				ZeroMemory(comando,6);
				posc=0;
			}
		}
	}

	return true;
}

//==================================================================
//==================================================================

bool CLanzador::ProcesarComandoM(char* comando,char* dato,UCHAR* acciones,UCHAR* reps,bool* holds)
{
	int idato=0;
	bool sinDato=(dato[0]==0)?true:false;
	UCHAR accionId;

	if(!sinDato) { idato=atoi(dato);}

	if(_strcmpi(comando,"KEYU")==0) {
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR17,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<0 || idato>232) {
			Traduce::Msg(NULL,LG_C_ERROR18,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=32;
	} else if(_strcmpi(comando,"KEYD")==0){
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR19,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<0 || idato>232) {
			Traduce::Msg(NULL,LG_C_ERROR20,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=0;
	} else if(_strcmpi(comando,"DXBU")==0) {
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR81,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<1 || idato>40) {
			Traduce::Msg(NULL,LG_C_ERROR81,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=48;
	} else if(_strcmpi(comando,"DXBD")==0){
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR81,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<1 || idato>40) {
			Traduce::Msg(NULL,LG_C_ERROR81,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=16;
	} else if(_strcmpi(comando,"DXPU")==0) {
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<11 || idato>48) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		} else if(idato>29 && ((idato%10)>4 || (idato%10)==0)) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		} else if(idato<30 && ((idato%10)==9 || (idato%10)==0)) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=49;
	} else if(_strcmpi(comando,"DXPD")==0){
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<11 || idato>48) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		} else if(idato>29 && ((idato%10)>4 || (idato%10)==0)) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		} else if(idato<30 && ((idato%10)==9 || (idato%10)==0)) {
			Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=17;
	} else if(_strcmpi(comando,"MS1U")==0){
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR21,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=33;
	} else if(_strcmpi(comando,"MS1D")==0){
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR22,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=1;
	} else if(_strcmpi(comando,"MS2U")==0){
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR23,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=34;
	} else if(_strcmpi(comando,"MS2D")==0){
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR24,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=2;
	} else if(_strcmpi(comando,"MS3U")==0){
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR25,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=35;
	} else if(_strcmpi(comando,"MS3D")==0){
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR26,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=3;
	} else if(_strcmpi(comando,"MSXL")==0){
		if(sinDato)  {
			Traduce::Msg(NULL,LG_C_ERROR27,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<0)  {
			Traduce::Msg(NULL,LG_C_ERROR28,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato==0) accionId=36; else accionId=4;
	} else if(_strcmpi(comando,"MSXR")==0){
		if(sinDato)  {
			Traduce::Msg(NULL,LG_C_ERROR29,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<0)  {
			Traduce::Msg(NULL,LG_C_ERROR30,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato==0) accionId=37; else accionId=5;
	} else if(_strcmpi(comando,"MSYU")==0){
		if(sinDato)   {
			Traduce::Msg(NULL,LG_C_ERROR31,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<0)   {
			Traduce::Msg(NULL,LG_C_ERROR32,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato==0) accionId=38; else accionId=6;
	} else if(_strcmpi(comando,"MSYD")==0){
		if(sinDato)  {
			Traduce::Msg(NULL,LG_C_ERROR33,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<0)  {
			Traduce::Msg(NULL,LG_C_ERROR34,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato==0) accionId=39; else accionId=7;
	} else if(_strcmpi(comando,"DLAY")==0){
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR35,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<1 || idato>250)  {
			Traduce::Msg(NULL,LG_C_ERROR36,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=8;
	} else if(_strcmpi(comando,"HOLD")==0){
		if(*holds) {
			Traduce::Msg(NULL,LG_C_ERROR37,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR38,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		*holds=true;
		accionId=9;
	} else if(_strcmpi(comando,"REPI")==0){
		if(*holds) {
			Traduce::Msg(NULL,LG_C_ERROR39,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR40,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		(*reps)++;
		accionId=10;
	} else if(_strcmpi(comando,"REPN")==0){
		if(*holds) {
			Traduce::Msg(NULL,LG_C_ERROR41,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR42,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<1 || idato>100) {
			Traduce::Msg(NULL,LG_C_ERROR43,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		(*reps)++;
		accionId=11;
	} else if(_strcmpi(comando,"REPF")==0){
		if(*reps==0) {
			Traduce::Msg(NULL,LG_C_ERROR44,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(!sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR45,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		(*reps)--;
		accionId=43;
	} else if(_strcmpi(comando,"SETM")==0){
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR46,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<1 || idato>3) {
			Traduce::Msg(NULL,LG_C_ERROR47,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=12;
		idato--;
	} else if(_strcmpi(comando,"SETX")==0){
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR48,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<1 || idato>3) {
			Traduce::Msg(NULL,LG_C_ERROR49,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=13;
		idato--;
	} else if(_strcmpi(comando,"SETK")==0){
		if(sinDato) {
			Traduce::Msg(NULL,LG_C_ERROR50,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		if(idato<0 || idato>1) {
			Traduce::Msg(NULL,LG_C_ERROR51,LG_C_CM,MB_ICONWARNING);
			return false;
		}
		accionId=14;
	} else {
		Traduce::Msg(NULL,LG_C_ERROR52,LG_C_CM,MB_ICONWARNING);
		return false;
	}

	if(acciones[0]==255){
		Traduce::Msg(NULL,LG_C_ERROR53,LG_C_CM,MB_ICONWARNING);
		return false;
	}
	acciones[(2*acciones[0])+1]=accionId;
	acciones[(2*acciones[0])+2]=(UCHAR)idato;
	acciones[0]+=1;
	return true;
}

int CLanzador::GetPosB(UCHAR a, UCHAR b, UCHAR c, UCHAR d, UCHAR e)
{ 
	return (a*648)+(b*216)+(c*72)+(d*36)+e;
}
int CLanzador::GetPosE(UCHAR a, UCHAR b, UCHAR c, UCHAR d)
{
	return (a*36)+(b*12)+(c*4)+d;
}
bool CLanzador::ProcesarComandoJ(char* comando,char sig,UCHAR btn,UCHAR* pinkie,UCHAR* modo,UCHAR* amodo,UCHAR* pulsar,char* tipo,bool* macro)
{
	char buf[7]={0,0,0,0,0,0,0};
	if(comando[1]==0) {
		return true;
	} else

	if(comando[2]==0) {
		buf[0]=comando[0]; buf[1]=comando[1];
		if(_strcmpi(buf,"/P")==0) {
			if(btn<1 || btn>40) {
				Traduce::Msg(NULL,LG_C_ERROR54,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			*pulsar=1;
			*macro=true;
		} else if(_strcmpi(buf,"/R")==0) {
			if(btn<1 || btn>40) {
				Traduce::Msg(NULL,LG_C_ERROR55,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			*pulsar=0;
			*macro=true;
		}
	} else

	if(comando[3]==0) {
		buf[0]=comando[0]; buf[1]=comando[1];
		if(_strcmpi(buf,"/K")==0) {
			if(btn==65 || btn==66)  {
				Traduce::Msg(NULL,LG_C_ERROR56,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			switch(comando[2]) {
				case '0': *pinkie=0; *macro=true; break;
				case '1': *pinkie=1; *macro=true; break;
				default: {
					Traduce::Msg(NULL,LG_C_ERROR57,LG_C_CJ,MB_ICONWARNING);
					return false;
				}
			}
		} else if(_strcmpi(buf,"/M")==0) {
			if(btn==65 || btn==66){
				Traduce::Msg(NULL,LG_C_ERROR56,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			switch(comando[2]) {
				case '1': *modo=0; *macro=true; break;
				case '2': *modo=1; *macro=true; break;
				case '3': *modo=2; *macro=true; break;
				default: {
					Traduce::Msg(NULL,LG_C_ERROR58,LG_C_CJ,MB_ICONWARNING);
					return false;
				}
			}
		} else if(_strcmpi(buf,"/X")==0) {
			if(btn==65 || btn==66){
				Traduce::Msg(NULL,LG_C_ERROR56,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			switch(comando[2]) {
				case '1': *amodo=0; *macro=true; break;
				case '2': *amodo=1; *macro=true; break;
				case '3': *amodo=2; *macro=true; break;
				default: {
					Traduce::Msg(NULL,LG_C_ERROR59,LG_C_CJ,MB_ICONWARNING);
					return false;
				}
			}
		} else if(_strcmpi(buf,"/T")==0) {
			if(btn==65 || btn==66){
				Traduce::Msg(NULL,LG_C_ERROR56,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			if(sig<48 || sig>57) {
				buf[2]=comando[2];
				char t=(char)atoi(&buf[2]);
				if(t==0) {
					Traduce::Msg(NULL,LG_C_ERROR60,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					*tipo=t-1;
					*macro=true;
				}
			}
		} else if(_strcmpi(buf,"/B")==0) {
			if(btn<67) {
				Traduce::Msg(NULL,LG_C_ERROR62,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			if(sig<48 || sig>57) {
				buf[2]=comando[2];
				char b=(char)atoi(&buf[2]);
				if(b==0) {
					Traduce::Msg(NULL,LG_C_ERROR61,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					*tipo=b+100;
					*macro=true;
				}
			}
		}
	} else

	if(comando[4]==0) {
		buf[0]=comando[0]; buf[1]=comando[1];
		if(_strcmpi(buf,"/T")==0) {
			buf[0]=comando[2]; buf[1]=comando[3];
			char t=(char)atoi(buf);
			if(t<1 || t>16) {
				Traduce::Msg(NULL,LG_C_ERROR60,LG_C_CJ,MB_ICONWARNING);
				return false;
			} else {
				*tipo=t-1;
				*macro=true;
			}
		} else if(_strcmpi(buf,"/B")==0) {
			buf[0]=comando[2]; buf[1]=comando[3];
			char b=(char)atoi(buf);
			if(b<1 || b>15){
				Traduce::Msg(NULL,LG_C_ERROR61,LG_C_CJ,MB_ICONWARNING);
				return false;
			} else {
				*tipo=b+100;
				*macro=true;
			}
		} else {
			buf[2]=comando[2];
			if(_strcmpi(buf,"/AS")==0) {
				_strupr(&comando[3]);
				switch(comando[3]) {
					case '1':
						if(btn<69) {
							Traduce::Msg(NULL,LG_C_ERROR63,LG_C_CJ,MB_ICONWARNING);
							return false;
						} else {
							if(btn==69) {
								if((conf.MapaRotatorios&3)!=0){
									Traduce::Msg(NULL,LG_C_ERROR65,LG_C_CJ,MB_ICONWARNING);
									return false;
								}
								conf.MapaRotatorios|=1;
							} else {
								if((conf.MapaRotatorios>>2)!=0){
									Traduce::Msg(NULL,LG_C_ERROR66,LG_C_CJ,MB_ICONWARNING);
									return false;
								}
								conf.MapaRotatorios|=4;
							}
							break;
						}
					case '2':
						if(btn<69) {
							Traduce::Msg(NULL,LG_C_ERROR64,LG_C_CJ,MB_ICONWARNING);
							return false;
						} else {
							if(btn==69) {
								if((conf.MapaRotatorios&3)!=0){
									Traduce::Msg(NULL,LG_C_ERROR65,LG_C_CJ,MB_ICONWARNING);
									return false;
								}
								conf.MapaRotatorios|=2;
							} else {
								if((conf.MapaRotatorios>>2)!=0){
									Traduce::Msg(NULL,LG_C_ERROR66,LG_C_CJ,MB_ICONWARNING);
									return false;
								}
								conf.MapaRotatorios|=8;
							}
							break;
						}
					case 'X':
						if(btn>68 || btn<65) {
							Traduce::Msg(NULL,LG_C_ERROR67,LG_C_CJ,MB_ICONWARNING);
							return false;
						} else {
							conf.MapaEjes[btn-65]=0;
							break;
						}
					case 'Y':
						if(btn>68 || btn<65) {
							Traduce::Msg(NULL,LG_C_ERROR68,LG_C_CJ,MB_ICONWARNING);
							return false;
						} else {
							conf.MapaEjes[btn-65]=1;
							break;
						}
					case 'Z':
						if(btn>68 || btn<65) {
							Traduce::Msg(NULL,LG_C_ERROR69,LG_C_CJ,MB_ICONWARNING);
							return false;
						} else {
							conf.MapaEjes[btn-65]=2;
							break;
						}
					case 'R':
						if(btn>68 || btn<65) {
							Traduce::Msg(NULL,LG_C_ERROR70,LG_C_CJ,MB_ICONWARNING);
							return false;
						} else {
							conf.MapaEjes[btn-65]=3;
							break;
						}
					case 'D':
						if(btn!=67 && btn!=68) {
							Traduce::Msg(NULL,LG_C_ERROR97,LG_C_CJ,MB_ICONWARNING);
							return false;
						} else {
							if(btn==67 && (acelPed&2)==2) {
								Traduce::Msg(NULL,LG_C_ERROR98,LG_C_CJ,MB_ICONWARNING);
							} else if(btn==68 && (acelPed&1)==1) {
								Traduce::Msg(NULL,LG_C_ERROR99,LG_C_CJ,MB_ICONWARNING);
							} else {
								if(btn==67) acelPed|=2; else acelPed|=1;
							}
							break;
						}
					default: {
						Traduce::Msg(NULL,LG_C_ERROR71,LG_C_CJ,MB_ICONWARNING);
						return false;
					}
				}
				*macro=true;
			}
		}
	} else

	if(comando[5]==0) {
		CopyMemory(buf,comando,4);
		_strupr(buf);
		if(_strcmpi(buf,"/INC")==0) {
			if(btn<67){
				Traduce::Msg(NULL,LG_C_ERROR72,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			if(sig<48 || sig>57) {
				buf[4]=comando[4];
				char p=(char)atoi(&buf[4]);
				if(p<0 || p>16){
					Traduce::Msg(NULL,LG_C_ERROR73,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					if(mapaToggles[GetPosE(*pinkie,*modo,*amodo,btn-67)]) {
						Traduce::Msg(NULL,LG_C_ERROR74,LG_C_CJ,MB_ICONWARNING);
						return false;
					} else {
						mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Incremental=TRUE;
						mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Indices[3]=p;
						*tipo=32;
						*macro=true;
					}
				}
			}
		} else if(_strcmpi(buf,"/DEC")==0) {
			if(btn<67) {
				Traduce::Msg(NULL,LG_C_ERROR75,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			if(sig<48 || sig>57) {
				buf[4]=comando[4];
				char p=(char)atoi(&buf[4]);
				if(p<0 || p>16){
					Traduce::Msg(NULL,LG_C_ERROR76,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					if(mapaToggles[GetPosE(*pinkie,*modo,*amodo,btn-67)]) {
						Traduce::Msg(NULL,LG_C_ERROR77,LG_C_CJ,MB_ICONWARNING);
						return false;
					} else {
						mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Incremental=TRUE;
						mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Indices[2]=p;
						*tipo=33;
						*macro=true;
					}
				}
			}
		}/* else if(_strcmpi(buf,"/DXB")==0) {
			if(btn>40) {
				Traduce::Msg(NULL,LG_C_ERROR78,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			if(conf.MapaBotones[0][btn-1]!=0) {
				Traduce::Msg(NULL,LG_C_ERROR79,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			if(sig<48 || sig>57) {
				buf[4]=comando[4];
				char b=(char)atoi(&buf[4]);
				if(b==0) {
					Traduce::Msg(NULL,LG_C_ERROR81,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					conf.MapaBotones[0][btn-1]=b;
					*macro=true;
				}
			}
		}*/
	} else {
		CopyMemory(buf,comando,4);
		_strupr(buf);
		if(_strcmpi(buf,"/INC")==0) {
			buf[4]=comando[4]; buf[5]=comando[5];
			char p=(char)atoi(&buf[4]);
			if(p<0 || p>16) {
					Traduce::Msg(NULL,LG_C_ERROR73,LG_C_CJ,MB_ICONWARNING);
					return false;
			} else {
				if(mapaToggles[GetPosE(*pinkie,*modo,*amodo,btn-67)]) {
					Traduce::Msg(NULL,LG_C_ERROR74,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Incremental=TRUE;
					mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Indices[3]=p;
					*tipo=32;
					*macro=true;
				}
			}
		} else if(_strcmpi(buf,"/DEC")==0) {
			buf[4]=comando[4]; buf[5]=comando[5];
			char p=(char)atoi(&buf[4]);
			if(p<0 || p>16) {
				Traduce::Msg(NULL,LG_C_ERROR76,LG_C_CJ,MB_ICONWARNING);
				return false;
			} else {
				if(mapaToggles[GetPosE(*pinkie,*modo,*amodo,btn-67)]) {
					Traduce::Msg(NULL,LG_C_ERROR77,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Incremental=TRUE;
					mapaEjes[GetPosE(*pinkie,*modo,*amodo,btn-67)].Indices[2]=p;
					*tipo=33;
					*macro=true;
				}
			}
		}/* else if(_strcmpi(buf,"/DXB")==0) {
			if(conf.MapaBotones[0][btn-1]!=0) {
				Traduce::Msg(NULL,LG_C_ERROR79,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			buf[4]=comando[4]; buf[5]=comando[5];
			char b=(char)atoi(&buf[4]);
			if(b<1 || b>40) {
				Traduce::Msg(NULL,LG_C_ERROR81,LG_C_CJ,MB_ICONWARNING);
				return false;
			} else {
				conf.MapaBotones[0][btn-1]=b;
				*macro=true;
			}
		} else if(_strcmpi(buf,"/DXP")==0) {
			if(btn>40)  {
				Traduce::Msg(NULL,LG_C_ERROR82,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			if(conf.MapaBotones[1][btn-1]!=0){
				Traduce::Msg(NULL,LG_C_ERROR83,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
			buf[4]=comando[4];
			char s=(char)atoi(&buf[4]);
			buf[4]=comando[5];
			char p=(char)atoi(&buf[4]);
			if((s>0 && s<3) && (p>0 && p<9) ) {
				conf.MapaBotones[1][btn-1]=(s-1)|(4*p);
				*macro=true;
			} else if((s>2 && s<5) && (p>0 && p<5) ) {
				conf.MapaBotones[1][btn-1]=(s-1)|(4*p);
				*macro=true;
			} else {
				Traduce::Msg(NULL,LG_C_ERROR84,LG_C_CJ,MB_ICONWARNING);
				return false;
			}
		}*/ else {
			Traduce::Msg(NULL,LG_C_ERROR71,LG_C_CJ,MB_ICONWARNING);
			return false;
		}
	}


	return true;
}

bool CLanzador::ProcesarMacro(char* nombre,UCHAR btn,UCHAR pinkie,UCHAR modo,UCHAR amodo,UCHAR pulsar,char tipo)
{
	if(tipo>100) {
		if(nombre[0]!='#' || strlen(nombre)>4){
			Traduce::Msg(NULL,LG_C_ERROR80,LG_C_CJ,MB_ICONWARNING);
			return false;
		}
		int banda=atoi(&nombre[1]);
		if(banda<1 || banda>255) {
			Traduce::Msg(NULL,LG_C_ERROR80,LG_C_CJ,MB_ICONWARNING);
			return false;
		}
		if(mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Incremental){
			Traduce::Msg(NULL,LG_C_ERROR85,LG_C_CJ,MB_ICONWARNING);
			return false;
		}
		if(mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Bandas[tipo-101]!=0) {
			Traduce::Msg(NULL,LG_C_ERROR86,LG_C_CJ,MB_ICONWARNING);
			return false;
		} else {
			mapaToggles[GetPosE(pinkie,modo,amodo,btn-67)]=true;
			mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Bandas[tipo-101]=(UCHAR)banda;
			return true;
		}
	} else {

	UINT16 idx=1;
	PNODO nodo=iniComandos;
	while(nodo!=NULL) {
		if(_strcmpi(nombre,nodo->nombre)==0) {
			if(btn>66 && btn<71) {
				if(tipo==32) {
					if(mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Indices[1]!=0) {
						Traduce::Msg(NULL,LG_C_ERROR87,LG_C_CJ,MB_ICONWARNING);
						return false;
					} else {
						mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Indices[1]=idx;
					}
				} else if(tipo==33) {
					if(mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Indices[0]!=0) {
						Traduce::Msg(NULL,LG_C_ERROR88,LG_C_CJ,MB_ICONWARNING);
						return false;
					} else {
						mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Indices[0]=idx;
					}
				} else {
					if(mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Incremental){
						Traduce::Msg(NULL,LG_C_ERROR89,LG_C_CJ,MB_ICONWARNING);
						return false;
					}
					if(mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Indices[tipo]!=0) {
						Traduce::Msg(NULL,LG_C_ERROR90,LG_C_CJ,MB_ICONWARNING);
						return false;
					} else {
						mapaToggles[GetPosE(pinkie,modo,amodo,btn-67)]=true;
						mapaEjes[GetPosE(pinkie,modo,amodo,btn-67)].Indices[tipo]=idx;
					}
				}
			} else if(btn<37) {
				if(mapaBotones[GetPosB(pinkie,modo,amodo,pulsar,btn-1)].Indices[tipo]!=0) {
					Traduce::Msg(NULL,LG_C_ERROR90,LG_C_CJ,MB_ICONWARNING);
					return false;
				} else {
					if(tipo>mapaBotones[GetPosB(pinkie,modo,amodo,pulsar,btn-1)].Estado) {
						mapaBotones[GetPosB(pinkie,modo,amodo,pulsar,btn-1)].Estado=tipo;
					}
					mapaBotones[GetPosB(pinkie,modo,amodo,pulsar,btn-1)].Indices[tipo]=idx;
				}
			}
			return true;
		}
		nodo=nodo->link;
		idx++;
	}
	}

	Traduce::Msg(NULL,LG_C_ERROR91,LG_C_CJ,MB_ICONWARNING);
	return false;
}


bool CLanzador::ComprobarBandas()
{
	for(char i=0;i<2;i++) {
		for(char j=0;j<3;j++) {
			for(char k=0;k<3;k++) {
				for(char l=0;l<4;l++) {
					if(!mapaEjes[GetPosE(i,j,k,l)].Incremental) {
						char maxt=0,maxb=0;
						short maxu=0;
						bool fin=false;
						for(char x=0;x<16;x++) {
							if(mapaEjes[GetPosE(i,j,k,l)].Indices[x]!=0) maxt++;
							if(x<15) if(mapaEjes[GetPosE(i,j,k,l)].Bandas[x]!=0) {
								if(fin) {
									Traduce::Msg(NULL,LG_C_ERROR92,LG_C_B,MB_ICONWARNING);
									return false;
								} else { 
									maxb++;
								}
							} else {
								fin=true;
							}
						}
						if(maxt==1) {
							Traduce::Msg(NULL,LG_C_ERROR93,LG_C_B,MB_ICONWARNING);
							return false;
						}
						if(maxb>=maxt && maxb!=0)  {
							Traduce::Msg(NULL,LG_C_ERROR94,LG_C_B,MB_ICONWARNING);
							return false;
						}
						for(char x=0;x<maxb;x++) {
							if(mapaEjes[GetPosE(i,j,k,l)].Bandas[x]<=maxu) {
								Traduce::Msg(NULL,LG_C_ERROR95,LG_C_B,MB_ICONWARNING);
								return false;
							} else {
								maxu=mapaEjes[GetPosE(i,j,k,l)].Bandas[x];
							}
						}
						if(maxb<(maxt-1)) {
							UCHAR spc=(256-maxu)/(maxt-maxb);
							if(spc==0)  {
								Traduce::Msg(NULL,LG_C_ERROR96,LG_C_B,MB_ICONWARNING);
								return false;
							}
							for(char y=maxb;y<(maxt-1);y++) {
								mapaEjes[GetPosE(i,j,k,l)].Bandas[y]=maxu+spc;
								maxu+=spc;
							}
						}
					}
				}
			}
		}
	}

	return true;
}
