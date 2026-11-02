/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

// Land 5's silver scrolls as quest runs, each played after a land entry's quest= has started it: Swap to Brown Bear,
// The Explorers Again, The Japanese Traitor, The Heavenly Fire and The Magic Dragon. The timelines do what the player
// would: they click the texts that wait for a click, press the scrolls with the hand, pick up the dung, set the hour
// for the stranger's evenings, keep the camera over the village the meteors are aimed at, heal the crusaders, light
// their pyre and look at the cave's blowholes. The points are map coordinates, taken from the places the challenge
// scripts name.

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
/// Seconds between the logs of a quest's state while it plays, so that a wait it never gets past shows where
constexpr size_t k_LogPeriodSeconds = 30;

/// Logs the globals and scripts named every period while the timeline goes on
[[nodiscard]] Command LogEvery(std::string_view globals, std::string_view scripts)
{
	return {.kind = Kind::LogGlobals, .value = k_LogPeriodSeconds, .global = globals, .script = scripts};
}

/// [shortcut] The hour of the day set, as the clock would reach it by waiting
[[nodiscard]] Command SetHourTo(float hour, float delay)
{
	return {.kind = Kind::SetHour, .delaySeconds = delay, .hour = hour};
}

// --- Swap to Brown Bear ---

/// The farmer's house, under the scroll, and how high the scroll stands over the land there
constexpr glm::vec2 k_FarmerHouse {2359.32f, 3390.025f};
constexpr float k_HouseRaised = 4.0f;
/// The eight piles of dung, each made once the one before is picked up
constexpr std::array<glm::vec2, 8> k_DungPiles {{
    {2254.907f, 3386.621f},
    {2226.376f, 3404.887f},
    {2240.606f, 3434.66f},
    {2272.533f, 3463.427f},
    {2243.217f, 3454.875f},
    {2213.621f, 3489.385f},
    {2198.929f, 3518.456f},
    {2217.4f, 3533.416f},
}};
/// A pile rolls a little once made on a slope; only one pile lies at a time, so a wide pick finds the one there is
constexpr float k_DungPickRadius = 40.0f;
/// The advisors' last line after the introduction, once the trail can be followed
constexpr uint32_t k_TrailStartsLine = 1880;
/// The evil advisor's line after the seventh pile, said before the eighth is made
constexpr uint32_t k_OneMorePileLine = 1886;
constexpr size_t k_LastPileRetries = 3;
/// Seconds the timeline keeps logging once the quest's script has ended, for the swap offer's first reminder
constexpr float k_SwapOfferSeconds = 70.0f;
constexpr std::string_view k_BearScripts = "SwapToBrownBear ChallengeHighlightNotify HelpSpritesTalk HelpSpritesTalk2 "
                                           "SwapCreatures";

