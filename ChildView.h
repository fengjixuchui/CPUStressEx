
// ChildView.h : interface of the CChildView class
//


#pragma once

#include "Thread.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

	enum { IDC_LIST = 123 };

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

private:
	CMFCListCtrl m_List;
	std::vector < std::unique_ptr<CThread>> m_Threads;

	void CreateThreads();
	std::unique_ptr<CThread> CreateThread();
	void AddThread(const CThread* thread);
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

	afx_msg void OnThreadActivate();
	afx_msg void OnUpdateThreadActivate(CCmdUI *pCmdUI);
};

