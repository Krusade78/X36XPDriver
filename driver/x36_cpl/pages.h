//===========================================================================
// PAGES.H
//
//===========================================================================

//===========================================================================
// (C) Copyright 1998 Microsoft Corp.  All rights reserved.
//
// You have a royalty-free right to use, modify, reproduce and
// distribute the Sample Files (and/or any modified version) in
// any way you find useful, provided that you agree that
// Microsoft has no warranty obligations or liability for any
// Sample Application Files which are modified.
//===========================================================================

// prototypes
// dialog callback functions
INT_PTR CALLBACK X36PROP_Page1_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam);
INT_PTR CALLBACK X36PROP_Page2_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam);
INT_PTR CALLBACK X36PROP_Page2_1_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam);
// helper functions
HRESULT InitDInput(HWND hWnd, CDIGameCntrlPropSheet_X *pdiCpl);
HRESULT DICreateDevice2FromJoyConfig(short nJoystickId, HWND hWnd,
                                         IDirectInput8* pdi,
                                         IDirectInputJoyConfig8* pdiJoyCfg,
                                         IDirectInputDevice8 **ppdiDevice2);

//---------------------------------------------------------------------------