void AddSwapToBrownBear(std::vector<Scenario>& all)
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("SwapToBrownBear"),
	    LogEvery("", k_BearScripts),
	    WaitTask("ChallengeHighlightNotify"),
	    CameraOn(k_FarmerHouse, k_EyeAbove + k_HouseRaised, k_EyeBack, k_HouseRaised),
	};
	PressScroll(commands, k_FarmerHouse);
	commands.push_back(WaitText(k_TrailStartsLine));
	// [shortcut] each pile taken into the hand where it lies, as the player's hand would pick it up
	for (size_t pile = 0; pile < k_DungPiles.size(); ++pile)
	{
		if (pile + 1 == k_DungPiles.size())
		{
			commands.push_back(WaitText(k_OneMorePileLine));
		}
		commands.push_back(
		    {.kind = Kind::HandTake,
		     .delaySeconds = 3.0f,
		     .thing = ThingNear {.type = script::ObjectType::Poo, .point = k_DungPiles.at(pile), .radius = k_DungPickRadius}});
	}
	// the last pile is made only once the evil advisor's line has gone, which may be after the take above: taken again
	// a little later, which finds nothing if it was already taken
	for (size_t retry = 0; retry < k_LastPileRetries; ++retry)
	{
		commands.push_back(
		    {.kind = Kind::HandTake,
		     .delaySeconds = 5.0f,
		     .thing = ThingNear {.type = script::ObjectType::Poo, .point = k_DungPiles.back(), .radius = k_DungPickRadius}});
	}
	commands.push_back(WaitTask("SwapToBrownBear", TaskWait::NotRunning));
	commands.push_back(LogGlobals("", k_BearScripts, 1.0f));
	commands.push_back(LogGlobals("", k_BearScripts, k_SwapOfferSeconds));
	all.push_back({
	    .id = "quest.swap_to_brown_bear",
	    .name = "Land 5 quest run: Swap to Brown Bear",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand5/BeginLand5/SwapToBrownBear on Land5.txt). The "
	                   "texts that wait for a click are clicked on. It presses on the scroll over the farmer's house, "
	                   "waits for the introduction and the advisors' lines, then takes each pile of dung into the hand "
	                   "where it lies, eight in turn, and waits for the quest to end.",
	    .expected = "The press brings the farmer out, his complaint, the log entry at 0 and the advisors' lines. Each "
	                "pile in the hand goes and the next appears, with the advisors' lines after the first and the "
	                "seventh; after the eighth the brown bear appears with its scene, a heal chest falls from the sky, the "
	                "entry is done and the creature swap is offered at the bear.",
	    .commands = std::move(commands),
	    .onLand = true,
	});
}

// --- The Explorers Again ---

/// Seconds before the scroll is looked at, while the quest builds the sailors' town and peoples it
constexpr float k_TownBuiltSeconds = 10.0f;
/// The scroll by the wreck stands 2 m over the land inside the wreck's hull, between its floor and its deck, so the
/// hull is nearer than the scroll's click sphere along almost every ray. The sphere is sized by the camera's distance
/// across the land, so it is full size only from 30 m or more away. From the north, low, a ray passing just over the
/// hull's east side, about 2.2 m east of the sphere's centre and within its 2.8 m, meets the sphere's depth before any
/// of the hull: the camera looks along that ray, which the pointer at the middle of the screen follows (worked out
/// from the wreck's triangles and the sphere as the hand's pick tests them, with some 0.17 m to spare). Map
/// coordinates, the heights over the land
constexpr glm::vec3 k_WreckEye {1587.06f, 20.12f, 3963.91f};
constexpr glm::vec3 k_WreckLook {1585.55f, 4.61f, 3919.80f};
constexpr std::string_view k_ExplorersScripts = "MissionariesReturned SwapCreatures";

void AddExplorersAgain(std::vector<Scenario>& all)
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("MissionariesReturned"),
	    LogEvery("", k_ExplorersScripts),
	    // [camera] the player's view of the scroll past the hull's east side
	    {.kind = Kind::CameraTo, .delaySeconds = k_TownBuiltSeconds, .eye = k_WreckEye, .look = k_WreckLook},
	    // [real] the pointer where the scroll shows past the hull, and the Action button pressed there and let go
	    {.kind = Kind::PointerTo, .delaySeconds = 1.0f, .point = {0.5f, 0.5f}},
	    {.kind = Kind::PointerPress, .delaySeconds = 0.5f, .value = k_ActionButton},
	    {.kind = Kind::PointerRelease, .delaySeconds = 0.2f, .value = k_ActionButton},
	};
	commands.push_back(WaitTask("MissionariesReturned", TaskWait::NotRunning));
	commands.push_back(LogGlobals("", k_ExplorersScripts, 1.0f));
	commands.push_back(LogGlobals("", k_ExplorersScripts, k_SwapOfferSeconds));
	all.push_back({
	    .id = "quest.the_explorers_again",
	    .name = "Land 5 quest run: The Explorers Again",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand5/BeginLand5/MissionariesReturned on "
	                   "Land5.txt). The texts that wait for a click are clicked on. Once the quest has built the "
	                   "sailors' town it presses on the scroll by the wreck and waits for the quest to end.",
	    .expected = "The wreck lies on the shore and a Norse town of eleven buildings and twenty villagers stands above "
	                "it. The press plays the film: three sailors dance and talk, a polar bear is brought and the "
	                "advisors speak; the log entry is done and the creature swap is offered at the bear.",
	    .commands = std::move(commands),
	    .onLand = true,
	});
}

