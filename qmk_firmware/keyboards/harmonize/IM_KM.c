#pragma once

// MK式   
#include "IM_ROMAJI_BASE.c"


static const uint8_t km_layout[] PROGMEM  = TRANSKEYCODE_DEF( \
    P,  D,  Z,  G,  B,   V,  Q,  I,  E,  X, \
    N,  T,  S,  K,  H,   L,  A,  U,  O,MINS, \
    F,  W,  R,  Y,  M,   J,  C,COMM,DOT,SLSH );

// 同時押しキー
enum {
    KM_A = 1U << 0,
    KM_I = 1U << 1,
    KM_U = 1U << 2,
    KM_E = 1U << 3,
    KM_O = 1U << 4,
    KM_Y = 1U << 5,
    KM_NN = 1U << 6,
};

// 同時押しキーから文字への変換
static const ROMAJI_BASE_CC_t const km_cctbl[] MEMCLASS = {
    {KM_Y,"y\0"},
    {KM_A,"a\0"},
    {KM_O,"o\0"},
    {KM_U,"u\0"},
    {KM_E,"e\0"},
    {KM_I,"i\0"},
    {KM_NN,"nn"},
};

// 単打キーキーから文字への変換
static const ROMAJI_BASE_CC_t const km_cctbl2[] MEMCLASS = {
    {'q',"ku"},
    {'x',"ki"},
    {'l',"lt"},{'l',"u\0"},
    {'-',"tu"},
    {'j',"nn"},
    {'c',"ti"},
    {'\'',"-\0"},
};

static const ROMAJI_BASE_KEYCODE_BIT_t const km_keycode_bit_tbl[] MEMCLASS = {
    {KC_A,     KM_A},
    {KC_I,     KM_I},
    {KC_U,     KM_U},
    {KC_E,     KM_E},
    {KC_O,     KM_O},
    {KC_Y,     KM_Y},    
    {KC_J,     KM_NN},

    {KC_SPC,   ROMAJI_BASE_RSHIFT},
    {KC_LSFT,  ROMAJI_BASE_LSHIFT},
    {KC_RSFT,  ROMAJI_BASE_RSHIFT},
};

static const ROMAJI_BASE_BIT_KEYCODE_t const km_bit_keycode_tbl[] MEMCLASS = {
};


//


// 同時押しキーへの変換
static uint16_t km_key_idx(uint16_t keycode) {
    return romaji_base_key_idx(km_keycode_bit_tbl,sizeof(km_keycode_bit_tbl)/sizeof(ROMAJI_BASE_KEYCODE_BIT_t),keycode);
}

// 単打キー、同時押しキーの特別な組み合わせから単打キーへの変換
uint16_t km_compose_keycode(uint16_t keycode,uint16_t kbits) {
    return romaji_base_compose_keycode(km_bit_keycode_tbl,sizeof(km_bit_keycode_tbl)/sizeof(ROMAJI_BASE_BIT_KEYCODE_t),keycode,kbits);
}

// 単打キーキーから文字への変換
char* km_char_to_str(char* p,char ch) {
    return romaji_base_char_to_str(km_cctbl2,sizeof(km_cctbl2)/sizeof(ROMAJI_BASE_CC_t),p,ch);
}

// 同時押しキーから文字への変換
char* km_bits_to_str(char* p,uint16_t bits) {
    return romaji_base_bits_to_str(km_cctbl,sizeof(km_cctbl)/sizeof(ROMAJI_BASE_CC_t),p,bits);
}

//
//

static uint8_t km_enabled = false;

void IM_KM_enable(uint8_t modef)
{
    km_enabled = modef;
    if( ! modef ) return;

    romaji_base_connect(
    km_key_idx,
    km_compose_keycode,
    km_char_to_str,
    km_bits_to_str
    );
    IM_ROMAJI_BASE_enable(modef);
    transkeycode_set(km_layout,"KM-Siki");
}

bool process_km(uint16_t keycode, keyrecord_t *record) {
    if( ! km_enabled )
        return true;
    return process_romaji_base(keycode,record);
}


