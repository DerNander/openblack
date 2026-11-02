/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 2's silver scroll The Slavers as quest runs on the game's own Land 2 (docs/wiki/project/scenarios.md, "Playing a
// timeline on a land"): after the arrival (quest=SetupLand2/EnterLand2), the Greek village is handed to the player, as
// winning it would, and the quest's watcher is started on it, as the land's control script starts it. The scroll at
// the villager's hut is pressed on with the hand and the introduction's texts are clicked on. One run trades animals
// at the slavers' pen until every slave is free, with a slave and a circus animal taken from them on the way; the other
// takes two slaves and then kills a slaver, and lets the war on the village play. The camp lies outside the player's
// influence but for the animal pen's ring, and its animals live far from it, so each animal is put into the pen by a
// script's set position, where a player would carry it. The points are map coordinates, taken from the places the
// quest's scripts name and the animals the land's map script makes; none has yet been seen in a running game.

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

/// The Greek village south-west of Khazar's home, where the land's map script makes it
constexpr glm::vec2 k_Village {2248.68f, 2240.36f};
constexpr ThingNear k_VillageThing {.type = script::ObjectType::Town, .point = k_Village, .radius = 30.0f};
/// The belief the village is given, more than anyone holds in it
constexpr float k_WinningBelief = 2.0f;
/// The villager's hut west of the village centre, where the scroll is put up
constexpr glm::vec2 k_Hut {2182.6f, 2209.541f};
/// The slavers' two pens on the hill south of the village: the animal pen, where animals are brought, and the slave pen
constexpr glm::vec2 k_AnimalPen {2276.1667f, 2001.518f};
constexpr glm::vec2 k_SlavePen {2234.8267f, 1974.1627f};
/// The slavers' camp fire
constexpr glm::vec2 k_Camp {2255.874f, 1975.426f};
/// Where a slave taken from the pen is put down: 25 south of the pen's centre, more than the 10 the slavers watch
constexpr glm::vec2 k_SlaveDrop {2234.8267f, 1949.1627f};
/// A spot 40 south of the animal pen, out of the 10 the slavers look in, where an animal is left before it is killed
constexpr glm::vec2 k_Staging {2276.1667f, 1961.518f};

/// The scripts' sub-types of the animals and the slaves used
constexpr uint32_t k_Lion = 0;
constexpr uint32_t k_Tiger = 1;
constexpr uint32_t k_Cow = 8;
constexpr uint32_t k_Horse = 9;
/// A Greek housewife, three of whom are among the slaves and none among the slavers
constexpr uint32_t k_Housewife = 42;
/// The slavers, Egyptian shepherds
constexpr uint32_t k_Slaver = 39;

/// The five lions the quest makes east of the pen and lets go, a herd of horses north of the village, the tigers
/// north-west of it and the cows west of it, where the land's map script makes them
constexpr ThingNear k_LionThing {
    .type = script::ObjectType::Animal, .subtype = k_Lion, .point = {2456.1826f, 2046.8511f}, .radius = 80.0f};
constexpr ThingNear k_HorseThing {
    .type = script::ObjectType::Animal, .subtype = k_Horse, .point = {2262.0f, 2440.0f}, .radius = 30.0f};
constexpr ThingNear k_TigerThing {
    .type = script::ObjectType::Animal, .subtype = k_Tiger, .point = {2124.0f, 2467.0f}, .radius = 30.0f};
constexpr ThingNear k_CowThing {
    .type = script::ObjectType::Animal, .subtype = k_Cow, .point = {2225.0f, 2155.0f}, .radius = 20.0f};
/// A cow left at the staging spot, and a horse of the circus at the pen
constexpr ThingNear k_StagedCow {.type = script::ObjectType::Animal, .subtype = k_Cow, .point = k_Staging, .radius = 5.0f};
constexpr ThingNear k_CircusHorse {
    .type = script::ObjectType::Animal, .subtype = k_Horse, .point = k_AnimalPen, .radius = 15.0f};
/// A slave in the slave pen, and a slaver at the camp
constexpr ThingNear k_SlaveThing {
    .type = script::ObjectType::Villager, .subtype = k_Housewife, .point = k_SlavePen, .radius = 5.0f};
constexpr ThingNear k_SlaverThing {.type = script::ObjectType::Villager, .subtype = k_Slaver, .point = k_Camp, .radius = 30.0f};

/// How long an uncertain beat is given before the timeline goes on: the trade scenes take about 20 seconds
constexpr float k_TradeSeconds = 30.0f;

/// The globals and scripts the quest is read by
constexpr std::string_view k_Globals = "SlaverFinished SlaverTotalLions SlaverTotalTigers SlaverTotalWolves "
                                       "SlaverTotalLeopard SlaverTotalTortoise SlaverTotalZebra SlaverTotalHorse "
                                       "SlaverVictimCount";
constexpr std::string_view k_Scripts = "SlaversCheck TheSlaversMain SlaversIntro SlaverChiefGuard SlaverGuardOne "
                                       "SlaverGuardTwo SlaverGuardThree SlaverGuardFour SlaverCheckAnimal "
                                       "AllSlavesReleased SlaversSlaveWarning SlaversAnimalWarning SlaversFinalWarning "
                                       "SlaverKilled SlaversVeryAngry TownAttack SlaverGoMad SlaversKilledEnough "
                                       "GiveSpellDispenserReward";
/// How long the camp is watched after the introduction, for the slavers' rounds about it: the chief's takes about a
/// minute
constexpr float k_RoundsSeconds = 75.0f;

/// The camera south of the animal pen and above it, with the pen on the screen, as the slavers' trade asks
Command CameraOnPen()
{
	return CameraOn(k_AnimalPen, 30.0f, 40.0f, 0.0f);
}

/// A thing of the land moved to a point by the scripts' own SET_POSITION, as a script moves it
Command MoveThingTo(const ThingNear& thing, glm::vec2 to, float delay = 1.0f)
{
	return {.kind = Kind::MoveThing, .delaySeconds = delay, .point = to, .thing = thing};
}

/// A script's SET_PROPERTY of HEALTH to 0 on the thing: it dies
Command Kill(const ThingNear& thing, float delay = 1.0f)
{
	return {.kind = Kind::ScriptSetHealth, .delaySeconds = delay, .amount = 0.0f, .thing = thing};
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

/// The start: the village handed to the player and the watcher started on it, the scroll pressed on and the
/// introduction let play to its end
std::vector<Command> Introduction()
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("EnterLand2", TaskWait::Finished),
	    // [shortcut] the Greek village takes more belief in the player than in anyone else, as winning it over would
	    // give it; its next fold hands it to the player (a direct change of hands is undone by that fold)
	    WinVillage(),
	    // the quest's watcher, started on the village as the land's control script starts it
	    {.kind = Kind::StartScript,
	     .script = "SlaversCheck",
	     .scriptArguments = {ScriptArgument {.kind = ScriptArgument::Kind::Thing,
	                                         .thingType = static_cast<uint8_t>(script::ObjectType::Town),
	                                         .thingPoint = k_Village,
	                                         .thingRadius = 30.0f}}},
	    WaitTask("ChallengeHighlightNotify"),
	    {.kind = Kind::LogGlobals, .delaySeconds = 1.0f, .value = 60, .global = k_Globals, .script = k_Scripts},
	    // [camera] the scroll over the villager's hut
	    CameraOn(k_Hut, k_EyeAbove, k_EyeBack, 4.0f),
	};
	// [real] the scroll pressed on
	PressScroll(commands, k_Hut);
	// the introduction: the villager's line, the camp, the chief's three lines and the camera given back
	commands.push_back(WaitText(1755));
	commands.push_back(WaitTask("SlaversIntro", TaskWait::Finished));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 1.0f));
	return commands;
}

