// Copyright 2026 Shrey Industries
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "animation.h" // Boot animation header

// --- ANIMATION BRIDGE (Resolves symbol mismatches with animation.h) ---
#define anim_frames       frame_allArray
#define TOTAL_ANIM_FRAMES frame_allArray_LEN

// Frame delays in ms (~67ms per frame = 15 FPS across 26 frames)
const uint16_t PROGMEM frame_delays[] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 175, 100, 100,
    100, 100, 100, 100, 100, 200
};

#define OLED_FRAME_BYTES ((128 * 32) / 8)  // width * height / 8 bits-per-byte

// --- LAYERS ---
enum layer_names {
    _OFFICE,
    _STUDY,
    _GAME,
    _SETTINGS
};

// --- CUSTOM KEYCODES ---
enum custom_keycodes {
    AP_GITHUB = SAFE_RANGE,
    AP_VSCODE,
    AP_PORTAL,
    AP_UNDERGLOW,
    AP_LIGHTMATRIX,
    AP_OLEDDISPLAY
};

// --- OLED STYLES ---
typedef enum {
    OLED_STYLE_MENU,      // Default menu list
    OLED_STYLE_VERTICAL,  // Compact vertical layout
    OLED_STYLE_STATS,     // Minimalist stats view
    OLED_STYLE_COUNT      // Total available styles
} oled_style_t;

// --- STATE VARIABLES ---
static bool is_animating_boot = true;
static uint16_t frame_timer = 0;
static uint8_t current_anim_frame = 0;

static bool is_encoder_pressed = false;
static oled_style_t current_oled_style = OLED_STYLE_MENU;

