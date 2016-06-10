
#pragma once

#include "Thread.h"
#include "ThreadsListCtrl.h"

// CChildView window

class CChildView : public CWnd {
	// Construction
public:
	CChildView();

	// Attributes
public:

	enum { IDC_LIST = 1234 };

	// Operations
public:

	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
public:
	virtual ~CChildView();

private:
	CThreadsListCtrl m_List;
	std::vector < std::unique_ptr<CThread>> m_Threads;

	void CreateThreads();
	std::unique_ptr<CThread> CreateThread();
	void AddThread(std::unique_ptr<CThread>& thread);
	static PCWSTR ActivityLevelToString(ActivityLevel level);
	static PCWSTR ThreadPriorityToString(int priority);

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	std::vector<std::pair<CThread*, int>> GetSelectedThreads() const;
	void UpdateThread(int n, const CThread* thread = nullptr);

	afx_msg void OnCustomDraw(NMHDR*, LRESULT*);
	afx_msg void OnThreadActivate();
	afx_msg void OnUpdateThreadActivate(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnThreadDeactivate();
	afx_msg void OnUpdateThreadDeactivate(CCmdUI *pCmdUI);
	void OnRClickList(NMHDR*, LRESULT*);

	void OnChangeThreadActivity(UINT id);
	void OnUpdateChangeThreadActivity(CCmdUI* pCmdUI);
	afx_msg void OnProcessCreatethread();
	afx_msg void OnUpdateProcessCreatethread(CCmdUI *pCmdUI);
	afx_msg void OnProcessCreate4threads();
	afx_msg void OnUpdateProcessCreate4threads(CCmdUI *pCmdUI);
};

