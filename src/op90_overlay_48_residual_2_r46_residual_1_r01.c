#pragma force_active on
#include "global.h"

typedef struct Ov48Param {
    u32 value;
} Ov48Param;

typedef struct Ov48List {
    u8 unk0[4];
} Ov48List;

typedef struct Ov48Unk {
    u8 unk0[0x1C];
    Ov48Param param;
    u8 unk20[0x204];
    Ov48List list;
} Ov48Unk;

u32 ov48_022591D8(Ov48Unk *data);
u32 ov48_02259BBC(Ov48List *list);
u32 ov48_0225932C(Ov48Unk *data, u32 index, Ov48Param param);
u32 ov48_02259B84(Ov48List *list, u32 index);
u32 ov48_02259BA0(Ov48List *list, u32 index);
BOOL ov48_02259188(Ov48Unk *data, u32 *out);

// @fn ov48_02259188
BOOL ov48_02259188(Ov48Unk *data, u32 *out) {
    u32 index = ov48_022591D8(data);
    if (index >= ov48_02259BBC(&data->list)) {
        return FALSE;
    }
    out[0] = ov48_0225932C(data, index, data->param);
    out[1] = ov48_02259B84(&data->list, index);
    out[2] = ov48_02259BA0(&data->list, index);
    return TRUE;
}
