#pragma force_active on
#include "global.h"
typedef struct Slot {
    u8 state;
    u8 kind;
    u8 friend;
    u8 pad3[5];
    u8 x8;
    u8 pad9[3];
    u32 id;
    u8 pad10[8];
} Slot;
typedef struct Owner {
    u8 pad0[0xc];
    Slot slots[32];
} Owner;
typedef struct Info {
    u32 playerIds[4];
    u8 pad10[8];
    u8 flags[4];
} Info;
typedef struct Sub {
    u32 id;
    u8 pad4[0x2c];
    Info info;
} Sub;
typedef struct Data {
    u8 pad0[0x50];
    Sub sub;
} Data;
extern const u16 _020FC824[];
void sub_0205AEA0(u8 *slots, int index, u8 value);
u8 PalPad_PlayerIdIsFriendOrMutual(void *palPad, u32 id);
BOOL sub_0205AD9C(Owner *owner, int idx, Data *data, void *palPad);
#pragma opt_strength_reduction off
// @fn sub_0205AD9C
BOOL sub_0205AD9C(Owner *owner, int idx, Data *data, void *palPad) {
    BOOL ret = FALSE;
    int i;
    int k;
    Sub *sub;
    Info *info;
    u32 *ids;
    if (data == NULL) {
        k = _020FC824[idx];
        for (i = 0; i < 4; i++) {
            sub_0205AEA0((u8 *)owner->slots, k, 3);
            k++;
        }
        return FALSE;
    }
    sub = &data->sub;
    info = &sub->info;
    if (sub->id != owner->slots[idx].id) {
        k = _020FC824[idx];
        for (i = 0; i < 4; i++) {
            sub_0205AEA0((u8 *)owner->slots, k, 3);
            k++;
        }
        return FALSE;
    }
    k = _020FC824[idx];
    ids = info->playerIds;
    for (i = 0; i < 4; i++) {
        switch (owner->slots[k].kind) {
        case 0:
            if (info->flags[i] != 0) {
                owner->slots[k].state = 2;
                owner->slots[k].x8 = info->flags[i] & 0x7f;
                owner->slots[k].friend = PalPad_PlayerIdIsFriendOrMutual(palPad, *ids);
                ret = TRUE;
            }
            break;
        case 2:
            if (info->flags[i] == 0) {
                sub_0205AEA0((u8 *)owner->slots, k, 3);
            } else {
                ret = TRUE;
            }
            break;
        case 4:
            owner->slots[k].state = 0;
            break;
        }
        k++;
        ids++;
    }
    return ret;
}
#pragma opt_strength_reduction reset
