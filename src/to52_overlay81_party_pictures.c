#include "to52_overlay81_party_pictures_private.h"

void ov81_02241DDC(void *work, int arg) {
    u32 yOffset, y, step, i;
    u8 *slot, *pic;
    if (W32(work, 0x47C) == 3) {
        y = 0x30;
        step = 0x50;
    } else {
        y = 0x48;
        step = 0x70;
    }
    i = 0;
    if (i < W32(work, 0x47C)) {
        yOffset = i;
        slot = work;
        pic = work;
        do {
            void *mon = Party_GetMonByIndex(PTR(work, 0x3C0), W16(slot, 0x3C8));
            ov81_02240F48(work, (u8)i, mon, y + yOffset, 0x90, arg);
            Pokepic_SetAttr(PTR(pic, 0x1AC), 6, 1);
            i++;
            yOffset += step;
            pic += 4;
            slot += 2;
        } while (i < W32(work, 0x47C));
    }
}

void ov81_02241E68(void *work, int index, int monIndex, int arg) {
    Pokepic_Delete(PTR((u8 *)work + index * 4, 0x1AC));
    if (W32(work, 0x47C) == 3) {
        void *mon = Party_GetMonByIndex(PTR(work, 0x3C0), monIndex);
        ov81_02240F48(work, (u8)index, mon, index * 0x50 + 0x30, 0x90, arg);
    } else {
        void *mon = Party_GetMonByIndex(PTR(work, 0x3C0), monIndex);
        ov81_02240F48(work, (u8)index, mon, index * 0x70 + 0x48, 0x90, arg);
    }
}

void ov81_02241EDC(void *work, int index, int monIndex, int arg) {
    Pokepic_Delete(PTR((u8 *)work + index * 4, 0x1AC));
    if (W32(work, 0x47C) == 3) {
        void *mon = Party_GetMonByIndex(PTR(work, 0x3C4), monIndex);
        ov81_02240F48(work, (u8)index, mon, index * 0x50 + 0x30, 0x90, arg);
    } else {
        void *mon = Party_GetMonByIndex(PTR(work, 0x3C4), monIndex);
        ov81_02240F48(work, (u8)index, mon, index * 0x70 + 0x48, 0x90, arg);
    }
}

void ov81_02241F50(void *work) {
    u32 y, step;
    u32 i;
    u32 offset;
    u8 *dst;
    if (W32(work, 0x47C) == 3) {
        y = 0x30;
        step = 0x50;
    } else {
        y = 0x48;
        step = 0x70;
    }
    i = 0;
    if (i < W32(work, 0x47C)) {
        offset = i;
        dst = work;
        do {
            void *obj = ov81_02242CBC((u8 *)work + 0x1C4, 1, 3, 1, 0, y + offset, 0x90);
            PTR(dst, 0x39C) = obj;
            ov81_02242D88(PTR(dst, 0x39C), 0);
            i++;
            offset += step;
            dst += 4;
        } while (i < W32(work, 0x47C));
    }
}
