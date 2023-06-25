//===========================================================================
//
// CPLSVR1 property pages implementation.
//
// Functions:
//  X36PROP_Page1_DlgProc()
//  InitDInput()
//
//===========================================================================

#include "ajd_cpl.h"
#include <prsht.h>
#include "pages.h"
#include "raton.h"
#include "dinfo.h"
#include "calibraje.h"
#include "idioma.h"

CDInfo* info = NULL;
CRaton* raton = NULL;

void TraduceDlg1(HWND hWnd);
void TraduceDlg2(HWND hWnd);

//===========================================================================
// X36PROP_Page1_DlgProc
//
// Callback proceedure CPLSVR1 property page #1 (General).
//
// Parameters:
//  HWND    hWnd    - handle to dialog window
//  UINT    uMsg    - dialog message
//  WPARAM  wParam  - message specific data
//  LPARAM  lParam  - message specific data
//
// Returns: BOOL
//
//===========================================================================
INT_PTR CALLBACK X36PROP_Page1_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CDIGameCntrlPropSheet_X *pdiCpl = (CDIGameCntrlPropSheet_X*)GetWindowLongPtr(hWnd, DWLP_USER);
    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            // perform any required initialization... 
            // get ptr to our object
            pdiCpl = (CDIGameCntrlPropSheet_X*)((PROPSHEETPAGE *)lParam)->lParam;

            // Save our pointer so we can access it later
            SetWindowLongPtr(hWnd, DWLP_USER, (LPARAM)pdiCpl);

            // initialize DirectInput
            if (FAILED(InitDInput(GetParent(hWnd), pdiCpl)))
            {
#ifdef _DEBUG
                OutputDebugString(TEXT("CPLSvr1 Page 1 - InitDInput failed!\n"));
#endif //_DEBUG
                return TRUE;
            }

            IDirectInputDevice8*    pdiDevice8  = NULL;
            IDirectInputJoyConfig8*  pdiJoyCfg = NULL;

            pdiCpl->GetDevice(&pdiDevice8);
            pdiCpl->GetJoyConfig(&pdiJoyCfg);

			if(pdiDevice8!=NULL && pdiJoyCfg!=NULL) {
				TraduceDlg1(hWnd);
				if(raton==NULL) raton = new CRaton();
				if(raton!=NULL) raton->IniciarDlg1(hWnd);
				info = new CDInfo(hWnd,pdiDevice8,pdiJoyCfg,raton);
				SetTimer(hWnd,0,100,NULL);
			}
        }
        break;
	case WM_TIMER:
		if(info!=NULL) info->Actualizar();
		break;
	case WM_CTLCOLORSTATIC:
		{
			if(GetDlgCtrlID((HWND)lParam)!=-1 && GetDlgCtrlID((HWND)lParam)<2000) //IDC_STATIC = -1 IDS_SZ = 2000
			{
				SetBkMode((HDC)wParam,TRANSPARENT);
				SelectObject((HDC)wParam,GetStockObject(DC_BRUSH));
				if(GetDlgCtrlID((HWND)lParam)>=1000)
				{
					COLORREF color = (COLORREF)GetWindowLongPtr((HWND)lParam,-21);
					SetTextColor((HDC)wParam,color);
					SetDCBrushColor((HDC)wParam,RGB(225,225,250));
					return (INT_PTR)GetStockObject(DC_BRUSH);
				} else {
					bool pres = (GetWindowLongPtr((HWND)lParam,-21)==1) ? true : false;
					SelectObject((HDC)wParam,GetStockObject(DC_BRUSH));
					if(pres) {
						SetTextColor((HDC)wParam,RGB(255,255,255));
						SetDCBrushColor((HDC)wParam,RGB(200,0,0));
					} else {
						SetTextColor((HDC)wParam,RGB(200,200,200));
						SetDCBrushColor((HDC)wParam,RGB(100,0,0));
					}
				}
				return (INT_PTR)GetStockObject(DC_BRUSH);
			}
		}
		break;
	case WM_CLOSE:
		KillTimer(hWnd,0);
		if(raton!=NULL) {delete raton; raton=NULL;}
		delete info; info= NULL;
		break;
    }

    return FALSE;
} //*** end X36PROP_Page1_DlgProc()

