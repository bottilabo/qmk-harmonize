#include <keymap_jp.h>
#include "transkeycode.h"


//#define _STR(X) #X
//#define INCLUDEFILE(STR) _STR(STR)
//#define _CONCAT(X,Y) #X #Y
//#define ADDNAME(STR) _CONCAT(STR,_NAME)
#ifdef KBLAYOUT1
    #define KBLAYOUT1S INCLUDEFILE(KBLAYOUT1)
    #include KBLAYOUT1S
    #define KBLAYOUT1_ ,KBLAYOUT1
    #define KBLAYOUT1_NAME ,ADDNAME(KBLAYOUT1)
#else
    #define KBLAYOUT1_ 
    #define KBLAYOUT1_NAME
#endif

#ifdef KBLAYOUT2
    #define KBLAYOUT2S INCLUDEFILE(KBLAYOUT2)
    #include KBLAYOUT2S
    #define KBLAYOUT2_ ,KBLAYOUT2
    #define KBLAYOUT2_NAME ,ADDNAME(KBLAYOUT2)
#else
    #define KBLAYOUT2_ 
    #define KBLAYOUT2_NAME
#endif

#ifdef KBLAYOUT3
    #define KBLAYOUT3S INCLUDEFILE(KBLAYOUT3)
    #include KBLAYOUT3S
    #define KBLAYOUT3_ ,KBLAYOUT3
    #define KBLAYOUT3_NAME ,ADDNAME(KBLAYOUT3)
#else
    #define KBLAYOUT3_ 
    #define KBLAYOUT3_NAME
#endif


#define KC__ KC_TRNS

//#define pgm_read_ptr(X) pgm_read_word(X)

#define TRANSKEYCODE_SYM_OFFSET 26

static const uint8_t MEMCLASS base_qwerty[] = {
KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, 
KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S, KC_T, KC_U, KC_V, KC_W, KC_X, KC_Y, KC_Z, 
KC_MINUS, KC_COMMA, KC_DOT, KC_SLASH
};



typedef struct _transkeycode_t
{
    const uint8_t* layout;
    const char* name;
} transkeycode_t;

static transkeycode_t _transkeycode;


static uint8_t klid = 1;

static const uint8_t *const kbls[] = {base_qwerty KBLAYOUT1_     KBLAYOUT2_     KBLAYOUT3_      };
static const char *const kbnames[] = {"QWERTY"    KBLAYOUT1_NAME KBLAYOUT2_NAME KBLAYOUT3_NAME };

void transkeycode_set(const uint8_t* layout,const char* name) {
    if( layout == NULL ) {
        layout = kbls[0];
        //layout = mem_read_ptr( kbls,0 );
    }
    
    _transkeycode.layout = layout;
    _transkeycode.name = name;
    TRANSKEYCODE_ON_CHANGE_KB_LAYOUT(-1,name);
}

void transkeycode_reset(void) {
    //transkeycode_set( pgm_read_ptr( &kbls[klid] ),pgm_read_ptr(&kbnames[klid]) );
    transkeycode_set( kbls[klid],kbnames[klid] );
}

const char* get_kb_layout(void) {
    return _transkeycode.name;
}

void select_kb_layout(uint8_t id) {
    klid = id;
    if( klid >= sizeof(kbls)/sizeof(uint8_t*) )
        klid = 0;

    TRANSKEYCODE_ON_CHANGE_KB_LAYOUT(klid,kbnames[klid]);
}



/*
__attribute__ ((weak)) 
uint16_t transkeycode_user(uint16_t kc)
{
    return kc;
}
*/

uint16_t transkeycode_2(uint16_t kc) {
    uint8_t idx;
    if( KC_A <= kc && kc <= KC_Z ) {
        idx = kc - KC_A;
    }
    else {
        switch(kc) {
            case K_SCLN: idx = TRANSKEYCODE_SYM_OFFSET;break;
            case K_COMM: idx = TRANSKEYCODE_SYM_OFFSET+1;break;
            case K_DOT:  idx = TRANSKEYCODE_SYM_OFFSET+2;break;
            case K_SLSH: idx = TRANSKEYCODE_SYM_OFFSET+3;break;
            default:return kc;
        }
    }
    uint8_t const *cur_kbl = _transkeycode.layout;
    return mem_read_byte(cur_kbl,idx);
}

uint16_t transkeycode_1(uint16_t kc)
{
    uint16_t newkc = kc & 0xffu;
    //if( newkc < KC_A || KC_CAPSLOCK < newkc )
    //    return kc;

    if( (KC_A <= kc && kc <= KC_CAPSLOCK) || (K_SCLN <= kc && kc <= K_SLSH ) ) {
        return transkeycode_2(kc);
    }

    bool modtap = (QK_MOD_TAP <= kc && kc <= QK_MOD_TAP_MAX) || \
                  (QK_LAYER_TAP <= kc && kc <= QK_LAYER_TAP_MAX);
                  //(QK_MODS <= kc && kc <= QK_MODS_MAX);
    
    if( modtap ) {
        return (kc & 0xff00u) | transkeycode_2(newkc);
    }
    else { 
      return kc;
    }
}

extern __attribute__ ((weak)) uint16_t transkeycode(uint16_t kc);
uint16_t transkeycode(uint16_t kc)
{
    return TRANSKEYCODE_USER( transkeycode_1(kc) );
}
    
extern __attribute__ ((weak)) uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key);
uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    uint16_t kc = pgm_read_word(&keymaps[(layer)][(key.row)][(key.col)]);
    return transkeycode(kc);
}

#ifdef TAP_DANCE_ENABLE

void transkeycode_tap_dance_pair_on_each_tap (qk_tap_dance_state_t *state, void *user_data) {
  qk_tap_dance_pair_t *pair = (qk_tap_dance_pair_t *)user_data;

  if (state->count == 2) {
    register_code16 ( transkeycode(pair->kc2) );
    state->finished = true;
  }
}

void transkeycode_tap_dance_pair_finished (qk_tap_dance_state_t *state, void *user_data) {
  qk_tap_dance_pair_t *pair = (qk_tap_dance_pair_t *)user_data;

  if (state->count == 1) {
    register_code16 ( transkeycode(pair->kc1) );
  } else if (state->count == 2) {
    register_code16 ( transkeycode(pair->kc2) );
  }
}

void transkeycode_tap_dance_pair_reset (qk_tap_dance_state_t *state, void *user_data) {
  qk_tap_dance_pair_t *pair = (qk_tap_dance_pair_t *)user_data;

  if (state->count == 1) {
    unregister_code16 ( transkeycode(pair->kc1) );
  } else if (state->count == 2) {
    unregister_code16 ( transkeycode(pair->kc2) );
  }
}

#endif
