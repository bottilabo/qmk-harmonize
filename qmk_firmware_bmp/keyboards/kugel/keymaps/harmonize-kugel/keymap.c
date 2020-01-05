/* Copyright 2019 sekigon-gonnoc
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
#include "pointing_device.h"
#include "bmp.h"
#include "bmp_custom_keycode.h"
#include "keycode_str_converter.h"


uint8_t tb_button    = 0;
uint8_t tb_scrl_flag = false;

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  K_XRESET = BMP_SAFE_RANGE,
  K_SCLN,
  K_COMM,
  K_DOT,
  K_SLSH,
  K_XIMMODE,
  K_BSSFT,
  K_LPRN,
  K_RPRN,
  K_REPRT,
  K_VOLUM,
  K_JPKB,
  K_ENKB,
  K_TKANA,
  K_TROMJ,
  K_IM_EN,
  K_IM_JP,
  K_KB_0,
  K_KB_1,
  K_KB_2,
  K_KB_3,
  K_IM_0,
  K_IM_1,
  K_IM_2,
  K_IM_3,
  K_WIN,
  K_MAC,
  K_LNX
};

const key_string_map_t custom_keys_user = {.start_kc = K_XRESET, .end_kc = K_IM_3, .key_strings = 
"K_XRESET\0"
"K_SCLN\0"
"K_COMM\0"
"K_DOT\0"
"K_SLSH\0"
"K_XIMMODE\0"
"K_BSSFT\0"
"K_LPRN\0"
"K_RPRN\0"
"K_REPRT\0"
"K_VOLUM\0"
"K_JPKB\0"
"K_ENKB\0"
"K_TKANA\0"
"K_TROMJ\0"
"K_IM_EN\0"
"K_IM_JP\0"
"K_KB_0\0"
"K_KB_1\0"
"K_KB_2\0"
"K_KB_3\0"
"K_IM_0\0"
"K_IM_1\0"
"K_IM_2\0"
"K_IM_3\0"
"K_WIN\0"
"K_MAC\0"
"K_LNX\0"
"\0"};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {KC_NO};

uint32_t keymaps_len() { return 43; }

#define TICKMS 10
#define KBD KBD_NONE
#define HARMONIZE_KEYCODES
#define HAS_THUMBROW
#include "../../../harmonize/harmonize.h"

void matrix_init_user(void) {
    harmonize_init();
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool continue_process = process_record_user_bmp(keycode, record);
    if (continue_process == false) {
        return false;
    }
    HARMONIZE_PROC_RECORD_USER;
    
    switch (keycode) {
        case KC_BTN1:
            if (record->event.pressed) {
                tb_button |= 1;
            } else {
                tb_button &= ~1;
            }
            return false;
            break;
        case KC_BTN2:
            if (record->event.pressed) {
                tb_button |= 1 << 1;
            } else {
                tb_button &= ~(1 << 1);
            }
            return false;
            break;
        case KC_BTN3:
            if (record->event.pressed) {
                tb_button |= 1 << 2;
            } else {
                tb_button &= ~(1 << 2);
            }
            return false;
            break;
        case KC_BTN4:
            if (record->event.pressed) {
                tb_button |= 1 << 3;
            } else {
                tb_button &= ~(1 << 3);
            }
            return false;
            break;
        case KC_BTN5:
            if (record->event.pressed) {
                tb_button |= 1 << 4;
            } else {
                tb_button &= ~(1 << 4);
            }
            return false;
            break;
    }

    return true;
}

report_mouse_t mouse_rep;
void           send_mouse(report_mouse_t *report);

static inline int sgn(int16_t x) {
    if (x > 0) {
        return 1;
    } else if (x < 0) {
        return -1;
    } else {
        return 0;
    }
}


void matrix_scan_user(void) {
    int16_t tb_div = 64;

    static int16_t          x_surplus, y_surplus;
    static uint32_t         cnt;
    const trackball_info_t *tb_info = get_trackball_info();
    bool                    btn_change;
 
    HARMONIZE_MATRIX_SCAN_USER;

    btn_change        = (mouse_rep.buttons != tb_button);
    mouse_rep.buttons = tb_button;

    if (layer_state == 0) {
        tb_scrl_flag = 0;
        tb_div = 64;
    } else if (layer_state == 0b100) {
        tb_scrl_flag = 1;
        tb_div = 64;
    } else if (layer_state == 0b10) {
        tb_scrl_flag = 0;
        tb_div = 256;
    }

    if (!tb_scrl_flag) {
        mouse_rep.h = 0;
        mouse_rep.v = 0;
        mouse_rep.x = (tb_info->x + x_surplus) / tb_div;
        mouse_rep.y = (tb_info->y + y_surplus) / tb_div;

        if (tb_info->x > 400 || tb_info->x < -400) {
            mouse_rep.x *= 2;
        }

        if (tb_info->y > 400 || tb_info->y < -400) {
            mouse_rep.y *= 2;
        }


        x_surplus = (tb_info->x + x_surplus) % tb_div;
        y_surplus = (tb_info->y + y_surplus) % tb_div;
    } else {
        mouse_rep.h = 1 * sgn((tb_info->x + x_surplus) / tb_div);
        mouse_rep.v = -1 * sgn((tb_info->y + y_surplus) / tb_div);
        mouse_rep.x = 0;
        mouse_rep.y = 0;

        x_surplus = (tb_info->x + x_surplus) % tb_div;
        y_surplus = (tb_info->y + y_surplus) % tb_div;
    }

    if (++cnt % 10 == 0 || (tb_info->motion_flag & 0x80)) {
        if (debug_mouse) dprintf("0x%2x: %6d %6d %3d\n", tb_info->motion_flag, tb_info->x, tb_info->y, tb_info->surface);
    }

    if (btn_change || (tb_info->motion_flag & 0x80)) {
        pointing_device_set_report(mouse_rep);
    }
}
