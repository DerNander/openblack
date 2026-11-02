/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

#pragma once

#include <vector>

#include <entt/entity/fwd.hpp>

#include "Creature/CaveCopyPick.h"

namespace openblack::ecs
{
class Registry;
}

namespace openblack::ecs::creature_skin
{

/// A creature's skin as it is drawn this frame, in the world: each sub-mesh of its own mesh the game's mesh test looks
/// at (not a physics one, of the nearest level of detail), its corners posed by the bones as drawn (else the mesh's rest
/// pose) and pulled towards the species' shapes as the body is; none without a loaded mesh
[[nodiscard]] std::vector<cave_copy_pick::Part> DrawnSkin(const Registry& registry, entt::entity creature);

} // namespace openblack::ecs::creature_skin
