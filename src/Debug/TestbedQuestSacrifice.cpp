/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 2's silver scroll The Sacrifice as a quest run on the game's own Land 2 (docs/wiki/project/scenarios.md,
// "Playing a timeline on a land"), played after Worship Site's timeline in the same run, which builds the player's
// worship site the quest's watcher looks for. The Indian village north-west of the temple is handed to the player, as
// winning it would, and the watcher is started on it, as the land's control script starts it. The scroll at the house
// is pressed on with the hand and the introduction's texts are clicked on; then a tree is pulled up with the hand and
// given to the altar, and the boy is taken up with the hand and given to the altar. The points are map coordinates,
// taken from the places the quest's script names and the trees the land's map script makes; none has yet been seen in
// a running game.

#include <cstdint>

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

/// The Indian village north-west of the temple whose family brings the offering, where the land's map script makes it
constexpr glm::vec2 k_Village {2196.11f, 3096.86f};
constexpr ThingNear k_VillageThing {.type = script::ObjectType::Town, .point = k_Village, .radius = 30.0f};
/// The belief the village is given, more than anyone holds in it
constexpr float k_WinningBelief = 2.0f;
/// The house north of that village where the scroll is put up and the family is made
constexpr glm::vec2 k_House {2195.354f, 3197.774f};
/// The altar of the player's worship site, the spot the watcher looks for the site at
constexpr glm::vec2 k_Altar {3343.191f, 3220.2014f};
/// The worship site the Indian village gets at the temple once it is the player's (its tribe's sub-type), the one
/// nearest the altar
constexpr ThingNear k_VillageSiteThing {
    .type = script::ObjectType::WorshipSite, .subtype = 4, .point = k_Altar, .radius = 50.0f};
/// A tree west of the temple, where the land's map script plants it
constexpr glm::vec2 k_Tree {3281.15f, 3269.76f};
constexpr ThingNear k_TreeThing {.type = script::ObjectType::Tree, .point = k_Tree, .radius = 5.0f};
/// The boy, an Indian farmer's child the quest makes, put beside the altar by its introduction
constexpr uint32_t k_Boy = 31;
constexpr ThingNear k_BoyThing {.type = script::ObjectType::Villager, .subtype = k_Boy, .point = k_Altar, .radius = 10.0f};
/// How long the hand holds the Action button down on a thing to take it
constexpr float k_GrabHoldSeconds = 0.6f;
/// How long the priest's judgement of an offering is given: the 2 seconds the script waits, and his line
constexpr float k_JudgeSeconds = 15.0f;

/// The globals and scripts the quest is read by
constexpr std::string_view k_Globals = "SacrificeFinished SacrificeCompleted";
constexpr std::string_view k_Scripts = "SacrificeCheck Sacrifice SacrificeIntro ChallengeHighlightNotify";

/// A thing taken into the hand: the pointer on it, the Action button pressed once the hand is over it and let go a
/// moment later, which leaves the thing in the hand
void Take(std::vector<Command>& commands, const ThingNear& thing)
{
	commands.push_back({.kind = Kind::PointerTo, .delaySeconds = 1.0f, .thing = thing});
	commands.push_back(
	    {.kind = Kind::PointerPress, .delaySeconds = 0.5f, .waitUntilFree = true, .value = k_ActionButton, .thing = thing});
	commands.push_back({.kind = Kind::PointerRelease, .delaySeconds = k_GrabHoldSeconds, .value = k_ActionButton});
}

/// What the hand holds given to the altar: the hand held over the altar and the Action button clicked there
void GiveToAltar(std::vector<Command>& commands)
{
	commands.push_back(CameraOn(k_Altar, k_EyeAbove, k_EyeBack, 0.0f));
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 1.0f, .point = k_Altar});
	commands.push_back({.kind = Kind::PointerPress, .delaySeconds = 1.0f, .value = k_ActionButton});
	commands.push_back({.kind = Kind::PointerRelease, .delaySeconds = 0.2f, .value = k_ActionButton});
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 0.5f, .value = 1});
}

/// The village takes more belief in the player than in anyone else, as a gift gives it; its next fold hands it over
Command WinVillage()
{
	return {.kind = Kind::TownGainsBelief,
	        .player = PlayerNames::PLAYER_ONE,
	        .delaySeconds = 1.0f,
	        .amount = k_WinningBelief,
	        .thing = k_VillageThing};
}

