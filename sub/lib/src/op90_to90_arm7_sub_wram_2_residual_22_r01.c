#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {
    void *buf;
    void *stack;
    u32 stackSize;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    void *callback;
    void *staElement;
    u32 unk30;
} WvrInitParam;
typedef struct {
    u32 prio[7];
} WvrPriority;
extern u8 wvrWlWork[];
extern u8 wvrWlStack[0x600];
extern u8 wvrWlStaElement[];
void __VENEER_WMSP_Init(WvrInitParam *param, WvrPriority *prio);
void WvrBegin(void *callback);
// @fn WvrBegin
void WvrBegin(void *callback) {
    WvrInitParam param;
    WvrPriority prio;
    param.buf = wvrWlWork;
    param.stack = wvrWlStack + sizeof(wvrWlStack);
    param.stackSize = sizeof(wvrWlStack);
    param.unk0C = 4;
    param.unk20 = 0;
    param.unk24 = 8;
    param.staElement = wvrWlStaElement;
    param.unk30 = 0x1c0;
    prio.prio[0] = 3;
    param.unk1C = 0x40;
    prio.prio[2] = 3;
    prio.prio[6] = 4;
    prio.prio[4] = 5;
    prio.prio[1] = 7;
    prio.prio[5] = 8;
    prio.prio[3] = 9;
    param.callback = callback;
    __VENEER_WMSP_Init(&param, &prio);
}
