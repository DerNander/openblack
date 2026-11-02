/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 5's gold scroll I Have a Surprise for You as a quest run, played after a land entry's quest= has started the
// land's control script: the land set up, the opening film, the curse and the loop on the three Wonder villages. The
// timeline does what the player would: it clicks the texts that wait for a click, lets the first night's curse strike,
// then hands the three Wonder villages to the player one at a time, a film each, which lifts the curse. The points are
// map coordinates, taken from the places the control script names; not yet seen in a running game.

#include <cstddef>

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
/// The three Wonder villages, Aztec, Greek and Tibetan, where the control script finds their towns
constexpr std::array<glm::vec2, 3> k_WonderVillages {{
    {2637.484f, 3747.7695f},
    {2220.5083f, 2633.8025f},
    {1678.1516f, 2322.4634f},
}};
/// The control script finds each within 100 of its point, so the pick looks as far
constexpr float k_VillagePickRadius = 100.0f;
/// More belief in the player than in anyone else, as winning a village gives
constexpr float k_WinningBelief = 2.0f;
/// The land starts at 13:00 and the curse first strikes after 18:00, about ten minutes of the game's day in; the first
/// village is handed over a minute after that strike, the others two minutes apart, each film's length
constexpr float k_FirstNightSeconds = 660.0f;
constexpr float k_FilmSeconds = 120.0f;
/// Seconds between the logs of the quest's state while it plays, so that a wait it never gets past shows where
constexpr size_t k_LogPeriodSeconds = 30;

constexpr std::string_view k_Globals = "LandNumber";
constexpr std::string_view k_Scripts = "LandControl5 CreatureCurse SetupLand5 BeginLand5";
} // namespace

void testbed_scenarios::AddQuestLand5SurpriseScenarios(std::vector<Scenario>& all)
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("CreatureCurse"),
	    {.kind = Kind::LogGlobals, .value = k_LogPeriodSeconds, .global = k_Globals, .script = k_Scripts},
	};
	// [shortcut] each Wonder village takes more belief in the player than in anyone else, as winning it gives, and its
	// next fold hands it over: the first after the first night's strike, the others once the film before has played
	for (size_t i = 0; i < k_WonderVillages.size(); ++i)
	{
		commands.push_back(
		    {.kind = Kind::TownGainsBelief,
		     .player = PlayerNames::PLAYER_ONE,
		     .delaySeconds = i == 0 ? k_FirstNightSeconds : k_FilmSeconds,
		     .amount = k_WinningBelief,
		     .thing =
		         ThingNear {.type = script::ObjectType::Town, .point = k_WonderVillages.at(i), .radius = k_VillagePickRadius}});
	}
	commands.push_back(LogGlobals(k_Globals, k_Scripts, k_FilmSeconds));
	all.push_back({
	    .id = "quest.i_have_a_surprise_for_you",
	    .name = "Land 5 quest run: I have a surprise for you",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=LandControl5 on Land5.txt). The texts that wait for a "
	                   "click are clicked on. It lets the land's first night come and the curse strike, then hands the "
	                   "Aztec, Greek and Tibetan Wonder villages to the player, the first eleven minutes in and the others "
	                   "two minutes apart, and logs the quest's state every 30 seconds.",
	    .expected = "The opening film and the curse: after 18:00 it strikes, the creature weakened, shrunk and turned, the "
	                "advisors' line, the creature looking at the hand. Each village won plays its film: Nemesis's hand, "
	                "the Wonder blasted, the step logged. With the three won the curse is lifted in the healing scene. "
	                "Unverified: that each film is over within two minutes.",
	    .commands = std::move(commands),
	    .onLand = true,
	});
}
