/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 2's silver scroll The Singing Stones as a quest run on the game's own Land 2 (docs/wiki/project/scenarios.md,
// "Playing a timeline on a land"), played after a quest run's start (quest=SetupLand2/EnterLand2): the quest's
// script is started as the run's timeline begins, as the land's control script starts it beside the arrival. The
// timeline drives it as a player would: once the arrival is over, the scroll on the priest's hut is pressed on with the
// hand and the introduction is let play; then each tune is tapped on the stones with the hand, one stone at a time,
// and its scene is let play: Twinkle Twinkle Little Star by day, which brings the night, then White Christmas and the
// funeral march on the tombstones the night puts in the stones' place. Once the priest has said his line in the
// funeral's scene he is killed inside the ring, so the raising brings him back. The points are map coordinates, taken
// from the places the quest's script names; none has yet been seen in a running game.

#include <cstddef>
#include <cstdint>

#include <array>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

#include "TestbedLand2Quests.h"
#include "TestbedScenarioRegistry.h"

using namespace openblack;
using namespace openblack::testbed_scenarios;
using namespace openblack::testbed_scenarios::land2_quests;

namespace
{
/// The priest's hut, where the scroll is put up and the priest made
constexpr glm::vec2 k_Hut {2134.7f, 3010.71f};
/// The circle's centre, and the nine stones' spots, stone 1 to stone 9
constexpr glm::vec2 k_Centre {2106.779f, 2997.927f};
constexpr std::array<glm::vec2, 9> k_Stones {{
    {2108.5923f, 3010.0273f},
    {2111.929f, 3008.0898f},
    {2115.0037f, 3005.1694f},
    {2116.4893f, 3000.9282f},
    {2117.0657f, 2996.4011f},
    {2114.1128f, 2992.7598f},
    {2110.3665f, 2990.1528f},
    {2105.964f, 2989.086f},
    {2101.9155f, 2989.3716f},
}};
/// By day each stone is a mobile static of the singing stone's info; by night a feature of the tombstone's. How near its
/// spot the pick looks for it: the stones stand about 3.5 apart, each on a base at its own spot
constexpr uint32_t k_SingingStoneInfo = 5;
constexpr uint32_t k_TombstoneInfo = 75;
constexpr float k_StonePickRadius = 1.5f;
/// The priest, a villager of the priest's info, at the spot the funeral's scene walks him to inside the ring
constexpr uint32_t k_Priest = 68;
constexpr glm::vec2 k_PriestInRing {2103.684f, 2999.995f};
/// The camera over the stones: close behind and well above the circle's centre, so each stone is pointed at from above
constexpr float k_StonesEyeHeight = 30.0f;
constexpr float k_StonesEyeBack = 15.0f;
/// The challenge's number, which its entry in the challenge log goes by
constexpr size_t k_Challenge = 12;
/// Seconds between two taps
constexpr float k_TapSeconds = 1.0f;

/// The tunes, by the stones tapped. Twinkle Twinkle Little Star is fixed stones; White Christmas (one up, one down, one
/// down, one up, one up, one up, one up) and the funeral march (the same four times, three up, one down, the same, two
/// down, the same, one down, one up) are steps, played here from stones 5 and 2
constexpr std::array<int, 14> k_Twinkle {1, 1, 8, 8, 9, 9, 8, 6, 6, 5, 5, 3, 3, 1};
constexpr std::array<int, 8> k_WhiteChristmas {5, 6, 5, 4, 5, 6, 7, 8};
constexpr std::array<int, 11> k_Funeral {2, 2, 2, 2, 5, 4, 4, 2, 2, 1, 2};

/// The globals and scripts the quest is read by
constexpr std::string_view k_Globals =
    "TinkleComplete FuneralComplete WhiteChristmasComplete SSMidnight SSDoneBats SSBringBackToLifeActive SSTapCount";
constexpr std::string_view k_Scripts = "SingingStonesSongsMain ChallengeHighlightNotify Introduction CheckForTap "
                                       "TwinkleSequence FuneralSequence WhiteChristmasSequence BringBackTheDead";

/// Logs the globals and scripts, some seconds after the command before it, and again every period if given
Command Log(float delay, size_t period = 0)
{
	return {.kind = Kind::LogGlobals, .delaySeconds = delay, .value = period, .global = k_Globals, .script = k_Scripts};
}

/// A tune tapped with the hand: for each stone, the pointer on it, the Action button pressed once the hand is over it
/// and let go, as a player taps it
void TapTune(std::vector<Command>& commands, std::span<const int> stones, uint32_t stoneInfo)
{
	for (const int stone : stones)
	{
		const ThingNear pick {.type = stoneInfo == k_SingingStoneInfo ? script::ObjectType::MobileStatic
		                                                              : script::ObjectType::Feature,
		                      .subtype = stoneInfo,
		                      .point = k_Stones.at(static_cast<size_t>(stone - 1)),
		                      .radius = k_StonePickRadius};
		commands.push_back({.kind = Kind::PointerTo, .delaySeconds = k_TapSeconds, .thing = pick});
		commands.push_back(
		    {.kind = Kind::PointerPress, .delaySeconds = 0.3f, .waitUntilFree = true, .value = k_ActionButton, .thing = pick});
		commands.push_back({.kind = Kind::PointerRelease, .delaySeconds = 0.2f, .value = k_ActionButton});
	}
}

std::vector<Command> ThreeTunes()
{
	std::vector<Command> commands {
	    ClickTexts(),
	    StartScript("SingingStonesSongsMain"),
	    WaitTask("EnterLand2", TaskWait::Finished),
	    Log(0.0f, 30),
	    // [camera] the scroll on the priest's hut
	    CameraOn(k_Hut, k_EyeAbove, k_EyeBack, 5.0f),
	};
	// [real] the scroll pressed on
	PressScroll(commands, k_Hut);
	// the introduction: the priest's walk down to the stones and his four lines, then the log at 0
	commands.push_back(WaitText(6018));
	commands.push_back(WaitTask("Introduction", TaskWait::Finished));
	commands.push_back(Log(1.0f));
	// [camera] over the stones
	commands.push_back(CameraOn(k_Centre, k_StonesEyeHeight, k_StonesEyeBack, 0.0f));
	// [real] Twinkle Twinkle Little Star tapped by day: its scene brings the night, and the stones become tombstones
	TapTune(commands, k_Twinkle, k_SingingStoneInfo);
	commands.push_back(WaitTask("TwinkleSequence"));
	commands.push_back(WaitTask("TwinkleSequence", TaskWait::Finished));
	commands.push_back(WaitGlobal("SSMidnight", 1.0f));
	commands.push_back(Log(1.0f));
	// [camera] over the tombstones; [real] White Christmas tapped on them: its scene and its snow
	commands.push_back(CameraOn(k_Centre, k_StonesEyeHeight, k_StonesEyeBack, 0.0f));
	TapTune(commands, k_WhiteChristmas, k_TombstoneInfo);
	commands.push_back(WaitTask("WhiteChristmasSequence"));
	commands.push_back(WaitTask("WhiteChristmasSequence", TaskWait::Finished));
	commands.push_back(Log(1.0f));
	// [camera] over the tombstones; [real] the funeral march tapped on them: its scene walks the priest into the ring,
	// starts the raising and has him say his line
	commands.push_back(CameraOn(k_Centre, k_StonesEyeHeight, k_StonesEyeBack, 0.0f));
	TapTune(commands, k_Funeral, k_TombstoneInfo);
	commands.push_back(WaitTask("FuneralSequence"));
	commands.push_back(WaitText(6019));
	// [shortcut] the priest killed inside the ring by a script's SET_PROPERTY of his health, in place of the player's
	// blow: the raising finds his body and brings him back as a skeleton
	commands.push_back(
	    {.kind = Kind::ScriptSetHealth,
	     .delaySeconds = 1.0f,
	     .amount = 0.0f,
	     .thing =
	         ThingNear {.type = script::ObjectType::Villager, .subtype = k_Priest, .point = k_PriestInRing, .radius = 5.0f}});
	commands.push_back(WaitTask("FuneralSequence", TaskWait::Finished));
	commands.push_back(WaitText(6020));
	commands.push_back(Log(5.0f));
	// the challenge's entry tapped in the challenge log: its reminder, the two advisors' lines
	commands.push_back({.kind = Kind::ReplayChallenge, .delaySeconds = 2.0f, .value = k_Challenge});
	commands.push_back(WaitText(2540));
	commands.push_back(Log(2.0f));
	return commands;
}
} // namespace

void testbed_scenarios::AddQuestSingingStonesLand2Scenarios(std::vector<Scenario>& all)
{
	all.push_back({
	    .id = "quest.the_singing_stones_land_2",
	    .name = "Land 2 quest run: The Singing Stones, the three tunes",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand2/EnterLand2 on Land2.txt). The texts that wait "
	                   "for a click are clicked on, the arrival's too. It starts SingingStonesSongsMain as the timeline "
	                   "begins, as the land's control script does beside the arrival; once the arrival is over, presses "
	                   "on the scroll on the priest's hut with the hand and lets the introduction play; taps Twinkle "
	                   "Twinkle Little Star on the stones with the hand and lets its scene play; once the night has put "
	                   "the tombstones in their place, taps White Christmas and then the funeral march on them, letting "
	                   "each scene play; once the priest has said his line in the ring, kills him there by a script's "
	                   "SET_PROPERTY of his health, standing in for the player's blow; then taps the "
	                   "challenge's entry in the challenge log.",
	    .expected = "The stones, their bases, the whistlers and the scroll are made; the press brings the priest and "
	                "the introduction (texts 6015 to 6018) and the log at 0. Each tap plays the stone's note and its "
	                "sparkle. Twinkle's scene makes the bats and the mist and moves the time to 23:00 (log at 0.33, "
	                "TinkleComplete=1, SSDoneBats=1); the tombstones come (SSMidnight=1). White Christmas's scene makes "
	                "the snow (log at 0.66); the funeral march's walks the priest into the ring, starts the raising "
	                "(SSBringBackToLifeActive=1) and his text 6019 (log at 0.99, then 1). His body is raised as a "
	                "skeleton and the evil advisor says text 6020; the log's entry replayed says texts 2539 and 2540. "
	                "Unverified: that the hand's taps reach the stones "
	                "without the player's influence there, and that the priest's walk ends exactly on the spot the "
	                "funeral's scene waits for.",
	    .commands = ThreeTunes(),
	    .onLand = true,
	});
}
