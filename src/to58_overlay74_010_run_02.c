#include "to58_overlay74_010_private.h"

extern void ClearFrameAndWindow2(void *window, int);
extern void ClearWindowTilemapAndCopyToVram(void *window);
extern void RemoveWindow(void *window);

void ov74_02231D48(u8 *dst, u8 *src, int unused, u32 value) {
    *(u32 *)(dst + 0) = *(u32 *)(src + 0x28);
    *(u32 *)(dst + 4) = (u32)(src + 0x184);
    *(u32 *)(dst + 0x10) = 0;
    *(u32 *)(dst + 0x14) = 0x1000;
    *(u32 *)(dst + 0x18) = 0x1000;
    *(u32 *)(dst + 0x1c) = 0x1000;
    *(u16 *)(dst + 0x20) = 0;
    *(u32 *)(dst + 0x24) = 0x14;
    *(u32 *)(dst + 0x28) = value;
    *(u32 *)(dst + 0x2c) = 0x4c;
}

u32 ov74_02231D70(u8 *base, int box, int slot) {
    u8 *boxes = *(u8 **)(base + 0xE880);
    return AGB_GetBoxMonData(boxes + 4 + box * 0x960 + slot * 0x50, 11, NULL);
}

u32 ov74_02231D94(u8 *base, int box, int slot) {
    u8 *boxes = *(u8 **)(base + 0xE880);
    return AGB_GetBoxMonData(boxes + 4 + box * 0x960 + slot * 0x50, 45, NULL);
}

u32 ov74_02231DB8(u8 *base, int box, int slot) {
    u8 *boxes = *(u8 **)(base + 0xE880);
    return AGB_GetBoxMonData(boxes + 4 + box * 0x960 + slot * 0x50, 0, NULL);
}

void ov74_02231DDC(u8 *out, int y, int x, int height, int width) {
    int halfWidth = width / 2;
    int halfHeight;
    out[0] = x - halfWidth;
    halfHeight = height / 2;
    out[2] = y - halfHeight;
    out[1] = x + halfWidth;
    out[3] = y + halfHeight;
}

void *ov74_02231E00(int narcId, int member, NNSG2dCharacterData **out, int heapId) {
    void *buffer = Heap_AllocAtEnd((enum HeapID)heapId, 0x1000);
    if (buffer != NULL) {
        ReadWholeNarcMemberByIdPair(buffer, (NarcId)narcId, member);
        if (!NNS_G2dGetUnpackedBGCharacterData(buffer, out)) {
            Heap_Free(buffer);
            return NULL;
        }
    }
    return buffer;
}

int ov74_02231E38(int member, NNSG2dCharacterData **out, void *buffer, NARC *narc) {
    NARC_ReadWholeMember(narc, member, buffer);
    return NNS_G2dGetUnpackedBGCharacterData(buffer, out);
}
