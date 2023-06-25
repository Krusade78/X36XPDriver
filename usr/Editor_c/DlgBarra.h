#pragma once


// CDlgBarra

class CDlgBarra : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarra)

public:
	CDlgBarra();
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};


