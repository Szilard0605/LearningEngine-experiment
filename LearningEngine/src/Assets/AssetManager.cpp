#include "AssetManager.h"

template<typename T, typename... Args>
inline Asset AssetManager::CreateAsset(Args... args)
{
	return T(...args);
}