#pragma once


// CMainDialogBar

class CMainDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CMainDialogBar)

public:
	CMainDialogBar();
	virtual ~CMainDialogBar();

	void DoDataExchange(CDataExchange* pDX);

protected:
	CMFCMenuButton m_ActivityButton;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


