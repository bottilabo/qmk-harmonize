/* Copyright 2019 bottilabo [twitter @bottinabo]
 *                          [https://github.com/bottilabo/qmk_harmonize]
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
 
/*
	# ぼっちラボ「禁呪薙刀式」
	
	禁呪薙刀式は
	- 薙刀式のキーを並べなおしたものです。
	- 人差し指シフトはありません。左親指シフト、右親指シフト、両親指シフトの３シフトを使います。
	- 同手シフトは濁音を入力します。　　　　｜
	- クロスシフトは裏レイヤーを入力します。｜二コラ配列と逆さまです。
	- 両シフトは小書きと拗音を単独で入力します。
	- しゃ、てぃ　などは同時押しでも入力できます。
	- 高速入力時のロールオーバー打鍵に対応しています。
	
	# reference
	- 大岡俊彦「薙刀式」
		http://oookaworks.seesaa.net/article/456099128.html
	- eswai「crkbd用 薙刀式日本語入力キーマップ」
		https://github.com/eswai/qmk_firmware/tree/master/keyboards/crkbd/keymaps/naginata
*/

#include QMK_KEYBOARD_H
#include "IM_K_NAGINATA.h"


#include "kana-type.c"

#define KANA_BUF_SZ 5

typedef struct {
    bool enabled;
    bool mod_shift;
    uint16_t moratorium;
    uint16_t press_cnt;
    uint16_t mod_cnt;
    uint16_t time_max;
    uint16_t time_pressed;
    uint16_t time_released;
    
    uint8_t chrcount;
    uint32_t keycomb;
    uint32_t keyreleased;
    int8_t candidate_idx;
    uint32_t candidate[KANA_BUF_SZ];
} k_naginata_t;
static k_naginata_t _k_naginata = {false,false,0,0,0,K_NAGINATA_TERM,0,0,   0,0UL,0UL,  0};



static const uint8_t PROGMEM k_naginata_layout[] = TRANSKEYCODE_DEF( \
    Q,   W,   E,   R,LEFT, RIGHT,   U,   I,   O,   P, \
    A,   S,   D,   F,   G,     H,   J,   K,   L,MINS, \
    Z,   X,   C,   V,   B,     N,   M,COMM, DOT,SLSH );

#define K_NAGINATA_KBIT_A    (1UL<<0)
#define K_NAGINATA_KBIT_B    (1UL<<1)
#define K_NAGINATA_KBIT_C    (1UL<<2)
#define K_NAGINATA_KBIT_D    (1UL<<3)
#define K_NAGINATA_KBIT_E    (1UL<<4)

#define K_NAGINATA_KBIT_F    (1UL<<5)
#define K_NAGINATA_KBIT_G    (1UL<<6)
#define K_NAGINATA_KBIT_H    (1UL<<7)
#define K_NAGINATA_KBIT_I    (1UL<<8)
#define K_NAGINATA_KBIT_J    (1UL<<9)

#define K_NAGINATA_KBIT_K    (1UL<<10)
#define K_NAGINATA_KBIT_L    (1UL<<11)
#define K_NAGINATA_KBIT_M    (1UL<<12)
#define K_NAGINATA_KBIT_N    (1UL<<13)
#define K_NAGINATA_KBIT_O    (1UL<<14)

#define K_NAGINATA_KBIT_P    (1UL<<15)
#define K_NAGINATA_KBIT_Q    (1UL<<16)
#define K_NAGINATA_KBIT_R    (1UL<<17)
#define K_NAGINATA_KBIT_S    (1UL<<18)
#define K_NAGINATA_KBIT_T    (1UL<<19)

#define K_NAGINATA_KBIT_U    (1UL<<20)
#define K_NAGINATA_KBIT_V    (1UL<<21)
#define K_NAGINATA_KBIT_W    (1UL<<22)
#define K_NAGINATA_KBIT_X    (1UL<<23)
#define K_NAGINATA_KBIT_Y    (1UL<<24)

