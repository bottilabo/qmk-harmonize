#undef SSD1306OLED



#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
  #include "glcdfont.c"
#endif


//#define USE_KEYLOG
//#define USE_LAYER_STATE
//#define USE_LOGO



extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;


#define HAS_THUMBROW

#define IMM K_XIMMODE

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
  LAYOUT( \
ESC , L05, L04, L03, L02, L01,                     R01, R02, R03, R04, R05, BSPC, \
CAPS, L15, L14, L13, L12, L11,                     R11, R12, R13, R14, R15, ENTR, \
LGUI, L25, L24, L23, L22, L21,                     R21, R22, R23, R24, R25,  ADJ, \
                     LALT, TL1,  TL0,   TR0, TR1, LCTL  \
    )
#define LAYOUT_DEF(...)                  LAYOUT_remapper(__VA_ARGS__)

#include "../../../harmonize/harmonize.h"

void on_change_kb_layout(uint8_t id,const char* name) {
}


int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
    
    //
    // Always init to default keyboard layout
    //
    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }
    //eeconfig_read_keymap()
    persistent_default_layer_set(0);

    harmonize_init();
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void matrix_scan_user(void) {
   iota_gfx_task();
   HARMONIZE_MATRIX_SCAN_USER;
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write(matrix,"layout: ");
    matrix_write_ln(matrix,get_kb_layout());

#ifdef USE_LAYER_STATE
    matrix_write_ln(matrix, read_layer_state());
#endif

#ifdef ROMAJIEX
#pragma message "** keymay ROMAJIEX **"
//    {
//        char* strROMAJI = "ROMAJI mode";
//        itoa(timer_read())
//        matrix_write_ln(matrix,strROMAJI);
//    }
    if( is_romaji_mode() )
    {
        static const char* strROMAJI = "ROMAJI mode";
        matrix_write_ln(matrix,strROMAJI);
    }
    else {
        static const char* strROMAJI = "normal mode";
        matrix_write_ln(matrix,strROMAJI);
    }
#endif

#ifdef USE_KEYLOG
    matrix_write_ln(matrix, read_keylog());
#endif
    //matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
#ifdef USE_LOGO
        matrix_write(matrix, read_logo());
#endif
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}

#endif


#define CHANGE_LAYOUT(X)    \
    case X:\
      if (record->event.pressed) {\
        persistent_default_layer_set(1UL<<_##X);\
      }\
      return false;\
      break;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef SSD1306OLED
  if (record->event.pressed) {
#ifdef USE_KEYLOG
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }
#endif

    HARMONIZE_PROC_RECORD_USER;
    return true;
}

