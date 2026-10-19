# Super Street Fighter II — features and roadmap

Where this port stands against a finished one, and what each missing piece needs. The reference is the
toolkit's own complete example, [vampire-savior-recomp](https://github.com/tmcleroy/vampire-savior-recomp)
(`games/vampire_savior/src/`), and the sibling cps2recomp ports (Hyper Street Fighter II, Street Fighter
Alpha, Marvel vs. Capcom), plus the toolkit's documents `docs/FEATURES.md` and `docs/FIGHT.md`.

Everything below is an **optional feature**: with every feature off, the port must run exactly as the
board does. Nothing from the game (no ROM, no RAM dump, no ripped art/text) may ever be committed — each
thing found is recorded in `PROGRESS.md` as *how* it was found (a RAM diff, a write watch, a trace),
never as the data itself.

## What works today

From the toolkit, with no game work:

- The **launcher**: chooses the ROM set, checks every file by CRC-32, sets the controls, Play.
- The **in-game menu** (Esc, `F1`, a stick's click, or Start+Select): save states (9 slots), Reset,
  Insert coin, Player 2 joins, Quit game.
- **Stop, step, slow** (P, O, `-`/`=`).
- **Run-ahead** and the **Display** options (scaling, picture shape, window size, full screen).
- Every key and button **remappable**, for each player.

The game's own features are **not built yet**: none of the game's memory has been found, so the port
has nothing game-specific in `games/ssf2/src/` and `--list-features` shows only the toolkit's own.

## What a finished port adds (this is the work)

### 0. Foundation — know the game's memory

The credits, the round's clock and the two players' records have to be found and named in
`games/ssf2/src/ssf2.h`: the health (and what the bar is drawn from), the place across the stage, the
facing, the state word (standing, walking, crouching, in the air, attacking, reeling), and the value
behind the super meter. This is the starting point for everything else.

How the toolkit finds these (all in `docs/FEATURES.md`):

- `--dump-ram F:FILE` + `tools/ramfind.py DUMP=VALUE ...` — addresses that held a known value across
  several dumps.
- `--trace LOW-HIGH:FILE` with a run that does known things at known frames (`--movie` drives the
  game to a known place first).
- `--watch LOW[-HIGH]` — every write to an address and the instruction that made it, read against
  `work/ssf2/program.lst`.
- `--poke ADDR=BYTE,...` to try a guess; `--menu FRAME:KEYS` to press the menu's own rows;
  `--shot-every N:PFX` to see a run.

### 1. The Features page (the game's own switches)

| Feature | Kind | Built | Needs |
| --- | --- | --- | --- |
| `free_play` | on/off | no | the credits word |
| `no_damage`, `no_damage_2` | on/off | no | each player's health and the round's full value |
| `infinite_time` | on/off | no | the round's clock |
| `one_hit` | on/off | no | the opponent's health |
| `full_gauge` | on/off | no | the value behind each player's super meter |
| Board/operator settings (difficulty, round time, free play) | choices | no | reading the EEPROM's operator settings |
| "The game's own test menu" | command | no | a routine address to jump to |

### 2. Training mode (the headline feature)

| Piece | What it adds | Built | Needs |
| --- | --- | --- | --- |
| Input display | what the stick holds, and for how long | no | each player's facing, so the notation turns to their side |
| Training mode | health and meter held full, the clock standing, and a readout | no | the health, the clock and the meter |
| Frame data | startup, active, recovery, and the advantage after a hit | no | the state that says a move is out, per player |
| Combo readout | hits, damage, and what of it comes back | no | the damage rule (a rewrite) |
| The dummy (player 2) | stand, crouch, jump, guard, record and replay | no | the game's idea of a round and of guarding |
| Hit boxes | the players' and projectiles' boxes, drawn on the picture | no | the box tables and the camera's place |

The toolkit's `fight/` parts (`fight/fight.h`) give the framework; the input display and the Training
mode session build on the toolkit's HUD alone and need only the underlying addresses.

### 3. Alternative modes

Each is a command with a page of its own: an **attract watcher** (the credits word held at zero), and
an **endless** game (the round/match state). A two-machine link is not possible (the board's link
hardware is a separate build).

### 4. Widescreen

A switch on the launcher's first page, off in every preset (it can change how the game plays). The
field is shown in 16:9, 512 columns where the board's 384, at the same height and scale. Needs rewrites
of the camera and the background scrollers and of the objects that must be kept while they are in the
wide picture. The toolkit's `widescreen.cpp` in Vampire Savior is the example.

### 5. Readable rewrites (correctness, and what the above builds on)

Rewrite the game's key routines by hand as named C++ (`docs/READABLE.md`): the players' movement and
collision, damage, throws, and the round's advance. This is what makes a clean `one_hit`, the training
mode and the widescreen possible, and it is the part most worth doing carefully.

## Recommended order

1. **The foundation** — the game's memory (credits, clock, the two player records).
2. **The Features page** — free play, then the cheats.
3. **Training mode** — the input display and the Training mode session, then the rest.
4. **Readable rewrites** — needed by 3 and 5, and correctness.
5. **Widescreen.**
6. **Alternative modes.**

## Before uploading: a checklist

- [x] `README.md` states the exact ROM set (`ssf2`, World 931005) and that **only that revision**
      works, with the Capcom disclaimer.
- [x] `rom/`, `build/`, `work/` are ignored and **absent** from the repository; no game data, keys or
      dumps anywhere (`.github/workflows/check.yml` fails a push that tracks one); no screenshots.
- [x] `PROGRESS.md` records how each finding was made (a RAM diff, a watch, a trace), not the data.
- [x] `make check`, `make check-toolkit`, `make check-common` all pass (the CI runs the last two).
- [x] `LICENSE` (GPL-3.0-only) and `THIRD_PARTY_NOTICES.md` (the cipher is MAME's, BSD-3-Clause); the
      release archives carry them and the licences of what they ship.
- [x] Repo description set (see README's title line / the About field).
- [x] First commit made and pushed; only source, docs and tooling are tracked (no ROM, key or derived
      data — `rom/`, `work/` and `build/` are ignored).
- [x] The repository builds from a fresh clone with the player's own ROM set (`make`, `make explore`,
      `make launcher`).
