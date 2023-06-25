typedef struct _ST_NODO {
	char nombre[33];
	UCHAR* acciones;
	struct _ST_NODO *link;
} NODO,*PNODO;
class CLanzador
{
public:
	CLanzador(char* mdf,char* jdf);
	virtual ~CLanzador();
	bool Compilar();
	bool Cargar();
	void CargarE(BYTE* mapaB, BYTE* mapaE,BYTE* confD,BOOLEAN* mapaAD);

private:
//=============== Tipos de datos =============================

typedef struct _ST_CONF
{
	//UCHAR MapaBotones[2][40]; // [0][] boton [1][] seta
	UCHAR MapaEjes[4];
	UCHAR MapaRotatorios;	//bit 1 y 2 rot 1, bit 3 y 4 rot 2
} CONF,*PCONF;

typedef	struct _ST_BOTONES {
		UCHAR Estado;	// 4 bit idc 4 bit npos
		UINT16 Indices[16];
}MBOTONES,*PMBOTONES;

typedef	struct _ST_EJES {
		BOOLEAN Incremental;
		UCHAR Bandas[15];
		UINT16 Indices[16];
}MEJES,PMEJES;

//============================================================

	char* mdf;
	char* jdf;
	PNODO iniComandos;
	PNODO finComandos;
	CONF conf;
	MBOTONES mapaBotones[2][3][3][2][36];
	MEJES mapaEjes[2][3][3][4];
	bool mapaToggles[2][3][3][4];
	char acelPed;

	void LimpiarMemoria();
	bool ProcesarLineaM(char* linea, UINT16 tam);
	bool ProcesarLineaJ(char* linea, UINT16 tam,UCHAR* btn,UCHAR* pinkie,UCHAR* modo,UCHAR* amodo,UCHAR* pulsar);
	bool ProcesarComandoM(char* comando, char* dato,UCHAR* acciones,UCHAR* reps,bool* holds);
	bool ProcesarComandoJ(char* comando,char sig,UCHAR btn,UCHAR* pinkie,UCHAR* modo,UCHAR* amodo,UCHAR* pulsar,char* tipo,bool* macro);
	bool ProcesarMacro(char* nombre,UCHAR btn,UCHAR pinkie,UCHAR modo,UCHAR amodo,UCHAR pulsar,char tipo);
	bool ComprobarBandas();
};