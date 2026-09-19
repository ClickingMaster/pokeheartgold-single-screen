#include "private_w09.h"

u32 ov13_02221410(u32 value, u32 mask) {
    u32 common = value & mask;
    mask = ~mask;
    value &= mask;
    value++;
    value |= common;
    mask |= common;
    if (value >= mask) {
        value = common | 1;
    }
    return value;
}
void ov13_02221428(void) {
    if (_0224CF98.unk0C != NULL) {
        ov13_022208F8(_0224CF98.unk0C);
        _0224CF98.unk0C = NULL;
    }
    if (_0224CF98.unk04 != NULL) {
        ov13_022208F8(_0224CF98.unk04);
        _0224CF98.unk04 = NULL;
    }
}
void ov13_02221454(u8 *src) {
    ov13_02222978(ov13_0224DCF8, 0, 8);
    _0224CF98.unk10 = 1;
    ov13_02222978(ov13_0224CFBC, 0, 0x1C);
    _0224CF98.unk24 = src + 6;
    _0224CF98.unk28 = *(u16 *)(src + 4);
    _0224CF98.unk2C = *(u16 *)src & 0xF;
    ov13_0224CFB8[0x1D] = src[2];
    _0224CF98.unk30 = 0;
    _0224CF98.unk34 = 0xC0A80B01;
    ov13_0224CFB8[0x1C] = 0;
}
void ov13_022214AC(int value) {
    _0224CF98.unk10 = value;
}
int ov13_022214B8(void) {
    return _0224CF98.unk10;
}
