# openblack wiki

How the original Black & White (v1.42) behaves, with addresses and formulas, and what openblack does.

- **Read it:** open `docs/index.html` through `docs/open_wiki.bat` (Windows), or browse [the pages](SUMMARY.md) here.
- **Start:** the [progress dashboard](wiki/project/dashboard.md) and [Reading this wiki](wiki/project/reading-the-wiki.md).

*This page is written by `tools/progress_stats.py`.*


<a id="where-we-are"></a>

## 📊 Where we are

**🎮 Features of the original**

![🟩🟩🟩🟩🟩🟩🟨⬜⬜⬜ 72%](wiki/project/bars/features-all.svg?v=4b2f63e6)

8009 of the original game's 12996 behaviours are done (5756 with a test, 166 proven in play, 83 of them both, 2170 with neither) and 7 done as the user chose; 2272 partial (10% of the rows' worth, 164 not measured yet); 181 inferred, not yet checked against the original. Online is parked: its 137 rows are counted apart, in no score.

**🔀 raffclar's work in our tree**

![🟩🟩🟩🟩🟨🟨⬜⬜⬜⬜ 51%](wiki/project/bars/raffclar-all.svg?v=136b0244)

332 of his 848 pull requests are in our tree, 203 more in part.

**🧹 Code clean-up**

![🟩🟩🟩🟩🟩🟩🟧🟧🟧🟧 6 of 10](wiki/project/bars/refactor-all.svg?v=d6a48625)

6 of the 10 code goals are reached; in 4 more, what is left stays on purpose.

<a id="features-of-the-original"></a>

## 🎮 Features of the original

Each behaviour of the original game is one row of a [progress file](wiki/project/progress/INDEX.md): done, chosen (done the way the user chose), partial with the share done, inferred (written from an inference, not checked against the original) or pending. The score counts done and chosen as one, a partial row as its share, inferred and pending as nothing. A done row is *with a test* when its notes name one of the tests, and *in play* when they cite a played or real-input run; a row with both counts in each column, and *neither* counts the rest ([the rule](wiki/project/progress/README.md#how-the-dashboard-groups-the-rows)).

🟩 done · 🟢 chosen (done the way the user chose) · 🟨 partial (the share done) · 🟪 inferred (not checked against the original; counts 0) · ⬜ pending

| Family | Progress | Done, with a test | Done, in play | Done, neither | Chosen | Partial | Inferred | Pending |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| 🌍 World | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 92%](wiki/project/bars/family-world.svg?v=622f5320) | 894 | 0 | 85 | 2 | 123 (92.6 done) | 18 | 47 |
| 🐉 Creature | ![🟩🟩🟩🟩🟩🟨🟨🟪⬜⬜ 68%](wiki/project/bars/family-creature.svg?v=5181b8f9) | 806 | 0 | 7 | 0 | 398 (237.8 done) | 134 | 192 |
| 🏘️ Villagers, towns and animals | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 83%](wiki/project/bars/family-villagers-towns-and-animals.svg?v=10b77635) | 707 | 3 | 31 | 0 | 137 (88.9 done) | 15 | 112 |
| ✋ Player and interface | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 84%](wiki/project/bars/family-player-and-interface.svg?v=bccff0e6) | 1477 | 77 | 190 | 4 | 426 (255.4 done) | 7 | 194 |
| 📜 Story | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 63%](wiki/project/bars/family-story.svg?v=a30f4dec) | 1454 | 86 | 1839 | 0 | 1039 (526.1 done) | 5 | 1801 |
| 💾 Save and load | ![🟨🟨⬜⬜⬜⬜⬜⬜⬜⬜ 19%](wiki/project/bars/family-save-and-load.svg?v=565be709) | 5 | 0 | 1 | 0 | 43 (19.2 done) | 0 | 81 |
| ⚙️ Engine | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 78%](wiki/project/bars/family-engine.svg?v=cac6c85f) | 413 | 0 | 17 | 1 | 106 (66.8 done) | 2 | 100 |
| ⏸️ Online, parked (in no score) | ![🟩🟩🟩🟩🟩⬜⬜⬜⬜⬜ 54%](wiki/project/bars/family-online-parked.svg?v=f180e84e) | 29 | 0 | 44 | 0 | 9 (0.8 done) | 0 | 55 |

