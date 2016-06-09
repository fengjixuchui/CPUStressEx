#include "stdafx.h"
#include "Globals.h"

int CGlobals::s_Processors;

int CGlobals::GetProcessorCount() {
	if (s_Processors == 0) {
		s_Processors = ::GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
	}
	return s_Processors;
}

