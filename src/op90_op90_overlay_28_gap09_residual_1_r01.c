#pragma force_active on
#include "global.h"
#include "unk_02005D10.h"

int abs(int x);

typedef struct Ov28Unk {
    u8 unk0[0x208];
    s16 unk208;
    s16 unk20A;
    u8 unk20C[4];
    int state;
    u32 unk214;
    u32 unk218;
    u8 unk21C[0x11C];
    u16 counter;
    s16 pitch;
} Ov28Unk;

void ov28_0225E374(Ov28Unk *data, u16 *x, u16 *y);
void ov28_0225E730(Ov28Unk *data);

#pragma opt_propagation off
// @fn ov28_0225E730
void ov28_0225E730(Ov28Unk *data) {
    u16 x;
    u16 y;
    s16 pitch;

    ov28_0225E374(data, &x, &y);
    pitch = (x + 2 * y) * 64 - 960;
    if (data->state == 4) {
        int tx = (data->unk214 >> 3) - 3;
        int ty = (data->unk218 >> 3) - 5;
        pitch = pitch + (16 - (abs(data->unk208 + 7 - tx) + abs(data->unk20A + 7 - ty))) * 64;
    }
    if (data->counter == 0) {
        PlaySE_SetPitch(0x93D, pitch);
    }
    if (data->counter == 10) {
        data->counter = 0;
    } else {
        data->counter++;
    }
    if (data->pitch != pitch) {
        data->pitch = pitch;
    }
}
#pragma opt_propagation reset