#define K_NAGINATA_KBIT_Z    (1UL<<25)
#define K_NAGINATA_KBIT_COMM (1UL<<26)
#define K_NAGINATA_KBIT_DOT  (1UL<<27)
#define K_NAGINATA_KBIT_SLSH (1UL<<28)
#define K_NAGINATA_KBIT_SCLN (1UL<<29)

#define K_NAGINATA_KBIT_LSFT (1UL<<30)
#define K_NAGINATA_KBIT_RSFT (1UL<<31)


#define K_NAGINATA_CH_SPC   K_NAGINATA_KBIT_RSFT
//#define K_NAGINATA_CH_SPC   K_NAGINATA_KBIT_LSFT
#define K_NAGINATA_CH_A     K_NAGINATA_KBIT_J
#define K_NAGINATA_CH_I     K_NAGINATA_KBIT_K
#define K_NAGINATA_CH_U     K_NAGINATA_KBIT_L
#define K_NAGINATA_CH_E     K_NAGINATA_KBIT_SLSH
#define K_NAGINATA_CH_O     K_NAGINATA_KBIT_DOT
#define K_NAGINATA_CH_YA    K_NAGINATA_KBIT_P
#define K_NAGINATA_CH_YU    K_NAGINATA_KBIT_O
#define K_NAGINATA_CH_YO    K_NAGINATA_KBIT_I

#define K_NAGINATA_CH_SHL   K_NAGINATA_KBIT_RSFT
#define K_NAGINATA_CH_SHR   K_NAGINATA_KBIT_LSFT
#define K_NAGINATA_CH_SHLR  K_NAGINATA_CH_SHR|K_NAGINATA_CH_SHL

#define K_NAGINATA_CH_SL1  K_NAGINATA_KBIT_V
#define K_NAGINATA_CH_SL2  K_NAGINATA_KBIT_F
#define K_NAGINATA_CH_SR1  K_NAGINATA_KBIT_M
#define K_NAGINATA_CH_SR2  K_NAGINATA_KBIT_J

#define MASK_SHIFT (0xc0000000)


typedef struct {
    uint32_t bits;
    uint16_t code;
} K_NAGINATA_BitsToCode_t;



uint32_t k_naginata_key_bit(uint16_t keycode)
{
    if( keycode < KC_A ) return 0;
    if( keycode <= KC_Z ) return (1UL << (keycode - KC_A) );
    switch(keycode) {
        case KC_COMM:return K_NAGINATA_KBIT_COMM;
        case KC_DOT:return K_NAGINATA_KBIT_DOT;
        case KC_SLSH:return K_NAGINATA_KBIT_SLSH;
        case KC_MINS:return K_NAGINATA_KBIT_SCLN;
        case KC_RSHIFT:
        case KC_SPC:
		return K_NAGINATA_KBIT_RSFT;
        case KC_LSHIFT:return K_NAGINATA_KBIT_LSFT;
    }
    return 0;
}


