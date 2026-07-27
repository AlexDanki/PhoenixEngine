#pragma once
#include <vector>
#include "AssetEntry.h"
#include <filesystem>

namespace mse
{
	class AssetDatabase
	{

	public:

		void Refresh();

		std::vector<mse::AssetEntry>& GetEntries();
		const std::vector<mse::AssetEntry>& GetEntries() const;

		std::vector<const mse::AssetEntry*> GetEntriesInDirectory(const std::filesystem::path& path);
		
		void SetDataType(mse::AssetEntry& assetEntry);
		const std::filesystem::path& GetRootDiretory() const;

	private:
		std::vector<mse::AssetEntry> m_entries;
		std::filesystem::path m_rootDirectory = "../../../../Assets";
	};
}

