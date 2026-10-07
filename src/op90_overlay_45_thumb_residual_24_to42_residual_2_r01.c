#pragma force_active on
#include "global.h"

typedef struct Ov45Unk {
    u8 unk0[0x128];
    u32 flags[4][3];
} Ov45Unk;

u32 ov45_0222C5B4(Ov45Unk *data);
void ov45_0222C514(Ov45Unk *data);

// @fn ov45_0222C514
void ov45_0222C514(Ov45Unk *data) {
    u32 index = ov45_0222C5B4(data);
    if (index != (u32)-1) {
        data->flags[index / 3][index % 3] = 1;
    }
}
