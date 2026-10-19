# Super Street Fighter II Recomp — changelog

## 0.1.0-beta.1

First build. The 68000 and Z80 programs of *Super Street Fighter II: The New Challengers* (World
931005) are translated to C++ ahead of time and run on cps2recomp's own machine. The launcher, the
in-game menu, save states, run-ahead, the display options and every control are in, and every key and
button is remappable.

The game's own features (the cheats and the training aids the sibling cps2recomp ports have) are still
to come; they wait on addresses found on the running board, recorded in
[games/ssf2/PROGRESS.md](games/ssf2/PROGRESS.md) as they are found. See [ROADMAP.md](ROADMAP.md).

The release archives carry no game data: the player brings their own `ssf2` ROM set, which the port only
reads.
