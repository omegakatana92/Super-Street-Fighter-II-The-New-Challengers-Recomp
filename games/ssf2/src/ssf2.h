// SPDX-License-Identifier: GPL-3.0-only
//
// Super Street Fighter II: The New Challengers (World 931005): names for what is known of the game's memory.
// Every address here was found by experiment on the running board with the toolkit's own tools (--movie,
// --dump-ram, --poke, --trace) and checked where it could against an independent cheat collection for the
// same ROM set (FinalBurn Neo's, which names the credits, the health and the clock); PROGRESS.md says how
// each one was found. No game data is written down here, only where the game keeps it.
#pragma once

#include "game.h"

#include <cstdint>

namespace ssf2 {

// The credits the game is holding, a byte: the title reads it out at the foot of the picture as "CREDIT n",
// and a start takes one. It is 0 in the attract show, and the title waits for one before it offers "PUSH 1P
// START" (or "PUSH 1P OR 2P START" with two). Found by holding a byte at 1 and at 2 with --poke and watching
// the title change from "INSERT COIN." to "PUSH 1P OR 2P START" with "CREDIT 2" (PROGRESS.md).
constexpr uint32_t kCredits = 0xFF82E9;
constexpr uint8_t kCreditsFree = 2; // (two: a second player can start)

// A player's record: the two are 0x400 apart, player 1's first. Only the parts the features use are named
// here; the record holds much more (the character, the state word, the places, the animation, ...).
constexpr uint32_t kPlayer0 = 0xFF83D0, kPlayerStride = 0x400;
inline constexpr uint32_t Player(int n) {
    return kPlayer0 + static_cast<uint32_t>(n) * kPlayerStride;
}

//   +0x29  the health, a byte (0x90 at the round's start: what a hit takes bits off)
//   +0x2B  the same health again, drawn down a step at a time: what the bars are drawn from
// Both read 0 outside a round (and in the attract show); a beaten player's first reads 0xFF. Found with
// --trace over a match (the two fall together as a player is hit and the second trails the first by a step),
// and a --poke of the first to 0 empties that player's bar and ends the round; the cheat collection names it
// (PROGRESS.md). The features hold both together, as the collection does.
constexpr uint32_t kHealth = 0x29, kHealthShown = 0x2B;

// A player's place across the stage, a word each: +0x04 is across it (larger is further right) and +0x08 is
// the height above the floor (about 0x28 on the ground, larger in the air). Found with --trace over a match
// (both words ramp with the walk, and the second with a jump, and the two fighters' are the same distance
// apart as they stand nose to nose) (PROGRESS.md). Which way a player faces is taken from the two places
// (the one further right faces left): no facing flag of its own has been confirmed yet.
constexpr uint32_t kPositionX = 0x04, kPositionY = 0x08;

// The round's clock, a byte: its BCD digits count down a step at a time (0x99 is "99"), and it is 0 outside a
// round. Holding it at 0x99 with --poke stands the digits still, which is what the cheat does; the cheat
// collection names the same address and value (PROGRESS.md).
constexpr uint32_t kClock = 0xFF8CCE;
constexpr uint8_t kClockFull = 0x99;

} // namespace ssf2