#define DEF_K_NAGINATA(MODL,MODR,Q,W,E,R,T,Y,U,I,O,P, A,S,D,F,G,H,J,K,L,SCLN, Z,X,C,V,B,N,M,COMM,DOT,SLSH) \
    {MODL|K_NAGINATA_KBIT_A,KANA_##A},{MODL|K_NAGINATA_KBIT_B,KANA_##B}      ,{MODL|K_NAGINATA_KBIT_C,KANA_##C}      ,{MODL|K_NAGINATA_KBIT_D,KANA_##D}    ,{MODL|K_NAGINATA_KBIT_E,KANA_##E}, \
    {MODL|K_NAGINATA_KBIT_F,KANA_##F},{MODL|K_NAGINATA_KBIT_G,KANA_##G}      ,{MODR|K_NAGINATA_KBIT_H,KANA_##H}      ,{MODR|K_NAGINATA_KBIT_I,KANA_##I}    ,{MODR|K_NAGINATA_KBIT_J,KANA_##J}, \
    {MODR|K_NAGINATA_KBIT_K,KANA_##K},{MODR|K_NAGINATA_KBIT_L,KANA_##L}      ,{MODR|K_NAGINATA_KBIT_M,KANA_##M}      ,{MODR|K_NAGINATA_KBIT_N,KANA_##N}    ,{MODR|K_NAGINATA_KBIT_O,KANA_##O}, \
    {MODR|K_NAGINATA_KBIT_P,KANA_##P},{MODL|K_NAGINATA_KBIT_Q,KANA_##Q}      ,{MODL|K_NAGINATA_KBIT_R,KANA_##R}      ,{MODL|K_NAGINATA_KBIT_S,KANA_##S}    ,{MODL|K_NAGINATA_KBIT_T,KANA_##T}, \
    {MODR|K_NAGINATA_KBIT_U,KANA_##U},{MODL|K_NAGINATA_KBIT_V,KANA_##V}      ,{MODL|K_NAGINATA_KBIT_W,KANA_##W}      ,{MODL|K_NAGINATA_KBIT_X,KANA_##X}    ,{MODR|K_NAGINATA_KBIT_Y,KANA_##Y}, \
    {MODL|K_NAGINATA_KBIT_Z,KANA_##Z},{MODR|K_NAGINATA_KBIT_SCLN,KANA_##SCLN},{MODR|K_NAGINATA_KBIT_COMM,KANA_##COMM},{MODR|K_NAGINATA_KBIT_DOT,KANA_##DOT},{MODR|K_NAGINATA_KBIT_SLSH,KANA_##SLSH}


static const K_NAGINATA_BitsToCode_t k_naginata_codes_1[] PROGMEM = {
        // 濁音
        {K_NAGINATA_KBIT_F | K_NAGINATA_KBIT_J,KANA_JI},  // 人差し指シフト補助 
       /* 
        // 外来音
        {K_NAGINATA_KBIT_G|K_NAGINATA_KBIT_F | K_NAGINATA_CH_A,KANA_VA},  // 人差し指シフト補助 
        {K_NAGINATA_KBIT_G|K_NAGINATA_KBIT_F | K_NAGINATA_CH_I,KANA_VI},  // 人差し指シフト補助 
        {K_NAGINATA_KBIT_G|K_NAGINATA_KBIT_F       ,KANA_VU},  // 人差し指シフト補助 
        {K_NAGINATA_KBIT_G|K_NAGINATA_KBIT_F | K_NAGINATA_CH_E,KANA_VE},  // 人差し指シフト補助 
        {K_NAGINATA_KBIT_G|K_NAGINATA_KBIT_F | K_NAGINATA_CH_O,KANA_VO},  // 人差し指シフト補助 
    */
        //
        DEF_K_NAGINATA(0,0, \
            HI, HO, SO, VU, __,    __,BSPC, RU,  SU,MINS, \
            HA, KA, TO, SI, KO,    NA,  _A, _I,  _U, KU,   \
            RO, KE, TE, KI, tu,    RE,  RA, NN,  TA, HE \
        ),

        // 外来音
        {K_NAGINATA_KBIT_M | K_NAGINATA_CH_A,KANA_FA},
        {K_NAGINATA_KBIT_M | K_NAGINATA_CH_I,KANA_FI},
        {K_NAGINATA_KBIT_M | K_NAGINATA_CH_E,KANA_FE},
        {K_NAGINATA_KBIT_M | K_NAGINATA_CH_O,KANA_FO},
        
        {K_NAGINATA_KBIT_F | K_NAGINATA_CH_E,KANA_SYE},
        {K_NAGINATA_KBIT_C | K_NAGINATA_CH_I,KANA_TELI},
        
        {K_NAGINATA_CH_U | K_NAGINATA_CH_I,KANA_ULI},
        {K_NAGINATA_CH_U | K_NAGINATA_CH_E,KANA_ULE},
        {K_NAGINATA_CH_U | K_NAGINATA_CH_O,KANA_ULO},
        {K_NAGINATA_KBIT_D | K_NAGINATA_CH_U,KANA_TOLU},
        {K_NAGINATA_KBIT_B | K_NAGINATA_CH_E,KANA_TILE},

        // 拗音同時
        {K_NAGINATA_KBIT_V | K_NAGINATA_CH_YA       ,KANA_KYA},
        {K_NAGINATA_KBIT_V | K_NAGINATA_CH_YU       ,KANA_KYU},
        {K_NAGINATA_KBIT_V | K_NAGINATA_CH_YO       ,KANA_KYO},
        {K_NAGINATA_KBIT_F | K_NAGINATA_CH_YA       ,KANA_SYA},
        {K_NAGINATA_KBIT_F | K_NAGINATA_CH_YU       ,KANA_SYU},
        {K_NAGINATA_KBIT_F | K_NAGINATA_CH_YO       ,KANA_SYO},
        {K_NAGINATA_KBIT_B | K_NAGINATA_CH_YA       ,KANA_TYA},
        {K_NAGINATA_KBIT_B | K_NAGINATA_CH_YU       ,KANA_TYU},
        {K_NAGINATA_KBIT_B | K_NAGINATA_CH_YO       ,KANA_TYO},
        {K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YA       ,KANA_HYA},
        {K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YU       ,KANA_HYU},
        {K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YO       ,KANA_HYO},
        {K_NAGINATA_KBIT_D | K_NAGINATA_CH_YA       ,KANA_NYA},
        {K_NAGINATA_KBIT_D | K_NAGINATA_CH_YU       ,KANA_NYU},
        {K_NAGINATA_KBIT_D | K_NAGINATA_CH_YO       ,KANA_NYO},
        {K_NAGINATA_KBIT_A | K_NAGINATA_CH_YA       ,KANA_MYA},
        {K_NAGINATA_KBIT_A | K_NAGINATA_CH_YU       ,KANA_MYU},
        {K_NAGINATA_KBIT_A | K_NAGINATA_CH_YO       ,KANA_MYO},
        {K_NAGINATA_KBIT_C | K_NAGINATA_CH_YA       ,KANA_RYA},
        {K_NAGINATA_KBIT_C | K_NAGINATA_CH_YU       ,KANA_RYU},
        {K_NAGINATA_KBIT_C | K_NAGINATA_CH_YO       ,KANA_RYO},

        //{K_NAGINATA_KBIT_Y|K_NAGINATA_CH_SHLR,KANA_daku},
        //{K_NAGINATA_KBIT_T|K_NAGINATA_CH_SHLR,KANA_hndk},

};

static const K_NAGINATA_BitsToCode_t k_naginata_codes_2[] PROGMEM = {
        // 小書き
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_A,KANA__a},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_I,KANA__i},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_U,KANA__u},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_E,KANA__e},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_O,KANA__o},
};


static const K_NAGINATA_BitsToCode_t k_naginata_codes_3[] PROGMEM = {
        // シフト
        DEF_K_NAGINATA(K_NAGINATA_CH_SHL,K_NAGINATA_CH_SHR, \
            __,  __,  NU,  __,  __,              __,  SA,  YO,  YU,  YA, \
            MI,  MA,  NI,  ME,COMM,             DOT,  NO,  MO,  TU,  WA, \
            SE,  WO,  RI,  NE,  TI,              __,  HU,  MU,  _O,  _E  \
        ),

        // 外来音
        {K_NAGINATA_KBIT_F | K_NAGINATA_CH_E | K_NAGINATA_CH_SHR,KANA_JE},
        {K_NAGINATA_KBIT_C | K_NAGINATA_CH_I | K_NAGINATA_CH_SHR,KANA_DELI},
        //{K_NAGINATA_KBIT_F | K_NAGINATA_CH_U | K_NAGINATA_CH_SHR,KANA_DELU},
        
        // 拗音
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_YA,KANA_ya},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_YU,KANA_yu},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_CH_YO,KANA_yo},

        // 濁音
        DEF_K_NAGINATA(K_NAGINATA_CH_SHR,K_NAGINATA_CH_SHL, \
            BI, BO, ZO, __ , __,     __, ZA , __, ZU, __, \
            BA, GA, DO, JI , GO,     __, __ , __, DU, GU, \
            ZE, GE, DE, GI , DI,     __, BU , __, DA, BE \
        ),


        // 半濁音
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_A   ,KANA_PA},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_Q   ,KANA_PI},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_M   ,KANA_PU},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_SLSH,KANA_PE},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_W   ,KANA_PO},

        // 半濁音拗音同時
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YA,KANA_PYA},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YU,KANA_PYU},
        {K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YO,KANA_PYO},

        // 濁音拗音同時
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_V | K_NAGINATA_CH_YA,KANA_GYA},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_V | K_NAGINATA_CH_YU,KANA_GYU},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_V | K_NAGINATA_CH_YO,KANA_GYO},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_F | K_NAGINATA_CH_YA,KANA_JYA},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_F | K_NAGINATA_CH_YU,KANA_JYU},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_F | K_NAGINATA_CH_YO,KANA_JYO},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_C | K_NAGINATA_CH_YA,KANA_DYA},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_C | K_NAGINATA_CH_YU,KANA_DYU},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_C | K_NAGINATA_CH_YO,KANA_DYO},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YA,KANA_BYA},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YU,KANA_BYU},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_Q | K_NAGINATA_CH_YO,KANA_BYO},


        // 外来音
        //{K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_G | K_NAGINATA_CH_A,KANA_VA},
        //{K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_G | K_NAGINATA_CH_I,KANA_VI},
        //{K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_G | K_NAGINATA_CH_E,KANA_VE},
        //{K_NAGINATA_CH_SHLR | K_NAGINATA_KBIT_G | K_NAGINATA_CH_O,KANA_VO},
        
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_L | K_NAGINATA_CH_A,KANA_TULA},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_L | K_NAGINATA_CH_I,KANA_TULI},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_L | K_NAGINATA_CH_E,KANA_TULE},
        {K_NAGINATA_CH_SHR | K_NAGINATA_KBIT_L | K_NAGINATA_CH_O,KANA_TULO},

 

        // 濁音外来音
        {K_NAGINATA_CH_SHR | K_NAGINATA_CH_I | K_NAGINATA_KBIT_C,KANA_DELI},
        {K_NAGINATA_CH_SHR | K_NAGINATA_CH_U | K_NAGINATA_KBIT_D,KANA_DOLU},
        {K_NAGINATA_CH_SHR | K_NAGINATA_CH_E | K_NAGINATA_KBIT_F,KANA_JE},
};


