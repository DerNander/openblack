/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

#include "PlayerSymbolSheet.h"

#include <cstddef>

#include <algorithm>
#include <array>
#include <string>

#include <spdlog/spdlog.h>

#include "ECS/PlayerColours.h"
#include "Enums.h"
#include "FileSystem/FileSystemInterface.h"
#include "Graphics/Texture2D.h"
#include "Locator.h"
#include "Magic/Core/Players.h"
#include "Resources/Loaders.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourcesInterface.h"

namespace openblack::player_symbol_sheet
{

namespace
{
/// The gods' images, by their index
constexpr std::array<std::string_view, 3> k_ImageFiles {"Lethis.cps", "Kazarr.cps", "nemesis.cps"};
/// The player type of a player the computer plays
constexpr int32_t k_ComputerType = 2;
/// The players whose cells are made
constexpr uint32_t k_Players = 8;
} // namespace

uint32_t ComputerImageIndex(uint32_t playerNumber, int32_t landNumber)
{
	auto index = static_cast<int32_t>(playerNumber) - 1;
	if (landNumber == 3 && playerNumber == 1)
	{
		index = 1;
	}
	if (landNumber == 5 && playerNumber == 1)
	{
		index = 2;
	}
	return static_cast<uint32_t>(std::clamp(index, 0, static_cast<int32_t>(k_ImageFiles.size()) - 1));
}

std::string_view ComputerImageFile(uint32_t index)
{
	return k_ImageFiles.at(std::min<size_t>(index, k_ImageFiles.size() - 1));
}

glm::u16vec2 CellOrigin(uint32_t playerNumber)
{
	return {static_cast<uint16_t>((playerNumber % k_CellsPerRow) * k_CellSize),
	        static_cast<uint16_t>((playerNumber / k_CellsPerRow) * k_CellSize)};
}

std::optional<std::vector<uint8_t>> CellTexels(std::span<const uint8_t> image)
{
	if (image.size() < k_ImageBytes)
	{
		return std::nullopt;
	}
	std::vector<uint8_t> texels(k_ImageBytes * 4);
	for (size_t i = 0; i < k_ImageBytes; ++i)
	{
		// the top four bits in each of the four channels of a 4-bit texel, read back as eight bits
		const auto value = static_cast<uint8_t>((image[i] >> 4u) * 17u);
		std::fill_n(texels.begin() + static_cast<std::ptrdiff_t>(i * 4), 4, value);
	}
	return texels;
}

void MakeComputerSymbols()
{
	if (!Locator::resources::has_value() || !Locator::filesystem::has_value())
	{
		return;
	}
	// The sheet is kept in memory and the texture made again from it: a bgfx texture created with data is immutable, so
	// an update of the sheet made empty with data never reached the GPU and no god's symbol was drawn
	static std::vector<uint8_t> s_sheet(static_cast<size_t>(k_SheetSize) * k_SheetSize * 4, 0);
	auto& textures = Locator::resources::value().GetTextures();
	const auto land = ecs::player_colours::LandNumber();
	const auto folder = Locator::filesystem::value().GetPath<filesystem::Path::Textures>();
	for (uint32_t number = 0; number < k_Players; ++number)
	{
		if (magic::players::ReadMagicOf(static_cast<PlayerNames>(number)).playerType != k_ComputerType)
		{
			continue;
		}
		const auto file = ComputerImageFile(ComputerImageIndex(number, land));
		const auto& image =
		    resources::LoadOptionalBlob(Locator::resources::value().GetBlobs(), folder / std::string(file), "god symbol");
		const auto texels = CellTexels(image);
		if (!texels.has_value())
		{
			continue;
		}
		const auto origin = CellOrigin(number);
		for (uint32_t row = 0; row < k_CellSize; ++row)
		{
			const auto src = texels->begin() + static_cast<std::ptrdiff_t>(row * k_CellSize * 4);
			const auto dst = s_sheet.begin() + static_cast<std::ptrdiff_t>(((origin.y + row) * k_SheetSize + origin.x) * 4);
			std::copy_n(src, k_CellSize * 4, dst);
		}
		SPDLOG_LOGGER_INFO(spdlog::get("game"), "Player symbols: player {} takes {}", number, file);
	}
	if (textures.Contains(k_TextureId))
	{
		textures.Erase(k_TextureId);
	}
	textures.Load(k_TextureId, resources::Texture2DLoader::FromRgbaTag {}, std::string("PlayersSymbols"), k_SheetSize,
	              k_SheetSize, s_sheet);
}

} // namespace openblack::player_symbol_sheet