// --- The Japanese Traitor ---

/// The three Wonders the land's control script builds as the land begins, which the stranger's secrets are about
constexpr std::array<glm::vec2, 3> k_Wonders {{
    {2663.767f, 3671.004f},
    {2331.919f, 2581.005f},
    {1758.438f, 2337.047f},
}};
constexpr float k_WonderDesire = 0.6f;
/// Where the stranger prays, by his fire, and the scroll made over him
constexpr glm::vec2 k_PrayingPlace {2197.82f, 3502.809f};
/// The advisor's daytime and night remarks on the fire, the last line of each
constexpr uint32_t k_DaytimeRemarkLine = 5573;
constexpr uint32_t k_NightRemarkLine = 5571;
/// Seconds the advisor waits after the daytime remark before it remarks again
constexpr float k_RemarkWaitSeconds = 65.0f;
/// The hours of the day the timeline sets: noon for the daytime remark, the evening for the night remark, morning for
/// the stranger to go once he has talked, and the next evening for him to come back
constexpr float k_Noon = 12.0f;
constexpr float k_Night = 19.0f;
constexpr float k_Morning = 10.0f;
constexpr float k_Evening = 17.0f;
/// The line of each night's talk that only that talk says, about one of the Wonders
constexpr std::array<uint32_t, 3> k_SecretLines {5583, 5585, 5587};
/// Seconds for the stranger to walk to his fire before the scroll is pressed, for the talk's end after its secret, and
/// for him to walk away before the next evening
constexpr float k_ArrivalSeconds = 30.0f;
constexpr float k_TalkEndSeconds = 5.0f;
constexpr float k_LeavingSeconds = 60.0f;
constexpr std::string_view k_TraitorGlobals = "MetTraitor";
constexpr std::string_view k_TraitorScripts = "JapaneseTraitor AdvisorSpotsFire";

void AddJapaneseTraitor(std::vector<Scenario>& all)
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("BeginLand5"),
	    WaitTask("BeginLand5", TaskWait::Finished),
	};
	// The land's control script builds the three Wonders as the land begins, before it starts the stranger's script
	for (const auto& wonder : k_Wonders)
	{
		commands.push_back(
		    {.kind = Kind::ScriptBuildBuilding, .delaySeconds = 1.0f, .point = wonder, .amount = k_WonderDesire});
	}
	commands.push_back(SetHourTo(k_Noon, 1.0f));
	commands.push_back(CameraOn(k_PrayingPlace, k_EyeAbove, k_EyeBack, 0.0f));
	commands.push_back(StartScript("JapaneseTraitor"));
	commands.push_back(WaitTask("JapaneseTraitor"));
	commands.push_back(LogEvery(k_TraitorGlobals, k_TraitorScripts));
	// the fire seen by day, then by night once the advisor's wait is over; the evening brings the stranger
	commands.push_back(WaitText(k_DaytimeRemarkLine));
	commands.push_back(SetHourTo(k_Night, k_RemarkWaitSeconds));
	commands.push_back(WaitText(k_NightRemarkLine));
	for (size_t night = 0; night < k_SecretLines.size(); ++night)
	{
		if (night > 0)
		{
			commands.push_back(SetHourTo(k_Evening, k_LeavingSeconds));
		}
		commands.push_back(CameraOn(k_PrayingPlace, k_EyeAbove, k_EyeBack, 0.0f));
		commands.back().delaySeconds = k_ArrivalSeconds;
		PressScroll(commands, k_PrayingPlace);
		commands.push_back(WaitText(k_SecretLines.at(night)));
		commands.push_back(SetHourTo(k_Morning, k_TalkEndSeconds));
	}
	commands.push_back(WaitTask("JapaneseTraitor", TaskWait::NotRunning));
	commands.push_back(LogGlobals(k_TraitorGlobals, k_TraitorScripts, 1.0f));
	all.push_back({
	    .id = "quest.the_japanese_traitor",
	    .name = "Land 5 quest run: The Japanese Traitor",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand5/BeginLand5 on Land5.txt). Once the land's "
	                   "arrival has ended it builds the three Wonders as the land's control script does and starts the "
	                   "stranger's script. The texts that wait for a click are clicked on. At noon the camera is put by "
	                   "the fire, and the hour is set to the evening after the advisor's wait; each night the scroll "
	                   "over the stranger is pressed, and after his secret the hour is set to the morning, then to the "
	                   "next evening, three nights in all.",
	    .expected = "The advisor remarks on the fire by day and by night. Each evening the stranger walks to the "
	                "lantern and prays under a scroll; the press plays his talk, the first with his introduction, each "
	                "about one Wonder in turn. After the third he goes for good and the script ends.",
	    .commands = std::move(commands),
	    .onLand = true,
	});
}

