/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 2's silver scroll The Idol as quest runs on the game's own Land 2 (docs/wiki/project/scenarios.md, "Playing a
// timeline on a land"), each played after a quest run's start (quest=SetupLand2/EnterLand2): once the arrival is
// over, the quest's script is started as the land's control script starts it once the player holds the idol's town
// and five more, which the run does not wait for. The timeline drives it as a player would: the scroll by the idol is
// pressed on with the hand and the introduction is let play; then the idol is set alight and kept burning, in place
// of the player's fire, until the script takes it as burnt, and the ending and the reward are let play. One run first
// lets the watch go on for three minutes, so the first worshippers set out; the other kills the idol's maker before
// the fire. The points are map coordinates, taken from the places the quest's script names; none has yet been seen in
// a running game.

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
/// The idol's spot, where the scroll is put up and the idol made; the maker's spot, about 12 in front of it
constexpr glm::vec2 k_Idol {2695.73f, 3662.4f};
constexpr glm::vec2 k_Maker {2698.915f, 3650.481f};
/// The idol, a mobile static of the idol's info, and its maker, an Indian trader villager, as the script makes them
constexpr uint32_t k_IdolInfo = 10;
constexpr uint32_t k_IndianTrader = 34;
constexpr ThingNear k_IdolPick {
    .type = script::ObjectType::MobileStatic, .subtype = k_IdolInfo, .point = k_Idol, .radius = 5.0f};
constexpr ThingNear k_MakerPick {
    .type = script::ObjectType::Villager, .subtype = k_IndianTrader, .point = k_Maker, .radius = 5.0f};
/// The challenge's number, which its entry in the challenge log goes by
constexpr size_t k_Challenge = 10;
/// The camera while the watch runs: well back and up from the idol, so the idol's spot stays on the screen and the
/// camera within the 150 the ending waits for
constexpr float k_WatchEyeHeight = 40.0f;
constexpr float k_WatchEyeBack = 60.0f;
/// The introduction's last camera move, the snapshot after it and the end of its scene, after its last line
constexpr float k_IntroductionEndSeconds = 15.0f;
/// How long the watch is let run before the fire: past the 1801 passes of a tenth of a second after which the first
/// worshippers are sent
constexpr float k_WorshippersSeconds = 190.0f;
/// The fire kept going: the idol set alight again every 2 seconds, so each of the script's looks, 2.1 seconds apart,
/// finds it burning, for longer than the 16 looks in a row that burn the idol
constexpr int k_FireTimes = 24;
constexpr float k_FireEverySeconds = 2.0f;

/// The globals and scripts the quest is read by
constexpr std::string_view k_Globals = "IdolAlive";
constexpr std::string_view k_Scripts = "IdolPyreMain ChallengeHighlightNotify StandardReminder";

/// Logs the globals and scripts, some seconds after the command before it, and again every period if given
Command Log(float delay, size_t period = 0)
{
	return {.kind = Kind::LogGlobals, .delaySeconds = delay, .value = period, .global = k_Globals, .script = k_Scripts};
}

/// The arrival let play, the quest started as the land's control script starts it, its scroll pressed on with the hand
/// and its introduction let play to its last line
std::vector<Command> Introduction()
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("EnterLand2", TaskWait::Finished),
	    StartScript("IdolPyreMain"),
	    // the scroll's notifier runs once the quest has made the idol and the scroll
	    WaitTask("ChallengeHighlightNotify"),
	    Log(0.0f, 30),
	    // [camera] the scroll by the idol
	    CameraOn(k_Idol, k_EyeAbove, k_EyeBack, 5.0f),
	};
	// [real] the scroll pressed on
	PressScroll(commands, k_Idol);
	commands.push_back(WaitText(1681));
	// [camera] the idol from well back once the introduction's scene is over
	commands.push_back(CameraOn(k_Idol, k_WatchEyeHeight, k_WatchEyeBack, 5.0f));
	commands.back().delaySeconds = k_IntroductionEndSeconds;
	commands.push_back(Log(1.0f));
	return commands;
}

/// [shortcut] The idol set alight by a script's SET_ON_FIRE, again and again, in place of the fire the player would
/// start and keep going near it, until the script takes it as burnt
void KeepTheIdolBurning(std::vector<Command>& commands)
{
	for (int i = 0; i < k_FireTimes; ++i)
	{
		commands.push_back({.kind = Kind::ScriptSetOnFire,
		                    .delaySeconds = i == 0 ? 1.0f : k_FireEverySeconds,
		                    .value = 1,
		                    .amount = 1.0f,
		                    .thing = k_IdolPick});
	}
	commands.push_back(WaitGlobal("IdolAlive", 1.0f));
	commands.push_back(Log(0.5f));
}