//===========================================================================
// X36PROP_Page2_DlgProc
//
// Callback proceedure CPLSVR1 property page #1 (General).
//
// Parameters:
//  HWND    hWnd    - handle to dialog window
//  UINT    uMsg    - dialog message
//  WPARAM  wParam  - message specific data
//  LPARAM  lParam  - message specific data
//
// Returns: BOOL
//
//===========================================================================
INT_PTR CALLBACK X36PROP_Page2_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CDIGameCntrlPropSheet_X *pdiCpl = (CDIGameCntrlPropSheet_X*)GetWindowLongPtr(hWnd, DWLP_USER);
    switch (uMsg)
    {
	case WM_INITDIALOG:
		{
            // perform any required initialization... 

            // get ptr to our object
            pdiCpl = (CDIGameCntrlPropSheet_X*)((PROPSHEETPAGE *)lParam)->lParam;

            // Save our pointer so we can access it later
            SetWindowLongPtr(hWnd, DWLP_USER, (LPARAM)pdiCpl);

            // initialize DirectInput
            if (FAILED(InitDInput(GetParent(hWnd), pdiCpl)))
            {
#ifdef _DEBUG
                OutputDebugString(TEXT("CPLSvr1 Page 1 - InitDInput failed!\n"));
#endif //_DEBUG
                return TRUE;
            }
			TraduceDlg2(hWnd);
			if(raton==NULL)	raton = new CRaton();
			if(raton!=NULL) raton->IniciarDlg2(hWnd);
		}
		break;
    case WM_COMMAND:
		if(wParam==IDB_CALIBRAR)
		{
            IDirectInputDevice8* pdiDevice8  = NULL;
            pdiCpl->GetDevice(&pdiDevice8);

			if(pdiDevice8!=NULL) {
				IniciarDlgCalibraje(pdiDevice8,ghInst,hWnd);
				if(info!=NULL) info->CargarParteEstatica();
			}
		} else if(wParam==IDB_Cambiar) {
			if(raton!=NULL) {
				raton->OnCambiar();
				if(info!=NULL) info->CargarParteEstatica();
			}
		}
		break;
	case WM_HSCROLL:
		raton->OnScroll();
		break;
	case WM_CLOSE:
		if(raton!=NULL) {delete raton; raton=NULL;}
		break;
    }

    return FALSE;
} //*** end X36PROP_Page2_DlgProc()

/***************************************************************
						Traducciones
****************************************************************/
void TraduceDlg1(HWND hWnd){
	Traduce::Dlg(GetDlgItem(hWnd,IDS_Servicio),LG_CPL_D1_GUI1);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_Calibrado),LG_CPL_D1_GUI2);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_Raton),LG_CPL_D1_GUI3);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_Retraso),LG_CPL_D1_GUI4);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_GBotones),LG_CPL_D1_GUI5);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SX),LG_CPL_D1_GUI6);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SY),LG_CPL_D1_GUI7);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SZ),LG_CPL_D1_GUI8);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SR),LG_CPL_D1_GUI9);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SR1),LG_CPL_D1_GUI10);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SR2),LG_CPL_D1_GUI11);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SP1),LG_CPL_D1_GUI12);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SP2),LG_CPL_D1_GUI13);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SP3),LG_CPL_D1_GUI14);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SP4),LG_CPL_D1_GUI15);
}

