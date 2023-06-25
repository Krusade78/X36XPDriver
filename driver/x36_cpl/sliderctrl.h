#pragma once

class CSliderCtrl
{
public:
	CSliderCtrl();
	void Attach(HWND hWnd, int nID);
	void SetRangeMax(LONG r);
	void SetRangeMin(LONG r);
	LONG GetRangeMax();
	LONG GetRangeMin();
	void SetRange(LONG min, LONG max);
	LONG GetPos();
	void SetPos(LONG r);
private:
	HWND hWnd;
	int nID;
};