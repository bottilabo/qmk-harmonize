# QMK Harmonizeライブラリ

Copyright 2019-2020 Bottilabo [twitter @bottilabo]
                         [https://github.com/bottilabo/qmk]

QMK Harmonizeライブラリは、
- 一つの仮想的なキーボードの定義から各キーボードに配列をマッピングさせることにより、仮想キーボードの定義を変更するだけで、対応するすべてのキーボードの配列を同じように変更することができます。
- QMKを使った一般的なキーボードであれば、簡単に組み込むことができます。
- 仮想キーボードの配列をベースにして、コンパイル時にキー配列を最大３種類、日本語入力モードを最大３種類設定し、動作時に切り替えることができます。
- 日本語入力モードのかな配列では、PCへのキー出力を「かな」もしくは「ローマ字」のどちらかを動作中に変更できます。


# 仮想キーボード
デフォルトのKBD_HARMONIZE　を　KBD_USER　のようにコピーして修正してください。  
新しく作った仮想キーボードの配列はビルド時に　-DKBD=KBD_USER　のように指定してください。  




# インストール方法

## 通常のQMKの場合

qmk-harmonize/qmk_firmware/keyboards　を　qmk_firmware/keyboards　にマージ／上書きコピーしてください。


## BMP(BLE Micro Pro)の場合

　BMPのファームウェアがキー配列をJSONから設定する機能とHarmonizeライブラリがQMKの同じ機能を使用して競合しているため、手動で次のようにコードを追加、修正してください。


qmk_firmware_bmp/tmk_core/protocol/nrf/bmp.c
```
__attribute__ ((weak))
uint16_t transkeycode(uint16_t kc)
{
	return kc;
}

uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
  return transkeycode( BMPAPI->app.keymap_key_to_keycode(layer, (bmp_api_keypos_t*)&key) );
}
```

qmk-harmonize/qmk_firmware_bmp/keyboards　を　qmk_firmware_bmp/keyboards　にマージ／上書きコピーしてください。

# ビルド方法
```
make crkbd:harmonize-crkbd EXTRAFLAGS='-DKBD=KBD_HARMONIZE -DKBLAYOUT1=KB_COLEMAK -DKBLAYOUT2=KB_DVORAK -DKBLAYOUT3=KB_MSIKI -DKBIM1=IM_ROMAJI_COLEMAK -DKBIM2=IM_TRON -DKBIM3=IM_SINJIS'
```

例のようにKBLAYOUT1-3 KBIM1-3に使用したい配列、日本語入力モードを指定してください。  
ProMicroは容量が少ないため、日本語入力モードを複数入れることができないことがあります。  
容量が足りない場合は、次のように必要なものだけ指定してビルドします。  
```
make crkbd:harmonize-crkbd EXTRAFLAGS='-DKBD=KBD_HARMONIZE -DKBLAYOUT1=KB_COLEMAK -DKBIM1=IM_ROMAJI_COLEMAK'
```

## 容量削減

```
-DNO_KANA
```
かなキーでの出力を無効にし、ローマ字でのみの出力にして使用する容量を削減します
。

# 作成済みのキー配列

- arensito
- Astarte
- beakl4opt-ergo
- capewell-dvorak
- colemak
- colemak-mod_dh
- dvorak
- eucalyn
- harmonix
- harmony
- M式
- mtgap
- mtgap2
- norman
- plum
- qgmlwy
- workman

# 作成済み日本語入力モード

## かな入力
- JISかな配列
- 親指シフト（ニコラ配列）
- トロン配列
- 新JIS配列
- 飛鳥配列
- いろは坂配列
- 薙刀式

かな入力配列では、キーの出力を「ローマ字」と「かな」のどちらでも行えます。ローマ字出力で後置の濁点を処理するため、新JISなどでは濁点をすばやく後置入力しなければなりません。また、後置入力を処理するため、入力から一呼吸おいてからキーが出力されます。


## ローマ字入力
- カタナ式
- KM式
- Ｍ式
- ローマ字colemak

# keymap.cへの追加方法
- 仮想キーボードからのマッピング定義
- harmonizeライブラリの読み込み
- matrix_init_userへの追加
- matrix_scan_userへの追加
- process_record_userへの追加
が必要です。

## keymap.c 例
```
#include QMK_KEYBOARD_H

#define HAS_THUMBROW

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
ESC , L05, L04, L03, L02, L01,                     R01, R02, R03, R04, R05, R06,BSPC, \
CAPS, L15, L14, L13, L12, L11,                     R11, R12, R13, R14, R15, ENTR , \
LCTL, L24, L23, L22, L21, L25,                     R21, R22, R23, R24, R25, ADJ, \
        LGUI ,TL1   ,      TL0,   TR0,              TR1,RCTL,RALT  \
    )
#define LAYOUT_DEF(...)                  LAYOUT_remapper(__VA_ARGS__)

#include "../../../harmonize/harmonize.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  HARMONIZE_PROC_RECORD_USER;
  return true;
}
void matrix_init_user(void) {
    harmonize_init();
}
void matrix_scan_user(void) {
   HARMONIZE_MATRIX_SCAN_USER;
}

```



# コンパイル済みファームウェア

- [crkbd_colemak_dvorak_nicola.hex](precompiled/crkbd_colemak_dvorak_nicola.hex)  
`make crkbd:harmonize-crkbd EXTRAFLAGS='-DNO_KANA -DKBLAYOUT1=KB_COLEMAK -DKBLAYOUT2=KB_DVORAK -DKBIM1=IM_NICOLA'`


# License
　特にこだわりはないのでとりあえずGPLにしました。
