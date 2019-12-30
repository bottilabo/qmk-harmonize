#include QMK_KEYBOARD_H

#define HAS_THUMBROW
#define IMM K_XIMMODE
#define ___ KC_TRNS

//     +    +    +    *                                  *    +    +    +
//4    4    3    2    1    1    1              1    1    1    2    3    4    4
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
  LAYOUT_planck_mit( \
  L05, L04, L03, L02, L01,    UP,          BSPC,   R01, R02, R03, R04, R05, \
  L15, L14, L13, L12, L11,  LEFT,          RIGT,   R11, R12, R13, R14, R15, \
  L25, L24, L23, L22, L21,  DOWN,          ENTR,   R21, R22, R23, R24, R25, \
  TL2, ___, ___, ___, TL1,          TL0,           TR0, TR1,RCTL,RALT,RGUI \
    )

#define LAYOUT_DEF(...)                  LAYOUT_remapper(__VA_ARGS__)

#include "../../../harmonize/harmonize.h"

void matrix_init_user(void) {
    harmonize_init();
}


void matrix_scan_user(void) {
   HARMONIZE_MATRIX_SCAN_USER;
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    HARMONIZE_PROC_RECORD_USER;
	return true;
}

void led_set_user(uint8_t usb_led) {

	if (usb_led & (1 << USB_LED_NUM_LOCK)) {

	} else {

	}

	if (usb_led & (1 << USB_LED_CAPS_LOCK)) {

	} else {

	}

	if (usb_led & (1 << USB_LED_SCROLL_LOCK)) {

	} else {

	}

	if (usb_led & (1 << USB_LED_COMPOSE)) {

	} else {

	}

	if (usb_led & (1 << USB_LED_KANA)) {

	} else {

	}

}