/// An animal put into the animal pen, once no trade or warning is playing, with the camera on the pen; then the trade
/// waited for to its end
void Deliver(std::vector<Command>& commands, const ThingNear& animal)
{
	commands.push_back(WaitTask("SlaverCheckAnimal", TaskWait::NotRunning));
	commands.push_back(CameraOnPen());
	// [shortcut] the animal put into the pen, where a player would carry it
	commands.push_back(MoveThingTo(animal, k_AnimalPen));
	commands.push_back(WaitTask("SlaverCheckAnimal", TaskWait::Finished));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 0.5f));
}

/// A slave taken out of the slave pen and put down outside it; then the warning it draws waited for to its end
void TakeSlave(std::vector<Command>& commands, std::string_view warning)
{
	commands.push_back(WaitTask("SlaverCheckAnimal", TaskWait::NotRunning));
	commands.push_back(CameraOnPen());
	// [shortcut] a slave put down 25 south of the slave pen, where a player would carry one; the pen is out of the
	// player's influence
	commands.push_back(MoveThingTo(k_SlaveThing, k_SlaveDrop));
	commands.push_back(WaitTask(warning, TaskWait::Finished));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 0.5f));
}

std::vector<Command> Trades()
{
	auto commands = Introduction();
	// [camera] the camp, watched while the slavers go about their rounds
	commands.push_back(CameraOn(k_Camp, 30.0f, 40.0f, 0.0f));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, k_RoundsSeconds));
	// a lion: worth two slaves
	Deliver(commands, k_LionThing);
	// a slave taken: the chief's first warning
	TakeSlave(commands, "SlaversSlaveWarning");
	// two horses, worth a slave each, and a third, of a kind the slavers have enough of
	Deliver(commands, k_HorseThing);
	Deliver(commands, k_HorseThing);
	Deliver(commands, k_HorseThing);
	// a cow, which the slavers do not want
	Deliver(commands, k_CowThing);
	// a dead cow: a cow left beside the pen, killed, then put into the pen; the trade, if the dead cow is taken for
	// one, is given a fixed time
	commands.push_back(WaitTask("SlaverCheckAnimal", TaskWait::NotRunning));
	// [shortcut] a cow put beside the pen, and then killed by a script's health of 0, where a player would kill one
	commands.push_back(MoveThingTo(k_CowThing, k_Staging));
	commands.push_back(Kill(k_StagedCow));
	commands.push_back(CameraOnPen());
	// [shortcut] the dead cow put into the pen, where a player would carry it
	commands.push_back(MoveThingTo(k_StagedCow, k_AnimalPen));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, k_TradeSeconds));
	// a horse of the circus killed: the circus is smaller, which the slavers take for a theft
	commands.push_back(WaitTask("SlaverCheckAnimal", TaskWait::NotRunning));
	commands.push_back(CameraOnPen());
	// [shortcut] a circus horse killed by a script's health of 0, where the player or the creature would take or kill
	// one
	commands.push_back(Kill(k_CircusHorse));
	commands.push_back(WaitTask("SlaversAnimalWarning", TaskWait::Finished));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, k_TradeSeconds));
	// a second lion and a tiger, worth two slaves each, which free the last of them
	Deliver(commands, k_LionThing);
	commands.push_back(WaitTask("SlaverCheckAnimal", TaskWait::NotRunning));
	commands.push_back(CameraOnPen());
	// [shortcut] the tiger put into the pen, where a player would carry it
	commands.push_back(MoveThingTo(k_TigerThing, k_AnimalPen));
	// the slaves freed: the circus finale, the log at 1, and the dispenser's reward with the evil advisor's lines, which
	// the finale's script waits for
	commands.push_back(WaitTask("AllSlavesReleased", TaskWait::Finished));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 2.0f));
	return commands;
}