std::vector<Command> Offerings()
{
	std::vector<Command> commands {
	    ClickTexts(),
	    // [shortcut] the Indian village takes more belief in the player than in anyone else, as winning it over would
	    // give it; its next fold hands it to the player (a direct change of hands is undone by that fold)
	    WinVillage(),
	    // [shortcut] the worship site the won village gets at the temple, built at once by a script's built percentage
	    // where its villagers would build it: the watcher waits for the site nearest the altar to be fully built
	    {.kind = Kind::AbodeBuilt, .delaySeconds = 5.0f, .built = 1.0f, .thing = k_VillageSiteThing},
	    // the quest's watcher, started on the village as the land's control script starts it; it finds that site
	    {.kind = Kind::StartScript,
	     .script = "SacrificeCheck",
	     .scriptArguments = {ScriptArgument {.kind = ScriptArgument::Kind::Thing,
	                                         .thingType = static_cast<uint8_t>(script::ObjectType::Town),
	                                         .thingPoint = k_Village,
	                                         .thingRadius = 30.0f}}},
	    WaitTask("ChallengeHighlightNotify"),
	    LogGlobals(k_Globals, k_Scripts, 1.0f),
	    // [camera] the scroll at the house
	    CameraOn(k_House, k_EyeAbove, k_EyeBack, 0.0f),
	};
	// [real] the scroll pressed on
	PressScroll(commands, k_House);
	// the introduction: the priest's lines at the house, the fade to the altar, his lines there and the advisors'
	commands.push_back(WaitText(1514));
	commands.push_back(WaitTask("SacrificeIntro", TaskWait::Finished));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 1.0f));
	// [camera] the tree west of the temple; [real] the tree pulled up with the hand and given to the altar
	commands.push_back(CameraOn(k_Tree, k_EyeAbove, k_EyeBack, 0.0f));
	Take(commands, k_TreeThing);
	GiveToAltar(commands);
	// the priest judges the offering, after the evil advisor's grumble at a small one
	commands.push_back(LogGlobals(k_Globals, k_Scripts, k_JudgeSeconds));
	// [shortcut] whatever the hand still holds is let go of where it is, should the altar not have taken the tree
	commands.push_back({.kind = Kind::HandDrop, .delaySeconds = 0.5f});
	// [camera] the altar; [real] the boy taken up with the hand and given to the altar
	commands.push_back(CameraOn(k_Altar, k_EyeAbove, k_EyeBack, 0.0f));
	Take(commands, k_BoyThing);
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 1.0f));
	GiveToAltar(commands);
	// the boy's ending: the priest's line, the log closed at 1 with alignment -1
	commands.push_back(WaitGlobal("SacrificeFinished", 1.0f));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 8.0f));
	return commands;
}
} // namespace

void testbed_scenarios::land2_quests::AddSacrificeScenarios(std::vector<Scenario>& all)
{
	all.push_back({
	    .id = "quest.the_sacrifice",
	    .name = "Land 2 quest run: The Sacrifice, the boy given to the altar",
	    .facet = Facet::Story,
	    .description = "Played after quest.worship_site in the same run (quest=SetupLand2/EnterLand2 on Land2.txt, "
	                   "timeline=quest.worship_site/quest.the_sacrifice). The texts that wait for a click are clicked "
	                   "on. It hands the Indian village north-west of the temple to the player and starts the quest's "
	                   "watcher on it, as the land's control script does; presses on the scroll at the house with the "
	                   "hand and lets the introduction play; pulls up a tree west of the temple with the hand and "
	                   "clicks with it on the altar; then takes up the boy beside the altar and clicks with him on the "
	                   "altar.",
	    .expected = "The watcher finds the built worship site and the player's village and starts the quest; the "
	                "scroll's press makes the family and plays the introduction (texts 1529, 1530, 1510, 1511, 1512, "
	                "1513 and 1514) with the log at 0, and the scroll moves beside the altar. The tree's sacrifice raises "
	                "the site's total: two seconds later the evil advisor grumbles (6488) and the priest judges it "
	                "(1516, 1520 or 1515), and the log's share rises. The boy taken in the hand and sacrificed ends the "
	                "quest with the priestess's line (1528, or 1527 should he leave a body), the log at 1 and alignment "
	                "-1; the end check logs SacrificeFinished=1 and SacrificeCompleted=1. Unverified: that the worship "
	                "site stands within 15 of the altar's spot, that the hand pulls the tree up and the altar takes it, "
	                "and that the altar takes the boy from the hand.",
	    .commands = Offerings(),
	    .onLand = true,
	});
}
