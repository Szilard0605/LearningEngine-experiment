#pragma once

namespace Platform
{ 
	enum OperatingSystem
	{
		Windows
	};

	static OperatingSystem GetOperatingSystem() { return OperatingSystem::Windows; }
}