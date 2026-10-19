# Super Street Fighter II — the game's memory, and how it was found

This file records what the port knows about *Super Street Fighter II: The New Challengers* (MAME set
`ssf2`, World 931005) beyond its program: where the game keeps a thing in RAM, and **how that was
found** — a RAM diff, a write watch, a trace. Never the data itself (no dumps, no ripped art).

Every address below was confirmed on the running board with the toolkit's own tools (`docs/FEATURES.md`);
where an independent cheat collection for the same ROM set (FinalBurn Neo's) names the same thing, it is
said so, and that address was still checked on the board itself.

## The board

The CPS-2 board this port models is in `cps2recomp/machine/`; the ROM set's layout is in
`games/ssf2/game.cfg`, and the two together are all the port needs to run the game.

- The 68000 program is five 512 KiB ROMs (`ssfe-03b`, `ssfe.04`, `ssfe.05`, `ssfe-06b`, `ssfe.07`),
  2.5 MiB in all, byte-swapped into words and deciphered with the set's key, exactly as the board does.
- The graphics are eight ROMs in two rows of four (`ssf.13m/15m/17m/19m`, then `ssf.14m/16m/18m/20m`).
- The Z80 sound program is one 128 KiB ROM (`ssf-01a`); the QSound samples are `ssf.q01` .. `ssf.q08`.

## How the game's own memory was found

The tools, in the order they were reached for (all in `docs/FEATURES.md`):

- `--menu FRAME:KEYS` drives the port's in-game menu with no window: a coin goes in without a hand.
- `--movie FILE` replays a hand-written input movie (`frame player1 player2`, the pads' bits in hex),
  and `--poke ADDR=BYTE` holds a byte at a guess; the two together are how the game is driven to a known
  place and a thought tried.
- `--dump-ram FRAME:FILE` writes the 64 KiB of work RAM after a frame; `--trace LOW-HIGH:FILE` appends a
  stretch of work RAM after every frame; `--shot-every N:PFX` writes pictures of the board.
- The port's own screen is the last word: a `--poke` is kept only when the picture agrees with it.

**Getting to a match.** The board boots into an attract show and a demo fight. A movie that presses
Coin then Start, or made-up play (`--auto SEED`) which coins and starts by itself, reaches a real,
player-controlled match at about frame 2200 (the demo fight runs first). Every trace below is taken over
such a run.

**The player records.** A `--trace` of the whole record region over a match (an `--auto` run to frame
4600) was searched for the bytes that fall as a player is hit and read 0 outside a round: a 0x400-byte
record whose health word steps down, and player 2's the same record 0x400 on. The two independent cheat
collections put this game's health at `FF83F9`/`FF87F9`, which a `--poke` of to 0 confirmed: it emptied
that player's bar and ended the round at once.

**The health the bars are drawn from** is the same value again at `+0x2B` (`FF83FB`/`FF87FB`): the trace
shows it trailing the first by a step or two as a hit is taken, which is the bar being drawn down. The
cheats hold both together, as the collection does.

**A player's place across the stage** is the word at `+0x04` (`FF83D4`/`FF87D4`): over a movie that walks
and jumps it ramps up and down with the walk, and holding it at `0x0300` with `--poke` moved the character
to the right of the other, which fixed both the address and its direction (larger is further right). The
word at `+0x08` (`FF83D8`/`FF87D8`) is the height above the floor: it sits at about `0x28` on the ground
and rises with a jump.

**The credits** are a byte at `FF82E9`: holding it at 1 and at 2 with `--poke` changed the title from
"INSERT COIN." to "PUSH 1P OR 2P START" ("PUSH 1P START" with one), with "CREDIT 2" at the foot of the
picture. FinalBurn Neo names other fields of the same small block (`FF82ED`, `FF82CF`).

**The clock** is the byte at `FF8CCE`, its BCD digits counting down a step at a time (`0x99` is "99") and
0 outside a round. A `--trace` across a match found it counting down; holding it at `0x99` with `--poke`
stood the digits still, and FinalBurn Neo's "Infinite Time" cheat names the same address and value.

## The game's memory

| Name | Address | Size | What it is | How it was found |
| --- | --- | --- | --- | --- |
| Credits | `FF82E9` | byte | What the title reads as "CREDIT n" before it offers Start; two make a 2-player start | `--poke`d 1 and 2: the title changed from "INSERT COIN." to "PUSH 1P OR 2P START", "CREDIT 2" |
| Round clock | `FF8CCE` | byte (BCD) | The digits the round counts down, 0 outside a round | `--trace`d a match for a byte counting down; holding it at `0x99` stood the digits still. FinalBurn Neo names the same |
| Player 1 record | `FF83D0` | 0x400 | Player 1's record (player 2's is 0x400 on, at `FF87D0`) | A 0x400-byte record whose health falls on a hit and reads 0 outside a round |
| Player record base | `FF83D0` (+`FF87D0`) | — | Player 1 (player 2)'s record, the two 0x400 apart | Found with the health below; the record holds much more than is named here |
| +`0x04` | `FF83D4`/`FF87D4` | word | **Place across the stage**: larger is further right | `--trace`d a walk-and-jump movie (the word ramps with the walk); `--poke`d `0x0300` moved the character past the other |
| +`0x08` | `FF83D8`/`FF87D8` | word | **Height above the floor**: about `0x28` on the ground, larger in the air | The same trace: the word sits at the floor and rises with a jump |
| +`0x29` | `FF83F9`/`FF87F9` | byte | **Health**: `0x90` at a round's start, `0xFF` once beaten, 0 outside a round | `--trace`d a match (it falls a step at a time); a `--poke` of it to 0 emptied the bar and ended the round. Both collections name the address |
| +`0x2B` | `FF83FB`/`FF87FB` | byte | The same health again: **what the bar is drawn from**, draining a step at a time | Trails the health byte on every hit; the cheats hold both together |

The health bytes are 0 outside a round (and in the attract show) and 0xFF once a player is beaten, which
is what the features use to tell a round being fought from everything else. No facing flag of its own has
been confirmed yet, so which way a player faces is taken from the two records' places across the stage
(the one further right faces left).

## Features built on it

| Feature | Category | What it does |
| --- | --- | --- |
| `free_play` | Game (Enhanced) | Holds two credits, so Start works without a coin and a second player can join. |
| `no_damage` | Cheats | Puts player 1's health back to the round's full value every frame (`+0x29` and its drawn copy `+0x2B`). |
| `no_damage_2` | Cheats | The same for player 2 (the opponent's, in a one-player game). |
| `infinite_time` | Cheats | Holds the round's clock where it stood when the round began, so it never runs out. |
| `one_hit` | Cheats | Leaves the opponent (player 2) a single point of health, so any hit ends the round. |
| `input_display` | Training | What each player holds, and for how long, at their side of the picture: the toolkit's `fight::InputHistory`, fed from the port's own panel and the game's facing (taken from the two players' places across the stage), so the notation is turned to the player's side. |
| `training_mode` / `end_training` | Training | A session the menu starts and ends: both fighters' health is held full and the clock stands, with a readout of each fighter's health and the last hit taken. |

Each was checked with no window against a run of the board, with `--shot-every` pictures:

- `free_play`: a movie pressing Start with **no** coin press reaches the character select (the title
  reads "PUSH 1P START"), where without the feature the same movie stays on "INSERT COIN.".
- `no_damage` / `no_damage_2`: over made-up play both bars stay full through a round that knocks a fighter
  down, where the same run without the features drains them.
- `infinite_time`: the round's digits stand still while the board runs on.
- `one_hit`: player 2's bar is held all but empty, so the first hit ends the round.
- `input_display`: the HUD shows each hold and for how long (e.g. a row of `LP MK`, `LP`, `LP HP` with
  their frame counts for player 1, turned so that "6" is towards the way the player faces).
- `training_mode`: the HUD shows "TRAINING" with each fighter's health, and the bars and the clock hold
  where the round began.

## Next

The rest of the training aids the sibling ports have want, at least:

- **The state word**: what a player is doing (standing, walking, crouching, in the air, attacking,
  reeling). The record is found and its places named, but the state is not yet told apart; the toolkit's
  `fight::StepView` wants exactly that for the frame data, the combo readout and the dummy.
- **The attack boxes**: the tables the game's own hit tests read, for the hit boxes drawn on the picture.
- **A rewrite of the damage rule** (`docs/READABLE.md`) for a cleaner `one_hit` than holding the health at
  one point, as the other ports have.
- **The widescreen picture**: the camera and the stage scrollers, and a `widescreen.cpp`.

They are found the way the above were — `--dump-ram` with `tools/ramfind.py`, `--watch` against
`work/ssf2/program.lst`, `--trace` on a run driven to a known place by a `--movie`, and `--poke` to try a
guess — and each is written up here with how it was found before it is used.
