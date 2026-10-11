#include "to52_r4_overlay15_prefix4_private.h"

void BagApp_LoadCursorPositions(void *work) {
    u8 *entries;
    u16 i;
    (*(u8 **)((u8 *)work + 0x234))[0x64] = 0;
    entries = *(u8 **)((u8 *)work + 0x234) + 4;
    if (*(void **)(*(u8 **)((u8 *)work + 0x234) + 0x6c) == NULL) {
        for (i = 0; i < 8; i++) {
            u8 *entry = entries + i * 12;
            if (*(void **)entry != NULL) {
                *(u16 *)(entry + 4) = 0;
                *(u16 *)(entry + 6) = 0;
            }
        }
        for (i = 0; i < 8; i++) {
            if (*(void **)(entries + i * 12) != NULL) {
                (*(u8 **)((u8 *)work + 0x234))[0x64] = i;
                return;
            }
        }
        return;
    }
    for (i = 0; i < 8; i++) {
        u8 *entry = entries + i * 12;
        if (*(void **)entry != NULL) {
            u8 pos[2];
            BagCursor_Field_PocketGetPosition(*(void **)(*(u8 **)((u8 *)work + 0x234) + 0x6c), entry[8], &pos[1], &pos[0]);
            *(u16 *)(entry + 4) = pos[1];
            *(u16 *)(entry + 6) = pos[0];
        }
    }
    {
        u32 pocket = BagCursor_Field_GetPocket(*(void **)(*(u8 **)((u8 *)work + 0x234) + 0x6c));
        if (*(void **)(entries + pocket * 12) == NULL) {
            u16 j;
            for (j = 0; j < 8; j++) {
                if (*(void **)(entries + j * 12) != NULL) {
                    pocket = j;
                    break;
                }
            }
        }
        for (i = 0; i < 8; i++) {
            u8 *entry = entries + i * 12;
            if (*(void **)entry != NULL && pocket == entry[8]) {
                (*(u8 **)((u8 *)work + 0x234))[0x64] = i;
                return;
            }
        }
    }
}

void BagApp_SaveCursorPositions(void *work) {
    u8 *state = *(u8 **)((u8 *)work + 0x234);
    u8 *entries;
    u32 i;
    if (*(void **)(state + 0x6c) == NULL) {
        return;
    }
    entries = state + 4;
    for (i = 0; i < 8; i++) {
        u8 *entry = entries + i * 12;
        if (*(void **)entry != NULL) {
            BagCursor_Field_PocketSetPosition(*(void **)(*(u8 **)((u8 *)work + 0x234) + 0x6c), entry[8], (u8) * (u16 *)(entry + 4), (u8) * (s16 *)(entry + 6));
        }
    }
    state = *(u8 **)((u8 *)work + 0x234);
    {
        u8 *entry = entries + state[0x64] * 12;
        BagCursor_Field_SetPocket(*(void **)(state + 0x6c), entry[8]);
    }
}