void TraduceDlg2(HWND hWnd){
	Traduce::Dlg(GetDlgItem(hWnd,IDS_GJoy),LG_CPL_D2_GUI1);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_Texto),LG_CPL_D2_GUI2);
	Traduce::Dlg(GetDlgItem(hWnd,IDB_CALIBRAR),LG_CPL_D2_GUI3);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_GRaton),LG_CPL_D2_GUI4);
	Traduce::Dlg(GetDlgItem(hWnd,IDS_SensiSt),LG_CPL_D2_GUI5);
	Traduce::Dlg(GetDlgItem(hWnd,IDB_Cambiar),LG_CPL_D2_GUI6);
}
//===========================================================================
// InitDInput
//
// Initializes DirectInput objects
//
// Parameters:
//  HWND                    hWnd    - handle of caller's window
//  CDIGameCntrlPropSheet_X *pdiCpl - pointer to Game Controllers property
//                                      sheet object
//
// Returns: HRESULT
//
//===========================================================================
HRESULT InitDInput(HWND hWnd, CDIGameCntrlPropSheet_X *pdiCpl)
{
    // protect ourselves from multithreading problems
    EnterCriticalSection(&gcritsect);
    HRESULT       hRes;

    // validate pdiCpl
    if ((IsBadReadPtr((void*)pdiCpl, sizeof(CDIGameCntrlPropSheet_X))) ||
        (IsBadWritePtr((void*)pdiCpl, sizeof(CDIGameCntrlPropSheet_X))) )
    {
#ifdef _DEBUG
        OutputDebugString(TEXT("InitDInput() - bogus pointer!\n"));
#endif // _DEBUG
        hRes = E_POINTER;
        goto exitinit;
    }

    IDirectInputDevice8*    pdiDevice8;

    // retrieve the current device object
    pdiCpl->GetDevice(&pdiDevice8);   

    IDirectInputJoyConfig8*  pdiJoyCfg ;

    // retrieve the current joyconfig object
    pdiCpl->GetJoyConfig(&pdiJoyCfg);   

    IDirectInput8* pdi;

    // have we already initialized DirectInput?
    if ((NULL == pdiDevice8) || (NULL == pdiJoyCfg))
    {
        // no, create a base DirectInput object
        hRes = DirectInput8Create(ghInst, DIRECTINPUT_VERSION, IID_IDirectInput8,(LPVOID*)&pdi, NULL);
        if (FAILED(hRes))
        {
#ifdef _DEBUG
            OutputDebugString(TEXT("DirectInputCreate() failed!\n"));
#endif //_DEBUG
            goto exitinit;
        }
    } else
    {
        hRes = S_FALSE;
        goto exitinit;
    }

    // have we already created a joyconfig object?
    if (NULL == pdiJoyCfg)
    {
        // no, create a joyconfig object
        hRes = pdi->QueryInterface(IID_IDirectInputJoyConfig8, (LPVOID*)&pdiJoyCfg);
        if (SUCCEEDED(hRes))
        {
            // store the joyconfig object
            pdiCpl->SetJoyConfig(pdiJoyCfg);
        } else
        {
#ifdef _DEBUG
            OutputDebugString(TEXT("Unable to create joyconfig!\n"));
#endif // _DEBUG
            goto exitinit;
        }
    } else hRes = S_FALSE;

    // have we already created a device object?
    if (NULL == pdiDevice8)
    {
        // no, create a device object

        if (NULL != pdiJoyCfg)
        {
            hRes = DICreateDevice2FromJoyConfig(pdiCpl->GetID(), hWnd, pdi, pdiJoyCfg, &pdiDevice8);
            if (SUCCEEDED(hRes))
            {
                // store the device object
                pdiCpl->SetDevice(pdiDevice8);
            } else
            {
#ifdef _DEBUG
                OutputDebugString(TEXT("DIUtilCreateDevice2FromJoyConfig() failed!\n"));
#endif //_DEBUG
                goto exitinit;
            }
        } else
        {
#ifdef _DEBUG
            OutputDebugString(TEXT("No joyconfig object... cannot create device!\n"));
#endif // _DEBUG
            hRes = E_FAIL;
            goto exitinit;
        }
    } else hRes = S_FALSE;

    if (NULL != pdi)
    {
        // release he base DirectInput object
        pdi->Release();
    }

    exitinit:
    // we're done
    LeaveCriticalSection(&gcritsect);
    return hRes;
} //*** end InitDInput()

