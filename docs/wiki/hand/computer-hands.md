# The computer players' hands

## Summary

Every player of the original has a computer player object, and with it a hand that a script or the player's mind can
drive. The hand works through a line of steps (sub-actions): move to a point, move to an object, pick it up, drop it,
cast a miracle, and so on. Some steps need a move of the hand first, which is put in front of them when they start. The
first step goes through three stages, one a turn. The story's scripts take the hand (`MOVE_COMPUTER_PLAYER_POSITION`,
`SET_COMPUTER_PLAYER_POSITION`), give it named actions of the player's mind (`FORCE_COMPUTER_PLAYER_ACTION`,
`QUEUE_COMPUTER_PLAYER_ACTION`) and wait until the line is empty (`COMPUTER_PLAYER_READY`). The rival gods' hands are
not seen: no computer hand is ever drawn. Where the hand is, the god shows a cloud of sparkles in its colour with its
symbol turning faintly in the middle (user, 2026-10-30; see [What is drawn](#what-is-drawn) and
[The god's sparkles and symbol](#the-gods-sparkles-and-symbol)). What is seen of them is also the object they hold
and their gesture sparkles (user,
2026-10-18 22:56, as the exe shows).

openblack keeps the hand as a `ComputerHand` on the player's entity with the original's line of steps, its table of
moves in front and its stages (`computer_hand::`, `ECS/ComputerHand.h`). Its moves, the natives and the named actions
are listed below as they are ported.

## The original

### The computer player and its hand

- `GPlayer::Init` (0x6491F4) gives every player a `GComputerPlayer(player, 90.0, 30.0, 50)` at `GPlayer` +0x944: the
  player's speed +0x58 is 90 metres a second. `GComputerPlayer::Init` (0x656E60) makes the decision tree (+0x2C,
  0x656880) and the `PlayerActionState` (+0x34, 0x64FFD0), whose speed (+0x68) starts at the player's speed.
- The computer player's flags: +0x1B8 on (the land script's toggle and native 311, see
  [the player's creature](../creature/players-creature.md#a-computer-players-start-on-the-land)), +0x1BC paused,
  +0x1C0 held by a script, +0x1F8 busy (a script gave it steps), +0x50 / +0x54 the queued actions and their count.
- The `PlayerActionState`: +0x28 / +0x2C the steps and their count, +0x30 "the first step is still to be looked at for
  a move in front", +0x34 "a move was put in front", +0x38 the first step's stage, +0x50 the hand's point, +0x5C the
  point the step under way goes to, +0x68 the speed, +0x14 the hand's place on the map grid (x and z times 6553.6, cut
  toward 0, no height).
- A step (`PlayerSubAction`): +0x14 its type, +0x18 its arguments. An argument (0x74 bytes): +0x14 its kind (0 a thing,
  1 a point, 3 a whole number, 5 a float), +0x18 the thing, +0x5C the number, +0x64 the float, +0x68 the point. They are
  made by 0x6510F0 (thing), 0x651120 (point), 0x6511B0 (number) and 0x651210 (float), and a step goes at the end of the
  line by 0x650280.
- Each turn the steps copy the hand's point into the player's interface status (0x650370, 0x5DC060), so what the
  hand holds is carried at that point.

### What is drawn

The rival gods' hands are not seen, only their symbol and colour (user, 2026-10-18; on 2026-10-30: the god's symbol
with sparkles in its colour where its hand is). In the exe no computer hand is ever drawn: `GComputerPlayer::Draw` (0x6573C0) is a bare return and `GPlayer::DrawComuterPlayer` (0x64C1D0) only calls
it; only the local interface (`MyInterface()`) gets a `CHand` (0x5CE953), and `CHand::AddDrawing` runs only for the
local hand (0x54DF4D). What is seen is the held object, drawn at the hand's point by `Object::DrawInHand` (0x5190E0),
sliding over the turn, and the god's sparkles round its symbol where the hand is (below).
The user confirmed it (2026-10-18 22:56): for the other gods you see the object they hold and their gesture
sparkles. The sparkles come from the end of the gesture step (0x654B50), which queues a recognised-gesture record on
the land at the gesture point (`FakeGestureOnLandscape` 0x689440); openblack does not draw them yet (Pending).

- **Where the held object is drawn.** `GGame::Process3dEngine` (0x54DF52..0x54DFCF), every rendered frame of the world
  view, gives each interface status other than the local one that holds something (+0x90) to its object's
  `DrawInHand` (vt +0xD0; `Object::DrawInHand` 0x5190E0 for villagers, animals, rocks, trees, piles, the magic objects,
  spell seeds and icons; a scaffold's 0x5181A0 calls it too). It marks the 3D object out of the map (+8 |= 0x40, so the
  map's draw skips it), adds the frame's game ms (`g_game` +0x205D48) to +0xE0, and puts the object at +0xC8 when +0xE4
  is 0, else at `start + (hand - start) × min(+0xE0, +0xE4) / +0xE4` with start +0xD4 and hand +0xC8 (the share is
  the whole numbers' float quotient, then each axis `way × share + start`). The point goes through a `MapCoords`
  (0x603340: x and z × 6553.6 cut toward 0, the height above the land there kept) and back, at the object's own scale
  (+0x50); with no time the matrix is set anew (`LH3DObject::SetPosition`, angle 0), else only its scale and
  translation. Then `DrawOutOfMap`.
- **Its points, settled from the exe.** The hand's turn of steps (0x650370) writes the hand's point into +0xC8 as the
  turn **begins**, before the step's stage moves the hand (0x6503B4..0x6503CB), and only at its end calls 0x5DC060
  (0x6505FC), which copies +0xC8 into +0xD4, puts +0xE0 into +0xE4 and clears +0xE0. So after each turn of steps the
  start and the end of the slide are the same point, where the hand was as that turn began: the held object is drawn
  there, a turn behind the hand, and does not slide. Only turns of steps (the hand busy or held) do this; a free hand
  keeps its last points.

### The god's sparkles and symbol

✅ Every player has a `GInterface` (`GPlayer::Init` 0x649190 → `GInterface::Init` 0x5CE670), and each interface a
pool of liquid particles of its own (`LiquidParticleGroup`, 0xB01C bytes: 0x400 particles, the count +0xB000, a point
+0xB004, the interface +0xB014 and a draw callback +0xB018 = `sparklecbstub` 0x5CDFC0), linked into the list every
pool is updated and drawn from (`UpdateLiquidParticles` 0x845D00, `DrawLiquidParticles` 0x845C50).

- **Each turn** (`GGame::AddPlayerSparkles` 0x552640, from `ProcessTurn` 0x54E6F8, after `CHand::GameTurnUpdate`):
  nothing unless the game's view mode is 0 (not in the citadel nor the falling spell's film, `g_game` +0x205A28) and
  the local interface's action state (+0x44) is not 16 (a creature fight). Then for each player from the first,
  stopping at the neutral one (`GGame::GetNextPlayer` 0x5508A0), whose type (+0x8E0) is not 0 and, for a computer
  player (2), whose computer player is on (+0x944 → +0x1B8); for each of its interface statuses but the local one,
  and for a person (1) only while its interface has a network player (+0x1BC): the point is the status's camera
  position (+0xB0); the colour is `GetPlayer3DColor` 0x64B590 with each channel halved, then by `LocalRand(7)`: 0
  half way to (255, 144, 144), 1 to (255, 255, 144), 2 to (144, 255, 144), 3 three quarters, 4 to 6 kept; alpha
  0x80. The kind is `2 - (LocalRand(2) != 0)` (a mote, 1, or a mote with its frames reversed, 2). The particle is
  added to that interface's pool (fn_00845D30: nothing past 0x400 or while the detail's liquid particles are off,
  then CRT `rand() % 16`) with size 10 and no speed,
  and the pool's point set to the camera position (fn_005529E0). The colour sums were checked over every colour and
  tint against the Ghidra C (test `PlayerSparkles.ASparkleIsTheGodsColourAtHalfStrengthChangedByItsTint`).
- **The camera position of a computer god** is where its hand is: each turn of steps writes the hand's point
  (+0x50 of the action state) into the leader status's +0xB0 as it ends (0x6505B7..0x6505D0), but not a turn that
  only put a move in front of the first step, which returns before it (0x650557). Before its first such turn it is what
  the god's citadel set: `Citadel::CreateCitadel` 0x463240 calls fn_005DBB40 for each status of the citadel's player
  (0x4632EC..0x46330E): the focus +0xBC is the citadel's place on the map grid at the land's height there plus the
  citadel's altitude (0 for a script's CREATE_CITADEL), and the position +0xB0 is the focus plus (0, 20, 20).
