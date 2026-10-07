#pragma force_active on
#include "global.h"

typedef struct Ov45Sub {
    u8 unk0[4];
    u16 id;
    u8 state;
    u8 unk7;
    u8 unk8[2];
    u8 active;
    u8 result;
} Ov45Sub;

typedef struct Ov45Unk {
    u32 unk0;
    u8 unk4[0x104];
    u8 unk108[0x20];
    u8 unk128[0x98];
    Ov45Sub sub;
    u8 unk1CC[0x360];
    u32 unk52C;
} Ov45Unk;

u32 ov45_0222EC68(void *obj);
BOOL ov45_0222BADC(void *a0, u32 a1);
BOOL ov45_0222A920(void *a0);
BOOL ov45_0222AFF8(Ov45Unk *data);
BOOL ov45_0222B048(Ov45Unk *data, u32 id);
void ov45_0222BE08(Ov45Unk *data, void *obj, BOOL flag);
BOOL ov45_0222BD74(Ov45Sub *sub, u32 id);
void ov45_0222BE48(Ov45Sub *sub);
void ov45_0222C688(void *obj, u16 *cmd, void *unused, Ov45Unk *data);

// @fn ov45_0222C688
void ov45_0222C688(void *obj, u16 *cmd, void *unused, Ov45Unk *data) {
    u32 id = ov45_0222EC68(obj);
    BOOL a;
    BOOL b;

    if (ov45_0222BADC(data->unk108, data->unk0) == FALSE) {
        data->unk52C = 1;
    }
    a = ov45_0222A920(data->unk128);
    b = ov45_0222AFF8(data);
    if (a != TRUE || id == (u32)-1 || cmd[1] != 1 || b == TRUE || ov45_0222B048(data, id) == FALSE) {
        ov45_0222BE08(data, obj, FALSE);
    } else {
        ov45_0222BE08(data, obj, TRUE);
        if (ov45_0222BD74(&data->sub, id) == FALSE) {
            data->sub.result = TRUE;
        } else {
            data->sub.result = FALSE;
        }
        data->sub.id = id;
        data->sub.state = 2;
        data->sub.active = 2;
        data->sub.unk7 = 0;
        ov45_0222BE48(&data->sub);
    }
}
