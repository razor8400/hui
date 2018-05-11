#pragma once

namespace engine
{
	class platform
	{
	public:
		static platform& instance();

		const char* get_platform_code() const;
		size_t get_memory_used() const;
	private:
		platform();
		~platform();
	};
}
