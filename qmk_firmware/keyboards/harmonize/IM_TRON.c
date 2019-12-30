#pragma once

// TRON配列   
#include "IM_KANA_BASE.c"


static const KANA_BASE_CC_t const _tron_cctbl0[] MEMCLASS = KANA_BASE_DEF(
 RA, RU, KO, HA, yo, KI, NO, KU, _A, RE,   \
 TA, TO, KA, TE, MO, WO, _I, _U, SI, NN,   \
 MA, RI, NI, SA, NA, SU, TU,COMM,DOT,tu,   \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const KANA_BASE_CC_t const _tron_cctbl1[] MEMCLASS = KANA_BASE_DEF(
 HI, SO, __, ya, HO, GI, GE, GU, __, __,   \
 NU, NE, yu, YO, HU,daku,DI, VU, JI, __,   \
 _e, _o, SE, YU, HE, ZU, DU,COMM,DOT,WA,  \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const KANA_BASE_CC_t const _tron_cctbl2[] MEMCLASS = KANA_BASE_DEF(
 BI, ZO, GO, BA, BO, _E, KE, ME, MU, RO,   \
 DA, DO, GA, DE, BU, _O, TI,MINS,MI, YA,   \
 __, __, ZE, ZA, BE, WA, _i, _a,hndk,_u,  \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static uint16_t tron_key_bit(uint16_t keycode) {
    switch(keycode) {
        case KC_LSHIFT: return KANA_BASE_LSHIFT;
        case KC_RSHIFT:
        case KC_SPC:    return KANA_BASE_RSHIFT;
        default:        return 0;
    }
}

static KANA_t tron_key_kana(uint16_t bits,uint16_t keycode) {
    bool lsh = (bits & KANA_BASE_LSHIFT);
    bool rsh = (bits & KANA_BASE_RSHIFT);
    uint8_t tbl = lsh*1+rsh*2;

    KANA_BASE_CC_t const * p;
    uint8_t sz;
    switch(tbl){

        case 1:
            p = _tron_cctbl1;
            sz = sizeof(_tron_cctbl1)/sizeof(KANA_BASE_CC_t);
            break;
        case 2:
            p = _tron_cctbl2;
            sz = sizeof(_tron_cctbl2)/sizeof(KANA_BASE_CC_t);
            break;
        case 3:
            return 0;
        case 0:
        default:
            p = _tron_cctbl0;
            sz = sizeof(_tron_cctbl0)/sizeof(KANA_BASE_CC_t);
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

static uint8_t tron_enabled = false;

void IM_TRON_enable(uint8_t modef)
{
    tron_enabled = modef;
    if( ! modef ) return;

    kana_base_connect(
        tron_key_bit,
        tron_key_kana,
        NULL
    );
    IM_KANA_BASE_enable(modef);
    transkeycode_set(NULL,"Tron");
    _kana_base.moratorium_term = MORATORIUM_TERM;
}

bool process_TRON(uint16_t keycode, keyrecord_t *record) {
    if( ! tron_enabled )
        return true;
    return process_kana_base(keycode,record);
}