KANA_t k_naginata_combination_to_str(uint32_t bits)
{
    K_NAGINATA_BitsToCode_t const* p = &k_naginata_codes_1[0];
    for(uint8_t i=0;i<sizeof(k_naginata_codes_1)/sizeof(K_NAGINATA_BitsToCode_t);i++,p++) {
        if( pgm_read_dword(&p->bits) == bits )
            return pgm_read_word(&p->code);
    }

    if( true ) {
        uint32_t tbits = bits;
        tbits &= ~K_NAGINATA_KBIT_RSFT;
        if( bits & K_NAGINATA_CH_SL1 )
            tbits |=  K_NAGINATA_KBIT_RSFT;
        tbits &= ~K_NAGINATA_CH_SL1;

        K_NAGINATA_BitsToCode_t const* p = &k_naginata_codes_2[0];
        for(uint8_t i=0;i<sizeof(k_naginata_codes_2)/sizeof(K_NAGINATA_BitsToCode_t);i++,p++) {
            if( pgm_read_dword(&p->bits) == tbits )
                return pgm_read_word(&p->code);
        }

    }

    for(int8_t i=0;i<=0;i++) 
    {
        uint32_t tbits = bits;
        switch( i ) {
            case 0:
                break;

            case 1:
            if( bits & K_NAGINATA_CH_SL1 )
                tbits |= K_NAGINATA_KBIT_LSFT | K_NAGINATA_KBIT_RSFT;
            tbits &= ~K_NAGINATA_CH_SL1;

            if( bits & K_NAGINATA_CH_SL2 )
                tbits |= K_NAGINATA_KBIT_RSFT;
            tbits &= ~K_NAGINATA_CH_SL2;
            break;

            case 2:
            if( bits & K_NAGINATA_CH_SR1 )
                tbits |= K_NAGINATA_KBIT_LSFT | K_NAGINATA_KBIT_RSFT;
            tbits &= ~K_NAGINATA_CH_SR1;
            
            if( bits & K_NAGINATA_CH_SR2 )
                tbits |= K_NAGINATA_KBIT_RSFT;
            tbits &= ~K_NAGINATA_CH_SR2;
            break;
        }

        K_NAGINATA_BitsToCode_t const* p = &k_naginata_codes_3[0];
        for(uint8_t i=0;i<sizeof(k_naginata_codes_3)/sizeof(K_NAGINATA_BitsToCode_t);i++,p++) {
            if( pgm_read_dword(&p->bits) == tbits )
                return pgm_read_word(&p->code);
        }
    }

    return KANA___;
};