std::vector<Command> IdolBurnt()
{
	auto commands = Introduction();
	// the watch goes on until the first worshippers set out from the town
	commands.push_back(Log(k_WorshippersSeconds));
	KeepTheIdolBurning(commands);
	// the ending: the idol's last scene, the maker's plea, the evil advisor's remark and the reward's two lines
	commands.push_back(WaitText(1686));
	commands.push_back(WaitText(1685));
	commands.push_back(WaitText(6109));
	commands.push_back(WaitTask("IdolPyreMain", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	// the challenge's entry tapped in the challenge log: its reminder, the good advisor's line
	commands.push_back({.kind = Kind::ReplayChallenge, .delaySeconds = 2.0f, .value = k_Challenge});
	commands.push_back(WaitText(2483));
	commands.push_back(Log(2.0f));
	return commands;
}

std::vector<Command> MakerKilled()
{
	auto commands = Introduction();
	// [shortcut] the maker killed by a script's SET_PROPERTY of his health, in place of the player's blow
	commands.push_back({.kind = Kind::ScriptSetHealth, .delaySeconds = 2.0f, .amount = 0.0f, .thing = k_MakerPick});
	// the advisors' two lines on his death
	commands.push_back(WaitText(1683));
	commands.push_back(Log(1.0f));
	KeepTheIdolBurning(commands);
	// the ending with the maker dead: the idol's last scene, the good advisor's line again and the reward's two lines
	commands.push_back(WaitText(1686));
	commands.push_back(WaitText(6109));
	commands.push_back(WaitTask("IdolPyreMain", TaskWait::Finished));
	commands.push_back(Log(2.0f));
	return commands;
}
} // namespace

void testbed_scenarios::AddQuestIdolScenarios(std::vector<Scenario>& all)
{
	all.push_back({
	    .id = "quest.the_idol",
	    .name = "Land 2 quest run: The Idol, the idol burnt",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand2/EnterLand2 on Land2.txt). The texts that wait "
	                   "for a click are clicked on, the arrival's too. Once the arrival is over it starts IdolPyreMain, "
	                   "as the land's control script does; presses on the scroll by the idol with the hand and lets the "
	                   "introduction play; lets the watch run for three minutes; then sets the idol alight every 2 "
	                   "seconds by a script's SET_ON_FIRE, standing in for the player's fire, until the script takes it "
	                   "as burnt, and lets the ending and the reward play; then taps the challenge's entry in the "
	                   "challenge log.",
	    .expected = "The idol and the scroll are made; the press starts the introduction: the maker made and praying, "
	                "texts 1678 to 1681, the snapshot at 0 and the idol's chant. After three minutes the first two "
	                "worshippers set out from the town. With the idol burning, 16 looks in a row find fire "
	                "(IdolAlive=1): the idol is set on fire for good, the ending's snapshot at 1 with alignment 0.2, "
	                "text 1686, the bangs and flashes and the idol deleted, the maker's plea (1687, or 1688 with "
	                "worshippers), the scroll deleted, the evil advisor's 1685, the fireball's two levels given to the "
	                "town and texts 6108 and 6109; the log's entry replayed says text 2483. Unverified: that the burning idol "
	                "is found by the fire question, "
	                "and how long a fire set on it lasts.",
	    .commands = IdolBurnt(),
	    .onLand = true,
	});
	all.push_back({
	    .id = "quest.the_idol_maker_dead",
	    .name = "Land 2 quest run: The Idol, the maker killed",
	    .facet = Facet::Story,
	    .description = "Plays the introduction as quest.the_idol does, then kills the idol's maker by a script's "
	                   "SET_PROPERTY of his health, standing in for the player's blow, and keeps the idol burning as "
	                   "quest.the_idol does until the script takes it as burnt.",
	    .expected = "The maker's death brings texts 1684 and 1683 and the log updated to 0.5 with alignment -0.6; the "
	                "idol then burns as in quest.the_idol, the ending skips the maker's plea, and the good advisor "
	                "repeats 1684 with the log updated to 1 with alignment -1.0 before the reward's texts 6108 and "
	                "6109.",
	    .commands = MakerKilled(),
	    .onLand = true,
	});
}