// --- The Heavenly Fire ---

/// The Greek village's centre, which the camera must look at from within 150 with the opening shot's point out of
/// view: looked at from its north, the opening shot's point lies behind the camera
constexpr glm::vec2 k_GreekTownCentre {2307.239f, 2589.891f};
constexpr float k_TownEyeHeight = 25.0f;
constexpr float k_TownEyeBack = 30.0f;
/// The good advisor's last line of the warning shot's scene, after which the camera is put back over the village
constexpr uint32_t k_WarningShotLastLine = 1844;
constexpr std::string_view k_FireScripts = "FireOnHighMain FireOnHighMeteor MakeSmokeComeOutOfMeteor rewardfromsky";

[[nodiscard]] Command CameraOverGreekTown(float delay)
{
	return {.kind = Kind::CameraTo,
	        .delaySeconds = delay,
	        .eye = {k_GreekTownCentre.x, k_TownEyeHeight, k_GreekTownCentre.y + k_TownEyeBack},
	        .look = {k_GreekTownCentre.x, 0.0f, k_GreekTownCentre.y}};
}

void AddHeavenlyFire(std::vector<Scenario>& all)
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("FireOnHighMain"),
	    LogEvery("GoodCatch", k_FireScripts),
	    CameraOverGreekTown(1.0f),
	    WaitText(k_WarningShotLastLine),
	    CameraOverGreekTown(5.0f),
	    WaitTask("FireOnHighMain", TaskWait::NotRunning),
	    LogGlobals("GoodCatch", k_FireScripts, 1.0f),
	};
	all.push_back({
	    .id = "quest.the_heavenly_fire",
	    .name = "Land 5 quest run: The Heavenly Fire",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand5/BeginLand5/FireOnHighMain on Land5.txt). The "
	                   "texts that wait for a click are clicked on. The camera is put over the Greek village's centre, "
	                   "looking at it from the north, and put there again after the warning shot's scene; no meteor is "
	                   "caught and nothing is done to protect the village.",
	    .expected = "Five minutes in, the opening shot lands by the village with the evil advisor's call, the record "
	                "opens and the warning shot follows; then five volleys of five meteors, each announced, hit the "
	                "village. The advisors judge the dead and the damage, a well-protected village earns a chest from "
	                "the sky, and the record closes with the judged alignment.",
	    .commands = std::move(commands),
	    .onLand = true,
	});
}

// --- The Magic Dragon ---