KANA_t k_naginata_combination_to_str_3(uint32_t bits)
{
    switch(bits) {
        case K_NAGINATA_CH_SPC: return KANA_SPC;
    }
    return KANA___;
}

void k_naginata_reset(void) {
    k_naginata_clear();
    _k_naginata.keycomb = 0UL;
    _k_naginata.keyreleased = 0UL;
}

KANA_t k_naginata_kana(uint32_t pressed,uint32_t released) {
    KANA_t ch = k_naginata_combination_to_str(pressed);
    if( ch ) {
        return ch;
    }

    ch = k_naginata_combination_to_str_3( pressed & released );
    return ch;

}

inline bool k_naginata_type_char(KANA_t ch) {
	if( _harmonize.type_mode )
		return type_kana( ch );
	else
		return type_romaji( ch );
}

// 一連のキー入力を文字に直して出力する
bool k_naginata_type(void) {
    KANA_t buf[KANA_BUF_SZ];
    int8_t ci = _k_naginata.candidate_idx;
    int8_t bi = 0;
    bool ffound = false;
    uint32_t mod = 0;
    while( 0 < ci ) {
        for(int8_t j=0; j<ci; j++) {
            uint32_t keycomb = 0; 
            for(int8_t i = ci -1; j <= i; i--) {
                keycomb |= _k_naginata.candidate[i];
            }
            KANA_t k = k_naginata_kana(keycomb|mod,_k_naginata.keyreleased);
            if( k ) {
                buf[bi++] = k;
                ci = j;
                ffound = true;
                break;
            }
        }
        if( ! ffound )
            ci--;
    }

    bool typed = false;
    for(int8_t i = bi -1; 0 <= i; i--) {
        typed |= k_naginata_type_char(buf[i]);
    }
    
    k_naginata_clear();

    return typed;
}


