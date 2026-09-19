#ifndef TO52_OV96_R109_R4_H
#define TO52_OV96_R109_R4_H
#include "global.h"
#include "bg_window.h"
#include <nitro/fx/fx_vec.h>
typedef u8 R109;
typedef struct R109Task {
    u8 state;
    u8 index;
    u8 counter;
    s8 result;
    void *task;
    R109 *owner;
    u8 pad_C;
    u8 timer;
    u8 direction;
    u8 alternate;
} R109Task;
extern const u8 ov96_0221D1F4[4];
extern void ov96_02214044(R109 *, u32, u32);
extern u32 ov96_022143FC(s32, s32);
extern void GF_AssertFail(void);
void ov96_02214258(R109 *, u32, u8, u8);
void ov96_02214278(void *, R109Task *);
u32 ov96_02214394(u32, u8);
typedef struct R109Pos { u8 pad[8]; s32 x; s32 y; } R109Pos;
u32 ov96_022143DC(const void *);
#endif
