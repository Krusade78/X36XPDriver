#include <windows.h>
#include <commctrl.h>
#include "sliderctrl.h"

CSliderCtrl::CSliderCtrl()
{
}

void CSliderCtrl::Attach(HWND hWnd, int nID) {
	this->hWnd=hWnd; this->nID=nID;
}
void CSliderCtrl::SetRangeMax(LONG r) {
	SendMessage(GetDlgItem(hWnd,nID),TBM_SETRANGEMAX,TRUE,(LPARAM)r);
}
void CSliderCtrl::SetRangeMin(LONG r) {
	SendMessage(GetDlgItem(hWnd,nID),TBM_SETRANGEMIN,TRUE,(LPARAM)r);
}
LONG CSliderCtrl::GetRangeMax() {
	return (LONG)SendMessage(GetDlgItem(hWnd,nID),TBM_GETRANGEMAX,0,0);
}
LONG CSliderCtrl::GetRangeMin() {
	return (LONG)SendMessage(GetDlgItem(hWnd,nID),TBM_GETRANGEMIN,0,0);
}
void CSliderCtrl::SetRange(LONG min, LONG max) {
	SendMessage(GetDlgItem(hWnd,nID),TBM_SETRANGEMIN,TRUE,(LPARAM)min);
	SendMessage(GetDlgItem(hWnd,nID),TBM_SETRANGEMAX,TRUE,(LPARAM)max);
}
void CSliderCtrl::SetPos(LONG r) {
	SendMessage(GetDlgItem(hWnd,nID),TBM_SETPOS,TRUE,(LPARAM)r);
}
LONG CSliderCtrl::GetPos() {
	return (LONG)SendMessage(GetDlgItem(hWnd,nID),TBM_GETPOS,0,0);
}
