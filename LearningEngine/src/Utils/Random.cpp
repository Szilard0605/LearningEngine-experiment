#include "Random.h"

#include <random>

static std::mt19937 s_RandomEngine;
static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

namespace Utils
{

	void Random::InitSeed()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	uint32_t Random::uint32()
	{
		return s_Distribution(s_RandomEngine);
	}
}