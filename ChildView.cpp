
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "CPUStressEx.h"
#include "ChildView.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView() : m_List(m_Threads) {}

CChildView::~CChildView() {}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_THREAD_ACTIVATE, &CChildView::OnThreadActivate)
	ON_UPDATE_COMMAND_UI(ID_THREAD_ACTIVATE, &CChildView::OnUpdateThreadActivate)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_THREAD_DEACTIVATE, &CChildView::OnThreadDeactivate)
	ON_UPDATE_COMMAND_UI(ID_THREAD_DEACTIVATE, &CChildView::OnUpdateThreadDeactivate)

	ON_COMMAND_RANGE(ID_ACTIVITYLEVEL_LOW, ID_ACTIVITYLEVEL_MAXIMUM, OnChangeThreadActivity)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ACTIVITYLEVEL_LOW, ID_ACTIVITYLEVEL_MAXIMUM, OnUpdateChangeThreadActivity)

END_MESSAGE_MAP()

void CChildView::DoDataExchange(CDataExchange* pDX) {
	CWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST, m_List);
}

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnCustomDraw(NMHDR* pHdr, LRESULT* pResult) {
	auto draw = (NMLVCUSTOMDRAW*)pHdr;
	draw->clrTextBk = RGB(255, 0, 0);
}

void CChildView::OnPaint() {
	CPaintDC dc(this); // device context for painting
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_List.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, CRect(), this, IDC_LIST);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_AUTOSIZECOLUMNS | LVS_EX_FLATSB);

	m_List.InsertColumn(0, L"#", 0, 40);
	m_List.InsertColumn(1, L"ID", LVCFMT_LEFT, 60);
	m_List.InsertColumn(2, L"Active?", LVCFMT_CENTER, 80);
	m_List.InsertColumn(3, L"Activity", LVCFMT_LEFT, 80);
	m_List.InsertColumn(4, L"Priority", LVCFMT_LEFT, 80);
	m_List.InsertColumn(5, L"Ideal CPU", LVCFMT_LEFT, 80);
	m_List.InsertColumn(6, L"Affinity", LVCFMT_LEFT, 100);

	CreateThreads();

	return 0;
}

void CChildView::CreateThreads() {
	for (int i = 0; i < 4; i++) {
		auto thread = CreateThread();
		if (i == 0)
			thread->Resume();
		AddThread(thread.get());
		m_Threads.push_back(move(thread));
	}
}

void CChildView::AddThread(const CThread* thread) {
	CString number;
	number.Format(L"%d", m_List.GetItemCount() + 1);
	int n = m_List.InsertItem(m_List.GetItemCount(), number);
	number.Format(L"%d", thread->GetThreadId());
	m_List.SetItemText(n, 1, number);
	UpdateThread(n, thread);
}

void CChildView::UpdateThread(int n, const CThread* thread) {
	if (thread == nullptr)
		thread = reinterpret_cast<CThread*>(m_List.GetItemData(n));
	m_List.SetItemText(n, 2, thread->IsActive() ? L"Yes" : L"");
	m_List.SetItemText(n, 3, ActivityLevelToString(thread->GetActivityLevel()));
	m_List.SetItemText(n, 4, ThreadPriorityToString(thread->GetPriority()));

	m_List.SetItemData(n, reinterpret_cast<DWORD_PTR>(thread));
}

PCWSTR CChildView::ActivityLevelToString(ActivityLevel level) {
	static const PCWSTR Levels[] = { L"Low", L"Medium", L"Busy", L"Maximum" };
	return Levels[(int)level - 1];
}

PCWSTR CChildView::ThreadPriorityToString(int priority) {
	static const PCWSTR Levels[] = {
		L"Idle", L"Lowest", L"Below Normal", L"Normal", L"Above Normal", L"Highest", L"Time Critical"
	};
	return Levels[::abs(priority) == 15 ? priority - 13 * priority / 15 : priority + 3];
}

unique_ptr<CThread> CChildView::CreateThread() {
	auto thread = make_unique<CThread>();
	return thread;
}

void CChildView::OnSize(UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);

	m_List.MoveWindow(0, 0, cx, cy);
}


vector<pair<CThread*, int>> CChildView::GetSelectedThreads() const {
	vector<pair<CThread*, int>> selectedThreads;
	for (auto pos = m_List.GetFirstSelectedItemPosition(); pos; ) {
		int n = m_List.GetNextSelectedItem(pos);
		auto data = m_List.GetItemData(n);

		selectedThreads.push_back(make_pair(reinterpret_cast<CThread*>(data), n));
	}
	return selectedThreads;
}



void CChildView::OnThreadActivate() {
	for (auto& item : GetSelectedThreads()) {
		item.first->Resume();
		UpdateThread(item.second, item.first);
	}
}


void CChildView::OnUpdateThreadActivate(CCmdUI *pCmdUI) {
	pCmdUI->Enable(m_List.GetSelectedCount() > 0);
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}


void CChildView::OnThreadDeactivate() {
	for (auto& item : GetSelectedThreads()) {
		item.first->Suspend();
		UpdateThread(item.second, item.first);
	}
}

void CChildView::OnUpdateThreadDeactivate(CCmdUI *pCmdUI) {
	pCmdUI->Enable(m_List.GetSelectedCount() > 0);
}

void CChildView::OnChangeThreadActivity(UINT id) {
	auto level = (ActivityLevel)(id - ID_ACTIVITYLEVEL_LOW + 1);

	for (auto& item : GetSelectedThreads()) {
		item.first->SetActivityLevel(level);
		UpdateThread(item.second, item.first);
	}
}

void CChildView::OnUpdateChangeThreadActivity(CCmdUI* pCmdUI) {
	pCmdUI->Enable(m_List.GetSelectedCount() > 0);
}
