# openblack wiki

How the original Black & White (v1.42) behaves, with addresses and formulas, and what openblack does.

- **Read it:** open `docs/index.html` through `docs/open_wiki.bat` (Windows), or browse [the pages](SUMMARY.md) here.
- **Start:** the [progress dashboard](wiki/project/dashboard.md) and [Reading this wiki](wiki/project/reading-the-wiki.md).

*This page is written by `tools/progress_stats.py`.*


<a id="where-we-are"></a>

## 📊 Where we are

**🎮 Features of the original**

![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 75%](wiki/project/bars/features-all.svg?v=f33c6f0e)

8555 of the original game's 12932 behaviours are done (5823 with a test, 195 proven in play, 91 of them both, 2628 with neither) and 8 done as the user chose; 2070 partial (9% of the rows' worth, 142 not measured yet); 180 inferred, not yet checked against the original. Online is parked: its 197 rows are counted apart, in no score.

**🔀 raffclar's work in our tree**

![🟩🟩🟩🟩🟨🟨⬜⬜⬜⬜ 50%](wiki/project/bars/raffclar-all.svg?v=08113981)

332 of his 862 pull requests are in our tree, 203 more in part.

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
| 🐉 Creature | ![🟩🟩🟩🟩🟩🟨🟨🟪⬜⬜ 68%](wiki/project/bars/family-creature.svg?v=3b5114df) | 809 | 1 | 7 | 0 | 395 (235.7 done) | 134 | 191 |
| 🏘️ Villagers, towns and animals | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 83%](wiki/project/bars/family-villagers-towns-and-animals.svg?v=11c12313) | 709 | 3 | 31 | 0 | 136 (88.2 done) | 15 | 111 |
| ✋ Player and interface | ![🟩🟩🟩🟩🟩🟩🟩🟨🟪⬜ 85%](wiki/project/bars/family-player-and-interface.svg?v=6f446748) | 1481 | 83 | 190 | 4 | 422 (252.8 done) | 7 | 170 |
| 📜 Story | ![🟩🟩🟩🟩🟩🟩🟨⬜⬜⬜ 70%](wiki/project/bars/family-story.svg?v=d692ee6e) | 1507 | 108 | 2297 | 1 | 842 (418.7 done) | 5 | 1465 |
| 💾 Save and load | ![🟨🟨⬜⬜⬜⬜⬜⬜⬜⬜ 19%](wiki/project/bars/family-save-and-load.svg?v=565be709) | 5 | 0 | 1 | 0 | 43 (19.2 done) | 0 | 81 |
| ⚙️ Engine | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 84%](wiki/project/bars/family-engine.svg?v=7368a793) | 418 | 0 | 17 | 1 | 109 (69.2 done) | 1 | 54 |
| ⏸️ Online, parked (in no score) | ![🟩🟩🟩🟩⬜⬜⬜⬜⬜⬜ 37%](wiki/project/bars/family-online-parked.svg?v=f521c63c) | 29 | 0 | 44 | 0 | 9 (0.8 done) | 1 | 114 |

Save and load gathers the rows about saved games from every domain, so each domain's line below counts the rest of its rows; online play and services are parked (user, 2026-10-24) and counted apart, in no score.

<a id="the-story-land-by-land"></a>

### 📜 The story, land by land

The story domain's rows by the land they belong to: the land's page, the quests the gold and silver scroll indexes list under it, and the sections about that land alone; a row about several lands, or none, is across the lands. The scripts, the rival gods and the easter eggs are not split by land. The story's open issues: [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:story).

