/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 2's silver scroll The Sea as quest runs on the game's own Land 2 (docs/wiki/project/scenarios.md, "Playing a
// timeline on a land"), each played after a quest run's start (quest=SetupLand2/EnterLand2): once the arrival is
// over, the quest's script is started as the land's watcher starts it once the player holds the two towns it waits
// for, which the run does not wait for. The timeline presses the scroll at the mother's hut with the hand, lets the
// introduction play and brings the camera down to the bay; then it plays one ending. The bay is shut to the player's
// influence, so the creature would fetch the boys: the hand's takes stand in for it. The points are map coordinates,
// taken from the places the quest's script names; none has yet been seen in a running game.

#include <cstddef>
#include <cstdint>

#include <array>
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
/// The mother's hut, where the scroll is put up and the mother made, and the spot she wanders round
constexpr glm::vec2 k_Hut {2781.1104f, 3053.49f};
constexpr glm::vec2 k_SoapBox {2786.5073f, 3064.325f};
/// The five spots in the bay the boys are made at, the first being the swimming spot the script watches
constexpr std::array<glm::vec2, 5> k_SwimSpots {{
    {2964.223f, 3084.874f},
    {2968.614f, 3089.957f},
    {2969.778f, 3082.155f},
    {2962.422f, 3078.884f},
    {2960.591f, 3084.722f},
}};
/// The beach the mother is taken to, within 40 of the swimming spot
constexpr glm::vec2 k_Shore {2943.603f, 3095.944f};
/// Where each boy is put down on the cliff top to walk home: about 12 from the hut, so he is not yet home when he lands,
/// in a row through the mother's spot across the way home, 4 apart. A boy let go onto one standing where the last was put
/// down knocks him over, and a walk the script orders while he flies is not taken
constexpr std::array<glm::vec2, 5> k_WalkHomeFrom {{
    {2779.3f, 3067.9f},
    {2782.9f, 3066.1f},
    k_SoapBox,
    {2790.1f, 3062.5f},
    {2793.7f, 3060.8f},
}};
/// The mother, as the script makes her: an Indian housewife (the boys are Indian farmers it makes children)
constexpr uint32_t k_Housewife = 28;
/// The challenge's number, which its entry in the challenge log goes by
constexpr size_t k_Challenge = 9;
/// The camera over the bay: within the 20 of the swimming spot the boys' call needs, the spot on the screen
constexpr float k_BayEyeHeight = 10.0f;
constexpr float k_BayEyeBack = 14.0f;
/// The camera over the cliff top, where a boy is put down
constexpr float k_CliffEyeHeight = 30.0f;
constexpr float k_CliffEyeBack = 40.0f;
/// The introduction's last camera moves and the end of its scene, after its last line
constexpr float k_IntroductionEndSeconds = 12.0f;

/// The globals and scripts the quest is read by
constexpr std::string_view k_Globals = "BaywatchHomeCounter BaywatchSwimmingCounter BaywatchMurder";
constexpr std::string_view k_Scripts = "BaywatchMain BaywatchSwimmingBoy BaywatchSwimmingLaugh ChallengeHighlightNotify";

/// Logs the globals and scripts, some seconds after the command before it, and again every period if given
Command Log(float delay, size_t period = 0)
{
	return {.kind = Kind::LogGlobals, .delaySeconds = delay, .value = period, .global = k_Globals, .script = k_Scripts};
}

/// A boy at a spot: the script makes him an Indian farmer and then sets his age to 11, so the scripts see a child
[[nodiscard]] ThingNear Boy(glm::vec2 at)
{
	return {.type = script::ObjectType::VillagerChild, .point = at, .radius = 3.0f};
}

/// [shortcut] A thing taken into the hand at once, in place of the creature fetching it, then let go over a point of
/// the land with the camera on it: it falls from the hand there
void TakeAndDrop(std::vector<Command>& commands, ThingNear thing, glm::vec2 to, float eyeHeight, float eyeBack)
{
	commands.push_back({.kind = Kind::HandTake, .delaySeconds = 1.0f, .thing = thing});
	commands.push_back(CameraOn(to, eyeHeight, eyeBack, 0.0f));
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 1.0f, .point = to});
	commands.push_back({.kind = Kind::HandDrop, .delaySeconds = 1.0f});
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 0.5f, .value = 1});
}

/// The arrival let play, the quest started as the land's watcher starts it, its scroll pressed on with the hand, the
/// introduction let play and the camera brought down over the bay, where a boy calls out
std::vector<Command> Introduction()
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("EnterLand2", TaskWait::Finished),
	    StartScript("BaywatchMain"),
	    // the scroll's notifier runs once the quest has made the scroll
	    WaitTask("ChallengeHighlightNotify"),
	    Log(0.0f, 30),
	    // [camera] the scroll at the mother's hut
	    CameraOn(k_Hut, k_EyeAbove, k_EyeBack, 0.0f),
	};
	// [real] the scroll pressed on
	PressScroll(commands, k_Hut);
	commands.push_back(WaitText(1589));
	// [camera] over the bay once the introduction's scene is over
	commands.push_back(CameraOn(k_SwimSpots.at(0), k_BayEyeHeight, k_BayEyeBack, 0.0f));
	commands.back().delaySeconds = k_IntroductionEndSeconds;
	commands.push_back(WaitText(1590));
	commands.push_back(Log(1.0f));
	return commands;
}

