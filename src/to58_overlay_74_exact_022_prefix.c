#include "global.h"

void ov74_02234468(u8 *data);
void ov74_02234488(u8 *data);

void ov74_02234468(u8 *data) {
    u32 *key = (u32 *)data;
    u32 *ptr;
    u32 i = 0;
    ptr = (u32 *)(data + 0x20);
    for (; i < 12; i++) {
        u32 x = *ptr ^ key[1];
        *ptr = x;
        *ptr++ = x ^ key[0];
    }
}

void ov74_02234488(u8 *data) {
    u32 *key = (u32 *)data;
    u32 *ptr;
    u32 i = 0;
    ptr = (u32 *)(data + 0x20);
    for (; i < 12; i++) {
        u32 x = *ptr ^ key[0];
        *ptr = x;
        *ptr++ = x ^ key[1];
    }
}
