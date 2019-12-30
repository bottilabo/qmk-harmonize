#pragma once

// いろは坂配列配列
#include "IM_KANA_BASE.c"


static const KANA_BASE_CC_t const _irohazaka_cctbl0[] MEMCLASS = KANA_BASE_DEF4(
 YA, RO, GA, RA, RI, RE, WA, HE, ME, DA, _A, SE, DO, SO, HI,
 YO, TA, TE, NO, _I, NN, MINS,KE,SA, yo, KU, SI, HA,
 RU, TO, DE, WO, NI, _U, MA, KI, KO, NA, MO, SU, KA,
 _a, yu, HO, BI, TU, MI, JI, TI, HU, MU, NE, _E, _O,
                  ON, SPC,tu, BS, GI,OFF
);

static const KANA_BASE_KK_t const _irohazaka_ante_tu[] MEMCLASS = {
    {KANA_YA,KANA_tu},
    {KANA_RE,KANA_PE},

    {KANA_YO,KANA_TAB},
    {KANA_NO,KANA_NU},

    {KANA_WO,KANA_ULO},
    {KANA_NI,KANA__i},
    {KANA__U,KANA_PU},
    {KANA_MA,KANA_PA},
    {KANA_MO,KANA_PO},

    {KANA__a,KANA_ya},
    {KANA_yu,KANA_YU},
    {KANA_BI,KANA_DI},
    {KANA_MI,KANA_PI},
    {KANA_MU,KANA__u},
    {KANA__E,KANA__e},
    {KANA__O,KANA__o},
};

static const KANA_BASE_KK_t const _irohazaka_post_yo[] MEMCLASS = {
    {KANA_HE,KANA_BE},
    {KANA_SE,KANA_ZE},
    {KANA_DE,KANA_DOLU},
    {KANA_SO,KANA_ZO},

    {KANA_TE,KANA_TELI},
    {KANA__I,KANA__I_e},
    {KANA_KE,KANA_GE},
    {KANA_SA,KANA_ZA},
    {KANA_KU,KANA_GU},
    {KANA_HA,KANA_BA},

    {KANA_TO,KANA_TOLU},
    {KANA_DE,KANA_DELI},
    {KANA__U,KANA__ULI},
    {KANA_KO,KANA_GO},
    {KANA_SU,KANA_ZU},

    {KANA_HO,KANA_BO},
    {KANA_TU,KANA_ZU},
    {KANA_HU,KANA_BU},
    {KANA_NE,KANA_NELE},

};

bool irohazaka_process_post_type(KANA_t prev,KANA_t ch,KANA_t* pnch)
{
    if( (uint16_t)ch == KANA_yo ) {
        for( uint8_t i=0; i<sizeof(_irohazaka_post_yo)/sizeof(KANA_BASE_KK_t);i++) {
            if( (uint16_t)prev == mem_read_word(&_irohazaka_post_yo[i].a,0) ) {
                *pnch = mem_read_word(&_irohazaka_post_yo[i].b,0);
                return true;
            }
        }
    }
    else if( (uint16_t)prev == KANA_tu) {
        for( uint8_t i=0; i<sizeof(_irohazaka_ante_tu)/sizeof(KANA_BASE_KK_t);i++) {
            if( (uint16_t)ch == mem_read_word(&_irohazaka_ante_tu[i].a,0) ) {
                *pnch = mem_read_word(&_irohazaka_ante_tu[i].b,0);
                return true;
            }
        }
    }
    return false;
}


static uint16_t irohazaka_key_bit(uint16_t keycode) {
    return 0;
}

static KANA_t irohazaka_key_kana(uint16_t bits,uint16_t keycode) {
    KANA_BASE_CC_t const * p;
    uint8_t sz;
    p = _irohazaka_cctbl0;
    sz = sizeof(_irohazaka_cctbl0)/sizeof(KANA_BASE_CC_t);

    for( uint8_t i=0; i<sz;i++) {
        if( keycode == mem_read_word(&p[i].code,0) )
            return mem_read_word(&p[i].kana,0);
    }
    return KANA___;

}

//
//

static uint8_t irohazaka_enabled = false;

void IM_IROHAZAKA_enable(uint8_t modef)
{
    irohazaka_enabled = modef;
    if( ! modef ) return;

    kana_base_connect(
        irohazaka_key_bit,
        irohazaka_key_kana,
        irohazaka_process_post_type
    );
    IM_KANA_BASE_enable(modef);
    transkeycode_set(NULL,"Tron");
    _kana_base.moratorium_term = MORATORIUM_TERM;
}

bool process_IROHAZAKA(uint16_t keycode, keyrecord_t *record) {
    if( ! irohazaka_enabled )
        return true;
    return process_kana_base(keycode,record);
}


