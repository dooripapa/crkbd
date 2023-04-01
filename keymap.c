/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>


#if 0
const uint16_t PROGMEM cap_combo1[] = {KC_U     , KC_I   , COMBO_END}; // 스마트 대소문자
const uint16_t PROGMEM num_combo1[] = {KC_LGUI  , KC_LALT, COMBO_END}; // 숫자로키로 맵핑
combo_t key_combos[COMBO_COUNT] = {
         COMBO(cap_combo1, CW_TOGG)
        ,COMBO(num_combo1, MO(2))
};
#endif


#define TIMEOUT_MS 200  // Timeout in milliseconds.
#define RECENT_SIZE 2    // Number of keys in `recent` buffer.

static uint16_t recent[RECENT_SIZE] = {KC_NO};
static uint16_t deadline = 0;

static void clear_recent_keys(void) {
    memset(recent, 0, sizeof(recent));  // Set all zeros (KC_NO).
}

// Handles one event. Returns true if the key was appended to `recent`.
static bool update_recent_keys(uint16_t keycode, keyrecord_t* record) {
    if (!record->event.pressed) { return false; }

    if (((get_mods() | get_oneshot_mods()) & ~MOD_MASK_SHIFT) != 0) {
        clear_recent_keys();  // Avoid interfering with hotkeys.
        return false;
    }

    // Handle tap-hold keys.
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            if (record->tap.count == 0) { return false; }
            keycode &= 0xff;  // Get tapping keycode.
    }

    switch (keycode) {
        case KC_A ... KC_I:  // These keys type letters, digits, symbols.
            return false;
        case KC_J ... KC_K:
            break;
        case KC_L ... KC_SLASH:  // These keys type letters, digits, symbols.
            return false;

        case KC_LSFT:  // These keys don't type anything on their own.
        case KC_RSFT:
        case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
            return false;

        default:  // Avoid acting otherwise, particularly on navigation keys.
            clear_recent_keys();
            return false;
    }

    // Slide the buffer left by one element.
    memmove(recent, recent + 1, (RECENT_SIZE - 1) * sizeof(*recent));

    recent[RECENT_SIZE - 1] = keycode;
    deadline = record->event.time + TIMEOUT_MS;
    return true;
}

void matrix_scan_user(void) {
    if (recent[RECENT_SIZE - 1] && timer_expired(timer_read(), deadline)) {
        clear_recent_keys();  // Timed out; clear the buffer.
    }
}

#if 0
static bool process_tap_or_long_press_key(
    keyrecord_t* record, uint16_t long_press_keycode) {
    if (record->tap.count == 0) {  // Key is being held.
        if (record->event.pressed) {
            tap_code16(long_press_keycode);
        }
        return false;  // Skip default handling.
    }
    return true;  // Continue default handling.
}
#endif


bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        // 왼족
//        case KC_Q:
        case KC_W:
        case KC_E:
        case KC_R:
        case KC_T:

//        case KC_A:
        case KC_S:
        case KC_D:
        case KC_F:
        case KC_G:

//        case KC_Z:
//        case KC_X:
        case KC_C:
        case KC_V:
        case KC_B:

        // 오른쪽
