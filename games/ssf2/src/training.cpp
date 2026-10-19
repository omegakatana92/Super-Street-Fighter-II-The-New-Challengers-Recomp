// SPDX-License-Identifier: GPL-3.0-only
//
// Super Street Fighter II: the training aids, put together from the toolkit's fight/ parts (fight/fight.h)
// and the game's own memory (ssf2.h).
//
// The input display needs only what the port read off the panel and the game's facing (taken from the two
// players' places across the stage), so it is the first one; the others the fight/ parts want (frame data,
// the combo readout, the dummy, the hit boxes) wait on more of the game's own memory being known: the
// players' records are found, with their health and places picked out, but what a player is doing (the state
// word) is not yet told apart, and the second thing they want is the attack boxes themselves (PROGRESS.md).
//
// "Training mode" is a session the menu starts and ends (a command each). While one is on, both fighters'
// health is held full and the round's clock stands, so a round can be practised in for as long as the player
// likes, and a readout shows each fighter's health and the last hit taken.
#include "ssf2.h"

#include "fight.h"
#include "machine.h"
#include "state.h"

#include <cstdio>

namespace cps2recomp {

namespace {
    using namespace ssf2;

    fight::InputHistory g_inputs;

    // A training session is on: what the port calls one (the aids that are kFeatureTraining work then).
    bool g_training = false;

    // A round is being fought when both players' health is a sensible amount (ssf2.h): outside a round the
    // record holds 0, and a beaten player's reads 0xFF.
    bool InRound() {
        for (int n = 0; n < 2; n++) {
            const uint8_t health = Ram8(Player(n) + kHealth);
            if (health == 0 || health == 0xFF) {
                return false;
            }
        }
        return true;
    }

    // Which way a player faces: the other is towards it when its place across the stage is the larger (the one
    // further right faces left). It is only meaningful while a round is fought (outside one the game holds 0
    // in the records), so a player outside one is taken to face right and the notation is left as it is.
    bool FacingLeft(int n) {
        if (!InRound()) {
            return false;
        }
        return Ram16(Player(n) + kPositionX) > Ram16(Player(1 - n) + kPositionX);
    }

    void ReadInputs(Machine &machine) {
        for (int n = 0; n < 2; n++) {
            g_inputs.Push(n, machine.players[n], FacingLeft(n));
        }
    }

    // What the round began with: the full health of each player (learned the first frames it is seen) and the
    // clock the round began at. 0: no round has been seen yet.
    uint8_t healthAtStart[2] = {0, 0};
    uint8_t clockAtStart = 0;

    // The readout's own working: the health each player's record held at the end of the step before, the
    // damage of the last hit taken (0: none yet), the step it landed on, and a count of the steps gone by.
    uint8_t healthSeen[2] = {0, 0};
    int lastHit[2] = {0, 0};
    uint32_t lastHitAt[2] = {0, 0};
    uint32_t steps = 0;

    constexpr int kHitShownFor = 150; // steps a hit stays in the readout (two and a half seconds)

    void ForgetRound() {
        healthAtStart[0] = healthAtStart[1] = 0;
        clockAtStart = 0;
        healthSeen[0] = healthSeen[1] = 0;
        lastHit[0] = lastHit[1] = 0;
        lastHitAt[0] = lastHitAt[1] = 0;
    }

    void StartTraining(Machine &) {
        g_training = true;
        g_inputs.Clear();
        ForgetRound();
    }

    void EndTraining(Machine &) {
        g_training = false;
        g_inputs.Clear();
        ForgetRound();
    }

    // Holds the round as it began: both fighters' health held full, and the clock still.
    void KeepTheRound() {
        for (int n = 0; n < 2; n++) {
            const uint8_t health = Ram8(Player(n) + kHealth);
            if (health > healthAtStart[n]) {
                healthAtStart[n] = health;
            }
            // (what the frame took off, before it is put back, is a hit: the round's own measure of one)
            if (healthSeen[n] > health && healthSeen[n] - health < healthAtStart[n]) {
                lastHit[n] = static_cast<int>(healthSeen[n] - health);
                lastHitAt[n] = steps;
            }
            healthSeen[n] = health;
            if (healthAtStart[n] != 0) {
                SetRam8(Player(n) + kHealth, healthAtStart[n]);
                SetRam8(Player(n) + kHealthShown, healthAtStart[n]);
            }
        }
        if (clockAtStart == 0) {
            clockAtStart = Ram8(kClock);
        }
        if (clockAtStart != 0) {
            SetRam8(kClock, clockAtStart);
        }
    }

    void Readout(std::vector<HudLine> &lines) {
        for (int n = 0; n < 2; n++) {
            const unsigned health = healthAtStart[n] != 0 ? healthAtStart[n] : Ram8(Player(n) + kHealth);
            char text[64];
            if (lastHit[n] != 0 && steps - lastHitAt[n] < kHitShownFor) {
                snprintf(text, sizeof(text), "P%d  health %u  (hit -%d)", n + 1, health, lastHit[n]);
            } else {
                snprintf(text, sizeof(text), "P%d  health %u", n + 1, health);
            }
            lines.push_back({n == 0 ? 0.02f : 0.98f, 0.075f, n == 0 ? -1 : 1, text});
        }
    }
} // namespace

void TrainingBeforeFrame(Machine &machine) {
    ReadInputs(machine);
}

void TrainingFrame(Machine &) {
    steps++;
    std::vector<HudLine> lines;
    if (g_training) {
        lines.push_back({0.5f, 0.02f, 0, "TRAINING"});
        if (!InRound()) {
            ForgetRound();
        } else {
            KeepTheRound();
        }
        Readout(lines);
    }
    if (FeatureOn("input_display")) {
        g_inputs.Hud(0, lines);
        g_inputs.Hud(1, lines);
    }
    SetHud(lines);
}

void TrainingState(Archive &ar) {
    ar.Pod(g_training);
    ar.Pod(healthAtStart[0]);
    ar.Pod(healthAtStart[1]);
    ar.Pod(clockAtStart);
    // (the input display's own frame counts are not part of the save state: a load starts the display over)
    if (!ar.Saving()) {
        g_inputs.Clear();
        ForgetRound();
    }
}

// Whether a training session is on (runtime/game.h: the aids that are kFeatureTraining work only then)
bool TrainingOn() {
    return g_training;
}

void RegisterTraining() {
    Game &game = TheGame();
    game.features.push_back({"input_display", "Training", "Input display",
                             "What each player holds, and for how long, at their side of the picture."});
    game.commands.push_back({"training_mode", "Training", "Training mode",
                             "Both fighters' health is held full and the round's clock stands, so a round can "
                             "be practised in as long as you like; the health and the last hit are read out "
                             "beside the picture.",
                             StartTraining, kKeyNone});
    game.commands.push_back(
        {"end_training", "Training", "End training", "The round is left to run as it is again.", EndTraining, kKeyNone});
    game.inTraining = TrainingOn;
}

} // namespace cps2recomp
