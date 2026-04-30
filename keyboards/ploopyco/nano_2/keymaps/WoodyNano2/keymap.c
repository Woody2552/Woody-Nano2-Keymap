/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "mousekey.h"
#include "pointing_device.h"

enum layers {
    _BASE,
    _SCROLL_L
};

typedef enum {
    TD_NONE,
    TD_SINGLE_TAP,
    TD_DOUBLE_TAP,
    TD_TRIPLE_TAP,
    TD_FIVE_TAPS
} td_state_t;

// --- GLOBAL VARIABLES ---
static td_state_t td_last_state = TD_NONE;
static uint8_t armed_button = 0;      
static uint16_t scrl_button_timer = 0; 

static int16_t vert_scroll_accum = 0;  
static int16_t horiz_scroll_accum = 0; 
#define DEFAULT_SCROLL_DIVIDER 20   // Effects zoom speed

// --- TAP DANCE LOGIC ---
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) return TD_SINGLE_TAP;
    if (state->count == 2) return TD_DOUBLE_TAP;
    if (state->count == 3) return TD_TRIPLE_TAP;
    if (state->count == 5) return TD_FIVE_TAPS;
    return TD_NONE;
}

void btn1_finished(tap_dance_state_t *state, void *user_data) {
    td_last_state = cur_dance(state);
    switch (td_last_state) {
        case TD_SINGLE_TAP:
            if (armed_button == 2) {
                register_code(MS_BTN2);
            } else if (armed_button == 3) {
                register_code(MS_BTN3);
            } else {
                register_code(MS_BTN1);
            }
            break;
        case TD_DOUBLE_TAP:
            armed_button = 2;
            break;
        case TD_TRIPLE_TAP:
            armed_button = 3;
            break;
        case TD_FIVE_TAPS:
            layer_on(_SCROLL_L);
            break;
        default:
            break;
    }
}

void btn1_reset(tap_dance_state_t *state, void *user_data) {
    if (td_last_state == TD_SINGLE_TAP) {
        if (armed_button == 2) {
            unregister_code(MS_BTN2);
            armed_button = 0; 
        } else if (armed_button == 3) {
            unregister_code(MS_BTN3);
            armed_button = 0; 
        } else {
            unregister_code(MS_BTN1);
        }
    }
    td_last_state = TD_NONE;
}

// Move this ABOVE process_record_user so the compiler knows it exists
tap_dance_action_t tap_dance_actions[] = {
    [0] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, btn1_finished, btn1_reset)
};

// --- CORE FUNCTIONS ---
void pointing_device_init_user(void) {
    pointing_device_set_cpi(1000);  // No effect on zoom speed
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_F24: 
            if (record->event.pressed) {
                scrl_button_timer = timer_read(); 
                register_code(KC_LCTL);           
            } else {
                unregister_code(KC_LCTL);         
                
                // If it's a tap (under 125ms)
                if (timer_elapsed(scrl_button_timer) < 125) { 
                    // This turns off ALL layers and returns to 0
                    layer_clear(); 
                    
                    // Reset our custom 'Armed' state
                    armed_button = 0;
                    
                    // We also want to clear any persistent "weak" mods 
                    // that might be lingering from the layer shift.
                    clear_keyboard();
                } 
            }
            return false;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE]     = LAYOUT(TD(0)),
    [_SCROLL_L] = LAYOUT(KC_F24)
};

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (layer_state_is(_SCROLL_L)) {
        vert_scroll_accum += mouse_report.y;
        horiz_scroll_accum += mouse_report.x;
        mouse_report.v = vert_scroll_accum / DEFAULT_SCROLL_DIVIDER;
        mouse_report.h = horiz_scroll_accum / DEFAULT_SCROLL_DIVIDER;
        vert_scroll_accum %= DEFAULT_SCROLL_DIVIDER;
        horiz_scroll_accum %= DEFAULT_SCROLL_DIVIDER;
        mouse_report.x = 0; 
        mouse_report.y = 0;
    } 
    return mouse_report;
}
