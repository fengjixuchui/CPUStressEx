// Thread.cpp : implementation file
//

#include "stdafx.h"
#include "CPUStressEx.h"
#include "Thread.h"
#include "Globals.h"

// CThread

CThread::CThread() {
	m_Affinity = (1 << CGlobals::GetProcessorCount()) - 1;
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
		
		if (m_ActivityLevel != ActivityLevel::Maximum) {
			auto time = ::GetTickCount();
			while (::GetTickCount() - time < (unsigned)m_ActivityLevel * 25)
				;
			::Sleep(100 - (int)m_ActivityLevel * 25);
		}
	}

	return 0;
}

void CThread::SetAfinity(DWORD_PTR affinity) {
	::SetThreadAffinityMask(m_hThread, m_Affinity = affinity);
}

DWORD_PTR CThread::GetAffinity() const {
	return m_Affinity;
}

void CThread::SetIdealCPU(int n) {
	::SetThreadIdealProcessor(m_hThread, n);
}

int CThread::GetIdealCPU() const {
	PROCESSOR_NUMBER n;
	::GetThreadIdealProcessorEx(m_hThread, &n);
	return n.Number;
}

void CThread::Terminate() {
	::SetEvent(m_hTerminate);
}