/// The Tibetan village by the crusaders' cave, whose influence reaches towards their camp once it is the player's
constexpr glm::vec2 k_TibetanVillage {1718.52f, 2337.6f};
constexpr float k_TibetanPickRadius = 100.0f;
/// More belief in the player than in anyone else, as winning a village gives
constexpr float k_WinningBelief = 2.0f;
/// The crusaders' camp, and the scroll put up 5 to its side and 20 above it
constexpr glm::vec2 k_Camp {1896.454f, 2346.574f};
constexpr glm::vec2 k_CampScroll {1901.454f, 2351.574f};
constexpr float k_ScrollEyeHeight = 35.0f;
constexpr float k_ScrollEyeBack = 35.0f;
constexpr float k_ScrollLookHeight = 15.0f;
/// The pyre by the cave's mouth
constexpr glm::vec2 k_Pyre {1883.299f, 2350.002f};
/// The camera looking over the land between the village and the camp, high enough to see both
constexpr glm::vec2 k_VillageToCamp {1830.0f, 2345.0f};
constexpr float k_WideEyeHeight = 80.0f;
constexpr float k_WideEyeBack = 100.0f;
/// The last lines of the introduction and of the pyre's scene, after which the crusaders sit down again
constexpr uint32_t k_HealUsLine = 5611;
constexpr uint32_t k_PyreSceneLastLine = 5616;
constexpr float k_SitDownSeconds = 8.0f;
/// The cave's five blowholes, in the order the fight is followed, and the camera put close above each, looking at it
constexpr std::array<glm::vec2, 5> k_Blowholes {{
    {1826.135f, 2305.804f},
    {1744.944f, 2225.273f},
    {1663.577f, 2166.369f},
    {1581.013f, 2122.308f},
    {1466.07f, 2083.855f},
}};
constexpr float k_BlowholeEyeHeight = 10.0f;
constexpr float k_BlowholeEyeBack = 10.0f;
/// Seconds after the charge's scene before the first blowhole is looked at, and the camera's stay at each, longer than
/// each blowhole's lines take
constexpr float k_AfterChargeSeconds = 3.0f;
constexpr float k_BlowholeStaySeconds = 25.0f;
/// The crusaders' leader's last line by the cave's exit
constexpr uint32_t k_TreasureLastLine = 5619;
constexpr std::array<std::string_view, 5> k_CrusaderScripts {
    "Crusader1Actions", "Crusader2Actions", "Crusader3Actions", "Crusader4Actions", "Crusader5Actions",
};
constexpr std::string_view k_DragonGlobals = "StopActing ChargeIn AllHealedUp";
constexpr std::string_view k_DragonScripts = "Crusaders Crusader1Actions Crusader2Actions Crusader3Actions "
                                             "Crusader4Actions Crusader5Actions ChallengeHighlightNotify "
                                             "GiveSpellDispenserReward";

/// [shortcut] A miracle given to the hand, as if from a bubble, and the hand put over a point by the testbed, or at the
/// edge of the player's influence nearest it; then the Action button is clicked there
void CastOver(std::vector<Command>& commands, SpellSeedType seed, glm::vec2 point)
{
	commands.push_back({.kind = Kind::HoldSeed, .delaySeconds = 1.0f, .value = static_cast<size_t>(seed)});
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 1.0f, .point = point, .value = 2});
	commands.push_back({.kind = Kind::PointerPress, .delaySeconds = 1.0f, .value = k_ActionButton});
	commands.push_back({.kind = Kind::PointerRelease, .delaySeconds = 0.3f, .value = k_ActionButton});
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 0.5f, .value = 1});
}

/// [shortcut] A fireball given to the hand, as if from a bubble, and thrown by the testbed at a point from the edge of
/// the player's influence nearest it; from a point within the influence it is let go over the point and falls on it
void ThrowFireballAt(std::vector<Command>& commands, glm::vec2 point)
{
	commands.push_back({.kind = Kind::HoldSeed, .delaySeconds = 1.0f, .value = static_cast<size_t>(SpellSeedType::Fire)});
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 1.0f, .point = point, .value = 2});
	commands.push_back({.kind = Kind::HandThrowAt, .delaySeconds = 1.0f, .point = point});
	commands.push_back({.kind = Kind::HandOver, .delaySeconds = 3.0f, .value = 1});
}

