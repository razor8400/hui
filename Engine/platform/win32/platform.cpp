#include "common.h"

#include <windows.h>
#include <psapi.h>

#include "platform/platform.h"

namespace engine
{
	static const char* platform_code = "win32";

	platform& platform::instance()
	{
		static platform instance;
		return instance;
	}

	platform::platform()
	{

	}

	platform::~platform()
	{

	}

	const char* platform::get_platform_code() const
	{
		return platform_code;
	}

	size_t platform::get_memory_used() const
	{
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		return pmc.WorkingSetSize / 1024;
	}
}