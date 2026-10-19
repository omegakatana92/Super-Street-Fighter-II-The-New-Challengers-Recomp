Super Street Fighter II - recomp port (Windows x64)
===================================================

A native PC port of Super Street Fighter II: The New Challengers (Capcom, 1993, CPS-2 board), made
with tmcleroy's cps2recomp (https://github.com/tmcleroy/cps2recomp) by static recompilation.
NOT affiliated with, endorsed by or sponsored by Capcom. Super Street Fighter II is a trademark of
Capcom Co., Ltd.

NO GAME FILES ARE INCLUDED. You need your own copy of the game: the arcade ROM set "ssf2"
(World, 931005), from a board you own. This archive cannot help you get one. Other regions and
revisions (ssf2r1, ssf2u, ssf2a, ssf2ar1, ssf2j, ssf2jr1, ssf2h, ssf2b, ssf2us2) do not work and
are refused on purpose.


How to play
-----------
1. Double-click ssf2-launcher.exe.
2. Choose your ROM set: point the launcher at the folder that holds the set's files, or at
   the set's .zip. It checks every file by CRC-32 before it will run.
3. Set the controls and any features you want, then Play.

In game: Esc or F1 opens the menu (save states, Insert coin, Player 2 joins, Reset, Quit).
P, O and -/= stop, step and slow the game. Everything is remappable in the launcher.

The port runs the game exactly as the board does until you switch a feature on. The game's own features
are in: free play, no damage for either player, infinite time, one-hit knockouts, an input display and a
Training session (health held full, the clock standing, with a readout). This game has no SUPER meter
(Turbo added it, the year after), so there is no "full meter" here. See the project's ROADMAP.md.


What is in here
---------------
ssf2.exe                 the game
ssf2-launcher.exe        the launcher (keep it beside the game)
SDL3.dll                \
zlib1.dll                >  the runtime libraries the two programs need
libgcc_s_seh-1.dll     /   (keep them all in this folder, beside the .exe files)
libstdc++-6.dll       /
libwinpthread-1.dll  /
licenses/              the licences of the port and of everything shipped with it

Keep the .exe and .dll files together in one folder. Save states, settings and the board's
eprom are kept beside them (or wherever you run the game from).

The port is GPL-3.0; its source, and how it was made, are at
https://github.com/omegakatana92/Super-Street-Fighter-II-The-New-Challengers-Recomp
You can also build it yourself from that source. To play, you still need your own ROM set.