void AddMagicDragon(std::vector<Scenario>& all)
{
	std::vector<Command> commands {
	    ClickTexts(),
	    WaitTask("Crusaders"),
	    LogEvery(k_DragonGlobals, k_DragonScripts),
	    // [shortcut] the Tibetan village takes more belief in the player than in anyone else, as winning it gives, and
	    // its next fold hands it over, for the player's influence the miracles are cast from
	    {.kind = Kind::TownGainsBelief,
	     .player = PlayerNames::PLAYER_ONE,
	     .delaySeconds = 1.0f,
	     .amount = k_WinningBelief,
	     .thing = ThingNear {.type = script::ObjectType::Town, .point = k_TibetanVillage, .radius = k_TibetanPickRadius}},
	    WaitTask("ChallengeHighlightNotify"),
	    CameraOn(k_CampScroll, k_ScrollEyeHeight, k_ScrollEyeBack, k_ScrollLookHeight),
	};
	PressScroll(commands, k_CampScroll);
	// the introduction; then a heal miracle cast over the camp
	commands.push_back(WaitText(k_HealUsLine));
	commands.push_back(CameraOn(k_Camp, k_EyeAbove, k_EyeBack, 0.0f));
	commands.back().delaySeconds = k_SitDownSeconds;
	CastOver(commands, SpellSeedType::Heal, k_Camp);
	// the pyre's scene; then a fireball let go over the pyre, which falls on it
	commands.push_back(WaitText(k_PyreSceneLastLine));
	commands.push_back(CameraOn(k_VillageToCamp, k_WideEyeHeight, k_WideEyeBack, 0.0f));
	commands.back().delaySeconds = k_SitDownSeconds;
	ThrowFireballAt(commands, k_Pyre);
	// the charge's scene ends once the last crusader has gone into the cave; then each blowhole is looked at in turn
	commands.push_back(WaitGlobal("ChargeIn", 1.0f));
	for (const auto script : k_CrusaderScripts)
	{
		commands.push_back(WaitTask(script, TaskWait::NotRunning));
	}
	for (size_t hole = 0; hole < k_Blowholes.size(); ++hole)
	{
		auto camera = CameraOn(k_Blowholes.at(hole), k_BlowholeEyeHeight, k_BlowholeEyeBack, 0.0f);
		camera.delaySeconds = hole == 0 ? k_AfterChargeSeconds : k_BlowholeStaySeconds;
		commands.push_back(camera);
	}
	// the treasure's scene, then the survivors' wait to be healed, which is left to run out
	commands.push_back(WaitText(k_TreasureLastLine));
	commands.push_back(WaitTask("Crusaders", TaskWait::NotRunning));
	commands.push_back(LogGlobals(k_DragonGlobals, k_DragonScripts, 1.0f));
	all.push_back({
	    .id = "quest.the_magic_dragon",
	    .name = "Land 5 quest run: The Magic Dragon",
	    .facet = Facet::Story,
	    .description = "Played after a quest run's start (quest=SetupLand5/BeginLand5/Crusaders on Land5.txt). The texts "
	                   "that wait for a click are clicked on. It hands the Tibetan village by the cave to the player, "
	                   "presses on the scroll over the crusaders' camp, casts a heal miracle over the camp after the "
	                   "introduction, lets a fireball go over the pyre after its scene, then, once the crusaders have "
	                   "charged into the cave, puts the camera close above each of the five blowholes in turn, and waits "
	                   "for the quest to end.",
	    .expected = "The press plays the introduction: the crusaders ask to be healed. Healed, they ask for the pyre to "
	                "be lit; lit, its smoke pours out of the five blowholes and the crusaders charge in. At each blowhole "
	                "the fight is heard; at the fifth three crusaders stagger out of the far exit with a flying-flock "
	                "miracle dispenser, the player gains an influence ring there, the scroll is done, and after two "
	                "minutes the survivors walk back into the exit.",
	    .commands = std::move(commands),
	    .onLand = true,
	});
}
} // namespace

void testbed_scenarios::AddQuestLand5SilverScenarios(std::vector<Scenario>& all)
{
	AddSwapToBrownBear(all);
	AddExplorersAgain(all);
	AddJapaneseTraitor(all);
	AddHeavenlyFire(all);
	AddMagicDragon(all);
}
