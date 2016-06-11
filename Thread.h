
#pragma once

enum class ActivityLevel {
	None = 0,
	Low = 1,
	Medium = 2,
	Busy = 3,
	Maximum = 4
};

// CThread command target

class CThread {
public:
	CThread();
	virtual ~CThread();

	DWORD GetThreadId() const {
		return m_ID;
	}

	int GetPriority() const {
		return ::GetThreadPriority(m_hThread);
	}

	ActivityLevel GetActivityLevel() const {
		return m_ActivityLevel;
	}

	bool IsActive() const {
		return m_Active;
	}

	void SetPriority(int priority) {
		::SetThreadPriority(m_hThread, priority);
	}
	
	void SetAfinity(DWORD_PTR affinity);
	DWORD_PTR GetAffinity() const;

	void SetIdealCPU(int n);
	int GetIdealCPU() const;

	void Terminate();
	void Suspend();
	void Resume();
	void SetActivityLevel(ActivityLevel level) {
		m_ActivityLevel = level;
	}

	static DWORD CALLBACK ThreadFunction(PVOID);
	DWORD CALLBACK ThreadFunction();

private:
	HANDLE m_hThread, m_hTerminate;
	DWORD m_ID;
	ActivityLevel m_ActivityLevel = ActivityLevel::Low;
	bool m_Active = false;
	DWORD_PTR m_Affinity;
};


