#pragma once

/* Copyright 2018-2019 bottilabo <@bottinabo>
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
	# ぼっちラボ製「いろは坂配列」

	# reference
	- めんめんつ作　いろは坂配列
		https://ch.nicovideo.jp/chidauyu/blomaga/ar1778989
*/

#include <string.h>

#include "kana-type.c"
#include "IM_KANA_BASE.h"



typedef struct _kana_base_t {
    volatile bool mode;
    uint16_t press_cnt;
    int16_t  moratorium_term;
    uint16_t time_max;
    uint16_t time_start;
    uint16_t keycode;
    uint16_t press;
    uint16_t pressing;
    uint16_t released;
    int16_t  moratorium;
    KANA_t   type_buf;

    uint16_t (*key_bit)(uint16_t keycode);
    KANA_t (*key_kana)(uint16_t kits,uint16_t keycode);
    bool (*process_post_type)(KANA_t prev,KANA_t ch,KANA_t* pnch);
} kana_base_t;

static kana_base_t _kana_base = {false,0,MORATORIUM_TERM,KANA_BASE_TERM,0,0,0,0,0,0,0};

static bool kana_base_process_post_type(KANA_t prev,KANA_t ch,KANA_t* pnch)
{
    return false;
}

void kana_base_connect(
    uint16_t (*key_bit)(uint16_t keycode),
    KANA_t (*key_kana)(uint16_t bits,uint16_t keycode),
    bool (*process_post_type)(KANA_t prev,KANA_t ch,KANA_t* pnch)
) {
    _kana_base.key_bit = key_bit;
    _kana_base.key_kana = key_kana;

    if( process_post_type == NULL )
        process_post_type = kana_base_process_post_type;
    
    _kana_base.process_post_type = process_post_type;
}


bool is_kana_base_mode(void) {
    return _kana_base.mode;
}

static bool kana_base_set_flg(uint16_t keycode,uint16_t bit,KANA_t kana) {
    if(bit) {
        _kana_base.press |= bit;
        _kana_base.pressing |= bit;
        return true;
    }
    if( !kana )
        return false;

    if( _kana_base.keycode == 0 ) {
        _kana_base.keycode = keycode;
        return true;
    }

    return false;
}

static void kana_base_clear(void) {
    _kana_base.keycode = 0;
    _kana_base.press = 0;
    _kana_base.released = 0;
}

void kana_base_reset(void) {
    _kana_base.press_cnt = 0;
    kana_base_clear();
    _kana_base.pressing = 0;
    _kana_base.type_buf = 0;
}

void IM_KANA_BASE_enable(bool tf) {
    kana_base_reset();
    _kana_base.mode = tf;
    if(tf) {
        transkeycode_set(NULL,"KANA_BASE IM");
    }
}

void kana_base_type_buf(KANA_t tch) {
    uint16_t ch = tch;

    if( ch == 0 )
        return;

    if( ch == KANA_max ) {
        ch = _kana_base.type_buf;
    }
    // 後置の濁音、半濁音処理
    else if( ch == KANA_daku || ch == KANA_hndk ) {
        ch |= _kana_base.type_buf;
    }
    else if( _kana_base.type_buf == 0) {
        _kana_base.type_buf = ch;
        return;
    }
    else {
        KANA_t nch;
        if( _kana_base.process_post_type(_kana_base.type_buf,ch,&nch) ) {
            ch = 0;
        }
        else {
            nch = _kana_base.type_buf;
        }

        if( _harmonize.type_mode )
            type_kana( nch );
        else
            type_romaji( nch );
        _kana_base.type_buf = ch;
        return;
    }

    if( _harmonize.type_mode )
        type_kana( ch );
    else
        type_romaji( ch );

    _kana_base.type_buf = 0;
}

static bool kana_base_type(void) {
    KANA_t kana;

    // 単打もしくは同時押し
    kana = _kana_base.key_kana(_kana_base.press|_kana_base.pressing,_kana_base.keycode);

    // 同時押しの濁音、半濁音処理
    if( (_kana_base.press&KANA_BASE_DAKU) || (_kana_base.pressing&KANA_BASE_DAKU) )
        kana |= KANA_daku;
    else if( (_kana_base.press&KANA_BASE_HNDK) || (_kana_base.press&KANA_BASE_HNDK) ) 
        kana |= KANA_hndk;

    bool lsp = (_kana_base.press & KANA_BASE_LSHIFT) && (_kana_base.released & KANA_BASE_LSHIFT);
    bool rsp = (_kana_base.press & KANA_BASE_RSHIFT) && (_kana_base.released & KANA_BASE_RSHIFT);
    if( (kana == 0) && (lsp || rsp) ) {
        kana = KANA_SPC;
    }
       

    kana_base_type_buf(kana);
    kana_base_clear();
    return kana;
} 

static bool kana_base_key(uint16_t keycode,bool pressed) {
    uint16_t now = timer_read();
    uint16_t bit = _kana_base.key_bit(keycode);
    KANA_t kana = _kana_base.key_kana(_kana_base.pressing,keycode);
    switch( (uint16_t)kana ) {
        case KANA_daku:bit = KANA_BASE_DAKU;keycode=0;kana=0;break;
        case KANA_hndk:bit = KANA_BASE_HNDK;keycode=0;kana=0;break;
        default:
            ;
    }    
    bool f = bit || kana;

    if(!f) {
        return false;
    }

	if (pressed) {
        bool update_time = true;
        if( bit == KANA_BASE_LSHIFT || bit == KANA_BASE_RSHIFT )
            update_time = false;

        if( update_time ) {
            if( _kana_base.press_cnt == 0 ) {
                _kana_base.time_start = now;
            }
            else {
                if( elapsed_time(now,_kana_base.time_start) > _kana_base.time_max ) {
                    _kana_base.time_start = now;
                    kana_base_type();
                }
            }
        }

        _kana_base.press_cnt++;
        if( kana_base_set_flg(keycode,bit,kana) )
            return true;

        kana_base_type();
        kana_base_set_flg(keycode,bit,kana);
    }
    else {
        _kana_base.press_cnt--;
		_kana_base.pressing &= ~bit;
        _kana_base.released |= bit;

        kana_base_type();

        if( _kana_base.moratorium_term == 0 )
            kana_base_type_buf(KANA_max);
        else
            _kana_base.moratorium = _kana_base.moratorium_term;
    }
    return true;
}


bool process_kana_base(uint16_t keycode, keyrecord_t *record) {
    if( is_kana_base_mode() ) {
        if(_kana_base.moratorium) {
            _kana_base.moratorium-=TICKMS;
            if(_kana_base.moratorium<=0) {
                kana_base_type_buf(KANA_max);
            }
        }

        if( keycode == 0 )
            return true;

        return ! kana_base_key(keycode,record->event.pressed);
    }

    return true;
}

