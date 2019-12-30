#pragma once

// asuka配列
#include "IM_KANA_BASE.c"

static const KANA_BASE_CC_t const _asuka_cctbl0[] MEMCLASS = KANA_BASE_DEF1B(
 __,MINS,ENT,BI, __,BSPC,ESC,TO, HA, PO, __,LEFT,  \
 KI, SI, _U, TE, GI, YU, NN, _I, KA, TA, HO, __,  \
 JI, TI, NI, RI, BU, ya, tu, yo, yu, SA,DOWN,  \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const KANA_BASE_CC_t const _asuka_cctbl1[] MEMCLASS = KANA_BASE_DEF1B(
 _i, HI, KE, _a, _u, VU,EXCL,ASTR,YO,HU, HE,RIGT,  \
 DA, _A, GA, BA, _e, ZU, RU, SU, MA, DE, GE,HOME,  \
 ZE, NE, SE, PI, _o, YA, _E,COMM,DOT,QSTN, UP, \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const KANA_BASE_CC_t const _asuka_cctbl2[] MEMCLASS = KANA_BASE_DEF1B(
 __, BE, RE, PE,TILD,DI, NU, DO, ME, ZO, GO,PGUP,  \
 WA, _O, NA, RA, PU, DU, KU, NO, KO, SO, RO,END,  \
 PA, GU, MI, ZA, __, MU, WO, TU, MO, BO,PGDN, \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const KANA_BASE_CC_t const _asuka_cctbl3[] MEMCLASS = {};


static uint16_t asuka_key_bit(uint16_t keycode) {
        switch(keycode) {
        case KC_LSHIFT: return KANA_BASE_LSHIFT;
        case KC_RSHIFT:
        case KC_SPC:    return KANA_BASE_RSHIFT;
        default:       return 0;
    }
}


static KANA_t asuka_key_kana(uint16_t bits,uint16_t keycode) {
    bool lsh = (bits & KANA_BASE_LSHIFT);
    bool rsh = (bits & KANA_BASE_RSHIFT);
    uint8_t tbl = lsh*1+rsh*2;

    KANA_BASE_CC_t const * p;
    uint8_t sz;
    switch(tbl){

        case 1:
            p = _asuka_cctbl1;
            sz = sizeof(_asuka_cctbl1)/sizeof(KANA_BASE_CC_t);
            break;
        case 2:
            p = _asuka_cctbl2;
            sz = sizeof(_asuka_cctbl2)/sizeof(KANA_BASE_CC_t);
            break;
        case 3:
            return 0;
        case 0:
        default:
            p = _asuka_cctbl0;
            sz = sizeof(_asuka_cctbl0)/sizeof(KANA_BASE_CC_t);
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

static uint8_t asuka_enabled = false;

void IM_ASUKA_enable(uint8_t modef)
{
    asuka_enabled = modef;
    if( ! modef ) return;

    kana_base_connect(
        asuka_key_bit,
        asuka_key_kana,
        NULL
    );
    IM_KANA_BASE_enable(modef);
    transkeycode_set(NULL,"Tron");
    _kana_base.moratorium_term = 0;
}

bool process_ASUKA(uint16_t keycode, keyrecord_t *record) {
    if( ! asuka_enabled )
        return true;
    return process_kana_base(keycode,record);
}