//        case KC_I:
//        case KC_P:

            return true;
        default:
            return false;
    }
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch(keycode) {
        // 왼족
        case KC_Q:
            register_code16((!shifted) ? KC_Q : KC_NO);
            break;
        case KC_W:
            register_code16((!shifted) ? KC_W : KC_AT);
            break;
        case KC_E:
            register_code16((!shifted) ? KC_E : KC_HASH);
            break;
        case KC_R:
            register_code16((!shifted) ? KC_R : KC_ASTR);
            break;
        case KC_T:
            register_code16((!shifted) ? KC_T : KC_AMPR);
            break;

        case KC_A:
            register_code16((!shifted) ? KC_A : KC_NO);
            break;
        case KC_S:
            register_code16((!shifted) ? KC_S : KC_EXLM);
            break;
        case KC_D:
            register_code16((!shifted) ? KC_D : KC_CIRC);
            break;
        case KC_F:
            register_code16((!shifted) ? KC_F : KC_DLR);
            break;
        case KC_G:
            register_code16((!shifted) ? KC_G : KC_PERC);
            break;

        case KC_Z:
            register_code16((!shifted) ? KC_Z : KC_NO);
            break;
        case KC_X:
            register_code16((!shifted) ? KC_X : KC_NO);
            break;
        case KC_C:
            register_code16((!shifted) ? KC_C : KC_LPRN);
            break;
        case KC_V:
            register_code16((!shifted) ? KC_V : KC_RPRN);
            break;
        case KC_B:
            register_code16((!shifted) ? KC_B : KC_NO);
            break;

        default:
            if (shifted) {
                add_weak_mods(MOD_BIT(KC_LSFT));
                add_weak_mods(MOD_BIT(KC_RSFT));
            }
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch(keycode) {
        // 왼족
        case KC_Q:
            unregister_code16((!shifted) ? KC_Q : KC_NO);
            break;
        case KC_W:
            unregister_code16((!shifted) ? KC_W : KC_AT);
            break;
        case KC_E:
            unregister_code16((!shifted) ? KC_E : KC_HASH);
            break;
        case KC_R:
            unregister_code16((!shifted) ? KC_R : KC_ASTR);
            break;
        case KC_T:
            unregister_code16((!shifted) ? KC_T : KC_AMPR);
            break;

        case KC_A:
            unregister_code16((!shifted) ? KC_A : KC_NO);
            break;
        case KC_S:
            unregister_code16((!shifted) ? KC_S : KC_EXLM);
            break;
        case KC_D:
            unregister_code16((!shifted) ? KC_D : KC_CIRC);
            break;
        case KC_F:
            unregister_code16((!shifted) ? KC_F : KC_DLR);
            break;
        case KC_G:
            unregister_code16((!shifted) ? KC_G : KC_PERC);
            break;

        case KC_Z:
            unregister_code16((!shifted) ? KC_Z : KC_NO);
            break;
        case KC_X:
            unregister_code16((!shifted) ? KC_X : KC_NO);
            break;
        case KC_C:
            unregister_code16((!shifted) ? KC_C : KC_LPRN);
            break;
        case KC_V:
            unregister_code16((!shifted) ? KC_V : KC_RPRN);
            break;
        case KC_B:
            unregister_code16((!shifted) ? KC_B : KC_NO);
            break;

            // 오른쪽
        case KC_I:
            unregister_code16((!shifted) ? KC_I : LSFT(KC_I) );
            break;
        case KC_P:
            unregister_code16((!shifted) ? KC_P : LSFT(KC_P) );
            break;

        default:
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            // The IS_RETRO check isn't really necessary here, always using
            // keycode & 0xFF would be fine.
            unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

//KC_QUOT
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_split_3x6_3(
KC_TAB    , KC_Q     , KC_W    , KC_E     , KC_R     , KC_T        , KC_Y         , KC_U     , KC_I     , KC_O     , KC_P     , KC_BSPC  ,
KC_RSFT   , KC_A     , KC_S    , KC_D     , KC_F     , KC_G        , KC_H         , KC_J     , KC_K     , KC_L     , KC_SCLN  , KC_RSFT  ,
KC_LCTL   , KC_Z     , KC_X    , KC_C     , KC_V     , KC_B        , KC_N         , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_ENT   ,
//                                 KC_LGUI  , KC_LALT  , LT(1,KC_SPC), LT(2,KC_SPC) , KC_RALT  , KC_RCTL
                                 KC_LALT  , KC_LGUI  , LT(1,KC_SPC), LT(2,KC_SPC) , KC_RALT  , KC_RCTL
),

[1] = LAYOUT_split_3x6_3(
KC_TRNS   , KC_NO    , KC_NO    , KC_NO    , KC_NO    , CW_TOGG     , RCS(KC_LEFT) , LSFT(KC_HOME)  , LSFT(KC_END)   , RCS(KC_RGHT) , KC_GRV   , KC_DEL   ,
KC_TRNS   , KC_NO    , KC_NO    , KC_LCTL  , KC_LSFT  , MO(2)       , KC_LEFT      , KC_DOWN        , KC_UP          , KC_RGHT      , KC_QUOT  , KC_PGUP  ,
KC_TRNS   , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO       , KC_MINS      , KC_EQL         , KC_LBRC        , KC_RBRC      , KC_BSLS  , KC_PGDN  ,
                                  KC_TRNS  , KC_TRNS  , KC_TRNS     , LSFT(KC_DEL) , LCTL(KC_INS)   , LSFT(KC_INS)
),

[2] = LAYOUT_split_3x6_3(
KC_TRNS   , KC_F9    , KC_F10   , KC_F11   , KC_F12   , KC_NO      , KC_NO        , KC_7     , KC_8     , KC_9     , KC_0     , KC_TRNS   ,
KC_TRNS   , KC_F5    , KC_F6    , KC_F7    , KC_F8    , KC_TRNS    , KC_NO        , KC_4     , KC_5     , KC_6     , KC_NO    , KC_TRNS   ,
KC_TRNS   , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_NO      , KC_NO        , KC_1     , KC_2     , KC_3     , KC_NO    , KC_TRNS   ,
                                  KC_LCTL  , KC_LALT  , KC_TRNS    , KC_TRNS      , KC_RALT  , KC_RCTL
)

};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180; // flips the display 180 degrees if offhand
    }
    return rotation;
}

#    define L_BASE 0
#    define L_LOWER 2
#    define L_RAISE 4
#    define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Default"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case L_ADJUST:
        case L_ADJUST | L_LOWER:
        case L_ADJUST | L_RAISE:
        case L_ADJUST | L_LOWER | L_RAISE:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
    }
}

char keylog_str[24] = {};

const char code_to_name[60] = {' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\', '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
    char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        keycode = keycode & 0xFF;
    }
    if (keycode < 60) {
        name = code_to_name[keycode];
    }

    // update keylog
    snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c", record->event.key.row, record->event.key.col, keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }

    if (update_recent_keys(keycode, record)) {
        // Expand "kj" to my email address.
        if (recent[RECENT_SIZE - 2] == KC_J &&
            recent[RECENT_SIZE - 1] == KC_K) {
            tap_code16(KC_BSPC);
            tap_code16(KC_ESC);
            //SEND_STRING(SS_TAP(X_BSPC) SS_TAP(X_BSPC) "myname@email.com");
            return false;
        }
        // Expand "jk" to my phone number.
        if (recent[RECENT_SIZE - 2] == KC_K &&
            recent[RECENT_SIZE - 1] == KC_J) {
            tap_code16(KC_BSPC);
            tap_code16(KC_ESC);
            //SEND_STRING(SS_TAP(X_BSPC) SS_TAP(X_BSPC) "123-546-7890");
            return false;
        }
    }

    switch (keycode) {
        default:
            return true;
    }

    return true;
}
#endif // OLED_ENABLE
