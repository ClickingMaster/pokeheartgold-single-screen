#pragma force_active on
#include "global.h"
typedef struct {
    u8 unk0;
    u8 slot;
    u8 unk2;
    u8 flag : 1;
    u8 rest : 7;
    u8 unk4;
} Entry;
typedef struct {
    u8 pad[8];
    u32 slots[4];
    u8 side;
} Info;
typedef struct {
    u8 pad[0x1c];
    Info *info;
    u8 pad2[0x334 - 0x20];
    Entry *entries;
    u8 pad3[0x435 - 0x338];
    u8 active;
} W;
int ov108_021EA52C(W *w, int index);
// @fn ov108_021EA52C
int ov108_021EA52C(W *w, int index) {
    if (!w->active) {
        return 4;
    }
    if (w->info->side != w->entries[index].flag) {
        if (w->info->side == 0) {
            return 2;
        }
        return 3;
    }
    if (*(u8 *)&w->info->slots[w->entries[index].slot] == 0) {
        return 1;
    }
    return 0;
}
