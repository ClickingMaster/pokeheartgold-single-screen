#ifndef OVERLAY49_ENTRY40_H
#define OVERLAY49_ENTRY40_H
#include "global.h"
typedef struct Ov49R13Interpolation {
    u32 raw[4];
} Ov49R13Interpolation;

typedef struct Ov49R13Position {
    u32 x;
    u32 z;
    u32 y;
} Ov49R13Position;

typedef struct Ov49R13Work {
    void *field;
    void *object;
    u16 state;
    u8 finished;
    u8 padding_0B;
    union {
        struct {
            s16 timer;
            s16 selected;
        };
        Ov49R13Interpolation interpolation;
    };
    u32 counter;
} Ov49R13Work;

typedef struct Ov49R14Anim {
    struct UnkStruct_ov42_02228110 *unk00;
    void *unk04;
    u16 unk08;
    u8 unk0A;
    u8 unk0B;
    u32 unk0C[4];
    s32 unk1C;
    s32 unk20;
} Ov49R14Anim;

typedef struct Ov49R14Spin {
    void *unk00;
    void *unk04;
    u16 unk08;
    u8 unk0A;
    u8 unk0B;
    s32 unk0C;
    VecFx32 unk10;
} Ov49R14Spin;

typedef struct { void *field; } Ov49InputWork;
typedef struct { void *unk0; void *obj; u16 state; u8 padA[2]; } Ov49R14Work;

typedef struct Ov49EntryR4 Ov49EntryR4;
typedef void (*Ov49EntryCallback)(void*,void*);
struct Ov49EntryR4 {
 union {u8 raw[36];struct {void *first,*second;u16 counter;u8 mode,state;u8 payload[24];} fields;Ov49R13Work transition;Ov49R14Anim animation;Ov49R14Spin spin;Ov49InputWork input;Ov49R14Work cleanup;} storage;
 union {Ov49EntryCallback callback;u32 bits;} tail;
};
#endif
