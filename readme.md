# QMK Harmonizeライブラリ

Copyright 2019 Bottilabo [twitter @bottilabo]
                         [https://github.com/bottilabo/qmk]

QMK Harmonizeライブラリは、
一つの仮想的なキーボードの定義から各キーボードに配列をマッピングさせることにより、仮想キーボードの定義を変更するだけで、対応するすべてのキーボードの配列を同じように変更することができます。
QMKを使った一般的なキーボードであれば、簡単に組み込むことができます。
仮想キーボードの配列をベースにして、コンパイル時にキー配列を最大３種類、日本語入力モードを最大３種類設定し、動作時に切り替えることができます。
日本語入力モードのかな配列では、PCへのキー出力を「かな」もしくは「ローマ字」のどちらかを動作中に変更できます。

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

# License
　特にこだわりはないのでとりあえずGPLにしました。