// バッファをクリアする
void k_naginata_clear(void) {
    _k_naginata.chrcount = 0;
    _k_naginata.time_pressed = 0;
    _k_naginata.candidate_idx = 0;
    _k_naginata.keycomb &= MASK_SHIFT;
    _k_naginata.keyreleased = 0;
}


// 入力モードか編集モードかを確認する
void k_naginata_mode(uint16_t keycode, keyrecord_t *record) {
  if (!_k_naginata.enabled) return;

  // modifierが押されたらレイヤーをオフ
  switch (keycode) {
    case KC_LCTRL:
    //case KC_LSHIFT:
    case KC_LALT:
    case KC_LGUI:
    case KC_RCTRL:
    //case KC_RSHIFT:
    case KC_RALT:
    case KC_RGUI:
      if (record->event.pressed) {
        _k_naginata.mod_cnt++;
      } else {
        _k_naginata.mod_cnt--;
        if (_k_naginata.mod_cnt == 0) {
        }
      }
      break;
  }
}

void IM_K_NAGINATA_enable(uint8_t modef) {
    if( modef ) {
		_k_naginata.enabled = true;
		k_naginata_reset();

        transkeycode_set(k_naginata_layout,"IM K_NAGINATA");
        set_kana_mode(modef);
    }
    else {
		_k_naginata.enabled = false;
		k_naginata_reset();
    }
}


