#pragma once

#include <stdint.h>

namespace Utils
{
	class Random
	{
	public:
		static void InitSeed();
		static uint32_t uint32();
	};
}