std::vector<Command> War()
{
	auto commands = Introduction();
	// a horse, so the circus has an animal when the war comes; worth one slave, so the slaves left never number four,
	// as the slavers do once one is dead
	Deliver(commands, k_HorseThing);
	// two slaves taken: the chief's first and final warnings
	TakeSlave(commands, "SlaversSlaveWarning");
	TakeSlave(commands, "SlaversFinalWarning");
	// [shortcut] a slaver killed by a script's health of 0, where the player or the creature would kill one
	commands.push_back(Kill(k_SlaverThing, 2.0f));
	// the evil advisor's remark and the chief's declaration of war; then the march on the village
	commands.push_back(WaitText(1772));
	commands.push_back(WaitTask("SlaversVeryAngry", TaskWait::Finished));
	// [camera] the village the slavers march on
	commands.push_back(CameraOn(k_Village, 60.0f, 70.0f, 0.0f));
	// the good advisor's lines on their arrival, then the killing until ten of the village are dead
	commands.push_back(WaitText(1773));
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 1.0f));
	commands.push_back({.kind = Kind::WaitGlobal,
	                    .waitUntilFree = true,
	                    .amount = 10.0f,
	                    .global = "SlaverVictimCount",
	                    .compare = Compare::AtLeast});
	// the slavers stopped where they stand and the log entry updated, a moment after the tenth death
	commands.push_back(LogGlobals(k_Globals, k_Scripts, 5.0f));
	return commands;
}
} // namespace