/// The challenge's entry tapped in the challenge log: its reminder, the good advisor's line
void Replay(std::vector<Command>& commands)
{
	commands.push_back({.kind = Kind::ReplayChallenge, .delaySeconds = 2.0f, .value = k_Challenge});
	commands.push_back(WaitText(1602));
	commands.push_back(Log(2.0f));
}

std::vector<Command> AllHome()
{
	auto commands = Introduction();
	// the first boy lifted out and let go over the water again: he swims on
	TakeAndDrop(commands, Boy(k_SwimSpots.at(0)), k_SwimSpots.at(0), k_BayEyeHeight, k_BayEyeBack);
	commands.push_back(Log(3.0f));
	// each boy lifted out and put down on the cliff top, each at his own spot by the mother's, so he walks home
	for (size_t i = 0; i < k_SwimSpots.size(); ++i)
	{
		TakeAndDrop(commands, Boy(k_SwimSpots.at(i)), k_WalkHomeFrom.at(i), k_CliffEyeHeight, k_CliffEyeBack);
	}
	// the all-home ending, the mother's thanks and the reward dispenser, waited for at once: the boys reach the hut
	// soon after the last is put down
	commands.push_back(WaitText(1593));
	commands.push_back(WaitTask("BaywatchMain", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	Replay(commands);
	return commands;
}

std::vector<Command> Beach()
{
	auto commands = Introduction();
	// the mother taken from her spot and let go over the beach, within 40 of the swimming boys
	TakeAndDrop(commands,
	            ThingNear {.type = script::ObjectType::Villager, .subtype = k_Housewife, .point = k_SoapBox, .radius = 10.0f},
	            k_Shore, k_BayEyeHeight, k_BayEyeBack);
	// the beach ending: a boy's cry, the family put back at the hut, the mother's thanks
	commands.push_back(WaitText(1593));
	commands.push_back(WaitTask("BaywatchMain", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	return commands;
}

std::vector<Command> OneKilled()
{
	auto commands = Introduction();
	// [shortcut] the last boy killed in the water by a script's SET_PROPERTY of his health, in place of the player's
	// or the creature's blow: the advisors' two lines
	commands.push_back({.kind = Kind::ScriptSetHealth, .delaySeconds = 1.0f, .amount = 0.0f, .thing = Boy(k_SwimSpots.at(4))});
	commands.push_back(WaitText(1598));
	commands.push_back(Log(1.0f));
	// the other four lifted out and put down on the cliff top, each at his own spot, so they walk home
	for (size_t i = 0; i < 4; ++i)
	{
		TakeAndDrop(commands, Boy(k_SwimSpots.at(i)), k_WalkHomeFrom.at(i), k_CliffEyeHeight, k_CliffEyeBack);
	}
	// the ending with a son lost, waited for at once as the all-home one: the mother's line, no reward
	commands.push_back(WaitText(1594));
	commands.push_back(WaitTask("BaywatchMain", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	return commands;
}
} // namespace

void testbed_scenarios::AddQuestSeaScenarios(std::vector<Scenario>& all)
{
	all.push_back({
	    .id = "quest.the_sea",
	    .name = "Land 2 quest run: The Sea, every son brought home",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand2/EnterLand2 on Land2.txt). The texts that wait "
	                   "for a click are clicked on, the arrival's too. Once the arrival is over it starts BaywatchMain, "
	                   "as the land's watcher does; presses on the scroll at the mother's hut with the hand, lets the "
	                   "introduction play and brings the camera down over the bay. The hand then takes the first boy, "
	                   "standing in for the creature, and lets him go over the water again; then takes each boy and "
	                   "lets him go over the cliff top, each at his own spot by the mother's; it lets the ending play "
	                   "and taps the challenge's entry in the challenge log.",
	    .expected = "The anti-influence ring, the scroll; the press makes the mother and the five boys and plays the "
	                "introduction (texts 1588 and 1589, the log at 0). Over the bay a boy says text 1590. The boy let go "
	                "over the water swims on; each boy let go on the cliff says text 1591 and walks home "
	                "(BaywatchHomeCounter up to 5). The mother's text 1593, the Big dispenser by the hut and the log at "
	                "1 with alignment 1; the replay says text 1602. Unverified: that the hand can take the boys inside "
	                "the anti-influence ring.",
	    .commands = AllHome(),
	    .onLand = true,
	});
	all.push_back({
	    .id = "quest.the_sea_beach",
	    .name = "Land 2 quest run: The Sea, the mother taken to the beach",
	    .facet = Facet::Story,
	    .description = "Plays the introduction as quest.the_sea does, then takes the mother into the hand, standing in "
	                   "for the player's grab, and lets her go over the beach by the swimming boys.",
	    .expected = "With all five boys swimming the beach ending plays: text 1592, the fade, the family put back at the "
	                "hut, text 1593 and the log at 1 with alignment 0.8, with no reward.",
	    .commands = Beach(),
	    .onLand = true,
	});
	all.push_back({
	    .id = "quest.the_sea_one_killed",
	    .name = "Land 2 quest run: The Sea, one son killed",
	    .facet = Facet::Story,
	    .description = "Plays the introduction as quest.the_sea does, kills the last boy in the water by a script's "
	                   "SET_PROPERTY of his health, standing in for a blow, then brings the other four home as "
	                   "quest.the_sea does.",
	    .expected = "The death brings texts 1597 and 1598 (BaywatchMurder=1); once the four are home the ending's log "
	                "update to 1 with alignment -0.2 and the mother's text 1594, with no reward.",
	    .commands = OneKilled(),
	    .onLand = true,
	});
}
