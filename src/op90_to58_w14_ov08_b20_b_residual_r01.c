#pragma force_active on
#include "global.h"

typedef struct Ov08Mon {
    u8 pad0[4];
    u16 species;
    u8 pad6[0x10 - 6];
    u16 hp;
    u8 pad12[0x17 - 0x12];
    u8 unk17_0 : 7;
    u8 unk17_7 : 1;
    u8 pad18[0x50 - 0x18];
} Ov08Mon;

typedef struct Ov08Work {
    void *context;
    Ov08Mon mons[6];
} Ov08Work;

extern u8 ov08_02225A56[], ov08_02225A57[];
const u16 *ov08_02221C20(Ov08Work *work, int index, int mode, u8 flag);
int ov08_0221DB24(Ov08Work *work, u8 index);
void ov08_02221D2C(Ov08Work *work, u16 *buf, int index, int mode, u8 flag);

// @fn ov08_02221D2C
void ov08_02221D2C(Ov08Work *work, u16 *buf, int index, int mode, u8 flag) {
    const u16 *src = ov08_02221C20(work, index, mode, flag);
    u8 width = ov08_02225A56[index * 4];
    u8 height = ov08_02225A57[index * 4];
    int size = width * height;
    u16 tiles[2];
    u8 i, j;

    memcpy(buf, src, size * 2);
    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (work->mons[index].species == 0) {
            break;
        }
        if (work->mons[index].unk17_7) {
            tiles[0] = buf[width * 2 + 5];
            tiles[1] = buf[width + width * 2 + 5];
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 9; j++) {
                    buf[(i + 2) * width + 6 + j] = tiles[i];
                }
            }
        } else if (work->mons[index].hp == 0) {
            for (i = 0; i < size; i++) {
                buf[i] = (buf[i] & 0xFFF) | 0x2000;
            }
        } else if (ov08_0221DB24(work, index) == 1) {
            for (i = 0; i < size; i++) {
                buf[i] = (buf[i] & 0xFFF) | 0x1000;
            }
        }
        break;
    case 27:
        for (i = 0; i < size; i++) {
            buf[i] = (buf[i] & 0xFFF) | 0xA000;
        }
        break;
    }
}
