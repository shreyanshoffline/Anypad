// Copyright 2026 Shrey Industries // SPDX-License-Identifier: GPL-2.0-or-later 
#include QMK_KEYBOARD_H

enum layer_names {
    _OFFICE,
    _STUDY,
    _GAME,
};

// Custom keycodes for placeholder "launch" macros.
enum custom_keycodes {
    AP_GITHUB = SAFE_RANGE, // opens a new browser tab to github.com
    AP_VSCODE,              // Spotlight-launches VS Code (macOS)
    AP_PORTAL,              // opens your school/LMS portal
};

/*
 * Physical layout (as populated on the PCB):
 * 
 *   ┌────┐   ┌────┐   ┌────┐
 *   │SW1 │   │SW5 │   │SW6 │  <- Top Row Layout
 *   └────┘   └────┘   └────┘
 *   ┌────┐   ┌────┐   ┌────┐
 *   │SW2 │   │SW3 │   │SW4 │  <- Middle Row Layout
 *   └────┘   └────┘   └────┘
 *   ┌────┐   ┌────┐   ┌────┐
 *   │SW7 │   │SW8 │   │SW9 │  <- Bottom Row Layout
 *   └────┘   └────┘   └────┘
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* OFFICE 
     * Top Row:    Save (SW1) | VSCode (SW5) | GitHub (SW6)
     * Middle Row: Copy (SW2) | Paste (SW3)  | Undo (SW4)
     * Bottom Row: Redo (SW7) | Find (SW8)   | New (SW9)
     */
    [_OFFICE] = LAYOUT_anypad(
        LGUI(KC_S), AP_VSCODE,  AP_GITHUB,
        LGUI(KC_C), LGUI(KC_V), LGUI(KC_Z),
        LSFT(LGUI(KC_Z)), LGUI(KC_F), LGUI(KC_N)
    ),

    /* STUDY 
     * Top Row:    Copy (SW1)  | NewTab (SW5)  | Save (SW6)
     * Middle Row: Paste (SW2) | Cut (SW3)     | CloseTab (SW4)
     * Bottom Row: PrevTab (SW7)| NextTab (SW8) | Portal (SW9)
     */
    [_STUDY] = LAYOUT_anypad(
        LGUI(KC_C), LGUI(KC_T), LGUI(KC_S),
        LGUI(KC_V), LGUI(KC_X), LGUI(KC_W),
        LSFT(LGUI(KC_LBRC)), LSFT(LGUI(KC_RBRC)), AP_PORTAL
    ),

    /* GAME 
     * Top Row:    W (SW1)     | E (SW5)    | R (SW6)
     * Middle Row: A (SW2)     | S (SW3)    | D (SW4)
     * Bottom Row: Shift (SW7) | Ctrl (SW8) | Space (SW9)
     */
    [_GAME] = LAYOUT_anypad(
        KC_W,    KC_E,    KC_R,
        KC_A,    KC_S,    KC_D,
        KC_LSFT, KC_LCTL, KC_SPC
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

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
    }
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        uint8_t current_layer = get_highest_layer(layer_state);
        if (clockwise) {
            layer_move((current_layer + 1) % 3);
        } else {
            layer_move((current_layer + 2) % 3);
        }
    }
    return false;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {
    // 5 spaces added ahead of the text to center it across the 21-character line limit
    oled_write_P(PSTR("     THE ANYPAD\n"), false);

    // Thin continuous divider line spanning the full 21-character width
    oled_write_P(PSTR("─────────────────────\n"), false);

    uint8_t current_layer = get_highest_layer(layer_state);

    // Menu block selections containing smart boolean line highlighters
    oled_write_P(PSTR(" 1. OFFICE \n"), (current_layer == _OFFICE));
    oled_write_P(PSTR(" 2. STUDY  \n"), (current_layer == _STUDY));
    oled_write_P(PSTR(" 3. GAME   \n"), (current_layer == _GAME));

    return false;
}
#endif
