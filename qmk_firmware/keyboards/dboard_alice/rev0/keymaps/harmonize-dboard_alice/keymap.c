/* Copyright 2019 discord4415
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

//      +    +    +    *                                 *    +    +    +
//4     4    3    2    1    1      1              1      1    1    2    3    4      4
#define LAYOUT_remapper( \
ESC ,   NL5, NL4, NL3, NL2, NL1,              NR1, NR2, NR3, NR4, NR5,   NR6, NR7, BSPC,  \
TAB ,   L05, L04, L03, L02, L01,              R01, R02, R03, R04, R05,   R06, R07, BSLS,   \
CAPS,   L15, L14, L13, L12, L11,              R11, R12, R13, R14, R15,   R16, R17, ENTR,  \
LSFT,   L25, L24, L23, L22, L21,              R21, R22, R23, R24, R25,   R26,      RSFT,  \
MOUS,  LCTL,LALT,LGUI,                                            RGUI,RALT,RCTL,   ADJ, \
                       TL2,                                   TR2,                       \
       PGUP,                TL1,                         TR1,                UP,         \
HOME,  PGDN,END ,                  TL0,           TR0,                 LEFT,DOWN,  RIGT, \
INS ,DEL   ,PRNT,BRK,                                                ZENHN,KANA,MHEN,HENK  \
  ) \
  LAYOUT_wrapper( \
     ADJ,   ESC,NL5, NL4,   NL3, NL2, NL1, NR1,            NR2, NR3, NR4, NR5,   NR6, NR0,          BSPC, \
    MOUS,   TAB,L05,        L04, L03, L02, L01,            R01, R02, R03, R04,   R05, R00, R06,     BSLS, \
_______,   CAPS,L15,        L14, L13, L12, L11,            R11, R12, R13, R14,   R15, R10,          ENTR, \
           LGUI,L25,        L24, L23, L22, L21,            ___, R21, R22, R23,   R24, R25,           ___, \
           LCTL,LALT,       TL1,           TL0,            TR0      , ___,TR1,                 UP ,      \
                                                                                          LEFT,DOWN,RIGT  \
)

#define LAYOUT_DEF(...)                  LAYOUT_remapper(__VA_ARGS__)


#define K_TL0 SFT_T(KC_BSPC)
//#define K_TL0 KC_LSFT
#define K_TL1 LT(_Ll1,KC_TAB)
//#define K_TL1 MO(_Ll1)
//#define K_TL2 MO(_Ll2)
#define K_TR0 LT(_Lr1,KC_SPC)
#define K_TR1 KC_RCTL

#define HAS_THUMBROW
#include "../../../../harmonize/harmonize.h"


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    HARMONIZE_PROC_RECORD_USER;
    return true;
}

void matrix_init_user(void) {
    harmonize_init();
}
void matrix_scan_user(void) {
   HARMONIZE_MATRIX_SCAN_USER;
}

void led_set_user(uint8_t usb_led) {

}