- **The draw.** Every rendered frame the pools are moved on (`UpdateLiquidParticles`) and each pool is one Z-sorter
  object at its point (`DrawLiquidParticles`, key (dz² + dy²) + dx²), whose draw (sub_845CD0) draws its particles
  (sub_845EE0) and then, while it holds any, calls the callback. `sparklecbstub` draws nothing for the local interface
  or while the local action state is 16, nor for a computer god whose computer player is off, nor for a person
  without a network player; else one static sprite (`LH3DSprite::Create(1, 1)`, made once): its angle, shared by every
  pool, goes on by `g_game_time_inc × 0.0005` (the float at 0x8AA39C) and is brought back once into -π..π
  (`angle_correct` 0x7FAAF0), cells per row 4 (+0x30), its cell the player's number & 0x3F, the material of mode 6
  (alpha blended, texture × colour; fn_00590440 → `CreateMaterial(6, [0xD19C3C])`) on the players' symbol sheet
  (`PlayerSymbol::OpenOnce` 0x5DE2F0: PlayersSymbols.raw), at the pool's point, colour 0x30000000 (black, alpha
  0x30), half size 3 (`SetSize(3)` inlined), then `LH3DSprite::Draw` 0x840530.
- **Going into the citadel** (`GGame::GoInsideCitadel` 0x553FFF → fn_00552A00) empties every other interface's
  pool.