//===========================================================================
// DIUtilCreateDevice2FromJoyConfig
//
// Helper function to create a DirectInputDevice2 object from a 
//  DirectInputJoyConfig object.
//
// Parameters:
//  short                   nJoystickId     - joystick id for creation
//  HWND                    hWnd            - window handle
//  LPDIRECTINPUT           pdi             - ptr to base DInput object
//  LPDIRECTINPUTJOYCONFIG  pdiJoyCfg       - ptr to joyconfig object
//  LPDIRECTINPUTDEVICE     *ppdiDevice2    - ptr to device object ptr
//
// Returns: HRESULT
//
//===========================================================================
HRESULT DICreateDevice2FromJoyConfig(short nJoystickId, HWND hWnd,
                                         IDirectInput8* pdi,
                                         IDirectInputJoyConfig8* pdiJoyCfg,
                                         IDirectInputDevice8 **ppdiDevice8)
{
    HRESULT                 hRes        = E_NOTIMPL;
    IDirectInputDevice8*    pdiDevTemp  = NULL;
    DIJOYCONFIG             dijc;

    // validate pointers
    if ( (IsBadReadPtr((void*)pdi, sizeof(IDirectInput8))) ||
         (IsBadWritePtr((void*)pdi, sizeof(IDirectInput8))) )
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - invalid pdi\n"));
        return E_POINTER;
    }
    if ( (IsBadReadPtr((void*)pdiJoyCfg, sizeof(IDirectInputJoyConfig8))) ||
         (IsBadWritePtr((void*)pdiJoyCfg, sizeof(IDirectInputJoyConfig8))) )
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - invalid pdiJoyCfg\n"));
        return E_POINTER;
    }
    if ( (IsBadReadPtr((void*)ppdiDevice8, sizeof(LPDIRECTINPUTDEVICE8))) ||
         (IsBadWritePtr((void*)ppdiDevice8, sizeof(LPDIRECTINPUTDEVICE8))) )
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - invalid ppdiDevice2\n"));
        return E_POINTER;
    }

    // get the instance GUID for device configured as nJoystickId
    // 
    // GetConfig will provide this information
    dijc.dwSize = sizeof(DIJOYCONFIG);
    hRes = pdiJoyCfg->GetConfig(nJoystickId, &dijc, DIJC_GUIDINSTANCE);
    if (FAILED(hRes))
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - GetConfig() failed\n"));
        return hRes;
    }

    // create temporary device object
    //
    // use the instance GUID returned by GetConfig()
    hRes = pdi->CreateDevice(dijc.guidInstance, &pdiDevTemp, NULL);
    if (FAILED(hRes))
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - CreateDevice() failed\n"));
        return hRes;
    }

    // query for a device2 object
    hRes = pdiDevTemp->QueryInterface(IID_IDirectInputDevice8, (LPVOID*)ppdiDevice8);

    // release the temporary object
    pdiDevTemp->Release();

    if (FAILED(hRes))
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - QueryInterface(IDirectInputDevice2) failed\n"));
        return hRes;
    }

    // set the desired data format
    //
    // we want to be a joystick
    hRes = (*ppdiDevice8)->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hRes))
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - SetDataFormat(Joystick) failed\n"));
        return hRes;
    }

    // set the cooperative level for the device
    //
    // want to set EXCLUSIVE | BACKGROUND
    hRes = (*ppdiDevice8)->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
    if (FAILED(hRes))
    {
        OutputDebugString(TEXT("DIUtilCreateDeviceFromJoyConfig() - SetCooperativeLevel() failed\n"));
        return hRes;
    }

	DIPROPDWORD dipdw; 
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        dipdw.diph.dwHow        = DIPH_DEVICE; 
		dipdw.diph.dwObj		= 0;
		dipdw.dwData			= DIPROPCALIBRATIONMODE_RAW;
	hRes = (*ppdiDevice8)->SetProperty( DIPROP_CALIBRATIONMODE, &dipdw.diph );

	return hRes;

} //*** end DIUtilCreateDevice2FromJoyConfig()
