/******************************************************************************
 * Copyright (c) 2018-2026 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

#pragma once

#include <cstdint>

#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <entt/entity/fwd.hpp>
#include <glm/vec3.hpp>

#include "3D/MapCoords.h"
#include "ECS/Components/ComputerHand.h"
#include "ECS/Components/ComputerMind.h"
#include "ECS/ComputerPlayerMind.h"
#include "Enums.h"

/// The rules of a computer player's hand: its line of steps, the move that some steps need in front of them, and how the
/// first step goes through its stages, one stage a turn.
namespace openblack
{
namespace v120
{
struct InfoConstants;
}
using InfoConstants = v120::InfoConstants;
} // namespace openblack

namespace openblack::computer_player
{
class ScoreWorld;
} // namespace openblack::computer_player

namespace openblack::computer_hand
{

/// What a stage of a step gives
enum class StageResult : uint8_t
{
	/// The stage is over: the next turn runs the next stage, and after the third stage the next step
	Done = 0,
	/// The step cannot be done: the hand gives up every step and lets go of what it holds
	Failed = 1,
	/// The stage goes on the next turn
	Going = 2,
};

/// The move that the step needs in front of it, or HandSubAction::None
[[nodiscard]] ecs::components::HandSubAction Prerequisite(ecs::components::HandSubAction type);

/// The hand has no step left, so a script waiting for it goes on
[[nodiscard]] bool IsReady(const ecs::components::ComputerHand& hand);

/// Where the player's interface reports its hand in the turn: where the hand was as the turn began when the turn gave it
/// steps, else where its steps left it; nowhere (the map's corner, on the ground) without a hand or before its first
/// turn of steps
[[nodiscard]] map_coords::MapCoords ReportedHandPlace(const ecs::components::ComputerHand* hand, uint32_t turn);

/// The step goes at the end of the line
void Append(ecs::components::ComputerHand& hand, ecs::components::HandStep step);

/// What the steps of the hand do in the game
/// Where a hand that follows a thing goes: the thing's place on the map, its height above the land there included,
/// and the thing's height
struct FollowedPlace
{
	map_coords::MapCoords place {};
	float height {0.0f};
};

class StageRunner
{
public:
	virtual ~StageRunner() = default;

	/// Runs the stage (0 start, 1 going on, 2 end) of the step
	virtual StageResult Run(ecs::components::ComputerHand& hand, ecs::components::HandStep& step, uint8_t stage) = 0;
	/// The hand lets go of what it holds, where it is
	virtual void LetGo(ecs::components::ComputerHand& hand) = 0;
	/// The thing that a step acts on is no longer in the game
	[[nodiscard]] virtual bool IsGone(entt::entity thing) const = 0;
	/// The thing may be picked up by a computer player's hand
	[[nodiscard]] virtual bool CanBePickedUp(entt::entity thing) const = 0;
	/// The thing goes into the hand; whether it went in
	virtual bool TakeIntoHand(ecs::components::ComputerHand& hand, entt::entity thing) = 0;
	/// The hand's player gives what the hand holds to the thing, as a player's hand gives a held object to the object
	/// under it: the thing takes it or not, and what it does not take stays in the hand
	virtual void GiveHeldTo(ecs::components::ComputerHand& hand, entt::entity thing) = 0;
	/// How much of the resource the thing holds (a store's pile answers with its store's)
	[[nodiscard]] virtual int32_t ResourceOf(entt::entity thing, ResourceType type) const = 0;
	/// The thing is a pot, or a pile, of the resource
	[[nodiscard]] virtual bool IsPotOf(entt::entity thing, ResourceType type) const = 0;
	/// The amount of the resource is taken out of the thing
	virtual void RemoveResource(entt::entity thing, ResourceType type, int32_t amount) = 0;
	/// The pot takes the amount of the resource
	virtual void AddToPot(entt::entity pot, ResourceType type, int32_t amount) = 0;
	/// A new pot of the kind a hand carries for the resource, holding the amount, made where the thing is; none when
	/// none was made
	virtual entt::entity MakeHandPot(entt::entity at, ResourceType type, int32_t amount) = 0;

	// ---- what the miracle steps ask

	/// The gesture a miracle is made with: its seed's selection gesture when `which` is 0, else its seed's own gesture
	[[nodiscard]] virtual uint8_t GestureOf(int32_t miracle, int32_t which) const = 0;
	/// The gesture's shape: its points, each from 0 to 1 across (x) and up (z)
	[[nodiscard]] virtual const std::vector<glm::vec3>& GestureShape(uint8_t gesture) const = 0;
	/// How long the gesture's shape is, against the circle's
	[[nodiscard]] virtual float GestureLengthRatio(uint8_t gesture) const = 0;
	/// The spell icon has a worship site: a site's icon its site, a town's icon its town's
	[[nodiscard]] virtual bool HasWorshipSite(entt::entity icon) const = 0;
	/// The thing is a worship site's icon
	[[nodiscard]] virtual bool IsWorshipIcon(entt::entity thing) const = 0;
	/// The icon is charging, for anyone
	[[nodiscard]] virtual bool IsIconCharging(entt::entity icon) const = 0;
	/// The icon may start charging for the hand's player at the power-up level, from its site's chants
	[[nodiscard]] virtual bool ValidForStartCharge(entt::entity icon, int32_t powerUp) const = 0;
	/// The icon starts charging for the hand's player at the power-up level, from its site's chants
	virtual void StartCharge(entt::entity icon, int32_t powerUp) = 0;
	/// The chants the icon still needs before its miracle is charged
	[[nodiscard]] virtual float ChantNeeded(entt::entity icon) const = 0;
	/// The power-up level a miracle is: -1 for a miracle at its base level
	[[nodiscard]] virtual int32_t PowerUpOf(int32_t miracle) const = 0;
	/// How long, in seconds, the hand stays with a thing after casting the miracle on it
	[[nodiscard]] virtual float SecondsWithTheThingAfterACast(int32_t miracle) const = 0;
	/// Where the hand follows a thing to: its place on the map and its height; none without a place
	[[nodiscard]] virtual std::optional<FollowedPlace> PlaceToFollow(entt::entity thing) const = 0;
	/// The icon's miracle cast on the thing, an object, from the hand's point: none when the icon still needs chants,
	/// the thing is no object or the miracle can't be cast on it
	virtual entt::entity CastOnObject(entt::entity thing, int32_t miracle, const glm::vec3& from, entt::entity icon) = 0;
	/// The hand's player's mind keeps the miracle it cast on the thing, the newest first
	virtual void KeepCast(entt::entity spell, entt::entity thing) = 0;
};

/// The hand gives up every step and lets go of what it holds; its next step will be looked at for a move again
void Clear(ecs::components::ComputerHand& hand, StageRunner& runner);

/// Every turn: when a thing that one of the steps acts on is gone, the hand gives up every step
void SweepGone(ecs::components::ComputerHand& hand, StageRunner& runner);

/// The hand's place on the map grid from where it is, as each turn of its steps keeps it
void KeepMapPlace(ecs::components::ComputerHand& hand);

/// One turn of the hand's steps: the move that the first step needs is put in front of it, or the first step's stage
/// runs. Then, unless it put a move in front, the hand's place becomes its reported camera. Whether the hand kept its
/// steps (false when a stage failed and the hand gave them all up).
bool Step(ecs::components::ComputerHand& hand, StageRunner& runner);

// ---- the moves of the hand ------------------------------------------------------------------------------------------

/// The length of the game's turn in milliseconds: a step of the hand is its speed over the turns in a second
constexpr uint32_t k_TurnMilliseconds = 100;
/// How high a hand that moves over the land is held above the ground, in metres
constexpr float k_HoverHeight = 10.0f;
/// A distance above which a move to an object takes long strides, and the stride
constexpr float k_FarFromObject = 300.0f;
constexpr float k_FarStride = 100.0f;
/// A distance above which (up to the far one) a move to an object takes short strides, and the stride
constexpr float k_NearObject = 100.0f;
constexpr float k_NearStride = 10.0f;
/// How near the object the hand must be for the move to it to end
constexpr float k_AtObject = 1.0f;

/// The height of the land at a place on the map grid
class Ground
{
public:
	virtual ~Ground() = default;

	[[nodiscard]] virtual float AltitudeAt(int32_t mapX, int32_t mapZ) const = 0;
};

/// How far the hand goes in a turn at the speed
[[nodiscard]] float StepLength(float speed);

/// The hand is held the hover height above the land under it
void HoverOverGround(ecs::components::ComputerHand& hand, const Ground& ground);

/// A point from a place on the map grid and a height above the land there, as a thing's place is kept
[[nodiscard]] glm::vec3 PointFromMap(int32_t mapX, int32_t mapZ, float heightAboveGround, const Ground& ground);

/// The start of a move: the hand will go to the point
void StartTowards(ecs::components::ComputerHand& hand, const glm::vec3& target);

/// A turn of the move to a point over the land: across the land only, held above the ground, until the target is no
/// further than a step. Done when it got there
[[nodiscard]] StageResult MoveOverGround(ecs::components::ComputerHand& hand, const Ground& ground);

/// A turn of the move straight to a point, its height too. Done when it got there
[[nodiscard]] StageResult MoveStraight(ecs::components::ComputerHand& hand);

/// A turn of the move to a thing: across the land, in long strides when far and short ones nearer, held above the
/// ground. Done when it is within a metre
[[nodiscard]] StageResult MoveToThing(ecs::components::ComputerHand& hand, const Ground& ground);

/// The hand is put at the point at once: held above the ground, or at the point's own height without a ground
void PutAt(ecs::components::ComputerHand& hand, const glm::vec3& point, const Ground* ground);

// ---- a script takes the hand ----------------------------------------------------------------------------------------

/// A step that takes the hand to a point
[[nodiscard]] ecs::components::HandStep StepToPoint(ecs::components::HandSubAction type, const glm::vec3& point);

/// A script takes the hand for the step: the hand gives up its steps and lets go of what it holds, takes the speed, and
/// the step is its only one; the hand is then held by the script and busy
void TakeForScript(ecs::components::ComputerHand& hand, ecs::components::HandStep step, float speed, StageRunner& runner);

/// A pause (or its end) holds the hand (or lets it go), and the steps take the player's speed again
void Pause(ecs::components::ComputerHand& hand, bool paused);

/// The player's speed and the speed of the steps under way
void SetSpeed(ecs::components::ComputerHand& hand, float speed);

/// A script is moving the hand: it gave it steps it has not finished, or queued actions for it
[[nodiscard]] bool MovedByScript(const ecs::components::ComputerHand& hand);

/// The hand goes through its steps this turn: it has some, or a script holds it
[[nodiscard]] bool TakesItsTurn(const ecs::components::ComputerHand& hand);

/// The land under the hand as of its last turn
[[nodiscard]] glm::vec3 OnGround(const ecs::components::ComputerHand& hand, const Ground& ground);

// ---- holding --------------------------------------------------------------------------------------------------------

/// A computer player's hand may not pick up a thing that a hand already holds: the player's own, or a computer player's
[[nodiscard]] bool MayBePickedUp(entt::entity thing, std::optional<entt::entity> heldByPlayersHand,
                                 std::span<const entt::entity> heldByComputerHands);

/// The player a computer player's hand's let-go is credited to: the hand lets go through its player's own interface, so
/// it is that player's throw; a hand of no player's (the neutral player has no mind) is no one's
[[nodiscard]] std::optional<PlayerNames> LetGoCreditedTo(PlayerNames handPlayer);

/// What a let-go leaves for the hand that let go (the rest of a scaffold put down gently, or nothing) goes straight
/// into that hand, as a pick-up's thing does; whether it went in
bool TakeLeftOver(ecs::components::ComputerHand& hand, entt::entity leftOver, StageRunner& runner);

/// A turn of the pick up of the thing: a hand that holds something lets go of it first and tries again the next turn; a
/// thing that may not be picked up fails the step; else the thing goes into the hand (or does not, and the step is over
/// either way)
[[nodiscard]] StageResult PickUpTurn(ecs::components::ComputerHand& hand, entt::entity thing, StageRunner& runner);

/// A turn of the drop: the hand lets go of what it holds where it is
[[nodiscard]] StageResult DropTurn(ecs::components::ComputerHand& hand, StageRunner& runner);

/// A turn of the giving of what the hand holds to the thing: the hand's player gives it (the thing takes it or not),
/// and the step's turn is over either way
[[nodiscard]] StageResult GiveHeldTurn(ecs::components::ComputerHand& hand, entt::entity thing, StageRunner& runner);

/// A turn of taking the resource out of a container into the hand: the amount asked for, less what a pot of the
/// resource in the hand holds already, and no more than the container has. A hand that holds something else waits; the
/// amount goes into the pot held, or into a new pot of the hand's kind that goes into the hand. Done when the hand has
/// a pot of it; the step fails when it has none
[[nodiscard]] StageResult TakeFromContainerTurn(ecs::components::ComputerHand& hand, entt::entity container, int32_t asked,
                                                ResourceType type, StageRunner& runner);

// ---- the miracle's gesture ------------------------------------------------------------------------------------------

/// How many metres across a computer hand draws a gesture's shape
constexpr float k_GestureSize = 30.0f;
/// How many turns a gesture as long as the circle takes
constexpr float k_GestureTurnsForTheCircle = 30.0f;

/// The gesture a miracle is made with: the selection gesture of the seed the game gives the miracle (its first seed
/// that has it) when `which` is 0, else that seed's own gesture; none for a miracle out of the tables or no seed has
[[nodiscard]] uint8_t GestureOfMiracle(const InfoConstants& info, int32_t miracle, int32_t which);

/// The hand's point at a point of a gesture's shape: the shape stands upright from where the gesture began, its
/// across along the land's x and its up as height; the hand keeps the z it began at
[[nodiscard]] glm::vec3 GesturePoint(const glm::vec3& origin, const glm::vec3& shapePoint);

/// The start of a gesture: it begins where the hand is, and the hand goes to the start of the shape
void StartGesture(ecs::components::ComputerHand& hand, const std::vector<glm::vec3>& shape);

/// A turn of a gesture: it goes on by one over the turns that a gesture of its length takes (the circle's 30 times its
/// length against the circle's), up to the whole of it. Done once it has the whole (the hand stays where it was), else
/// the hand goes to that point of the shape
[[nodiscard]] StageResult GestureTurn(ecs::components::ComputerHand& hand, const std::vector<glm::vec3>& shape,
                                      float lengthRatio);

/// The end of a gesture: the hand goes back where it began
void EndGesture(ecs::components::ComputerHand& hand);

/// A stage of the MakeGesture step (its miracle and which of its gestures): the stages above, with the gesture's shape
/// from the game
[[nodiscard]] StageResult GestureStage(ecs::components::ComputerHand& hand, const ecs::components::HandStep& step,
                                       uint8_t stage, StageRunner& runner);

// ---- the worshippers' chant for a miracle ---------------------------------------------------------------------------

/// How long, in seconds, the hand waits for its icon to start charging
constexpr uint32_t k_SecondsToStartCharging = 5;
/// How long, in seconds, the hand waits for its icon to be charged
constexpr uint32_t k_SecondsToCharge = 30;

/// The whole seconds the hand's count of turns makes (the turns over the turns in a second, cut toward zero)
[[nodiscard]] uint32_t CountedSeconds(int32_t turns);

/// A stage of the MakeWorshippersChantForSpell step (its miracle and its icon). Its start counts from 0 and needs an
/// icon with a worship site. Each turn of it counts one: it needs a worship site's icon; it is done once the icon is
/// charging for anyone, or once it starts charging for the hand's player at the miracle's power-up level; else it fails
/// when the icon has no site, or once more than 5 seconds are counted, and goes on otherwise. Its end needs a worship
/// site's icon, and is done once the icon needs no more chants; else it counts one, fails once more than 30 seconds are
/// counted, and goes on
[[nodiscard]] StageResult ChantStage(ecs::components::ComputerHand& hand, const ecs::components::HandStep& step, uint8_t stage,
                                     StageRunner& runner);

// ---- the cast of a miracle at an object ----------------------------------------------------------------------------

/// How high over the land a hand following a thing is, in the thing's heights
constexpr float k_HeightsOverAFollowedThing = 2.5f;

/// A turn of the hand following a thing: it goes to the thing's place again, across the land, in long strides when
/// far and short ones nearer, no further than the thing, and is held two and a half of the thing's heights over the
/// land
void FollowThing(ecs::components::ComputerHand& hand, const FollowedPlace& at, const Ground& ground);

/// A stage of the CastSpellAtObject step (the thing, the miracle and its icon). Its start does nothing more. Its turn
/// follows the thing once and casts the icon's miracle on it from where the
/// hand is: none fails; else the mind keeps the cast and the hand stays with the thing for the miracle's time after a
/// cast. Its end follows the thing until that time is over, counting it down a turn at a time
[[nodiscard]] StageResult CastAtObjectStage(ecs::components::ComputerHand& hand, const ecs::components::HandStep& step,
                                            uint8_t stage, StageRunner& runner, const Ground& ground);

// ---- waiting --------------------------------------------------------------------------------------------------------

/// The turns a wait of so many seconds lasts: the turns in a second times the seconds, cut toward zero
[[nodiscard]] int32_t WaitTurns(float seconds);

/// The start of a wait: the hand will count down the turns of so many seconds
void StartWait(ecs::components::ComputerHand& hand, float seconds);

/// A turn of a wait: a turn less, and done when none is left. A wait of no turns goes below zero and does not end
[[nodiscard]] StageResult WaitTurn(ecs::components::ComputerHand& hand);

// ---- the actions of the mind that a script forces or queues ---------------------------------------------------------

/// The action of a node of the computer player's mind that a script names
enum class MindAction : uint8_t
{
	/// A node whose action openblack does not have yet
	NotPorted,
	/// The scaffold given is picked up and dropped at the marker
	FindPlaceToUseScaffoldForBuilding,
	/// The thing given is picked up and dropped at the marker, at once
	DirectPickupAndDropObject,
	/// The villager is taken from its town to the worship site's gate
	TakeVillagerFromTownToWorshipSite,
	/// Food is taken from another town's store and given to the object above
	GetFoodFromOtherTownForObject,
	/// Wood is taken from another town's store and given to the thing above, or a building site's building
	GetWoodFromOtherTown,
	/// Food is taken from another town's store and given to where the town above takes it
	GetFoodFromOtherTownForStoragePit,
	/// The miracle of the icon given is cast on the store of the town given
	CastIconsMiracleOnTownStore,
};

/// The first node of the mind whose name, up to its ':', the name begins with, in the order the game looks: none when
/// no node has such a name. The nodes are known by their rows' names as they are now
[[nodiscard]] std::optional<MindAction>
FindMindAction(std::string_view name, const computer_player::MindRowNames& names = computer_player::OwnRowNames());

/// A name that begins with "DIRECT", in any case: its node's action runs at once
[[nodiscard]] bool IsDirectAction(std::string_view name);

/// The two leaves' steps: the thing is picked up and dropped at the point, unless it may not be picked up
bool PickUpAndDrop(ecs::components::ComputerHand& hand, entt::entity thing, const glm::vec3& point, StageRunner& runner);

/// How long the hand holds a villager at a worship site's gate before it drops it, in seconds
constexpr float k_SecondsAtTheGate = 1.0f;
/// The number the move to the gate carries, which the move does not read
constexpr float k_GateMoveNumber = 900.0f;

/// The steps that take a villager to a worship site's gate: picked up at the player's speed, the move to the gate, a
/// second held there, and the drop at the gate. None when there is no gate, or the villager may not be picked up
void TakeToGate(ecs::components::ComputerHand& hand, entt::entity villager, std::optional<glm::vec3> gate, StageRunner& runner);

/// The steps that fetch food from a store's pile for an object: the move to the pile, the food asked for taken out of
/// it into the hand, and given to the object
void FetchFoodFor(ecs::components::ComputerHand& hand, entt::entity pile, const glm::vec3& pilePoint, int32_t asked,
                  entt::entity object);

/// The steps that fetch wood from a store's pile for a thing: the wood asked for taken out of the pile into the hand
/// (the move to the pile goes in front of it), and given to the thing
void FetchWoodFor(ecs::components::ComputerHand& hand, entt::entity pile, uint32_t asked, entt::entity thing);

/// From this many metres away from its icon the hand makes the miracle's gesture instead of going to the icon
constexpr float k_GestureFromIconMetres = 200.0f;

/// The steps that cast an icon's miracle on a thing. Nearer than 200 m to the icon the hand goes to it, else it makes
/// the miracle's gesture; the icon's worshippers chant for the miracle; then a miracle cast at an object is cast at the
/// thing, and any other kind of miracle no further. False, with no steps, for a miracle cast at a place (fireballs,
/// lightning, storms, tornadoes and shields), which openblack's hand does not cast yet
bool CastIconsMiracleAt(ecs::components::ComputerHand& hand, entt::entity icon, MagicType miracle, float metresToIcon,
                        entt::entity thing);

/// What the game knows of the things a mind's action acts on
class ThingFacts
{
public:
	virtual ~ThingFacts() = default;

	/// The thing is an object, as against a town, a marker or the like
	[[nodiscard]] virtual bool IsObject(entt::entity thing) const = 0;
	/// The thing has a place on the land
	[[nodiscard]] virtual bool HasPlace(entt::entity thing) const = 0;
	/// Where the thing is, when it is a script's marker
	[[nodiscard]] virtual std::optional<glm::vec3> MarkerPoint(entt::entity thing) const = 0;
	/// The thing is a villager
	[[nodiscard]] virtual bool IsVillager(entt::entity thing) const = 0;
	/// The thing is a worship site
	[[nodiscard]] virtual bool IsWorshipSite(entt::entity thing) const = 0;
	/// The point of a worship site's gate, through the map grid; none when the site has no gate
	[[nodiscard]] virtual std::optional<glm::vec3> GatePoint(entt::entity site) const = 0;
	/// Where the thing is, through the map grid
	[[nodiscard]] virtual glm::vec3 MapPoint(entt::entity thing) const = 0;
	/// The building of a building site; none for a thing that is not a building site, or a site without one
	[[nodiscard]] virtual entt::entity SiteBuilding(entt::entity thing) const = 0;
	/// How much of the resource the thing holds (a store's pile answers with its store's)
	[[nodiscard]] virtual uint32_t ResourceOf(entt::entity thing, ResourceType type) const = 0;
	/// The thing is a town
	[[nodiscard]] virtual bool IsTown(entt::entity thing) const = 0;
	/// The resource of a pot or a pile
	[[nodiscard]] virtual ResourceType ResourceTypeOf(entt::entity thing) const = 0;
	/// Where a town takes the resource: its store, else its temporary pot (made when it has none); none when it has
	/// neither
	[[nodiscard]] virtual entt::entity ResourceDropObject(entt::entity town, ResourceType type) const = 0;
	/// The miracle of a worship site's icon; none for a thing that is no such icon
	[[nodiscard]] virtual std::optional<MagicType> IconMagicType(entt::entity thing) const = 0;
	/// The thing's place on the map grid
	[[nodiscard]] virtual map_coords::MapCoords MapPlace(entt::entity thing) const = 0;
};

/// The actions of the mind that openblack has, done by the hand: the two leaves that pick a thing up and drop it at a
/// point, the villager taken to a worship site, the food and wood fetched from another town, and an icon's miracle cast
/// on a town's store. With what the score
/// rules read of the game it scores every way openblack has (computer_player::Score); without, it scores nothing. A
/// chosen action it lacks is told to `missing`, when one is given
class HandActions final: public computer_player::MindWorld
{
public:
	using Missing = std::function<void(computer_player::ActionKind, uint16_t)>;

	HandActions(ecs::components::ComputerHand& hand, StageRunner& runner, const ThingFacts& facts,
	            computer_player::ScoreWorld* scores = nullptr, Missing missing = {});

	float Score(computer_player::ScoreKind kind, ecs::components::ComputerMind& mind, uint16_t node) override;
	[[nodiscard]] bool HasScore(computer_player::ScoreKind kind) const override;
	[[nodiscard]] bool HasAction(computer_player::ActionKind kind) const override;
	void GiveUpSteps() override;
	/// FindPlaceToUseScaffoldForBuilding: the thing nearest above, one with a place, is picked up and dropped at the
	/// point of the node the node chose. DIRECTPickupAndDropObject: the thing nearest above, an object, is picked up and
	/// dropped at the node's second thing, a marker. TakeVillagerFromTownToWorshipSite: the node's villager is picked up,
	/// taken to the gate of the worship site above, held there a second and dropped; done with no steps when the site
	/// has no gate or the villager may not be picked up. GetFoodFromOtherTownForObject: the node's pile and the object above
	/// its parent, both objects, and the food its parent asks for: the food is fetched from the pile and given to it.
	/// GetWoodFromOtherTown: the node's pile, with a place and some wood, and the thing above its parent with a place
	/// (or a building site's building): all the pile's wood is fetched and given to it. GetFoodFromOtherTownForStoragePit:
	/// the node's pile, an object, and the town above its parent with somewhere to take it: the food the node asks for,
	/// cut to a whole number, is fetched and given to where the town takes it. ShallCastFoodSpell (the casting leaves
	/// under the towns' food and wood wants): the icon nearest above, a worship site's, and the town above its parent with
	/// somewhere to take the miracle's resource (wood for the wood miracle, else food): the icon's miracle is cast on it
	/// (CastIconsMiracleAt), from where the hand is
	bool RunAction(computer_player::ActionKind kind, ecs::components::ComputerMind& mind, uint16_t node) override;
	void TimeSteps() override;
	void ActionMissing(computer_player::ActionKind kind, uint16_t node) override;

private:
	ecs::components::ComputerHand& _hand;
	StageRunner& _runner;
	const ThingFacts& _facts;
	computer_player::ScoreWorld* _scores;
	Missing _missing;
};

/// A script forces the action named on the two things through the player's mind, as the game does
/// (computer_player::Force): the hand gives up its steps when a node's action runs, and is busy when that gave it steps.
/// Whether it did; nothing for a name no node has, a node whose action openblack lacks or a weighing it cannot make. The
/// nodes are known by their rows' names as they are now
bool ForceAction(ecs::components::ComputerHand& hand, ecs::components::ComputerMind& mind, std::string_view name,
                 entt::entity first, entt::entity second, uint32_t turn, StageRunner& runner, const ThingFacts& facts,
                 const computer_player::MindRowNames& names = computer_player::OwnRowNames());
/// The same through a world of the mind's own (the game's, which scores and tells the actions it lacks)
bool ForceAction(ecs::components::ComputerHand& hand, ecs::components::ComputerMind& mind, std::string_view name,
                 entt::entity first, entt::entity second, uint32_t turn, const ThingFacts& facts,
                 computer_player::MindWorld& world,
                 const computer_player::MindRowNames& names = computer_player::OwnRowNames());

/// A script queues the action, to be forced when the hand is free
void QueueAction(ecs::components::ComputerHand& hand, std::string name, entt::entity first, entt::entity second);

/// Every turn: a queued action one of whose things is gone is dropped
void SweepQueue(ecs::components::ComputerHand& hand, const StageRunner& runner);

/// The next queued action, taken off the queue
[[nodiscard]] std::optional<ecs::components::QueuedHandAction> TakeNext(ecs::components::ComputerHand& hand);

// ---- drawing what the hand holds ------------------------------------------------------------------------------------

/// What the draw keeps for the thing a computer hand holds: the point it slides from, the point it slides to, and the
/// game milliseconds drawn since the last turn of the hand and in the turn before
struct HeldDraw
{
	glm::vec3 start {0.0f};
	glm::vec3 hand {0.0f};
	int32_t elapsedMs {0};
	int32_t durationMs {0};
	/// The hand's turns of steps the draw has seen
	uint32_t turnsSeen {0};
};

/// The hand had one or more turns of steps since the draw last looked, the last one beginning at the point: the thing
/// slides from there to there, over the time drawn in the turn before (none when more than one turn went by), so it is
/// drawn a turn at a time, as the original draws it. With `smooth` (the View menu's smooth drawing, not as the
/// original) it slides from where the hand was as the turn before began
void NewTurnsOfTheHand(HeldDraw& draw, const glm::vec3& turnStartPoint, uint32_t turns, bool smooth = false);

/// Where the thing is between the two points after `elapsedMs` of `durationMs`: at the end point once the time is up,
/// or when there is no time
[[nodiscard]] glm::vec3 SlideOfHeld(const glm::vec3& start, const glm::vec3& hand, int32_t elapsedMs, int32_t durationMs);

/// A frame of the draw of the held thing: the frame's game milliseconds go by, and the point it is drawn at
[[nodiscard]] glm::vec3 HeldDrawPoint(HeldDraw& draw, uint32_t frameGameMs);

/// The point through the map grid, as a thing's place is kept: x and z on the grid, the height above the land kept
[[nodiscard]] glm::vec3 OnTheMapGrid(const glm::vec3& point, const Ground& ground);

} // namespace openblack::computer_hand