- **The symbol sheet.** `PlayerSymbol::OpenOnce` 0x5DE2F0, once a run, writes PlayersSymbols.raw and
  PlayersSymbolsa.raw as zeros and makes the texture [0xD19C3C] from them: 256 × 256, every cell empty.
  `PlayerSymbol::CreateFinalTextureSymbols` 0x5DEF00 runs at the end of `GGame::Init` (0x54FCF3, after the start's
  land and `PrecalculateMap` 0x54FC30: a new campaign's first land, a skirmish's land or a loaded game; not a land
  change, which goes through `GGame::LoadMap`) and from the profile editor's page (sub_53E740). For players 0 to 7:
  one whose type (+0x8E0) is 2 then gets `UpdateComputerSumbol` 0x5E15B0 with image index `number - 1`, but 1 for the
  second player on land 3 and 2 on land 5 (`g_game` LandNumber); the others go through the profile's tattoo rebuild
  (the local player's cell). `CopyBlockComputerSymbol` 0x5E1440 clamps the index to 0..2 and reads
  `Data\Textures\Lethis.cps`, `Kazarr.cps` or `nemesis.cps` (the strings 0x100 apart from 0xBF2B60;
  `LethisL.cps`... from 0xBF2E60 with the low-resolution textures on, 32 × 32), 64 × 64 bytes, into the cell at
  ((number & 3) × 64, (number >> 2) × 64) of the 4444 surface: each texel `v << 12 | v << 8 | v << 4 | v` with
  v = byte >> 4, colour and alpha alike; then the sheet is saved to the two files (fn_005DE1E0). A god switched on
  after the game's start keeps the cell it had: in a campaign begun on Land 1 the rival gods' cells of Land 2 are
  empty, and only their motes show, unless an earlier game of the same run filled them; a skirmish's gods have theirs.

### The table of steps

`crt_xc_fn_PlayerAction_0064E720` fills 37 rows of 0x90 bytes from 0xBFF240: the step's name (0x40 bytes), then four
member functions (start, going on, end, the time it takes) from +0x40, then at +0x80 the step that must come first, 0x25
for none. The stage functions of step n are at 0xBFF280 + n × 0x90 + stage × 0x10. The moves in front, rebuilt from
the initialiser (it is straight-line code: every store is a constant or one of two registers set once, ebx = 2 and
edx = 0x25):

| Step | Number | Move in front |
|---|---|---|
| PickUpObject | 0 | MoveHandToObject |
| MoveHandToPos, MoveHandToObject, MoveHandToCreatureHead, MoveHandNearPos, MoveHandNearObject, MoveHandToPosStopWhenOutOfInfluence | 1 to 6 | none |
| ThrowObjectInHandAtPos | 7 | MoveHandNearPos |
| DropObjectInHandAtPos | 8 | MoveHandToPos |
| SayText, SetLeash, AttachLeashToObject | 9, 11, 12 | none |
| GiveObjectInHandToCreature, SlapCreature, StrokeCreature, CastSpellAtObject | 10, 13, 14, 15 | MoveHandToObject |
| CastSpellAtPos, CastShieldAtPos | 16, 18 | MoveHandToPos |
| CastSpellTowardPos, CloseDownCurrentSpell, MakeGesture, Wait | 17, 19, 22, 23 | none |
| RaiseTotem, LowerTotem, DropObjectInHandOnObject … SetCreatureSpeedForJourney | 20, 21, 24 to 31 | MoveHandToObject |
| MakeWorshippersChantForSpell(NoWait), SetHandToPos, MoveHandToPosDirectYInterpolation, SetHandToPosDirectY | 32 to 36 | none |

The steps that cast at an object or at a point (15 and 16) take a nearer move instead (MoveHandNearObject or
MoveHandNearPos) when the miracle's effect info (0xCC6630 + magic type × 284, +0x98) is 2 (0x65040D..0x650460).

✅ **Who gives each step** (2026-10-30): the 122 calls of the line's append (0x650280), each read for the step number
it pushes last (the first argument). The steps that throw or move near are given only by actions of the mind:

- **ThrowObjectInHandAtPos** (7), with MoveHandNearPos (4) in front: `ThrowFoodatCreature` 0x65AA20 (0x65AD13),
  `DestroytownWithObject` 0x65CC50 (0x65CD42), `ThrowThingsToImpress` 0x65CF50 (0x65D0A1), `ThrowrockatCreature`
  0x665880 (0x665961), and two sites in code whose function start xref does not find (0x65B7B0, 0x65B862).
- **MoveHandNearObject** (5): `AttackCreatureWithSpells` 0x65CA10 (0x65CB15); 4 and 5 also come in front of a cast
  (above).
- **MoveHandToPosStopWhenOutOfInfluence** (6): the casts of the miracles of magic types 1 to 6 at a place inside the
  influence (0x665FC0 at 0x666104, after `GPlayer::FindNearestPosInsideInfluence`; 0x666410 at 0x666555).
- **WaitForObjectToBeNearPos** (27): no call gives it, and no move-in-front table names it: never given.

So these steps run only with those actions, none of which openblack has yet ([the minds](computer-minds.md)).

### A turn of the steps

`GComputerPlayer::NewProcess` (0x657420), in each player's turn (`GPlayer::Process` 0x6496E7, not the neutral
player's):

1. The computer player's place takes the hand's map place; the queued actions whose things are gone are dropped
   (0x6644D0); when any thing argument of any step is gone or missing, every step is given up (0x650300).
2. Only while it is on, not paused and not held by the wide screen (0x658B70, see
   [the player's creature](../creature/players-creature.md#a-computer-players-start-on-the-land)): when it has steps
   or a script holds it, `fn_006573E0` runs one turn of the steps (0x650370) and clears the busy flag once the line is
   empty; when it is free and has queued actions, the next one is forced (0x664480); else its mind chooses.
3. A turn of the steps (0x650370) keeps the hand's map place, then looks at the first step. If it is still to be looked
   at and its row names a move in front, that move is put in front with a copy of the step's first argument, and the
   turn ends. Otherwise the step's stage function runs (none counts as done): **0** the stage is over (0x6506E0: the
   next stage; after the third the step is taken off the line, and the next step is looked at for a move unless the
   one taken off was such a move), **1** the step failed: every step is given up and the hand lets go of what it holds
   (0x650250, 0x652300), **2** the stage goes on. So a step takes at least three turns.

### The moves

Every move runs at the 24-bit FPU, so each operation rounds to a float. The step of a turn is the speed over the turns
in a second, `speed / (1000 / GGameInfo +0x40)` with +0x40 = 100 ms (`GGame::Init` 0x54F4A5), a whole 10. The
distances below that are "the game's" go through `GUtils::GetDistance` (0x74CDE0): across the land only, by the
approximate `hypotenuse` (0x74F6C0) that our `gutils::GetDistance` ports. Held above the ground (0x6508F0) is the land's
height (`LH3DIsland::GetAltitude`) at (x × 65536 × 0.1, z × 65536 × 0.1) cut to the map grid, plus 10.

- **MoveHandToPos** (1): its start (0x652750) takes the first argument's point as the target. A turn (0x652790): the way
  is across the land, normalised by a true square root when it is not 0; when the game's distance is no more than a step
  the hand is put on the target and held above the ground, done (0); else it goes a step along the way, held above the
  ground, going on (2). Its end (0x652BD0) is done at once.
- **MoveHandToPosDirectYInterpolation** (35): the same start and end; a turn (0x652A90) goes along the straight way,
  height included, and arrives at the target's own height when the game's distance across the land is no more than a
  step.
- **MoveHandToObject** (2): its start (0x652CA0) takes the object's place (`GetInteractPos`, virtual +0x170, the
  object's own map place for villagers and scaffolds, 0x4019F0): x and z from the map grid, the height the land's there
  plus the place's height. A turn (0x652D10): the true distance across the land d; the step is the speed's but no more
  than d, then 100 when d is over 300 and 10 when d is over 100; the hand goes the step and is held above the ground;
  done when what is left across the land is no more than 1 m.
- **SetHandToPos** (34) and **SetHandToPosDirectY** (36): no start or end; their turn (0x652BE0, 0x652C40) puts the
  hand on the point, held above the ground for 34, at the point's height for 36, done.

### Holding

- **PickUpObject** (0): its start (0x6525B0) takes the object's place as the target, as MoveHandToObject's (its move in
  front) does. A turn (0x652620): a hand that holds something lets go of it (0x652300) and the stage goes on (2); an
  object with either of the bits 0x4 or 0x8 of its flags (+0x24, 0x652681, also `sub_658AA0` 0x658AA5) fails the step
  (1). Bit 0x4 is "in a god's hand": the only immediate store of it is 0x5FB014 (`fn_005FAFC0`), reached only from
  `GInterface::PlaceObjectInMagicHand` (0x5DA7C1 → 0x5DC330 → 0x5DC2A0), which every hand's pick-up goes through
  (`GInterfaceStatus::PlaceObjectInMagicHand` 0x5DC870 too); it is cleared as the thing leaves the hand
  (`GMagicHand::RemoveFromHand` 0x5FB105, `Object::ThrowObjectFromHand` 0x638645, 0x5FB07C from
  `GInterface::ApplyChangesToHand`); creatures hold things otherwise. Bit 0x8 has no immediate store anywhere in the exe
  (a scan of every decoded function for `or`/`and`/`mov` of an immediate at +0x24; bw1-decomp's
  `Villager::SetAge` `Flags |= 8` is the villager's own +0xE0 in the exe, 0x75291B), so on a fresh land nothing reachable
  sets it; else it goes into the player's hand
  (`GInterfaceStatus::PlaceObjectInMagicHand` 0x5DC870), whose answer is not looked at, and a field gives a food pile of
  its food instead (`Field::RemoveFood` 0x5295A0); done (0).
- **DropObjectInHandAtPos** (8): no start; a turn (0x653820) lets go of what the hand holds at the hand's point with no
  speed, through the interface's release (0x5DA400, the same one 0x652300 uses: the object's own release from the hand,
  vt +0x724 / +0x728, then `GInterface::ApplyChangesToHand`); done. Its move in front, MoveHandToPos, has taken the hand
  there first.
- **The release's second half**, for any object (0x5DA4AD..0x5DA4B6): vt +0x728 (`Mobile::ApplyThisToMapCoord`
  0x606BF0 for a scaffold: it throws it and returns 0x16) gives the code to `GInterface::ApplyChangesToHand` 0x5DA100;
  0x16 leads to fn_005CED60, which runs fn_005DC1E0 on the interface's own status (`[edi + 0x39C]`, 0x5CED64), and only
  then, for `MyInterface` alone (0x5CED7B..0x5CEDA5), `CHand::ThrowObject`. fn_005DC1E0 takes the object off the
  status's hand list (fn_005FB060: clears the in-hand bit 0x4, calls vt +0x704 with the status, then
  `FireEffect::SetOutMagicHand`), then records the status as the one that last dropped it: the status that had it
  before has its `LastDroppedObject` (+0x124) cleared (`Object::GetInterfaceStatusWhoLastDroppedMe` 0x63A710) and
  this one's is set (0x5DC27E..0x5DC28A). For a scaffold vt +0x704 is `Scaffold::InterfaceSetOutMagicHand` 0x6E9770:
  its first test is the status's player (`GInterfaceStatus::GetPlayer`, vt +0x1C, 0x5DBC20) and that player's type
  (+0x8E0, bw1-decomp `GPlayer::type`) not 2 (`PLAYER_TYPE_COMPUTER`) (0x6E9782..0x6E97A1); only then the surplus rule
  (a planned, gently dropped scaffold worth more than the plan needs makes a new scaffold of the surplus and puts it
  in that status's hand, `GInterfaceStatus::PlaceObjectInMagicHand` 0x5DC870, 0x6E9842). A player's type becomes 2
  only through `GPlayer::ToggleComputerPlayer` 0x64C400 (its computer god switched on), so a computer god's hand puts
  a scaffold down whole; a hand that moves for a player whose god was never switched on would keep the surplus.
  See [building-sites.md](../world/building-sites.md#leaving-the-hand-and-the-physics).
- ✅ **DropObjectInHandOnObject** (24, read 2026-10-28): its start (0x654C60) and end (0x654D00) do nothing (done);
  its turn (0x654C70) gives the object of its first argument what the hand holds through the player's leader
  interface, the apply-to-object handler of the player's own hand (0x5DA1A0, packet 0x11: the target interactable, a
  held object that is interactable and `ValidToApplyThisToObject` (vt +0x71C) for it, then its `ApplyThisToObject`
  (vt +0x720) and `GInterface::ApplyChangesToHand`); not valid, nothing, and the object stays held; done either way.
  Its move in front is MoveHandToObject. No random draw.
- ✅ **CreateFoodFromContainer** (29) and **CreateWoodFromContainer** (30) (read 2026-10-28): no start and no end;
  their turn (0x655150 / 0x655160 → 0x655170 with the resource 0 or 1): the first object in the leader's hand counts
  only when it is a pot (vt +0x4B4) of the resource (vt +0x690); with no space in the hand (`IsSpaceInHands`
  0x5DC2F0: it holds something) and no such pot the stage goes on (2). The amount is the second argument's number
  (+0x5C), less what the pot holds (vt +0x98); when it is not below what the container (the first argument's thing)
  holds (vt +0x98, signed) it is that. Above 0: the container gives it up (vt +0xA0 with the resource, the amount, 0,
  0), and the pot takes it (vt +0x9C) or a new pot is made at the container's place (vt +0x170) with
  `Pot::Create(place, GPotInfo::InfoList[12 food / 11 wood] (HAND_FOOD / HAND_WOOD), amount, 0, container vt +0x48,
  0, 0.0, 1.0, 0)` (0x66CF10) and put in the leader's hand (`GInterfaceStatus::PlaceObjectInMagicHand` 0x5DC870).
  Done (0) with a pot, else the step fails (1, "Failed to create food from container"). Their move in front is
  MoveHandToObject. No random draw.
- ✅ **The town of a new pot** (read 2026-10-28): `Pot::Create` passes its town to every class it makes (`PileFood`
  0x66E070 → `PileResource` 0x66EC70 → `PotStructure` 0x66D910 → `Pot::Pot` 0x66CE60, which keeps it in `ParentTown`
  +0x6C). Nothing in play reads it: the only accesses of +0x6C in every pot class are the constructors' writes,
  `Pot::Save` 0x66D730 (written) and `Pot::Load` 0x66D820 (read back) (`xref.py field` over Pot, PotStructure,
  PileResource, PileFood, PileWood, MagicFood, MagicWood and PuzzleGrain; `gx.py grep ParentTown`). Only the save
  reads it. `Pot::Create` has 23 callers (`xref.py callers 0x66CF10`), each with its own town.
- ✅ **Wait** (23, read 2026-10-28): its start (0x654BE0) keeps in the action state (+0x44) the turns of its first
  argument's seconds, `_ftol(1000 / GGameInfo +0x40 × seconds)` (10 × seconds, cut toward zero); its turn
  (0x654C40) counts one down and goes on (2) until none is left (0); no end (done). A wait of 0 s counts below zero
  and goes on. Oracle (2026-10-28, the start and turn run in the exe): 1 s 10 turns, 0.5 s 5, 2.5 s 25, 3.7 s 37,
  0.33 s 3, 0.25 s 2; 0 s still going after 40 turns, at −40. No move in front, no random draw.

The oracle's answers for these turns are pinned in `test/test_computer_hand.cpp` (`ComputerHandMoves`).

### The miracle steps

✅ Read from the exe (2026-11-02). The stage functions come from the table's initialiser (0x64F178..0x64F1BD for
step 15, 0x64F67B..0x64F6F6 for 22, 0x64FCB5..0x64FD26 for 32).

- **MakeGesture** (22; start 0x6549D0, turn 0x654A70, end 0x654B50, time 0x655980; no move in front). Its arguments
  are the miracle and a whole number; the gesture is 0x650AE0(miracle, n): the miracle's seed (`GMagicInfo` +0x28,
  the seed type) gives its seed info's +0x100 for n = 0 (file +0xF0, the selection gesture) and +0x104 otherwise
  (the gesture). The town's casting action asks for n = 0. **Start**: the gesture's origin (+0x7C) is the hand's
  point (+0x50), its progress (+0x78) 0, and the hand goes to 0x650A70(gesture, 0): the shape's point at t
  (`fn_0068C650`, the gesture's shape; `sub_68C1D0`, the point at t) is drawn upright from the origin, x =
  shape x × 30 + origin x, height = shape z × 30 + origin height (the shape's own height is not read), z the
  origin's (the float 30 at 0x8BF51C, each operation rounded at the 24-bit FPU); done. **Turn**: progress +=
  1 / (ratio × 30) with ratio = `fn_0068C110` (the shape's length +8 over the circle's, gesture 4) and the 30 at
  0xC00A1C (in .data, written by nothing: `xref.py writers` 0); a progress of 1 or more is kept at 1 and the turn is
  done, the hand staying where it was; else the hand goes to the shape's point at the progress, going on (2).
  **End**: the hand goes back to the origin, and `UR_GesturingRecognised::FakeGestureOnLandscape` lays the gesture's
  sparkles there, 30 across, for the god's leader interface (its own row); done. No random draw.
- **MakeWorshippersChantForSpell** (32; start 0x655470, turn 0x6554D0, end 0x655600, time 0x655990; no move in
  front). Its arguments are the miracle and the icon. **Start**: the count (+0x44, the one a wait counts down) = 0;
  done when the icon (cast to `SpellIcon`) has a worship site (vt +0x30C `GetWorshipSite`, 0x77F0A0 for a site's
  icon: its +0x118; a town's icon its town's site), else failed (`neg; sbb; inc`). **Turn**: the count goes up one;
  the icon as a `WorshipSpellIcon`, else failed; `IsCharging(null)` (0x77F6D0: with no status, charging for anyone)
  → done; else `ValidForStartCharge(leader status, power-up, 1)` → `StartCharge(the same)` and done (its answer not
  read); else no site → failed; else the count over the turns in a second (`1000 / GGameInfo +0x40`, unsigned whole
  numbers) as a 64-bit whole number against 5.0 (0x8AB6E4): above it failed, else going on. The power-up is the
  miracle's `GMagicInfo` +0x30 (its power-up level as the load fills it, −1 for a base miracle; openblack
  `magic::GetPowerUpLevelOfMagicInfo`). **End**: the icon as a `WorshipSpellIcon`,
  else failed; `GetChantNeededForSpellSeed` 0x77FE40 at or below 0 → done; else the count goes up one, and above
  30.0 seconds (0x8C36A8) failed, else going on. No random draw. A computer player's charged icon puts no seed in a
  hand (`ValidForPutFullyChargedSeedInHand` asks for a human player), so its charge stays in the icon's store.
- **CastSpellAtObject** (15; start 0x654230, turn 0x6542A0, end 0x654450, time 0x655990; MoveHandToObject in front,
  MoveHandNearObject for an angry miracle, below). Its arguments are the thing, the miracle and the icon.
  **Start**: 0x6669D0(miracle), then the action state's +0x74 = the thing (read only by the save, 0x6519E7); done.
  0x6669D0 would untie the player's leash (leader status +0x12C, its +0x24 = 0) when the hand is within the
  creature's size × 10 + 200 of the creature, but only when the creature's free time (0x658BC0) is below
  0x6588F0's `(1 − r) × free time` (r = how often it has seen the miracle over how often it must, at most 1, else
  1): with r from 0 to 1 and a free time from 0 to 1 that is never so, so it never unties
  ([not in the original](../project/not-in-the-original.md#rival_godsai)). **Turn**: the thing as an `Object`;
  unless the miracle's effect info (+0x98, `perceivedPlayerDesire[0]`) is 2 (anger), the hand follows the thing
  once (0x653E70, below); then fn_0070BB20(the object, the miracle, the hand's point as a `MapCoords`, the icon,
  check 1, radius 1.0, the effect's +0x70 `timerWhenComputerPlayerCasting`; the curl and the direction are the
  0 and the zeroed vector left on the stack by the call to 0x5FB7D0, which pops nothing): no spell when the icon,
  a worship site's, still needs chants (`GetChantNeededForSpellSeed` > 0), or the class's check on the object (vt
  +0x2C) refuses; castData {1.0, the effect's +0x74 `initialChants`, the time, −1}, the process info {from, the
  object's place − from, no direction, power 1, curl 0}, the cast on the object (fn_005FB520, the icon its
  creator), and the icon's store less what it needed (`RemoveFromChantStore(GetChantRequiredForSpellSeed)`). A
  spell: the computer player keeps it at the head of its casts (0x665CF0, `GComputerSpellCast` with the turn), the
  count +0x44 = `_ftol(10 × the effect's +0x118 computerCastDuration)`, done; none: an error text into +0x8C and
  failed. **End**: the count goes down one; at 0 done, else the hand follows the thing and goes on (a count of 0
  goes below zero and never ends). No random draw.
- **Following a thing** (0x653E70): the thing's place (`GetInteractPos` vt +0x170, a `GameThingWithPos`'s own
  place, 0x4019F0) is the target, its height `GetHeight` (vt +0x42C) plus the place's own height above the land;
  the way across the land, normalised when not 0; the step the speed's over the turns in a second, no more than
  the distance, then 100 when the distance is over 300 and 10 when over 100; then the hand is held 2.5 (0x8C581C)
  of the thing's heights over the land at its point (`(x × 65536) × 0.1` cut toward zero). It does not say whether
  it got there.
- **The nearer move** (0x65040D..0x650460): for CastSpellAtObject (15) or CastSpellAtPos (16) whose second argument,
  the miracle, is from 0 to 41 and whose effect info +0x98 is 2, the move put in front is MoveHandNearObject (5) or
  MoveHandNearPos (4) instead.

### Forced and queued actions

- **The tree.** The decision player (+0x2C, 0x656880) is a fixed tree built from static rows of 0x78 bytes: the root
  "Master Decision Tree" (0xD47F68) with six children (0xD4BA30 + n × 0x78), then three more levels (0xD4B148,
  0xD497F8, 0xD48088), each row naming up to eight children by index at +0x58 (-1 ends them; `GetNumChildren` 0x655B70).
  A row: +8 its name ("Name: line "), +0x18 its weighing member function, +0x38 its action member function. Every
  row and its fields are in [the computer players' minds](computer-minds.md#the-rows).
- **The lookup** (`CPDesireNode::FindDesireNode` 0x6640A0): from the root, each node before its children, the first
  node whose name up to its last ':' is how the name asked for begins (`strncmp` over the node's length). openblack's
  table of the 117 names in that order was read out of the static initialisers.
- **FORCE** (`GScript::ForceComputerPlayerAction` 0x6EEE70) POPs the second thing, the first, the name and the player,
  then `GComputerPlayer::ForceAction` (0x663BE0, weight 1.0). No node: nothing. A name beginning "DIRECT" (`_strnicmp`
  with 0xC01C10, 6 letters): the node keeps the first thing (+0x94) and the second (+0x34) and its action runs at once
  (0x656170, which first gives up the hand's steps; true sets the busy flag). Otherwise, or when that failed, a second
  thing that is a `ScriptMarker` goes with its point to 0x663EA0, any other to 0x663DA0: the node, or its first child,
  is given the things and the chosen node (+0xA8) runs (0x6563E0 → 0x656080, which climbs to the first node with an
  action).
- **QUEUE** (0x6EEF20) keeps the name and the two things in a `GComputerPlayerQueue` at the end of the computer
  player's list (0x664420, 0x664390); `ForceNext` (0x664480) forces the first and deletes it whatever came of it, when
  the hand has no steps and no script holds it. Each turn an entry whose thing is gone is deleted (0x6644D0).
- **FindPlaceToUseScaffoldForBuilding** ("…: 113", row 0xD4AE00, action 0x669A30, its one child
  "FindLocationToUseScaffold: 181" with no action): with the marker the point goes to the child and the node runs its
  action: the first thing (any thing with a place) is picked up (0x658AA0: refused by the bits above, else PickUpObject
  with the player's speed as a float) and dropped at the child's point (DropObjectInHandAtPos). The story forces it on
  Khazar with the scaffold and a marker (GiveScaffold, GiveOtherScaffolds).
- **DIRECTPickupAndDropObject** ("…: 182", row 0xD494B0, action 0x664790): the thing nearest above the node (0x655CB0,
  the node's own first) must be an `Object` and the node's second thing (+0x34) a `ScriptMarker`; the same pick-up,
  then the drop at the marker's place.
- ✅ **FindPlaceToUseScaffoldForBuilding**'s action (0x669A30) reads the thing nearest above the node (0x655CB0) as a
  `GameThingWithPos` and the point (+0x84) of the node it chose (0x655CD0: its chosen node unless that is itself). A
  second thing that is not a marker (0x663DA0) makes the child the choice without a point, so the drop is at the
  child's point as it was: (0, 0, 0) until a marker gave it one, the nodes being made zeroed (`calloc` 0x7DB790).
  openblack does the same. ShowDisciple forces it on Khazar with a
  new villager.
- **ShallCastWoodSpell**: the first node of the name is "ShallCastWoodSpell: 132" (row 0xD483D0, action 0x6678C0,
  under "TownWithHighestWoodDesire: 84"); the one under "GiveBuildingSiteWood: 86" (": 136", 0x668480, which needs a
  building site) is never found first. LearnGestures queues it four times on the town and the wood icon. Its action:
  the icon (`WorshipSpellIcon`, the node's thing) and the town (its parent's); the town's store for the miracle's
  resource (`Town::GetResourceDropObject`, wood for magic type 21); within 200 m of the icon the hand moves to it,
  farther it makes the miracle's gesture (0x665DD0); the worshippers chant (MakeWorshippersChantForSpell); the miracle
  is cast at the store (0x665FC0, CastSpellAtObject).
  - **Whether it runs from a script.** A script alone never makes it run. QUEUE (0x6EEF20) → NewProcess 0x657420 →
    ForceNext 0x664480 → ForceAction 0x663BE0 → FindDesireNode → 0x663DA0, which writes the node's +0xA4 = 1.0,
    its parent's thing (+0x94) = the town and its own = the icon, then runs the node's chosen node (+0xA8) through
    0x6563E0. With +0xA8 = 0 that returns 0, and nothing falls back. ForceAction returns false and ForceNext
    destroys the queue entry anyway. Oracle runs on the exe's own code with a made-up node: +0xA8 = 0 → the action
    is not called; +0xA8 = the node → it runs once.
  - **Who sets +0xA8.** Every write to +0xA8 in 0x64E000..0x66D000 was scanned. The script paths set it only on
    nodes with children. For a leaf, the only write is the mind's weighing 0x655DF0 (0x655E42), which sets it to
    the node itself whatever the score, after which it stays set. Only the god AI's own step weighs: the whole tree
    (0x656930, from NewProcess) and each top-level node while the hand is busy and not held (0x656980 → 0x656010).
  - **Khazar's mind is on in Land 2.** Land2.txt's `TOGGLE_COMPUTER_PLAYER("PLAYER_TWO", 1)` sets cp+0x1B8 through
    0x64C400, and LearnGestures starts only after ExpansionCheck's 1200 s, so his mind has weighed for 20 minutes
    by then. Even without that, the drain weighs the whole tree on the second failed entry's turn, so at least
    casts 3 and 4 would run.
  - 🔍 **Whether the weighing reaches node 132.** It does only down root → TownInteract:15 → AttendTownDesires:32 →
    TownWithHighestWoodDesire:84, under the sibling gate 0x655EB0: a later sibling is weighed only while the
    parent's personality × suppression is at least the best score so far, and a node scoring 0 skips its children.
    It also needs a town of Khazar's with a storage pit wanting wood (the three fields at Town +0xC4 / +0x108 /
    +0x14C, index 1). These are values of Khazar's own state, which only a memory read of node 132's +0xA8 in the
    running original at LearnGestures can settle.
  - ✅ **So the action is reachable, and ported** (2026-11-02): it runs whenever a weighing has reached node 132
    before the queue is drained, since each weighing of a node with a score sets it as its own choice again
    (0x655DF0), and no shipped script changes a weight on its path (the 25 personality calls of the story's scripts
    name other nodes, as the GetWoodFromOtherTown entry of [the minds](computer-minds.md) reads). openblack weighs
    the same tree with the same gates and scores, so the action (0x6678C0) is ported as the original's
    (`computer_hand::CastIconsMiracleAt`, [the minds](computer-minds.md)), and whether Land 2's four casts come is
    what openblack's own weighing of Khazar's mind gives.

### The script's natives

✅ Read in runblack.exe v1.42 with the xref index (2026-10-26). Each POPs the script player last, as a float cut
toward zero (`__ftol`), turns it into the game's (`ConvertScriptPlayerToGamePlayer` 0x6EB9A0: 0 the player in the
neutral slot, n the game player n − 1) and takes `GGame::GetPlayer` 0x5509B0 (none from 8 up, unsigned), then the
player's computer player (`GPlayer` +0x944).

- **311 `ENABLE_DISABLE_COMPUTER_PLAYER`** (`GScript::SetComputerPlayerOnOff` 0x6EECC0): POPs the player, then the
  switch, and writes the switch as it is to +0x1B8, nothing else (see
  [the player's creature](../creature/players-creature.md#a-computer-players-start-on-the-land)). No player, or no
  computer player: a script error (0xBE01C4, 0xC0C2EC) and nothing.
- **313 `SET_COMPUTER_PLAYER_POSITION`** (`GScript::SetComputerPlayerPos` 0x6EE9D0): POPs the flag, z, y, x, then the
  player. No player: nothing, without a message; no computer player: a script error (0xC0C2D0). A flag not 0 calls
  0x658410, 0 calls 0x658490; both, with the action state (+0x34): give up the steps (0x6587C0 → 0x650250: 0x650170
  empties the line, +0x38 the stage = 0, +0x30 = 1, +0x34 = 0, then lets go 0x652300 when the hand holds something,
  vt +0x1C), the steps' speed (+0x68) = the player's (+0x58), let go again (0x652300), one step 34
  (`SetHandToPos`, held above the ground) or 36 (`SetHandToPosDirectY`, at the point's height) with the point
  (0x651120, 0x650280), the action state's time estimate (0x6507D0), busy (+0x1F8) = 1 and held (+0x1C0) = 1.
- **370 `RELEASE_COMPUTER_PLAYER`** (`GScript::ReleaseComputerPlayer` 0x6EEFD0): with a player and its computer
  player, `GComputerPlayer::Pause(0)` 0x658610: paused (+0x1BC) = held (+0x1C0) = 0 and `SetSpeed(+0x58)` 0x6583F0
  (the player's speed, and the action state's +0x68). The steps and busy are left as they are. No message either way.
- **374 `CALL_COMPUTER_PLAYER`** (`GScript::GetComputerPlayer` 0x6EE840): with a player and its computer player,
  `AddScriptGameThing(the computer player, 0)` 0x70D0F0 (a found thing, not the script's creation) pushed as an
  object (type 4); no player: a script error (0xC0C2B0) and 0; no computer player: the errors 0xC0C2D0 and 0xC0C2B0
  and 0.
- **454 `GAME_CLEAR_COMPUTER_PLAYER_ACTIONS`** (`GScript::ClearComputerPlayerActions` 0x6EF2F0): with a player and
  its computer player, 0x658B30: the steps given up and the hand let go (0x6587C0, as above), every queued action
  deleted (its vt +0xC, unless bit 1 of its +0xA) and the queue's head and count (+0x50, +0x54) = 0. Held, busy and
  the speeds are left as they are. No message either way.

## openblack

- `ecs::components::ComputerHand` (`ECS/Components/ComputerHand.h`) on the player's entity holds the hand's point, the
  speeds, the map place, the steps (`HandStep`, `HandSubAction` with the original's numbers), the first step's stage,
  the two flags of the move in front, the held and busy flags, what it holds and the queued actions.
- The miracle steps ([above](#the-miracle-steps)): MakeGesture as `computer_hand::GestureStage` (`StartGesture`,
  `GestureTurn`, `EndGesture`, `GesturePoint`), the gesture from the seed the game gives the miracle at load (`computer_hand::GestureOfMiracle`, through
  `magic::GetSpellSeedOfMagicInfo`: info.dat's rows name no seed, 2026-11-02) and its shape (`GameHandRunner::GestureOf`,
  `magic::gestures::ShapeOf`, the point at t `ShapePointAt`); the sparkles at its end are not drawn yet (Pending).
  Tests `ComputerHandGesture.*`.
  MakeWorshippersChantForSpell as `computer_hand::ChantStage` (the icon's charge through `worship::icon`:
  `IsCharging`, `ValidForStartCharge`, `StartCharge`, `GetChantNeeded`, for the hand's player). Tests
  `ComputerHandChant.*`.
  CastSpellAtObject as `computer_hand::CastAtObjectStage` and `FollowThing`, the cast through
  `GameHandRunner::CastOnObject` (`magic::cast_rules::CanCastOn`, `magic::CastAtObject` with the icon as creator)
  and the mind's casts (`ComputerMind::spellsCast`, the thing's place kept with it). The cast always has the move to
  the object in front and follows it: the angry miracle's nearer move is not ported yet (Pending). Tests
  `ComputerHandCast.*`.
- `computer_hand::` (`ECS/ComputerHand.h`): the table of moves in front (`Prerequisite`), `IsReady`, `Append`, `Clear`,
  `SweepGone`, `KeepMapPlace` and `Step`, the original's turn of the steps, over a `StageRunner` that the game gives the
  stages through. Tests: `test/test_computer_hand.cpp` (group `test_magic`).
- ✅ The god's sparkles and symbol ([above](#the-gods-sparkles-and-symbol)): `computer_hand::Step` keeps the hand's
  point as its reported camera (`ComputerHand::reportedCamera`) at the end of each turn but one that only put a move
  in front; before it the camera is the one over the god's citadel (`player_sparkles::CameraOverCitadel`, through
  `worship::citadel::Of`). Each turn, at the start of the dead list's step (`Game.cpp`, after the hand's turn),
  `ecs::other_player_sparkles::AddForTurn` (`ECS/OtherPlayerSparkles.h`) walks the players as the original does and
  adds one sparkle to each showing god's own pool in the dynamics system (`AddPlayerPuff`, `SetPlayerPuffPoint`; at
  most 1024, moved on with the dust, emptied with it at a new map and by `TempleInterior::Activate`), drawing
  `LocalRand(7)`, `LocalRand(2)` and the pool's CRT `rand`, as the original: the local seed and the CRT seed move on
  every turn a computer god's mind plays (Land 2 on). The renderer draws each pool as one Z object at its point, the
  puffs as the dust, then the symbol (`ecs::other_player_sparkles::SymbolShown`) turned by
  `player_sparkles::TurnSymbol` outside the reflection, from the players' symbol sheet (`3D/PlayerSymbolSheet.h`, the
  texture cache's `player_symbols/sheet`): made empty once a run, and at each new game's first land
  (`Game::StartNewLand` with a new game) each player the computer plays then gets its god's image
  (`player_symbol_sheet::MakeComputerSymbols`, logged "Player symbols: player N takes Lethis.cps"). openblack does not
  write the two files back. The pure rules are `Particles/PlayerSparkles.h`; tests
  `test/test_player_sparkles.cpp`, `test/test_player_symbol_sheet.cpp` and
  `ComputerHand.ItsCameraIsReportedWhereEachTurnOfStepsLeavesIt`. The debug
  window Computer hands lists each god's camera, its sparkles and its symbol; the log line `Player sparkles: player N
  shows at (x, y, z)` marks each pool that starts to show. Testbed scenario `hand.rival_god_sparkles`.
- The natives (`src/CHLApi.cpp`) go through `Locator::computerPlayerSystem` (`ComputerPlayerSystem`): 310
  `MOVE_COMPUTER_PLAYER_POSITION` → `MoveHand`, 313 `SET_COMPUTER_PLAYER_POSITION` → `PutHand`, 312
  `GET_COMPUTER_PLAYER_POSITION` → `HandOnGround`, 344 `COMPUTER_PLAYER_READY` → `IsHandReady`, 371
  `SET_COMPUTER_PLAYER_SPEED` → `SetHandSpeed`, 370 `RELEASE_COMPUTER_PLAYER` → `Release`, 345 → `Pause` (which now
  holds the hand too), 374 `CALL_COMPUTER_PLAYER` (the player's entity as a found thing) and 454
  `GAME_CLEAR_COMPUTER_PLAYER_ACTIONS` → `ClearActions`; their pure parts are `computer_hand::StepToPoint`,
  `TakeForScript`, `Pause`, `SetSpeed`, `MovedByScript`, `TakesItsTurn` and `OnGround`. The hand's component is made the
  first time a native gives it something.
- ✅ As the original ([the script's natives](#the-scripts-natives)): 311 pops the player, then the switch, and
  `SwitchOn` writes only the mind's switch; 313 pops the flag, the point and the player, and `PutHand` gives up the
  steps, lets go, takes the player's speed and one step 34 or 36, held and busy; 370 `Release` unpauses the mind,
  lets go of the hold and gives the steps the player's speed, leaving the steps and busy; 454 `ClearActions` gives up
  the steps, lets go and drops the queue, leaving held, busy and the speeds; 374 gives the player's entity (openblack's
  computer player) as a found thing. Tests (`test/test_script_query_natives.cpp`):
  `ScriptQueryNatives.EnableDisableComputerPlayerPopsThePlayerThenTheSwitch`,
  `ScriptQueryNatives.SetComputerPlayerPositionPopsTheFlagThePointAndThePlayer`,
  `ScriptQueryNatives.ReleaseAndClearComputerPlayerActionsAskForTheGamesPlayer`,
  `ScriptQueryNatives.CallComputerPlayerGivesThePlayersEntityAsAFoundThing`,
  `ComputerPlayerScripts.SwitchingTheMindOnOrOffLeavesThePlayersType`,
  `ComputerPlayerScripts.PuttingTheHandGivesUpItsStepsForOneSetStepAtThePlayersSpeed`,
  `ComputerPlayerScripts.ReleasingTheHandUnpausesTheMindAndGivesTheStepsThePlayersSpeed` and
  `ComputerPlayerScripts.ClearingTheActionsGivesUpTheStepsAndTheQueueButNotTheScriptsHold`.
- Its turn: `ComputerPlayerSystem::ProcessTurn`, in the players' turn, sweeps every hand for gone things, then for a mind
  that is on, not paused and not held by the wide screen (which a hand moved by a script passes) starts it on the land
  once and runs a turn of its hand's steps while it has some or a script holds it. The stages are the game's
  (`GameHandRunner`): the moves above; a step not yet ported fails, so the hand gives up its line.
- The debug bar's **Computer hands** window (`src/Debug/ComputerHands.cpp`) shows each hand: its mind's switches, held,
  busy and ready, where it is and goes, its speeds, its steps and queue.
- A thing already held by the player's hand or by a computer hand may not be picked up (`computer_hand::MayBePickedUp`,
  the original's bit 0x4); bit 0x8 is set by nothing a fresh land reaches.
- Holding: `computer_hand::PickUpTurn` and `DropTurn` over the runner's `CanBePickedUp`, `TakeIntoHand` and `LetGo`.
  In the game (`GameHandRunner`) a thing goes into the computer player's hand by the steps the player's hand takes
  (out of the physics, `villager_physics::InterfaceSetInMagicHand`, `object_physics::ArtefactTaken` for the computer
  player, out of the map's cells); while held it stays out of them, as the player's held object does (the turn's
  refiling skips whatever a computer hand holds, `map_cells::lists::ReadFilter`, 2026-10-28: the original takes it out
  of the map at the pick-up and nothing files it again before it is let go; without this a scaffold set to clear its
  way, `Scaffold::DestroyThingsInWay` 0x6EAB90, found itself in its own cells as Khazar's hand let it go, pushed itself
  aside and planned no building); while held it is carried at the hand's point each turn; let go, it is put at the
  hand's point and released with no speed (`physics::from_hand::Throw`, not a forced drop, so a scaffold put down
  gently can build), as the computer player's own throw (`computer_hand::LetGoCreditedTo`: the original lets go
  through the player's interface, `[edi + 0x39C]` at 0x5DA496..0x5DA4A7, whose `ApplyThisToMapCoord` (vt +0x728)
  calls `Object::ThrowObjectFromHand(status, ...)` 0x6385E0, which hands it to `InitialisePhysicsFromHand` 0x6386F1).
  Every runner knows its hand's player, the script's moves and clears too. The throw clears the new body's drag when
  a script took the hand's player's resistance to wind away (`ecs::wind_resistance::AfterThrowFromHand`, 2026-10-22,
  as `Object::ThrowObjectFromHand` does for every hand; see
  [Holding and throwing in openblack](grab-hold-and-throw.md#holding-and-throwing-in-openblack)). After the throw, a scaffold's own part of
  leaving the hand (`scaffolds::OnLeftHand(object, player)`, as the player's hand does it): what it leaves over goes
  into the same hand (`computer_hand::TakeLeftOver`, through `TakeIntoHand`); a computer player's hand leaves nothing
  over (see the release, above). Then the computer player's interface keeps it as the last thing it let go of
  (`ecs::last_dropped::RecordDrop`, fn_005DC1E0's record: the hand that kept it before forgets it, see
  [the last dropped object](grab-hold-and-throw.md#the-release)). Tests
  `ComputerHandHolding.WhatALetGoLeavesOverGoesBackIntoTheHand`, `BuildingSitesTest.AComputerPlayersHandPutsAScaffoldDownWhole`
  and `BuildingSitesTest.AHandOfAPlayerNotYetAComputerPlayerLeavesTheSurplus`.
- Forced and queued actions: `computer_hand::FindMindAction` over the mind's node names, `IsDirectAction`,
  `ForceAction`, `PickUpAndDrop` (the two leaves' steps), `QueueAction`, `SweepQueue` and `TakeNext`, through
  `ComputerPlayerSystem::ForceAction` / `QueueAction` (natives 366 and 367). A force goes through the player's mind as
  the original's does (`computer_player::Force`, see
  [the computer players' minds](computer-minds.md#forcing-an-action-through-the-nodes)): the nodes keep the things, the
  point and the worth, and the node chosen runs; the hand's actions are `computer_hand::HandActions`, over a
  `ThingFacts` seam the game answers (`script_type::IsObject`, a `ScriptMarker`'s place). A free hand takes its next
  queued action in its turn. Six nodes' actions are ported (`HandActions::HasAction`): FindPlaceToUseScaffoldForBuilding,
  DIRECTPickupAndDropObject, TakeVillagerFromTownToWorshipSite (200970fe8), GetFoodFromOtherTownForObject (1b17c531c),
  GetWoodFromOtherTown (4d6e71a67) and GetFoodFromOtherTownForStoragePit (af2bba89e), each in
  [the mind's turn](computer-minds.md#openblack), tests in `test/test_computer_hand.cpp`; every other
  node's action is not run (with a warning), and a force that would weigh a way of scoring openblack lacks does
  nothing.
- The held object's draw point: each turn of steps keeps the hand's point as it began (`ComputerHand::turnStartPoint`,
  counted by `stepTurns`); `computer_hand::NewTurnsOfTheHand`, `SlideOfHeld`, `HeldDrawPoint` and `OnTheMapGrid` are the
  original's slide, with tests.
- The draw: `RenderingSystem::BeginPrepareDraw` takes, for each computer hand that holds a thing, its draw point this
  frame (the slide, then through the map grid), and the row of that thing is written at that point, turned and scaled
  as it is (`WriteEntityRow`); like the player's held object it casts no static shadow (it is out of the map's cells,
  and the blocks' bake reads only theirs, `land_prints::ObjectsOverBlock`, `LandPrints.cpp`). Draw only: nothing in the game's state changes. No hand, symbol or colour is drawn for it, as the original. The **Computer
  hands** window shows what each hand holds and where it is drawn.
- The steps that give and take (2026-10-28): `computer_hand::GiveHeldTurn` (24), `TakeFromContainerTurn` (29, 30)
  and the wait (`WaitTurns`, `StartWait`, `WaitTurn` over `ComputerHand::waitTurns`, 23), over the runner's
  `GiveHeldTo`, `ResourceOf`, `IsPotOf`, `RemoveResource`, `AddToPot` and `MakeHandPot`. In the game the give is the
  player's hand's apply to an object (both interactable, `held_apply::ValidToApplyThisToObject` and
  `held_apply::ApplyThisToObject` with the computer player's interface; leaving the hand, the fire's
  `SetOutMagicHand`), for the classes a store takes (a held thing of another class logs that it is not ported); the
  take is `object_resources::GetResource` / `RemoveResource` / `AddResource` and a new HAND_FOOD / HAND_WOOD pot
  (`PotArchetype::Create`) at the container's place, taken into the hand. Tests `ComputerHandHolding.Giving*`,
  `ComputerHandHolding.TheGiveWaitsForTheMoveToTheThing`, `ComputerHandContainer.*` and `ComputerHandWait.*` (the
  oracle's turns).
  What the port covers: the give is for a held thing a store takes (`held_apply::KindOfHeld` not `Other`), which is
  all a ported action can hold at a give: GetFoodFromOtherTownForObject's give comes after its take, which waits
  while the hand holds anything but a pot of food and ends only once it has made one; another class logs that it is not
  ported. The new pot keeps no town: only the save reads the original's (above). openblack's saved-game writer
  writes a pot loaded from a save from the record its load kept, town included; a pot made since the load waits for
  the pot and pile writer, which ports the town at the 23 creation sites as its first reader.
- The moves: `StepLength`, `HoverOverGround`, `PointFromMap`, `StartTowards`, `MoveOverGround` (1), `MoveStraight`
  (35), `MoveToThing` (2) and `PutAt` (34 / 36), in float as the game's 24-bit FPU, over a `Ground` seam; their tests
  are the original's own numbers.

## Pending

- The players' symbol sheet: the local player's cell (the profile's tattoo rebuild) and a network person's are not
  made, and the low-resolution images are not used (openblack draws at full detail); none of them shows in the
  other gods' sparkles.
- 🔍 A god whose mind plays with no citadel and no turn of steps yet: what its interface status reports as its camera
  (+0xB0, not set by `GInterfaceStatus::SetToZero` 0x5DBA00) is not read; openblack shows no sparkles for it and draws
  nothing from the random streams. Nor does openblack keep the camera a destroyed citadel left for a god whose hand has
  had no turn of steps.
- A computer player's gesture sparkles on the land (`FakeGestureOnLandscape` 0x689440 from the gesture step's end
  0x654B50): not drawn yet; its own row.
- The actions of the other nodes of the mind; among the story's: GetCreaturetoHelp, TakeOverTown, DestroyTown and
  DefendTown (Land 5).
- An angry miracle's cast (effect info +0x98 = 2): the nearer move put in front of it (MoveHandNearObject, 5, or
  MoveHandNearPos, 4), those two moves' stages, and the cast's turn not following the thing first. It comes with
  the first angry-cast action; no action openblack has gives such a cast yet.

- A field picked up by a computer hand gives no food pile yet (the original's `Field::RemoveFood` branch).

- (approximate) A thing's height above the land is taken from its height in the world, as openblack keeps it
  (`GameHandRunner::PointOfThing`).
- A god whose script let go of its hand waits: the original's mind then chooses its own actions
  ([How a computer god decides](../project/progress/rival_gods/ai.md)).

- The nearer move in front of the steps that cast at an object or a point (only the casting steps use it; none is
  ported).
- (approximate) The frame time of the held object's slide is counted on the frames whose draw lists are written
  (`RenderingSystem::BeginPrepareDraw`), not every frame: it only matters when the slide's two ends differ, which the
  original's turn of steps never leaves.
- 🔍 The action state's time estimate that a script's put of the hand starts (0x6507D0: the turn into +0x3C, a sum
  into +0x40, see [the computer players' minds](computer-minds.md)) is not kept by openblack; its readers are not
  traced, so whether anything sees it is not known.
- 🔍 `CALL_COMPUTER_PLAYER` for a player that has no entity in openblack: the original gives that player's computer
  player whenever `GetPlayer` gives the player (every game player 0..7, the one in the neutral slot for script player
  0), where openblack gives 0. Whether the original's players without a land entity (the neutral one) have a computer
  player made by `GPlayer::Init` is not read, and no shipped script asks for one.
- 🔍 Where the hand is before a script first places it (the action state's `SetToZero` 0x6500C0, the map place
  (128, 128) at +0x14).
