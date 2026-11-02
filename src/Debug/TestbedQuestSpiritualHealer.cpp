/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 2's village event The Spiritual Healer as quest runs on the game's own Land 2 (docs/wiki/project/scenarios.md,
// "Playing a timeline on a land"), each played after a quest run's start (quest=SetupLand2/EnterLand2): once the
// arrival is over, the event's script is started as the land's watcher starts it once the player holds the Greek
// village it waits for, which the run does not wait for. The camera watches the village while the healer takes his
// first children; then one run picks him up and puts him down by the village, then twice far away, banishing him; the
// other kills him. The hand's takes stand in for the player's grab outside the player's influence. The points are map
// coordinates, taken from the places the event's scripts name; none has yet been seen in a running game.

#include <cstddef>
#include <cstdint>

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
/// The healer's spot by the village centre, which his rounds go about
constexpr glm::vec2 k_Village {2017.1619f, 2594.753f};
/// A spot of the land far from the village, more than 200 from it: a did-you-know sign's, east of it
constexpr glm::vec2 k_FarAway {2739.382f, 3052.035f};
/// The healer, a villager of the priest's info, found anywhere on his rounds
constexpr uint32_t k_Priest = 68;
constexpr ThingNear k_HealerPick {
    .type = script::ObjectType::Villager, .subtype = k_Priest, .point = k_Village, .radius = 80.0f};
/// The camera on the village: well back and up, within the 100 and the 150 the healer's lines need, and him on screen
constexpr float k_EyeHeight = 40.0f;
constexpr float k_EyeBack = 60.0f;
/// The camera on the far spot, where the healer lands
constexpr float k_FarEyeHeight = 30.0f;
constexpr float k_FarEyeBack = 40.0f;
/// How long the healer is let take children before he is picked up: past his first two, half a minute apart
constexpr float k_ChildrenSeconds = 75.0f;
/// How long he is held: past the half minute between his looks for a child, when his being held is seen
constexpr float k_HoldSeconds = 35.0f;
/// The four villagers made at his house three minutes after the end
constexpr float k_NewVillagersSeconds = 185.0f;

/// The globals and scripts the event is read by
constexpr std::string_view k_Globals = "SaidSpiritualHealerChildKillCount";
constexpr std::string_view k_Scripts = "SpiritualHealerMain Walkabout TownAge KillChild HealerKilledOrBanished "
                                       "FirstTimeBanished LandedSafeInTown";

/// Logs the globals and scripts, some seconds after the command before it, and again every period if given
Command Log(float delay, size_t period = 0)
{
	return {.kind = Kind::LogGlobals, .delaySeconds = delay, .value = period, .global = k_Globals, .script = k_Scripts};
}

/// The arrival let play, the event started as the land's watcher starts it, and the camera on the village while the
/// healer takes his first children
std::vector<Command> Beginning()
{
	return {
	    ClickTexts(),
	    WaitTask("EnterLand2", TaskWait::Finished),
	    StartScript("SpiritualHealerMain"),
	    Log(0.0f, 30),
	    // [camera] the village
	    CameraOn(k_Village, k_EyeHeight, k_EyeBack, 0.0f),
	};
}

/// [shortcut] The healer taken into the hand at once, in place of the player's grab, held, then let go over a point
/// of the land with the camera on it: he falls from the hand there
void TakeHealerTo(std::vector<Command>& commands, glm::vec2 to, float eyeHeight, float eyeBack)
{
	commands.push_back({.kind = Kind::HandTake, .delaySeconds = 1.0f, .thing = k_HealerPick});
	commands.push_back(CameraOn(to, eyeHeight, eyeBack, 0.0f));
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 1.0f, .point = to});
	commands.push_back({.kind = Kind::HandDrop, .delaySeconds = k_HoldSeconds});
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 0.5f, .value = 1});
}

std::vector<Command> Banished()
{
	auto commands = Beginning();
	commands.push_back(Log(k_ChildrenSeconds));
	// put down by the village: he lands safe and his rounds start again
	TakeHealerTo(commands, k_Village, k_EyeHeight, k_EyeBack);
	commands.push_back(WaitTask("LandedSafeInTown", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	// put down far away: his first warning
	TakeHealerTo(commands, k_FarAway, k_FarEyeHeight, k_FarEyeBack);
	commands.push_back(WaitTask("FirstTimeBanished", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	// put down far away again: banished for good, with the camera on him, and the village dies
	TakeHealerTo(commands, k_FarAway, k_FarEyeHeight, k_FarEyeBack);
	commands.push_back(WaitText(1720));
	commands.push_back(WaitTask("HealerKilledOrBanished", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	// three minutes later four villagers come to his house
	commands.push_back(WaitTask("SpiritualHealerMain", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	return commands;
}

std::vector<Command> Killed()
{
	auto commands = Beginning();
	// [shortcut] the healer killed by a script's SET_PROPERTY of his health, in place of the player's blow, before he
	// takes his first child
	commands.push_back({.kind = Kind::ScriptSetHealth, .delaySeconds = 15.0f, .amount = 0.0f, .thing = k_HealerPick});
	commands.push_back(WaitText(1720));
	commands.push_back(WaitTask("HealerKilledOrBanished", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	commands.push_back(WaitTask("SpiritualHealerMain", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	return commands;
}
} // namespace

void testbed_scenarios::AddQuestSpiritualHealerScenarios(std::vector<Scenario>& all)
{
	all.push_back({
	    .id = "quest.the_spiritual_healer",
	    .name = "Land 2 quest run: The Spiritual Healer, banished",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand2/EnterLand2 on Land2.txt). The texts that wait "
	                   "for a click are clicked on, the arrival's too. Once the arrival is over it starts "
	                   "SpiritualHealerMain, as the land's watcher does, and watches the village for a little over a "
	                   "minute; then takes the healer into the hand, standing in for the player's grab, holds him and "
	                   "lets him go by the village, then twice far away from it, and waits for the four villagers.",
	    .expected = "The healer is made at his house and walks his rounds, calling out text 1722 near the camera; every "
	                "half minute he takes a child of the village, the first two with the advisors' texts 1726 and 1727 "
	                "and his 1721. Held, he says 1718; let go by the village, 1723 and his rounds again; far away, "
	                "1724; far away again the banishment scene (1719, the fade, the village's villagers dead, 1720). "
	                "Three minutes later four villagers are made at his house. Unverified: that the village has "
	                "children, and that the hand can take him outside the player's influence.",
	    .commands = Banished(),
	    .onLand = true,
	});
	all.push_back({
	    .id = "quest.the_spiritual_healer_killed",
	    .name = "Land 2 quest run: The Spiritual Healer, killed",
	    .facet = Facet::Story,
	    .description = "Plays the beginning as quest.the_spiritual_healer does, then kills the healer by a script's "
	                   "SET_PROPERTY of his health, standing in for the player's blow, before he takes a child.",
	    .expected = "The killed scene: text 1725, the fade, the village's villagers dead, texts 1723 and 1720; three "
	                "minutes later four villagers are made at his house.",
	    .commands = Killed(),
	    .onLand = true,
	});
}
