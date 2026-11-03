# Launch switches and stray files

The original game's command-line switches, the alternative settings file that can point it at other challenge
scripts, and the files it quietly writes about the player's creature. openblack's own switches and tools are
listed in [openblack's debug controls](../../not-in-the-original.md#openblacks-debug-controls-kept-exempt-from-removal); online services (the creature web site, the lobby)
are in [../pc_integration/](../pc_integration/) and [../multiplayer/](../multiplayer/).

**Progress: 2/12 done, 0 chosen, 3 partial, 0 inferred — 28%**

## Command-line switches

Each switch starts with `-` or `/` and is matched by its start, in capitals, anywhere on the command line.

| Behaviour | Status | Where / notes |
|-----------|--------|---------------|
| `NEWGAME` skips the main menu and starts a new game at once | partial 60% | our tree's `--start-level` loads a land directly (`src/main.cpp`), but there is no main-menu flow to skip |
| `SKIRMISH` goes straight to the skirmish set-up box | pending | our tree has no such switch; `NEWGAME` overrides it, and it overrides `MULTIPLAYER`; see [../multiplayer/skirmish.md](../multiplayer/skirmish.md) |
| `PREINTROVIDEO` plays the video before the intro, which otherwise plays only while no player profile exists | partial | the pre-intro plays with our own `--pre-intro` switch (`src/main.cpp`), or while no player profile exists (`startup::PlaysPreIntro`, `src/Gui/StartupRules.h`, from `src/GameStartup.cpp`; test `Startup.PreIntro`); the switch is not the original's `PREINTROVIDEO`; see [../video/](../video/) |
| `VERSION` writes `mpver.txt` beside the game holding the program's checksum (the number multiplayer uses to match versions), then carries on | done | `-VERSION` (or `/version`, in any case, anywhere on the command line) writes `mpver.txt` in the working folder as the game starts: the sum of the program file's bytes as a whole number, nothing after it (`version_file`, `src/Game/VersionFile.cpp`, from `src/main.cpp` and `Game::WriteVersionFile`); tests `VersionFile.*`; Debug > Start-up and Loading > "Write mpver.txt now (VERSION)"; the sum is of openblack's own program, as the original sums its own ([the VERSION switch](../../../engine/loop.md#the-version-switchs-file)) |
| `NOLOADMUSIC` skips loading the music | pending | our tree has no such switch |
| `SETTINGS:<file>` reads that file (up to the next space) instead of the usual settings file | pending | our tree has no such switch |
| In that settings file, a line `QUESTDIR <folder>` makes the game load its challenge scripts from another folder, so a modded challenge file can be played without replacing the original | pending | our tree always reads `Scripts/Quests/challenge.chl` (`Game.cpp`); lines starting with `//` are comments in the game |

## Online: command-line switches

| Behaviour | Status | Where / notes |
|-----------|--------|---------------|
| `MULTIPLAYER` goes straight to the online lobby (ignored if `NEWGAME` came first) | pending | our tree has no such switch; no online lobby; online only: the switch (`ScanParameters` 0x641AEB) only sets a start mode that `pc_main` reads (0x641DFE) to open the internet lobby, whose service is gone |

## Files the game writes about the creature

| Behaviour | Status | Where / notes |
|-----------|--------|---------------|
| About once a minute of play (every 600 game turns), the game writes a web page about the player's own creature, `html/<creature name>.html` in the profile's folder, from the template `Scripts/creature_html.tmpl` | partial 75% | written at the end of the creatures' turn when the local player's creature's turns lived reach a multiple of 600, into the player's folder (`ecs::creature_web_page_writer::ProcessTurn`, `src/ECS/CreatureWebPageWriter.cpp`; `IsSaveTurn`, tests `CreatureWebPage.*`; Debug > Players > "Write the creature's web page now"); missing: no game run has yet shown it written at the 600th turn (a run of the checks has no player's folder); the conditions are on [the player's creature](../../../creature/players-creature.md#the-creatures-files) |
| The page, titled "Creature Cave", has a parchment frame and a floating spirit animation, and lists the creature's name, age, alignment, strength, health, energy, villagers killed and battles won; the template can also show mushrooms eaten, poos done, battles fought, creatures, animals and people killed, amount of poo, itchiness, thirst, tiredness, fatness, its beliefs, its desires and the miracles it has received | done | the template filled in as the original fills it, every value but the pictures' captions (`creature_web_page::FieldsOf`, `Fill`, `src/Creature/CreatureWebPage.cpp`); tests `CreatureWebPage.*`, with the game's data `CreatureWebPage.TheShippedTemplateGivesTheOriginalsPage` (the original's sisi.html byte for byte); the pictures it shows (`creatureshot_N.jpg`) are taken by the creature shots, not ported |
| At the same moment the creature's mind and body are saved to `Scripts/CreatureMind/<id>.erc` and `Scripts/CreatureMind/Physique<id>.erc` | pending | nothing saves the creature's mind once a minute in our tree; the mind and its physique are written when the player's creature is saved, as a land changes (`player_creature::SaveMyCreature`, `src/ECS/PlayerCreature.cpp`); see [../creature/saves_and_files.md](../creature/saves_and_files.md) |
| Ctrl+Alt+T writes `creaturesnapshot.csn` | pending | see [hidden_keys_and_cheats.md](hidden_keys_and_cheats.md) |
