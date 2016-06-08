// Thread.cpp : implementation file
//

#include "stdafx.h"
#include "CPUStressEx.h"
#include "Thread.h"


// CThread

CThread::CThread() {
	m_hTerminate = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	m_hThread = ::CreateThread(nullptr, 0, ThreadFunction, this, CREATE_SUSPENDED, &m_ID);
}

CThread::~CThread() {
	if (m_hThread)
		::CloseHandle(m_hThread);
}

void CThread::Suspend() {
	if (!IsActive()) return;

	::SuspendThread(m_hThread);
	m_Active = false;
}

void CThread::Resume() {
	if (IsActive()) return;

	::ResumeThread(m_hThread);
	m_Active = true;
}

DWORD CThread::ThreadFunction(PVOID p) {
	return reinterpret_cast<CThread*>(p)->ThreadFunction();
}

DWORD CThread::ThreadFunction() {
	for (;;) {
		if (::WaitForSingleObject(m_hTerminate, 0) == WAIT_OBJECT_0)
			break;

		if (m_ActivityLevel != ActivityLevel::Maximum)
			::Sleep(100 - (int)m_ActivityLevel * 25);
	}

	return 0;
}

void CThread::Terminate() {
	::SetEvent(m_hTerminate);
}


