# Super Street Fighter II Recomp — changelog

## 0.1.0-beta.2

The game's own features, built on addresses found on the running board with the toolkit's own tools
(`--movie`, `--dump-ram`, `--poke`, `--trace`) and written up in
[games/ssf2/PROGRESS.md](games/ssf2/PROGRESS.md):

- **Free play** (Game) — the title's credit word is held at two, so Start works with no coin and a
  second player can join, as on a board with free play set.
- **Cheats** — no damage for either player, infinite time (the round's clock stands), and one-hit
  knockouts (the opponent goes down with the first hit).
- **Input display** (Training) — what each player holds, and for how long, at their side of the
  picture, turned so that "6" is always towards the way the player faces.
- **Training mode** (Training) — a session the menu starts and ends: both fighters' health is held
  full and the round's clock stands, with a readout of each fighter's health and the last hit taken.

This game has no SUPER meter (*Super Street Fighter II Turbo* added it, the year after), so there is no
"full gauge" cheat. The rest of the training aids the sibling ports have wait on more of the game's own
memory being known. `make check`, `make check-toolkit` and `make check-common` all pass.

## 0.1.0-beta.1

First build. The 68000 and Z80 programs of *Super Street Fighter II: The New Challengers* (World
931005) are translated to C++ ahead of time and run on cps2recomp's own machine. The launcher, the
in-game menu, save states, run-ahead, the display options and every control are in, and every key and
button is remappable.

The game's own features (the cheats and the training aids the sibling cps2recomp ports have) were still
to come; they wait on addresses found on the running board, recorded in
[games/ssf2/PROGRESS.md](games/ssf2/PROGRESS.md) as they are found. See [ROADMAP.md](ROADMAP.md).

The release archives carry no game data: the player brings their own `ssf2` ROM set, which the port only
reads.
