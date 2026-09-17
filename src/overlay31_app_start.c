#include "global.h"

typedef struct Overlay31State {
    u8 data[0x300];
} Overlay31State;
extern void Heap_Create(int, int, int);
extern void *CreateSysTaskAndEnvironment(void *, int, int, int);
extern void *SysTask_GetData(void *);
extern void *Save_PlayerData_GetOptionsAddr(void *);
extern void *Save_PlayerData_GetProfile(void *);
extern void *Save_Pokeathlon_Get(void *);
extern void ov31_0225D7A0(void *, Overlay31State *);
extern void ov31_0225DAC4(Overlay31State *);
extern void ov31_0225DB38(Overlay31State *);
extern void ov31_0225D60C(Overlay31State *);
extern void ov31_0225D684(Overlay31State *, int);
extern void ov31_0225DE84(Overlay31State *);
extern void ov31_0225DF98(Overlay31State *);
extern void ov31_0225DD14(Overlay31State *);
extern void ov03_022581BC(void *);
extern void GfGfx_EngineBTogglePlanes(int, int);
extern void TextFlags_SetCanTouchSpeedUpPrint(int);
extern void TextFlags_SetFastForwardTouchButtonHitbox(const void *);
extern const u8 _0225EE40[];
void *ov31_0225D520(void *, void *, void *, void *);

void *ov31_0225D520(void *bgConfig, void *arg1, void *fieldSystem, void *work) {
    void *task;
    Overlay31State *state;

    Heap_Create(3, 8, 3 << 15);
    task = CreateSysTaskAndEnvironment(ov31_0225D7A0, 0x190, 0xA, 8);
    state = SysTask_GetData(task);
    *(void **)&state->data[8] = task;
    *(u32 *)&state->data[0] = 0;
    *(void **)&state->data[4] = bgConfig;
    *(void **)&state->data[0x18] = arg1;
    *(void **)&state->data[0x1C] = fieldSystem;
    *(u32 *)&state->data[0x30] = 0;
    *(void **)&state->data[0x164] = Save_PlayerData_GetOptionsAddr(*(void **)((u8 *)*(void **)&state->data[0x1C] + 0xC));
    *(void **)&state->data[0x168] = Save_PlayerData_GetProfile(*(void **)((u8 *)*(void **)&state->data[0x1C] + 0xC));
    *(void **)&state->data[0x16C] = Save_Pokeathlon_Get(*(void **)((u8 *)*(void **)&state->data[0x1C] + 0xC));
    *(u32 *)&state->data[0xC] = 0;
    *(void **)&state->data[0x14] = work;
    ov31_0225DAC4(state);
    ov31_0225DB38(state);
    ov31_0225D60C(state);
    ov31_0225D684(state, 0);
    ov31_0225DE84(state);
    ov31_0225DF98(state);
    ov31_0225DD14(state);
    ov03_022581BC(*(void **)&state->data[0x14]);
    *(vu32 *)0x04001000 &= 0xFFFF1FFF;
    GfGfx_EngineBTogglePlanes(1, 1);
    GfGfx_EngineBTogglePlanes(2, 1);
    GfGfx_EngineBTogglePlanes(4, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    TextFlags_SetFastForwardTouchButtonHitbox(_0225EE40);
    return task;
}
