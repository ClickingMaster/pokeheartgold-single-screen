#pragma force_active on
#include "global.h"

typedef struct Ov48Unk {
    int state;
    u8 unk4[0x1B4];
    u8 unk1B8[0x12C];
    u8 unk2E4[4];
} Ov48Unk;

void ov48_0225A338(Ov48Unk *data, void *arg);
void ov48_0225A354(Ov48Unk *data, void *a1);
BOOL ov48_0225A41C(Ov48Unk *data, void *arg);
void ov48_0225A428(Ov48Unk *data, void *a1, void *arg);
BOOL ov48_0225A430(Ov48Unk *data, void *arg);
void ov48_0225AD54(void *a0, void *a1);
void ov48_0225A158(Ov48Unk *data, void *a1, void *arg);

// @fn ov48_0225A158
void ov48_0225A158(Ov48Unk *data, void *a1, void *arg) {
    switch (data->state) {
    case 0:
        ov48_0225A338(data, arg);
        ov48_0225AD54(data->unk1B8, data->unk2E4);
        break;
    case 1:
        ov48_0225A354(data, a1);
        data->state = 2;
        break;
    case 2:
        if (ov48_0225A41C(data, arg) == TRUE) {
            data->state = 0;
        }
        break;
    case 3:
        ov48_0225A428(data, a1, arg);
        ov48_0225A430(data, arg);
        data->state = 4;
        break;
    case 4:
        if (ov48_0225A430(data, arg) == TRUE) {
            data->state = 0;
        }
        break;
    }
}