// 薙刀式の入力処理
bool process_K_NAGINATA(uint16_t keycode, keyrecord_t *record) {

    switch(keycode) {
        case KC_LSHIFT:
        case KC_RSHIFT:
            if( ! record->event.pressed ) {
                if( _k_naginata.mod_shift )
                {
                    _k_naginata.mod_shift = false;
                    unregister_code(KC_LSHIFT);
                }
            }
        break;
    }

    k_naginata_mode(keycode,record);
    
    if (!_k_naginata.enabled )
        return true;
    
    if( _k_naginata.mod_cnt > 0) {
        transkeycode_reset();
        k_naginata_type();
        return true;
    }

#ifndef NO_MORATORIUM
    if(0<_k_naginata.moratorium) {
        _k_naginata.moratorium-=10;
        if(_k_naginata.moratorium<=0) {
            _k_naginata.moratorium = 0;
            if( _k_naginata.chrcount > 0 )
                k_naginata_type();
        }
    }
#endif
    if( keycode == 0 )
        return true;

    // k_naginata入力に関係なければフラッシュしてデフォルトの処理をさせる
    uint32_t bitcode = k_naginata_key_bit(keycode);
    if( !bitcode ) {
        k_naginata_type();

        if( _k_naginata.keycomb & K_NAGINATA_KBIT_LSFT ) {
            _k_naginata.mod_shift = true;
            register_code(KC_LSHIFT);
        }
        return true;
    }

  uint16_t now = timer_read();
  if (record->event.pressed) {

        bool update_time = true;
        switch( bitcode ) {
            case K_NAGINATA_KBIT_RSFT:
              //update_time = false;
            case K_NAGINATA_KBIT_LSFT:
              _k_naginata.mod_shift = true;
              update_time = false;
              break;
        }

        if( _k_naginata.time_pressed == 0 ) {
            if( update_time ) {
                _k_naginata.time_pressed = now;
            }
        }
        else {
            if( elapsed_time(now,_k_naginata.time_pressed) > _k_naginata.time_max ) {
                _k_naginata.time_pressed = now;
                k_naginata_type();
            }
        }

        _k_naginata.press_cnt++;
        _k_naginata.chrcount++;

        _k_naginata.keyreleased &= ~bitcode;
        _k_naginata.keycomb  |= bitcode;
        if( _k_naginata.candidate_idx < KANA_BUF_SZ ) {
            _k_naginata.candidate[ _k_naginata.candidate_idx++ ] = bitcode | (_k_naginata.keycomb & MASK_SHIFT);
        }
        return false;
  }
  // key release
  else {
        _k_naginata.keyreleased  |= bitcode;
        _k_naginata.press_cnt--;
        
#ifdef NO_MORATORIUM
        if( _k_naginata.chrcount > 0 )
            k_naginata_type();
#else
        _k_naginata.moratorium = 100;
#endif
        _k_naginata.keycomb &= ~bitcode;
		_k_naginata.time_released = now;
        return false;
  }
  return true;
}


