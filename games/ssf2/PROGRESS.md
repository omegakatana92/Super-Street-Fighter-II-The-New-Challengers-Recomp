# Super Street Fighter II — the game's memory, and how it was found

This file records what the port knows about *Super Street Fighter II: The New Challengers* (MAME set
`ssf2`, World 931005) beyond its program: where the game keeps a thing in RAM, and **how that was
found** — a RAM diff, a write watch, a trace. Never the data itself (no dumps, no ripped art).

Nothing game-specific is built yet, so the table below is empty and the port runs exactly as the
board. This is the same starting point the sibling cps2recomp ports (Hyper Street Fighter II, Street
Fighter Alpha, Street Fighter Alpha 3) had at their first release; the addresses they found are the
shape of what goes here.

## The board

The CPS-2 board this port models is in `cps2recomp/machine/`; the ROM set's layout is in
`games/ssf2/game.cfg`, and the two together are all the port needs to run the game.

- The 68000 program is five 512 KiB ROMs (`ssfe-03b`, `ssfe.04`, `ssfe.05`, `ssfe-06b`, `ssfe.07`),
  2.5 MiB in all, byte-swapped into words and deciphered with the set's key, exactly as the board does.
- The graphics are eight ROMs in two rows of four (`ssf.13m/15m/17m/19m`, then `ssf.14m/16m/18m/20m`).
- The Z80 sound program is one 128 KiB ROM (`ssf-01a`); the QSound samples are `ssf.q01` .. `ssf.q08`.

## The game's memory

| Name | Address | Size | What it is | How it was found |
| --- | --- | --- | --- | --- |
| — | — | — | not found yet | — |

## Next

The cheats and training aids of the sibling ports want, at least:

- The credits word, so **free play** can offer Start without a coin.
- Each player's record: **health** (and the value the bar is drawn from), the place across the stage,
  the facing, the state word, and the value behind the meter/super gauge.
- The round's **clock**.

They are found the way the toolkit's `docs/FEATURES.md` says — `--dump-ram` with `tools/ramfind.py`,
`--watch` against `work/ssf2/program.lst`, `--trace` on a run driven to a known place by a `--movie`,
and `--poke` to try a guess — and each is written up here with how it was found before it is used.
