//　同時押しでモディファイアとして使うキー 
enum {
    JISKANA_LSHIFT = 1U << 0,
    JISKANA_RSHIFT  = 1U << 1,
    JISKANA_DAKU = 1U << 2,
    JISKANA_HNDK  = 1U << 3,
};

// nicola配列
static const JISKANA_CC_t const _jiskana_cctbl0[] MEMCLASS = JISKANA_DEF3(
 __, yu, KO, TO, SA, ya, WA, KI, SI, KU, TI, __,  \
CDOT,TA, KA,COMM,TE, NO, HA, _I,DOT, _U, NN,MINS,  \
 __, yo, MA, NA, RU, NI, MO, SU, RI, TU, tu, __, \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const JISKANA_CC_t const _jiskana_cctbl1[] MEMCLASS = JISKANA_DEF3(
 __, HI, NE, DO, ME, HU, VU, GI, DU, GU, DI, __,  \
CDOT,DA, GA, WO, DE, MU, __, _O, __, RE, RO,SLSH,  \
 __, HE, SO, SE, KE, HO, _o, _e, _u, _i, _a, __, \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const JISKANA_CC_t const _jiskana_cctbl2[] MEMCLASS = JISKANA_DEF3(
 __, PI, GO, NU, ZA, PU, BU, YU, JI, MI, BI, __,  \
COLN,RA,yoU, __,yuU, PA, BA, YO, WO, _A, YA,MINS,  \
 __, PE, ZO, ZE, GE, PO, BO, ZU, _E,DESU,BE, __, \
 ZENHAN,EISU,MHEN,SPC,HEN,KANA
);

static const JISKANA_CC_t const _jiskana_cctbl3[] MEMCLASS = {};


static uint16_t jiskana_key_bit(uint16_t keycode) {
        switch(keycode) {
        case KC_LSHIFT: return JISKANA_LSHIFT;
        case KC_RSHIFT:
        case KC_SPC:    return JISKANA_RSHIFT;
        default:       return 0;
    }
}