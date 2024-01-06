#include "LuaRegistry.h"

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"
#include "lstate.h"

#include "glm.hpp"

void LuaRegistry::RegisterFunctions(sol::state* state)
{
	// vec3
	{
		auto mult_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 * f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f * v1; }
		);

		auto add_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 + v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 + f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f + v1; }
		);

		auto sub_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 - v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 - f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f - v1; }
		);
		auto div_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 / v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 / f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f / v1; }
		);

		state->new_usertype<glm::vec3>("vec3",
			sol::constructors<glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			sol::meta_function::addition, add_overloads,
			sol::meta_function::subtraction, sub_overloads,
			sol::meta_function::multiplication, mult_overloads,
			sol::meta_function::division, div_overloads
		);
	}

}

static glm::vec3 Entity_GetPosition(entt::entity)
{
	return glm::vec3(0);
}