/*
 * PURE ELECTRICAL MATRIX LAYOUT (0-indexed for QMK / 1-indexed in KiCad):
 * Row 0: (0,0) [KiCad R1,C1] | (0,1) [KiCad R1,C2] | (0,2) [KiCad R1,C3]
 * Row 1: (1,0) [KiCad R2,C1] | (1,1) [KiCad R2,C2] | (1,2) [KiCad R2,C3]
 * Row 2: (2,0) [KiCad R3,C1] | (2,1) [KiCad R3,C2] | (2,2) [KiCad R3,C3]
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_OFFICE] = LAYOUT_anypad(
        LGUI(KC_C), LGUI(KC_N), LGUI(KC_F), // Row 0
        LGUI(KC_V), LGUI(KC_Z), LSFT(LGUI(KC_Z)), // Row 1 -- macOS Redo: Cmd+Shift+Z
        AP_VSCODE,  AP_GITHUB, LGUI(KC_S)  // Row 2
    ),

    [_STUDY] = LAYOUT_anypad(
        LGUI(KC_C), LGUI(KC_X), LGUI(KC_F), // Row 0
        LGUI(KC_V), LGUI(KC_Z), LSFT(LGUI(KC_Z)), // Row 1 -- macOS Redo: Cmd+Shift+Z
        AP_PORTAL,  LSFT(LGUI(KC_T)), LGUI(KC_S)  // Row 2
    ),

    [_GAME] = LAYOUT_anypad(
        KC_W,    KC_A,    KC_S,   // Row 0
        KC_D,    KC_E,    KC_R,   // Row 1
        KC_LSFT, KC_LCTL, KC_SPC  // Row 2
    ),

    [_SETTINGS] = LAYOUT_anypad(
        AP_UNDERGLOW, KC_NO,          KC_NO,          // Row 0
        KC_NO,        AP_LIGHTMATRIX, AP_OLEDDISPLAY, // Row 1
        KC_NO,        KC_NO,          KC_NO           // Row 2
    ),
};

// --- PROCESS KEYPRESSES & MACROS ---
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.key.row == 2 && record->event.key.col == 0) {
        is_encoder_pressed = record->event.pressed;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case AP_GITHUB:
                SEND_STRING(SS_LGUI("t") "github.com" SS_TAP(X_ENTER));
                return false;
            case AP_VSCODE:
                SEND_STRING(SS_LGUI(" ") "Visual Studio Code" SS_TAP(X_ENTER));
                return false;
            case AP_PORTAL:
                SEND_STRING(SS_LGUI("t") "myolsd.us" SS_TAP(X_ENTER));
                return false;

            case AP_UNDERGLOW:
                #ifdef RGB_MATRIX_ENABLE
                    rgb_matrix_toggle();
                #endif
                return false;

            case AP_LIGHTMATRIX:
                #ifdef RGB_MATRIX_ENABLE
                    rgb_matrix_step();
                #endif
                return false;

            case AP_OLEDDISPLAY:
                #ifdef OLED_ENABLE
                    current_oled_style = (oled_style_t)((current_oled_style + 1) % OLED_STYLE_COUNT);
                    oled_clear();
                #endif
                return false;
        }
    }
    return true;
}

// --- ROTARY ENCODER CONTROLS ---
bool encoder_update_user(uint8_t index, bool clockwise) {
    (void)index;

    uint8_t current_layer = get_highest_layer(layer_state);

    if (is_encoder_pressed) {
        if (clockwise) {
            layer_move((current_layer + 1) % 4);
        } else {
            layer_move((current_layer + 3) % 4);
        }
        return false;
    }

    if (current_layer == _GAME) {
        tap_code(clockwise ? MS_WHLD : MS_WHLU);
    } else if (current_layer == _SETTINGS) {
        #ifdef RGB_MATRIX_ENABLE
            clockwise ? rgb_matrix_increase_hue() : rgb_matrix_decrease_hue();
        #endif
    } else {
        tap_code(clockwise ? KC_VOLU : KC_VOLD);
    }

    return false;
}

// --- OLED DISPLAY DRIVER ---
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {
    if (is_animating_boot) {
        if (frame_timer == 0) {
            frame_timer = timer_read();
        }

        uint16_t delay = pgm_read_word(&frame_delays[current_anim_frame]);

        if (timer_elapsed(frame_timer) >= delay) {
            current_anim_frame++;
            frame_timer = timer_read();

            if (current_anim_frame >= TOTAL_ANIM_FRAMES) {
                is_animating_boot = false;
                current_anim_frame = 0;
                frame_timer = 0;
                oled_clear();
                return false;
            }
        }

        const char *frame_ptr = (const char *)pgm_read_ptr(&anim_frames[current_anim_frame]);
        oled_write_raw_P(frame_ptr, OLED_FRAME_BYTES);
        return false;
    }

    uint8_t current_layer = get_highest_layer(layer_state);

    switch (current_oled_style) {
        case OLED_STYLE_MENU:
            oled_write_P(PSTR("     THE ANYPAD\n"), false);
            oled_write_P(PSTR("---------------------\n"), false);
            oled_write_P(PSTR(" 1. OFFICE   \n"), (current_layer == _OFFICE));
            oled_write_P(PSTR(" 2. STUDY    \n"), (current_layer == _STUDY));
            oled_write_P(PSTR(" 3. GAME     \n"), (current_layer == _GAME));
            oled_write_P(PSTR(" 4. SETTINGS \n"), (current_layer == _SETTINGS));
            break;

        case OLED_STYLE_VERTICAL:
            oled_write_P(PSTR("ANYPAD\n======\nLAYER:\n"), false);
            switch (current_layer) {
                case _OFFICE:   oled_write_P(PSTR("> OFFICE\n"), false); break;
                case _STUDY:    oled_write_P(PSTR("> STUDY \n"), false); break;
                case _GAME:     oled_write_P(PSTR("> GAME  \n"), false); break;
                case _SETTINGS: oled_write_P(PSTR("> SETTNG\n"), false); break;
            }
            break;

        case OLED_STYLE_STATS:
            oled_write_P(PSTR("[CURRENT MODE]\n> "), false);
            if (current_layer == _OFFICE)       oled_write_P(PSTR("OFFICE WORK\n"), false);
            else if (current_layer == _STUDY)    oled_write_P(PSTR("STUDY SESSION\n"), false);
            else if (current_layer == _GAME)     oled_write_P(PSTR("GAMING MODE\n"), false);
            else                                 oled_write_P(PSTR("SYSTEM CONFIG\n"), false);
            break;

        default:
            break;
    }

    return false;
}
#endif