Save and load gathers the rows about saved games from every domain, so each domain's line below counts the rest of its rows; online play and services are parked (user, 2026-10-24) and counted apart, in no score.

<a id="the-story-land-by-land"></a>

### 📜 The story, land by land

The story domain's rows by the land they belong to: the land's page, the quests the gold and silver scroll indexes list under it, and the sections about that land alone; a row about several lands, or none, is across the lands. The scripts, the rival gods and the easter eggs are not split by land. The story's open issues: [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:story).

| Land | Progress | Rows | Gold scrolls | Silver scrolls | The rest |
|---|---|---:|---:|---:|---:|
| [Land 1](wiki/project/progress/story/land_1.md) | ![🟩🟩🟩🟩🟨⬜⬜⬜⬜⬜ 50%](wiki/project/bars/story-land-1.svg?v=8691d1d5) | 1158 | 42% of 529 | 60% of 557 | 31% of 72 |
| [Land 2](wiki/project/progress/story/land_2.md) | ![🟩🟩🟩🟩🟩🟩⬜⬜⬜⬜ 61%](wiki/project/bars/story-land-2.svg?v=4763a319) | 1261 | 84% of 518 | 49% of 679 | 2% of 64 |
| [Land 3](wiki/project/progress/story/land_3.md) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 61%](wiki/project/bars/story-land-3.svg?v=029e6e27) | 326 | 73% of 178 | 60% of 115 | 0% of 33 |
| [Land 4](wiki/project/progress/story/land_4.md) | ![🟩🟩🟩🟩🟩🟩🟨⬜⬜⬜ 67%](wiki/project/bars/story-land-4.svg?v=ced9b2f7) | 446 | 76% of 301 | 72% of 99 | 1% of 46 |
| [Land 5](wiki/project/progress/story/land_5.md) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 59%](wiki/project/bars/story-land-5.svg?v=e55e4121) | 543 | 66% of 212 | 60% of 300 | 0% of 31 |
| [Tutorial island](wiki/project/progress/story/tutorial_island.md) | ![🟩🟩🟨🟨⬜⬜⬜⬜⬜⬜ 45%](wiki/project/bars/story-tutorial-island.svg?v=623816b0) | 52 | 45% of 51 | — | 0% of 1 |
| [Across the lands](wiki/project/progress/INDEX.md#story) | ![🟩🟩🟩🟩🟨🟨⬜⬜⬜⬜ 58%](wiki/project/bars/story-across-the-lands.svg?v=918d73a6) | 527 | — | 29% of 111 | 66% of 416 |

*Playable, land by land*: not counted. A land is playable when every system it uses works, and the rows do not say which lands use which system, so that share cannot be read from them; each land's line counts its story rows only.

<details><summary>Every domain</summary>

| Domain | Progress | Done, with a test | Done, in play | Done, neither | Chosen | Partial | Inferred | Pending | Issues |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|
| [Ocean](wiki/project/progress/INDEX.md#ocean) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/features-ocean.svg?v=f11f245a) | 37 | 0 | 22 | 0 | 0 | 0 | 0 |  |
| [Sky](wiki/project/progress/INDEX.md#sky) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/features-sky.svg?v=5d1775bd) | 181 | 0 | 0 | 2 | 2 (1.9 done) | 0 | 0 |  |
| [Weather](wiki/project/progress/INDEX.md#weather) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/features-weather.svg?v=f07a209e) | 116 | 0 | 2 | 0 | 1 (0.8 done) | 0 | 0 |  |
| [Terrain](wiki/project/progress/INDEX.md#terrain) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟨 98%](wiki/project/bars/features-terrain.svg?v=892c45d9) | 89 | 0 | 19 | 0 | 6 (4.8 done) | 0 | 1 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:terrain) |
| [Physics](wiki/project/progress/INDEX.md#physics) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 93%](wiki/project/bars/features-physics.svg?v=fe57ef62) | 256 | 0 | 26 | 0 | 51 (37.7 done) | 4 | 5 | [5 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:physics,area:particles) |
| [Food, wood and resources](wiki/project/progress/INDEX.md#resources) | ![🟩🟩🟩🟩🟩🟩🟩🟨🟨⬜ 88%](wiki/project/bars/features-resources.svg?v=d715bedd) | 82 | 0 | 0 | 0 | 22 (18.0 done) | 1 | 9 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:resources) |
| [Nature](wiki/project/progress/INDEX.md#nature) | ![🟩🟩🟩🟩🟩🟩🟨🟨⬜⬜ 76%](wiki/project/bars/features-nature.svg?v=1720a2d1) | 133 | 0 | 16 | 0 | 41 (29.5 done) | 13 | 32 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:nature) |
| [Creature](wiki/project/progress/INDEX.md#creature) | ![🟩🟩🟩🟩🟩🟨🟨🟪⬜⬜ 68%](wiki/project/bars/features-creature.svg?v=5181b8f9) | 806 | 0 | 7 | 0 | 398 (237.8 done) | 134 | 192 | [25 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:creature) |
| [Worship and belief](wiki/project/progress/INDEX.md#worship) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩⬜ 89%](wiki/project/bars/features-worship.svg?v=6634a4de) | 108 | 0 | 0 | 0 | 8 (4.7 done) | 2 | 8 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:worship) |
| [Animals](wiki/project/progress/INDEX.md#animal) | ![🟩🟩🟩🟩🟩🟩🟩🟨🟨⬜ 86%](wiki/project/bars/features-animal.svg?v=e6194c56) | 62 | 0 | 0 | 0 | 18 (13.7 done) | 3 | 5 | [3 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:animal) |
| [Buildings](wiki/project/progress/INDEX.md#building) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 83%](wiki/project/bars/features-building.svg?v=62beaaa2) | 162 | 1 | 2 | 0 | 41 (26.3 done) | 3 | 20 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:building) |
| [Villagers](wiki/project/progress/INDEX.md#villager) | ![🟩🟩🟩🟩🟩🟩🟩🟩⬜⬜ 82%](wiki/project/bars/features-villager.svg?v=f7e09311) | 255 | 2 | 29 | 0 | 30 (16.2 done) | 5 | 47 | [5 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:villager) |
| [Towns](wiki/project/progress/INDEX.md#town) | ![🟩🟩🟩🟩🟩🟩🟨🟨⬜⬜ 76%](wiki/project/bars/features-town.svg?v=61e0163e) | 120 | 0 | 0 | 0 | 40 (27.9 done) | 2 | 32 | [3 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:town) |
| [Gestures](wiki/project/progress/INDEX.md#gesture) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟨 95%](wiki/project/bars/features-gesture.svg?v=b5592f56) | 106 | 0 | 0 | 0 | 3 (2.6 done) | 0 | 5 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:gesture) |
| [The hand](wiki/project/progress/INDEX.md#hand) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 90%](wiki/project/bars/features-hand.svg?v=063a3acf) | 517 | 68 | 1 | 0 | 86 (55.2 done) | 2 | 34 | [6 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:hand) |
| [Temple](wiki/project/progress/INDEX.md#temple) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 90%](wiki/project/bars/features-temple.svg?v=d3334e9f) | 70 | 0 | 1 | 0 | 18 (12.8 done) | 0 | 4 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:temple) |
| [Interface](wiki/project/progress/INDEX.md#interface) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 87%](wiki/project/bars/features-interface.svg?v=781f774c) | 235 | 9 | 1 | 3 | 35 (19.6 done) | 2 | 23 | [3 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:interface) |
| [Miracles](wiki/project/progress/INDEX.md#miracles) | ![🟩🟩🟩🟩🟩🟩🟩🟨🟨⬜ 86%](wiki/project/bars/features-miracles.svg?v=21963023) | 412 | 0 | 126 | 0 | 191 (134.9 done) | 2 | 47 | [9 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:miracles) |
| [Camera](wiki/project/progress/INDEX.md#camera) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 82%](wiki/project/bars/features-camera.svg?v=8f8f5212) | 133 | 0 | 0 | 1 | 55 (30.3 done) | 1 | 10 | [5 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:camera) |
| [Skirmish and multiplayer lands](wiki/project/progress/INDEX.md#multiplayer) | ![🟩🟩🟩🟩⬜⬜⬜⬜⬜⬜ 37%](wiki/project/bars/features-multiplayer.svg?v=c5942a28) | 4 | 0 | 61 | 0 | 38 | 0 | 71 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:multiplayer) |
| [Land 1](wiki/project/progress/story/land_1.md) | ![🟩🟩🟩🟩🟨⬜⬜⬜⬜⬜ 50%](wiki/project/bars/story-land-1.svg?v=8691d1d5) | 15 | 19 | 429 | 0 | 286 (112.1 done) | 0 | 409 |  |
| [Land 2](wiki/project/progress/story/land_2.md) | ![🟩🟩🟩🟩🟩🟩⬜⬜⬜⬜ 61%](wiki/project/bars/story-land-2.svg?v=4763a319) | 10 | 60 | 627 | 0 | 123 (72.4 done) | 0 | 441 |  |
| [Land 3](wiki/project/progress/story/land_3.md) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 61%](wiki/project/bars/story-land-3.svg?v=029e6e27) | 2 | 0 | 176 | 0 | 36 (21.0 done) | 0 | 112 |  |
| [Land 4](wiki/project/progress/story/land_4.md) | ![🟩🟩🟩🟩🟩🟩🟨⬜⬜⬜ 67%](wiki/project/bars/story-land-4.svg?v=ced9b2f7) | 0 | 0 | 262 | 0 | 65 (38.4 done) | 0 | 119 |  |
| [Land 5](wiki/project/progress/story/land_5.md) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 59%](wiki/project/bars/story-land-5.svg?v=e55e4121) | 1 | 0 | 278 | 0 | 69 (41.4 done) | 0 | 195 |  |
| [Tutorial island](wiki/project/progress/story/tutorial_island.md) | ![🟩🟩🟨🟨⬜⬜⬜⬜⬜⬜ 45%](wiki/project/bars/story-tutorial-island.svg?v=623816b0) | 2 | 2 | 10 | 0 | 31 (11.2 done) | 0 | 9 |  |
| [Across the lands](wiki/project/progress/INDEX.md#story) | ![🟩🟩🟩🟩🟨🟨⬜⬜⬜⬜ 58%](wiki/project/bars/story-across-the-lands.svg?v=918d73a6) | 186 | 3 | 43 | 0 | 119 (74.4 done) | 0 | 176 |  |
| [Scripts](wiki/project/progress/INDEX.md#scripts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 86%](wiki/project/bars/features-scripts.svg?v=4dc344e3) | 1142 | 2 | 7 | 0 | 185 (104.6 done) | 5 | 117 | [5 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:scripts) |
| [Rival gods](wiki/project/progress/INDEX.md#rival_gods) | ![🟩🟩🟩🟨⬜⬜⬜⬜⬜⬜ 37%](wiki/project/bars/features-rival-gods.svg?v=b49e617c) | 89 | 0 | 7 | 0 | 111 (45.1 done) | 0 | 172 |  |
| [Easter eggs](wiki/project/progress/INDEX.md#easter_eggs) | ![🟩🟨⬜⬜⬜⬜⬜⬜⬜⬜ 17%](wiki/project/bars/features-easter-eggs.svg?v=bf741f40) | 7 | 0 | 0 | 0 | 14 (5.5 done) | 0 | 51 |  |
| [💾 Loading a save](wiki/project/progress/engine/saving_and_loading.md) | ![🟨🟨🟨⬜⬜⬜⬜⬜⬜⬜ 34%](wiki/project/bars/save-loading.svg?v=6874c72d) | 0 | 0 | 0 | 0 | 6 (3.1 done) | 0 | 3 |  |
| [💾 Writing a save](wiki/project/progress/engine/saving_and_loading.md) | ![🟩🟩🟨🟨⬜⬜⬜⬜⬜⬜ 43%](wiki/project/bars/save-saving.svg?v=92c5a140) | 4 | 0 | 0 | 0 | 8 (3.7 done) | 0 | 6 |  |
| [💾 Kept in the save (one row for both halves)](wiki/project/progress/engine/saving_and_loading.md) | ![🟨⬜⬜⬜⬜⬜⬜⬜⬜⬜ 14%](wiki/project/bars/save-both.svg?v=b8213e15) | 1 | 0 | 1 | 0 | 29 (12.4 done) | 0 | 72 |  |
| [Sound and music](wiki/project/progress/INDEX.md#audio) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 99%](wiki/project/bars/features-audio.svg?v=a9b435bb) | 105 | 0 | 1 | 0 | 1 (0.5 done) | 0 | 1 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:audio) |
| [Video](wiki/project/progress/INDEX.md#video) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟨 97%](wiki/project/bars/features-video.svg?v=6f52d4b1) | 55 | 0 | 1 | 0 | 2 (1.2 done) | 0 | 1 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:video) |
| [Engine and files](wiki/project/progress/INDEX.md#engine) | ![🟩🟩🟩🟩🟩🟨🟨🟨⬜⬜ 79%](wiki/project/bars/features-engine.svg?v=f5d9fb49) | 96 | 0 | 5 | 0 | 68 (45.8 done) | 0 | 18 | [8 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:engine) |
| [Rendering](wiki/project/progress/INDEX.md#rendering) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 77%](wiki/project/bars/features-rendering.svg?v=ef9be164) | 114 | 0 | 7 | 1 | 33 (18.1 done) | 0 | 26 | [15 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:rendering) |
| [PC integration](wiki/project/progress/INDEX.md#pc_integration) | ![🟩🟩🟩🟩🟨⬜⬜⬜⬜⬜ 45%](wiki/project/bars/features-pc-integration.svg?v=033002eb) | 43 | 0 | 3 | 0 | 2 (1.2 done) | 2 | 54 |  |
| [⏸️ Firestorm](wiki/project/progress/multiplayer/maps/firestorm.md) | ![🟩🟩🟩🟩🟩🟩⬜⬜⬜⬜ 65%](wiki/project/bars/online-multiplayer-maps-firestorm.svg?v=026c3517) | 0 | 0 | 20 | 0 | 6 | 0 | 5 |  |
| [⏸️ Multiplayer rules](wiki/project/progress/multiplayer/multiplayer_rules.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-multiplayer-rules.svg?v=8bb63ed6) | 0 | 0 | 0 | 0 | 0 | 0 | 21 |  |
| [⏸️ Network play](wiki/project/progress/multiplayer/network_play.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-network-play.svg?v=99990afc) | 0 | 0 | 0 | 0 | 1 | 0 | 18 |  |
| [⏸️ Online services](wiki/project/progress/multiplayer/online_services.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-online-services.svg?v=7a302ce4) | 0 | 0 | 0 | 0 | 0 | 0 | 2 |  |
| [⏸️ Online services and PC extras](wiki/project/progress/pc_integration/online_services.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-pc-integration-online-services.svg?v=efe8a375) | 0 | 0 | 0 | 0 | 0 | 0 | 9 |  |
| [⏸️ Playground scripts](wiki/project/progress/scripts/playground_scripts.md) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 98%](wiki/project/bars/online-scripts-playground-scripts.svg?v=938a427c) | 29 | 0 | 24 | 0 | 2 (0.8 done) | 0 | 0 |  |

🐞 [Vanilla bugs](wiki/project/progress/vanilla_bugs/README.md): 241 bugs of the original logged (not features, so no score).

</details>

<a id="unification-with-raffclars-tree"></a>
<a id="raffclars-work-in-our-tree"></a>

## 🔀 raffclar's work in our tree

Every pull request merged into his master (`e36919e4`, 2026-11-02), checked against our tree.

![🟩🟩🟩🟩🟨🟨⬜⬜⬜⬜ 51%](wiki/project/bars/raffclar-all.svg?v=136b0244)

**332** in · **203** partial · **313** pending · **183** n/a, not taken: online play and services, parked until the end (user, 2026-10-24) (62); CI and build tooling (60); his inspector and MCP adapter, parked by decision C-1 (34); his deviations from the original, not chosen by us (8); his own docs pages (8); developer tools and behaviour the original does not have (7); reverts (2); formatting (1); force-feedback device behaviour, not ported (user, 2026-10-27) (1).

The list, pull request by pull request: [raffclar-status.md](wiki/project/raffclar-status.md).

<a id="refactor"></a>

## 🧹 Code clean-up

How far the code follows the [code conventions](wiki/project/refactor-conventions.md#progress-counts); the notes on each goal are there.

🟩 done · 🟧 left on purpose · ⬜ pending

| Goal | Progress |
|---|---|
| [Mutable globals and file- or function-scope statics in game code](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 ~96%](wiki/project/bars/refactor-mutable-globals-and-file-or-function.svg?v=6139d414) |
| [`Game::Instance()` call sites](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/refactor-game-instance-call-sites.svg?v=07084c46) |
| [Plain `enum` declarations](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟧🟧🟧🟧 ~64%](wiki/project/bars/refactor-plain-enum-declarations.svg?v=7d4b6c3e) |
| [`new` / `delete` in game code](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/refactor-new-delete-in-game-code.svg?v=07084c46) |
| [C arrays](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/refactor-c-arrays.svg?v=07084c46) |
| [Direct file reads (`std::ifstream`, `fopen`)](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟧🟧🟧🟧 ~60%](wiki/project/bars/refactor-direct-file-reads-std-ifstream-fopen.svg?v=ffebe33c) |
| [Comments naming the original binary (`fn_…`)](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/refactor-comments-naming-the-original-binary-fn.svg?v=07084c46) |
| [Comments quoting the original's addresses](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/refactor-comments-quoting-the-originals-addresses.svg?v=07084c46) |
| [Identifiers named after the original binary or unclear](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 ~95%](wiki/project/bars/refactor-identifiers-named-after-the-original.svg?v=e87ec08c) |
| [Files that fail the CI's clang-format 17 check](wiki/project/refactor-conventions.md#progress-counts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%](wiki/project/bars/refactor-files-that-fail-the-cis-clang-format-17.svg?v=07084c46) |

Also counted, with no goal: Locator services 29 → 161; Test files 151 → 950.

<a id="milestones"></a>

## 🏁 Milestones

- **2026-10-22** · [The checks read the original's meshes](wiki/project/milestones.md#the-checks-read-the-originals-meshes)
- **2026-10-22** · [Mods: the SDK's version, the Mods page and data mods](wiki/project/milestones.md#mods-the-sdks-version-the-mods-page-and-data-mods)
- **2026-10-21** · [Faster builds](wiki/project/milestones.md#faster-builds)
- **2026-10-21** · [The leashes a creature knows in the state hash](wiki/project/milestones.md#the-leashes-a-creature-knows-in-the-state-hash)
- **2026-10-21** · [The top detail level draws the high meshes, chosen from Maximum Detail](wiki/project/milestones.md#the-top-detail-level-and-maximum-detail)
- **2026-10-20** · [Recreate the original, on raffclar's architecture](wiki/project/milestones.md#recreate-the-original-on-raffclars-architecture)

[All milestones](wiki/project/milestones.md).
