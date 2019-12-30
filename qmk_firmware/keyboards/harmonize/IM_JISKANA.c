#pragma once

// JISかな配列
#include "IM_KANA_BASE.c"


static const KANA_BASE_CC_t const _jiskana_cctbl0[] MEMCLASS = KANA_BASE_DEF3(
 NU, HU, _A, _U, _E, _O, YA, YU, YO, WA, HO, HE,MINS, \
 TA, TE, _I, SU, KA, NN, NA, NI, RA, SE,daku,hndk, \
 TI, TO, SI, HA, KI, KU, MA, NO, RI, RE, KE, MU, \
 TU, SA, SO, HI, KO, MI, MO, NE, RU, ME, RO, \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const KANA_BASE_CC_t const _jiskana_cctbl1[] MEMCLASS = KANA_BASE_DEF3(
 __, __, _a, _u, _e, _o, ya, yu, yo, WO, DOU, __, __, \
 __, __, _i, __, __, __, __, __, __, __, __, __, \
 __, __, __, __, __, __, __, __, __, __, __, __, \
 tu, __, __, __, __, __, __,COMM,DOT,SLSH,SLSH, \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static uint16_t jiskana_key_bit(uint16_t keycode) {
    switch(keycode) {
        case KC_LSHIFT: return KANA_BASE_LSHIFT;
        case KC_RSHIFT:
        case KC_SPC:    return KANA_BASE_RSHIFT;
        default:        return 0;
    }
}

static KANA_t jiskana_key_kana(uint16_t bits,uint16_t keycode) {
    bool lsh = (bits & KANA_BASE_LSHIFT);
    bool rsh = (bits & KANA_BASE_RSHIFT);
    uint8_t tbl = (lsh || rsh) * 1;

    KANA_BASE_CC_t const * p;
    uint8_t sz;
    switch(tbl){
        case 1:
            p = _jiskana_cctbl1;
            sz = sizeof(_jiskana_cctbl1)/sizeof(KANA_BASE_CC_t);
            break;

        case 0:
        default:
            p = _jiskana_cctbl0;
            sz = sizeof(_jiskana_cctbl0)/sizeof(KANA_BASE_CC_t);
            break;                          
    }
    for( uint8_t i=0; i<sz;i++) {
        if( keycode == mem_read_word(&p[i].code,0) )
            return mem_read_word(&p[i].kana,0);
    }
    return KANA___;

}

//
//

static uint8_t jiskana_enabled = false;

void IM_JISKANA_enable(uint8_t modef)
{
    jiskana_enabled = modef;
    if( ! modef ) return;

    kana_base_connect(
        jiskana_key_bit,
        jiskana_key_kana,
        NULL
    );
    IM_KANA_BASE_enable(modef);
    transkeycode_set(NULL,"Tron");
    _kana_base.moratorium_term = MORATORIUM_TERM;
}

bool process_JISKANA(uint16_t keycode, keyrecord_t *record) {
    if( ! jiskana_enabled )
        return true;
    return process_kana_base(keycode,record);
}


