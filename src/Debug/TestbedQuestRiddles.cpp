/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 2's silver scroll The Riddles as a quest run on the game's own Land 2 (docs/wiki/project/scenarios.md, "Playing a
// timeline on a land"): after the arrival (quest=SetupLand2/EnterLand2), the Indian village with town id 13 is handed to
// the player, as winning it would, and the quest's script is started as the land's control script starts it once its
// check has seen the village won. The scroll at Annika's hut is pressed on with the hand and her riddle's texts are
// clicked on; then the riddle's four things are put in the ring of toadstools at once: a creature's dropping and a wolf
// by the scripts' own create and set position, where the creature and the hand would bring them, a shield miracle drawn
// over the ring with the hand, and a fireball thrown into it with the hand. The zebra's scroll is then pressed on. The
// points are map coordinates, taken from the places the quest's script names and the wolves the land's map script
// makes; none has yet been seen in a running game.

#include <cstddef>

#include <string_view>
#include <vector>

#include "TestbedLand2Quests.h"
#include "TestbedScenarioRegistry.h"

using namespace openblack;
using namespace openblack::testbed_scenarios;
using namespace openblack::testbed_scenarios::land2_quests;

namespace
{
using testbed_land_timeline::ScriptArgument;

/// The second Indian village, where the land's map script makes it
constexpr glm::vec2 k_Village {2771.34f, 2444.15f};
constexpr ThingNear k_VillageThing {.type = script::ObjectType::Town, .point = k_Village, .radius = 30.0f};
/// The belief the village is given, more than anyone holds in it
constexpr float k_WinningBelief = 2.0f;
/// Annika's hut, where the scroll is put up
constexpr glm::vec2 k_Hut {2645.4934f, 2477.0095f};
/// The centre of the ring of toadstools, and a spot inside the ring a little off it where the fireball is thrown
constexpr glm::vec2 k_Ring {2630.444f, 2524.9048f};
constexpr glm::vec2 k_FireSpot {2633.444f, 2527.9048f};
/// A wolf of the pack the land's map script makes east of the ring
constexpr ThingNear k_WolfThing {
    .type = script::ObjectType::Animal, .subtype = 2, .point = {2681.0f, 2392.0f}, .radius = 30.0f};
/// The scripts' type of a creature's dropping
constexpr size_t k_PooType = static_cast<size_t>(script::ObjectType::Poo);

/// The globals and scripts the quest is read by
constexpr std::string_view k_Globals = "PuzzleSolved WolfPackUnwanted";
constexpr std::string_view k_Scripts = "LostTreasureMain Riddle CheckObjects NightWolves SwapCreatures";

/// The village takes more belief in the player than in anyone else, as a gift gives it; its next fold hands it over
Command WinVillage()
{
	return {.kind = Kind::TownGainsBelief,
	        .player = PlayerNames::PLAYER_ONE,
	        .delaySeconds = 1.0f,
	        .amount = k_WinningBelief,
	        .thing = k_VillageThing};
}

std::vector<Command> Riddles()
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("EnterLand2", TaskWait::Finished),
	    // [shortcut] the village takes more belief in the player than in anyone else, as winning it over would
	    // give it; its next fold hands it to the player (a direct change of hands is undone by that fold)
	    WinVillage(),
	    // the quest's script, started as the land's control script starts it once its check sees the village won
	    StartScript("LostTreasureMain"),
	    WaitTask("ChallengeHighlightNotify"),
	    {.kind = Kind::LogGlobals, .delaySeconds = 1.0f, .value = 60, .global = k_Globals, .script = k_Scripts},
	    // [camera] the scroll over Annika's hut
	    CameraOn(k_Hut, k_EyeAbove, k_EyeBack, 5.0f),
	};
	// [real] the scroll pressed on
	PressScroll(commands, k_Hut);
	// the riddle: Annika's lines and clues, and the good advisor's line after it
	commands.push_back(WaitText(1628));
	commands.push_back(WaitText(1623));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 2.0f));
	// [camera] the ring of toadstools, from above
	commands.push_back(CameraOn(k_Ring, 40.0f, 30.0f, 0.0f));
	// [shortcut] a creature's dropping made in the ring by a script's create, where the creature would leave one
	commands.push_back({.kind = Kind::ScriptCreate, .delaySeconds = 1.0f, .point = k_Ring, .value = k_PooType});
	// [real] a shield miracle given to the hand, as if from a bubble, and drawn as a circle over the ring
	commands.push_back({.kind = Kind::HoldSeed, .delaySeconds = 1.0f, .value = static_cast<size_t>(SpellSeedType::Shield)});
	commands.push_back(
	    {.kind = Kind::DrawGesture, .delaySeconds = 1.0f, .point = k_Ring, .value = static_cast<size_t>(GestureType::Circle)});
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 3.0f));
	// a fire miracle given to the hand, as if from a bubble
	commands.push_back({.kind = Kind::HoldSeed, .delaySeconds = 1.0f, .value = static_cast<size_t>(SpellSeedType::Fire)});
	// [shortcut] a wolf put into the ring, where the hand would carry one
	commands.push_back({.kind = Kind::MoveThing, .delaySeconds = 1.0f, .point = k_Ring, .thing = k_WolfThing});
	// [real] the fireball thrown into the ring at once, before the wolf walks off
	commands.push_back({.kind = Kind::HandThrowAt, .delaySeconds = 0.3f, .point = k_FireSpot});
	// the riddle solved: the zebra's arrival and the advisors' lines on it
	commands.push_back(WaitText(1626));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 5.0f));
	// [real] the zebra's scroll pressed on: with the player's creature away from it, the evil advisor's line
	PressScroll(commands, k_Ring);
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 10.0f));
	return commands;
}
} // namespace

void testbed_scenarios::land2_quests::AddRiddlesScenarios(std::vector<Scenario>& all)
{
	all.push_back({
	    .id = "quest.the_riddles",
	    .name = "Land 2 quest run: The Riddles, the riddle solved",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand2/EnterLand2 on Land2.txt). The texts that wait "
	                   "for a click are clicked on, the arrival's too. Once the arrival is over it hands the Indian "
	                   "village with town id 13 to the player and starts the quest's script, as the land's control "
	                   "script does after its check; presses on the scroll at Annika's hut with the hand and lets the "
	                   "riddle play. Then it makes a creature's dropping in the ring of toadstools by a script's create, "
	                   "draws a shield miracle there with the hand, puts a wolf into the ring by a script's set position "
	                   "and throws a fireball into it with the hand; once the zebra has come, it presses on its scroll.",
	    .expected = "The eight toadstools and the scroll are made; the press plays Annika's scene (1622, 1645 to 1649, "
	                "1628) with the log at 0 and the good advisor's 1623. With the wolf, the dropping, the shield and "
	                "the fire in the ring at once the riddle is solved: the wolf and the dropping fade, the log at 1, "
	                "the zebra made and waving, and the advisors' 1623 to 1627. The zebra's scroll draws 5103 with the "
	                "camera near, and its press 5104 with the player's creature away. Unverified: that a thrown "
	                "fireball leaves a fire in the ring while the wolf is still there, and that the drawn shield stands "
	                "within 5 of the ring's centre.",
	    .commands = Riddles(),
	    .onLand = true,
	});
}
