#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct State {
    u8 unk00[4];
    void *(*alloc)(u32 size);
    u8 unk08[4];
    u32 unk0C;
    u32 unk10;
    u8 unk14[0x1C];
    void *stack;
    u8 unk34[4];
    volatile int state; /* shared with the worker thread */
    u8 unk3C[0x28];
    int active;
    u8 unk68[0xC];
    int ready;
    int error;
    u32 unk7C;
} State;
typedef struct Config {
    u8 unk00[8];
    u32 stackSize;
    u32 deadline;
    volatile u32 count;
} Config;
typedef struct Work {
    u32 words[58];
} Work;
extern State ov13_0224DF30;
extern Config ov13_02245A58;
extern OSThread ov13_0224DFDC;
extern Work ov13_0224E09C;
int ov13_02223EE0(u32 count);
void ov13_02225320(void *arg);
void ov13_02226F3C(void);
u32 ov13_02226CBC(void);
int ov13_02226D74(u32 priority, u32 count, u32 unk7C, void *(*alloc)(u32), u32 unk0C, u32 stackSize);
// @fn ov13_02226D74
int ov13_02226D74(u32 priority, u32 count, u32 unk7C, void *(*alloc)(u32), u32 unk0C, u32 stackSize) {
    int result;
    int state;

    state = ov13_0224DF30.state;
    if (state >= 1 && state <= 5) {
        return -10;
    }
    ov13_02245A58.count = count;
    ov13_0224DF30.state = 7;
    ov13_0224DF30.unk7C = unk7C;
    ov13_0224DF30.alloc = alloc;
    ov13_0224DF30.unk0C = unk0C;
    ov13_02245A58.stackSize = stackSize;
    result = ov13_02223EE0(count);
    ov13_0224DF30.ready = 1;
    if (result < 0) {
        ov13_0224DF30.error = result;
        return result;
    }
    ov13_0224DF30.stack = ov13_0224DF30.alloc(ov13_02245A58.stackSize);
    if (ov13_0224DF30.stack == NULL) {
        ov13_0224DF30.error = -1;
        return -1;
    }
    if (OS_IsThreadAvailable() != TRUE) {
        ov13_0224DF30.error = -9;
        return -9;
    }
    OS_CreateThread(&ov13_0224DFDC, ov13_02225320, NULL, (u8 *)ov13_0224DF30.stack + (ov13_02245A58.stackSize & ~7), ov13_02245A58.stackSize, priority);
    ov13_0224DF30.state = 1;
    ov13_02245A58.deadline = ov13_02226CBC() + 60000;
    ov13_0224DF30.unk10 = 0;
    memset(&ov13_0224E09C, 0, sizeof(Work));
    ov13_02226F3C();
    OS_WakeupThreadDirect(&ov13_0224DFDC);
    ov13_0224DF30.active = 1;
    return 1;
}
