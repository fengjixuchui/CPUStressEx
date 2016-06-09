// MainDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "CPUStressEx.h"
#include "MainDialogBar.h"


// CMainDialogBar

IMPLEMENT_DYNAMIC(CMainDialogBar, CDialogBar)

CMainDialogBar::CMainDialogBar() 
{

}

CMainDialogBar::~CMainDialogBar()
{
}


BEGIN_MESSAGE_MAP(CMainDialogBar, CDialogBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CMainDialogBar::DoDataExchange(CDataExchange* pDX) {
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THREAD_ACTIVITY, m_ActivityButton);
}

// CMainDialogBar message handlers


int CMainDialogBar::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	auto pWnd = GetDlgItem(IDC_THREAD_ACTIVITY);
	m_ActivityButton.Attach(pWnd->GetSafeHwnd());
	CMenu menu;
	menu.LoadMenuW(IDR_CONTEXTMENU);
	m_ActivityButton.m_hMenu = menu.GetSubMenu(0)->GetSafeHmenu();

	return 0;
}

