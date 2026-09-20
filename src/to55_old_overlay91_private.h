#ifndef OVERLAY_91_PRIVATE_H
#define OVERLAY_91_PRIVATE_H

#include "global.h"

/* No tracked header declares any ov91_* symbol; these are private to the
 * scratch and listed as prototype proposals in report.md. */

typedef struct Ov91Work {
    /* 0x00 */ u8 unk00[0x08];
    /* 0x08 */ u32 unk08;
    /* 0x0C */ u8 unk0C[0x28];
    /* 0x34 */ void *unk34;
    /* 0x38 */ u32 unk38;
    /* 0x3C */ u32 unk3C;
    /* 0x40 */ u8 unk40[0x08];
    /* 0x48 */ u32 unk48;
    /* 0x4C */ void *unk4C;
    /* 0x50 */ u8 unk50[0x30];
    /* 0x80 */ u32 unk80;
    /* 0x84 */ u32 unk84;
} Ov91Work;

typedef struct Ov91Sub {
    /* 0x00 */ u8 unk00[0x04];
    /* 0x04 */ s32 unk04;
    /* 0x08 */ u32 unk08;
} Ov91Sub;

typedef struct Ov91Blk16 {
    u32 a, b, c, d;
} Ov91Blk16;

typedef struct Ov91Rec40 {
    u8 data[0x40];
} Ov91Rec40;

typedef struct Ov91F3D4 {
    u8 unk00[0x10];
    u32 unk10;
    u32 unk14;
} Ov91F3D4;

typedef struct Ov91Ring {
    /* 0x00 */ u8 unk00[0x24];
    /* 0x24 */ u16 head;
    /* 0x26 */ u16 tail;
} Ov91Ring;

typedef struct Ov91Cnt {
    /* 0x00 */ u8 unk00[0x34];
    /* 0x34 */ u16 unk34;
} Ov91Cnt;

typedef struct Ov91Ev {
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
} Ov91Ev;

typedef struct Ov91Cam {
    /* 0x00 */ u8 unk00[0x40];
    /* 0x40 */ s32 unk40;
} Ov91Cam;

typedef struct Ov91Banks {
    u32 w[10];
} Ov91Banks;

#endif /* OVERLAY_91_PRIVATE_H */