void testbed_scenarios::land2_quests::AddSlaversScenarios(std::vector<Scenario>& all)
{
	all.push_back({
	    .id = "quest.the_slavers",
	    .name = "Land 2 quest run: The Slavers, the slaves bought free",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand2/EnterLand2 on Land2.txt). The texts that wait "
	                   "for a click are clicked on, the arrival's too. Once the arrival is over it hands the Greek "
	                   "village to the player and starts the quest's watcher on it, as the land's control script does; "
	                   "presses on the scroll at the villager's hut with the hand and lets the introduction play. Then, "
	                   "with the camera on the animal pen, it puts into the pen by a script's set position a lion, three "
	                   "horses, a cow, a dead cow, a second lion and a tiger, one at a time once each trade is over; "
	                   "after the first lion it puts a slave down outside the slave pen, and after the dead cow it kills "
	                   "a horse of the circus.",
	    .expected = "The scroll's press plays the introduction (texts 1775, 1753, 1754 and 1755) and the log at 0. The "
	                "lion pays two slaves (1759), each horse one (1758) until the third, of which there are plenty "
	                "(1779); the cow is not wanted (1757) and the dead cow is turned away (1756 or 1771); each turned "
	                "away is beamed off. The slave taken joins the village and draws the chief's warning (1762); the "
	                "dead circus horse draws the animal warning (1761) and a recount. The second lion pays two and the "
	                "tiger the last: the finale (1765, 1760, 1777), the log at 1 and the wolf pack dispenser's reward "
	                "(5253 to 5255); the end check logs SlaverFinished=1. Unverified: that a dead cow is found in the "
	                "pen, and that the horses, the cows and the tiger are where the land's map script makes them.",
	    .commands = Trades(),
	    .onLand = true,
	});
	all.push_back({
	    .id = "quest.the_slavers_war",
	    .name = "Land 2 quest run: The Slavers, a slaver killed and the war",
	    .facet = Facet::Story,
	    .description = "Plays the start as quest.the_slavers does and trades a horse; then puts two slaves down outside "
	                   "the slave pen one after the other, each once the warning before is over, kills a slaver by a "
	                   "script's health of 0 and watches the village from above.",
	    .expected = "The first slave taken draws the chief's warning (1762), the second his final warning (1778); the "
	                "dead slaver draws the evil advisor's remark (1763) and the chief's declaration of war (1772). The "
	                "horse is driven to the village and set loose, the scroll moves into the village, and the slavers "
	                "march there: a house is set alight and the good advisor says 1773 and 1774. Each slaver then "
	                "kills the village's people; at ten dead they stand frozen and the log is at 1 with alignment -1. "
	                "Unverified: how long the march takes, and that the village has ten people within the slavers' "
	                "reach.",
	    .commands = War(),
	    .onLand = true,
	});
}
