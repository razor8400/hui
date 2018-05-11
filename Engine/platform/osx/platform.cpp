#include "common.h"
#include "platform.h"

#include <sys/time.h>
#include <sys/resource.h>

namespace engine
{
	static const char* platform_code = "osx";

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
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0)
            return usage.ru_maxrss / 1024;
        
        return 0;
    }
}
