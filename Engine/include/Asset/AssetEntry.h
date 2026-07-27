#pragma once
#include <string>
#include <filesystem>
#include "Asset/AssetType.h"


namespace mse
{
	struct AssetEntry
	{
		mse::AssetType Type;
		std::string Name;
		std::filesystem::path Path;
		bool IsDirectory;
	};
}