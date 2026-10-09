#pragma force_active on
#include "global.h"
#include "save.h"
#include "system.h"

typedef struct {
    u8 unk0[0x14];
    int state;
    SaveData *save;
    u8 unk1C[0x18];
} Ov74SaveWork;

extern u32 ov74_0223D450;
extern Ov74SaveWork ov74_0223D454;

#define SAVE_WORK ((Ov74SaveWork *)(&ov74_0223D450 + 1))

int ov74_022360B0(void);
void ov74_02236128(void);

// @fn ov74_022360B0
int ov74_022360B0(void) {
    Ov74SaveWork *work = SAVE_WORK;
    int ret;

    switch (ov74_0223D454.state) {
    case 0:
        sub_0201A728(4);
        Save_PrepareForAsyncWrite(work->save, 2);
        work->state++;
        break;
    case 1:
        ret = Save_WriteFileAsync(work->save);
        if (ret == 3) {
            work->state = 3;
        } else if (ret == 2) {
            work->state = 2;
        } else if (ret == 1) {
            work->state = 4;
        }
        if (ret == 2 || ret == 3) {
            sub_0201A738(4);
        }
        return ret;
    case 2:
    case 3:
    case 4:
        break;
    }
    return 0;
}

// @fn ov74_02236128
void ov74_02236128(void) {
    Ov74SaveWork *work = SAVE_WORK;

    if (ov74_0223D454.state == 4) {
        work->state = 1;
    }
}
