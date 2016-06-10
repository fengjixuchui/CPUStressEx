
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CPUStressEx.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_OPTIONS_ALWAYSONTOP, &CMainFrame::OnOptionsAlwaysontop)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALWAYSONTOP, &CMainFrame::OnUpdateOptionsAlwaysontop)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame() {
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame() {}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if(!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(), this, AFX_IDW_PANE_FIRST)) {
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if(!m_wndDlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create dialogbar\n");
		return -1;
	}

	// main icon window

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const {
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const {
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/) {
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	// let the view have first crack at the command

	if(m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnOptionsAlwaysontop() {
	bool onTop = (GetExStyle() & WS_EX_TOPMOST) > 0;
	::SetWindowPos(GetSafeHwnd(), onTop ? HWND_NOTOPMOST : HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}


void CMainFrame::OnUpdateOptionsAlwaysontop(CCmdUI *pCmdUI) {
	pCmdUI->SetCheck((GetExStyle() & WS_EX_TOPMOST) > 0);
}