| Land | Progress | Rows | Gold scrolls | Silver scrolls | The rest |
|---|---|---:|---:|---:|---:|
| [Land 1](wiki/project/progress/story/land_1.md) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 61%](wiki/project/bars/story-land-1.svg?v=2d51e155) | 1158 | 56% of 529 | 68% of 556 | 43% of 73 |
| [Land 2](wiki/project/progress/story/land_2.md) | ![🟩🟩🟩🟩🟩🟩🟩⬜⬜⬜ 70%](wiki/project/bars/story-land-2.svg?v=771e7970) | 1261 | 86% of 518 | 59% of 679 | 50% of 64 |
| [Land 3](wiki/project/progress/story/land_3.md) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 75%](wiki/project/bars/story-land-3.svg?v=13f4cb39) | 326 | 76% of 178 | 85% of 115 | 39% of 33 |
| [Land 4](wiki/project/progress/story/land_4.md) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 76%](wiki/project/bars/story-land-4.svg?v=97d0eec5) | 446 | 79% of 301 | 79% of 99 | 47% of 46 |
| [Land 5](wiki/project/progress/story/land_5.md) | ![🟩🟩🟩🟩🟩🟩🟩⬜⬜⬜ 70%](wiki/project/bars/story-land-5.svg?v=a4a165d0) | 543 | 72% of 212 | 71% of 300 | 42% of 31 |
| [Tutorial island](wiki/project/progress/story/tutorial_island.md) | ![🟩🟩🟩🟨🟨⬜⬜⬜⬜⬜ 48%](wiki/project/bars/story-tutorial-island.svg?v=c137ae41) | 52 | 49% of 51 | — | 0% of 1 |
| [Across the lands](wiki/project/progress/INDEX.md#story) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 61%](wiki/project/bars/story-across-the-lands.svg?v=e8d12aa5) | 527 | — | 36% of 111 | 68% of 416 |

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
| [Creature](wiki/project/progress/INDEX.md#creature) | ![🟩🟩🟩🟩🟩🟨🟨🟪⬜⬜ 68%](wiki/project/bars/features-creature.svg?v=3b5114df) | 809 | 1 | 7 | 0 | 395 (235.7 done) | 134 | 191 | [25 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:creature) |
| [Worship and belief](wiki/project/progress/INDEX.md#worship) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩⬜ 89%](wiki/project/bars/features-worship.svg?v=6634a4de) | 108 | 0 | 0 | 0 | 8 (4.7 done) | 2 | 8 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:worship) |
| [Animals](wiki/project/progress/INDEX.md#animal) | ![🟩🟩🟩🟩🟩🟩🟩🟨🟨⬜ 86%](wiki/project/bars/features-animal.svg?v=e6194c56) | 62 | 0 | 0 | 0 | 18 (13.7 done) | 3 | 5 | [3 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:animal) |
| [Buildings](wiki/project/progress/INDEX.md#building) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 83%](wiki/project/bars/features-building.svg?v=62beaaa2) | 162 | 1 | 2 | 0 | 41 (26.3 done) | 3 | 20 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:building) |
| [Villagers](wiki/project/progress/INDEX.md#villager) | ![🟩🟩🟩🟩🟩🟩🟩🟩⬜⬜ 82%](wiki/project/bars/features-villager.svg?v=a630ee5e) | 257 | 2 | 29 | 0 | 29 (15.5 done) | 5 | 46 | [5 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:villager) |
| [Towns](wiki/project/progress/INDEX.md#town) | ![🟩🟩🟩🟩🟩🟩🟨🟨⬜⬜ 76%](wiki/project/bars/features-town.svg?v=61e0163e) | 120 | 0 | 0 | 0 | 40 (27.9 done) | 2 | 32 | [3 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:town) |
| [Gestures](wiki/project/progress/INDEX.md#gesture) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟨 95%](wiki/project/bars/features-gesture.svg?v=b5592f56) | 106 | 1 | 0 | 0 | 3 (2.6 done) | 0 | 5 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:gesture) |
| [The hand](wiki/project/progress/INDEX.md#hand) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 90%](wiki/project/bars/features-hand.svg?v=14f8e82c) | 521 | 73 | 1 | 0 | 82 (51.6 done) | 2 | 34 | [6 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:hand) |
| [Temple](wiki/project/progress/INDEX.md#temple) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 90%](wiki/project/bars/features-temple.svg?v=d3334e9f) | 70 | 0 | 1 | 0 | 18 (12.8 done) | 0 | 4 | [2 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:temple) |
| [Interface](wiki/project/progress/INDEX.md#interface) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 87%](wiki/project/bars/features-interface.svg?v=ed61492e) | 235 | 9 | 1 | 3 | 36 (20.6 done) | 2 | 22 | [3 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:interface) |
| [Miracles](wiki/project/progress/INDEX.md#miracles) | ![🟩🟩🟩🟩🟩🟩🟩🟨🟨⬜ 86%](wiki/project/bars/features-miracles.svg?v=21963023) | 412 | 0 | 126 | 0 | 191 (134.9 done) | 2 | 47 | [9 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:miracles) |
| [Camera](wiki/project/progress/INDEX.md#camera) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 82%](wiki/project/bars/features-camera.svg?v=8f8f5212) | 133 | 0 | 0 | 1 | 55 (30.3 done) | 1 | 10 | [5 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:camera) |
| [Skirmish and multiplayer lands](wiki/project/progress/INDEX.md#multiplayer) | ![🟩🟩🟩🟩⬜⬜⬜⬜⬜⬜ 43%](wiki/project/bars/features-multiplayer.svg?v=11ec4af3) | 4 | 0 | 61 | 0 | 37 | 0 | 48 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:multiplayer) |
| [Land 1](wiki/project/progress/story/land_1.md) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 61%](wiki/project/bars/story-land-1.svg?v=2d51e155) | 19 | 36 | 565 | 0 | 224 (87.8 done) | 0 | 314 |  |
| [Land 2](wiki/project/progress/story/land_2.md) | ![🟩🟩🟩🟩🟩🟩🟩⬜⬜⬜ 70%](wiki/project/bars/story-land-2.svg?v=771e7970) | 16 | 63 | 755 | 0 | 83 (48.7 done) | 0 | 344 |  |
| [Land 3](wiki/project/progress/story/land_3.md) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 75%](wiki/project/bars/story-land-3.svg?v=13f4cb39) | 2 | 0 | 231 | 0 | 21 (12.4 done) | 0 | 72 |  |
| [Land 4](wiki/project/progress/story/land_4.md) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 76%](wiki/project/bars/story-land-4.svg?v=97d0eec5) | 3 | 0 | 316 | 0 | 33 (19.5 done) | 0 | 94 |  |
| [Land 5](wiki/project/progress/story/land_5.md) | ![🟩🟩🟩🟩🟩🟩🟩⬜⬜⬜ 70%](wiki/project/bars/story-land-5.svg?v=a4a165d0) | 5 | 0 | 348 | 0 | 43 (26.0 done) | 0 | 147 |  |
| [Tutorial island](wiki/project/progress/story/tutorial_island.md) | ![🟩🟩🟩🟨🟨⬜⬜⬜⬜⬜ 48%](wiki/project/bars/story-tutorial-island.svg?v=c137ae41) | 4 | 2 | 11 | 0 | 29 (9.9 done) | 0 | 8 |  |
| [Across the lands](wiki/project/progress/INDEX.md#story) | ![🟩🟩🟩🟩🟩🟨⬜⬜⬜⬜ 61%](wiki/project/bars/story-across-the-lands.svg?v=e8d12aa5) | 192 | 5 | 53 | 0 | 120 (75.1 done) | 0 | 159 |  |
| [Scripts](wiki/project/progress/INDEX.md#scripts) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟨⬜ 87%](wiki/project/bars/features-scripts.svg?v=d51efb91) | 1167 | 2 | 11 | 1 | 164 (88.5 done) | 5 | 112 | [4 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:scripts) |
| [Rival gods](wiki/project/progress/INDEX.md#rival_gods) | ![🟩🟩🟩🟨⬜⬜⬜⬜⬜⬜ 37%](wiki/project/bars/features-rival-gods.svg?v=200a5e94) | 90 | 0 | 7 | 0 | 110 (44.5 done) | 0 | 172 |  |
| [Easter eggs](wiki/project/progress/INDEX.md#easter_eggs) | ![🟩🟨⬜⬜⬜⬜⬜⬜⬜⬜ 23%](wiki/project/bars/features-easter-eggs.svg?v=33e5a8fb) | 9 | 0 | 0 | 0 | 15 (6.3 done) | 0 | 43 |  |
| [💾 Loading a save](wiki/project/progress/engine/saving_and_loading.md) | ![🟨🟨🟨⬜⬜⬜⬜⬜⬜⬜ 34%](wiki/project/bars/save-loading.svg?v=6874c72d) | 0 | 0 | 0 | 0 | 6 (3.1 done) | 0 | 3 |  |
| [💾 Writing a save](wiki/project/progress/engine/saving_and_loading.md) | ![🟩🟩🟨🟨⬜⬜⬜⬜⬜⬜ 43%](wiki/project/bars/save-saving.svg?v=92c5a140) | 4 | 0 | 0 | 0 | 8 (3.7 done) | 0 | 6 |  |
| [💾 Kept in the save (one row for both halves)](wiki/project/progress/engine/saving_and_loading.md) | ![🟨⬜⬜⬜⬜⬜⬜⬜⬜⬜ 14%](wiki/project/bars/save-both.svg?v=b8213e15) | 1 | 0 | 1 | 0 | 29 (12.4 done) | 0 | 72 |  |
| [Sound and music](wiki/project/progress/INDEX.md#audio) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 99%](wiki/project/bars/features-audio.svg?v=a9b435bb) | 105 | 0 | 1 | 0 | 1 (0.5 done) | 0 | 1 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:audio) |
| [Video](wiki/project/progress/INDEX.md#video) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟨 97%](wiki/project/bars/features-video.svg?v=6f52d4b1) | 55 | 0 | 1 | 0 | 2 (1.2 done) | 0 | 1 | [1 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:video) |
| [PC integration](wiki/project/progress/INDEX.md#pc_integration) | ![🟩🟩🟩🟩🟩🟩🟩🟩⬜⬜ 81%](wiki/project/bars/features-pc-integration.svg?v=29c37234) | 47 | 0 | 3 | 0 | 4 (2.8 done) | 1 | 10 |  |
| [Engine and files](wiki/project/progress/INDEX.md#engine) | ![🟩🟩🟩🟩🟩🟨🟨🟨⬜⬜ 79%](wiki/project/bars/features-engine.svg?v=f5d9fb49) | 96 | 0 | 5 | 0 | 68 (45.8 done) | 0 | 18 | [8 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:engine) |
| [Rendering](wiki/project/progress/INDEX.md#rendering) | ![🟩🟩🟩🟩🟩🟩🟩🟨⬜⬜ 78%](wiki/project/bars/features-rendering.svg?v=30a392fc) | 115 | 0 | 7 | 1 | 34 (19.0 done) | 0 | 24 | [15 open](https://github.com/diegoscood-ai/openblack/issues?q=is:issue+is:open+label:area:rendering) |
| [⏸️ Launch switches and stray files](wiki/project/progress/easter_eggs/launch_switches_and_files.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-easter-eggs-launch-switches-and-files.svg?v=f371a1be) | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  |
| [⏸️ Firestorm](wiki/project/progress/multiplayer/maps/firestorm.md) | ![🟩🟩🟩🟩🟩🟩🟩⬜⬜⬜ 69%](wiki/project/bars/online-multiplayer-maps-firestorm.svg?v=0a06009e) | 0 | 0 | 20 | 0 | 6 | 0 | 3 |  |
| [⏸️ Four Gods](wiki/project/progress/multiplayer/maps/four_gods.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-maps-four-gods.svg?v=e41d6658) | 0 | 0 | 0 | 0 | 0 | 0 | 6 |  |
| [⏸️ Three Gods](wiki/project/progress/multiplayer/maps/three_gods.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-maps-three-gods.svg?v=045536a1) | 0 | 0 | 0 | 0 | 0 | 0 | 7 |  |
| [⏸️ Two Gods](wiki/project/progress/multiplayer/maps/two_gods.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-maps-two-gods.svg?v=045536a1) | 0 | 0 | 0 | 0 | 0 | 0 | 7 |  |
| [⏸️ Multiplayer rules](wiki/project/progress/multiplayer/multiplayer_rules.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-multiplayer-rules.svg?v=8bb63ed6) | 0 | 0 | 0 | 0 | 0 | 0 | 21 |  |
| [⏸️ Network play](wiki/project/progress/multiplayer/network_play.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-network-play.svg?v=99990afc) | 0 | 0 | 0 | 0 | 1 | 0 | 18 |  |
| [⏸️ Online services](wiki/project/progress/multiplayer/online_services.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-online-services.svg?v=7a302ce4) | 0 | 0 | 0 | 0 | 0 | 0 | 2 |  |
| [⏸️ Skirmish](wiki/project/progress/multiplayer/skirmish.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-multiplayer-skirmish.svg?v=7a302ce4) | 0 | 0 | 0 | 0 | 0 | 0 | 2 |  |
| [⏸️ Online services and PC extras](wiki/project/progress/pc_integration/online_services.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-pc-integration-online-services.svg?v=efe8a375) | 0 | 0 | 0 | 0 | 0 | 0 | 9 |  |
| [⏸️ Villagers named after the player's contacts](wiki/project/progress/pc_integration/villager_names_from_contacts.md) | ![⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0%](wiki/project/bars/online-pc-integration-villager-names-from.svg?v=579d3a06) | 0 | 0 | 0 | 0 | 0 | 1 | 38 |  |
| [⏸️ Playground scripts](wiki/project/progress/scripts/playground_scripts.md) | ![🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 98%](wiki/project/bars/online-scripts-playground-scripts.svg?v=938a427c) | 29 | 0 | 24 | 0 | 2 (0.8 done) | 0 | 0 |  |

🐞 [Vanilla bugs](wiki/project/progress/vanilla_bugs/README.md): 241 bugs of the original logged (not features, so no score).

</details>

<a id="unification-with-raffclars-tree"></a>
<a id="raffclars-work-in-our-tree"></a>

## 🔀 raffclar's work in our tree

Every pull request merged into his master (`0be85893`, 2026-11-03), checked against our tree.

![🟩🟩🟩🟩🟨🟨⬜⬜⬜⬜ 50%](wiki/project/bars/raffclar-all.svg?v=08113981)

**332** in · **203** partial · **327** pending · **183** n/a, not taken: online play and services, parked until the end (user, 2026-10-24) (62); CI and build tooling (60); his inspector and MCP adapter, parked by decision C-1 (34); his deviations from the original, not chosen by us (8); his own docs pages (8); developer tools and behaviour the original does not have (7); reverts (2); formatting (1); force-feedback device behaviour, not ported (user, 2026-10-27) (1).

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

Also counted, with no goal: Locator services 29 → 161; Test files 151 → 961.

<a id="milestones"></a>

## 🏁 Milestones

- **2026-10-22** · [The checks read the original's meshes](wiki/project/milestones.md#the-checks-read-the-originals-meshes)
- **2026-10-22** · [Mods: the SDK's version, the Mods page and data mods](wiki/project/milestones.md#mods-the-sdks-version-the-mods-page-and-data-mods)
- **2026-10-21** · [Faster builds](wiki/project/milestones.md#faster-builds)
- **2026-10-21** · [The leashes a creature knows in the state hash](wiki/project/milestones.md#the-leashes-a-creature-knows-in-the-state-hash)
- **2026-10-21** · [The top detail level draws the high meshes, chosen from Maximum Detail](wiki/project/milestones.md#the-top-detail-level-and-maximum-detail)
- **2026-10-20** · [Recreate the original, on raffclar's architecture](wiki/project/milestones.md#recreate-the-original-on-raffclars-architecture)

[All milestones](wiki/project/milestones.md).
