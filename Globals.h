#pragma once
class CGlobals {
public:
	static int GetProcessorCount();

private:
	static int s_Processors;
};

