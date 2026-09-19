#include "global.h"

typedef struct OpponentDataScratch {
    u8 pad00[0x18];
    void *sprite;
    void *fallback;
    void *primary;
    u8 pad24[4];
    u8 hpBar[0x58];
    u8 tail[8];
} OpponentDataScratch;

typedef struct CleanupTaskData {
    void *battleSystem;
    u8 pad04[4];
    u8 result;
    u8 battler;
    u8 pad0A[2];
    u8 result2;
    u8 battler2;
} CleanupTaskData;

typedef struct SmallTaskData {
    void *battleSystem;
    void *object;
    u8 result;
    u8 battler;
    u8 state;
    u8 done;
} SmallTaskData;

typedef struct TextTaskData {
    void *battleSystem;
    u8 result;
    u8 battler;
    u8 printer;
    u8 state;
    u8 timer;
} TextTaskData;

typedef struct PartyGaugeTaskData {
    void *battleSystem;
    void *object;
    u8 pad08[4];
    u8 result;
    u8 battler;
    u8 state;
} PartyGaugeTaskData;

typedef struct PokepicMoveTaskData {
    void *battleSystem;
    void *pokepic;
    u8 result;
    u8 battler;
    u8 state;
    u8 target;
    u8 counter;
    u8 period;
} PokepicMoveTaskData;

typedef struct MenuTaskData {
    u8 state;
    u8 pad01[0x0B];
    void *battleSystem;
    void *taskHandle;
    u8 pad14[0x10];
    u8 battler;
    u8 pad25[0x0B];
    void *menu;
    u8 pad34[0x0C];
    void *effect;
    u8 pad44[8];
    u8 result;
    u8 pad4D;
    u8 counter;
} MenuTaskData;

extern void MI_CpuFill8(void *dest, u8 value, u32 size);
extern void ManagedSprite_SetDrawFlag(void *sprite, int flag);
extern void ov12_0226430C(void *battleSystem, u8 battler, u8 result);
extern void Heap_Free(void *ptr);
extern void SysTask_Destroy(void *task);
extern void *ov12_0223BB78(void *battleSystem);
extern void sub_0200F450(void *object);
extern void ov12_0223BB80(void *battleSystem, int value);
extern int TextPrinterCheckActive(u8 printerId);
extern void ov12_02263A00(void *battleSystem, u8 battler);
extern void BattleHpBar_BeginExpBarFullFlashEffect(void *hpBar, u8 *done);
extern void PlaySE(u32 seqNo);
extern void GF_AssertFail(void);
extern void *ov07_0221FB90(void *battleSystem, int value, u8 battler);
extern int ov07_0221FE70(void *object);
extern void ov07_0221FE3C(void *object, u8 battler);
extern int Pokepic_GetAttr(void *pokepic, int attr);
extern void Pokepic_SetAttr(void *pokepic, int attr, int value);
extern void ov12_02264DCC(MenuTaskData *data, void *menu);
extern int ov12_02264E00(MenuTaskData *data);
extern void ov12_02264E34(MenuTaskData *data, void *effect);
extern int ov12_02264E68(MenuTaskData *data);
extern void StopSE(u32 seqNo, int fadeFrames);
void ov12_0225F960(void *task, CleanupTaskData *data);
void ov12_0225FC80(void *task, CleanupTaskData *data);
void ov12_0225FCA0(void *task, CleanupTaskData *data);
void ov12_02260584(void *task, SmallTaskData *data);
void ov12_022605D0(void *task, TextTaskData *data);
void ov12_02260614(void *task, TextTaskData *data);
void ov12_02260B30(void *task, PokepicMoveTaskData *data);
void ov12_0225FF80(void *task, PokepicMoveTaskData *data);
void ov12_0225FFDC(void *task, MenuTaskData *data);
void ov12_02260030(void *task, MenuTaskData *data);
u8 ov12_02261258(void *data);
u8 ov12_02261264(void *data);
void *ov12_02261270(OpponentDataScratch *data);
void *OpponentData_GetHpBar(OpponentDataScratch *data);
void *ov12_02261280(OpponentDataScratch *data);
void ov12_02261284(OpponentDataScratch *data);
void ov12_02261294(OpponentDataScratch *data, int flag);

#if W17_GROUP == 6
u8 ov12_02261258(void *data)
{
    return ((u8 *)data)[0x195];
}

u8 ov12_02261264(void *data)
{
    return ((u8 *)data)[0x196];
}

void *ov12_02261270(OpponentDataScratch *data)
{
    if (data->primary != NULL) {
        return data->primary;
    }
    return data->fallback;
}

void *OpponentData_GetHpBar(OpponentDataScratch *data)
{
    return data->hpBar;
}

void *ov12_02261280(OpponentDataScratch *data)
{
    return data->tail;
}

void ov12_02261284(OpponentDataScratch *data)
{
    MI_CpuFill8(data->tail, 0, 8);
}

void ov12_02261294(OpponentDataScratch *data, int flag)
{
    if (data->sprite != NULL) {
        ManagedSprite_SetDrawFlag(data->sprite, flag);
    }
}
#endif

#if W17_GROUP == 1
void ov12_0225F960(void *task, CleanupTaskData *data)
{
    ov12_0226430C(data->battleSystem, data->battler, data->result);
    Heap_Free(data);
    SysTask_Destroy(task);
}
#endif

#if W17_GROUP == 2
void ov12_0225FC80(void *task, CleanupTaskData *data)
{
    ov12_0226430C(data->battleSystem, data->battler2, data->result2);
    Heap_Free(data);
    SysTask_Destroy(task);
}

void ov12_0225FCA0(void *task, CleanupTaskData *data)
{
    ov12_0226430C(data->battleSystem, data->battler2, data->result2);
    Heap_Free(data);
    SysTask_Destroy(task);
}
#endif

#if W17_GROUP == 4
void ov12_02260584(void *task, SmallTaskData *data)
{
    switch (data->state) {
    case 0:
        BattleHpBar_BeginExpBarFullFlashEffect(data->object, &data->done);
        PlaySE(0x70C);
        data->state++;
        break;
    case 1:
        if (data->done == 1) {
            ov12_0226430C(data->battleSystem, data->battler, data->result);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}

void ov12_022605D0(void *task, TextTaskData *data)
{
    if (ov12_0223BB78(data->battleSystem) != NULL) {
        sub_0200F450(ov12_0223BB78(data->battleSystem));
        ov12_0223BB80(data->battleSystem, 0);
    }
    if (!TextPrinterCheckActive(data->printer)) {
        ov12_0226430C(data->battleSystem, data->battler, data->result);
        Heap_Free(data);
        SysTask_Destroy(task);
    }
}

void ov12_02260614(void *task, TextTaskData *data)
{
    switch (data->state) {
    case 0:
        if (!TextPrinterCheckActive(data->printer)) {
            data->state++;
            data->timer = 0;
        }
        break;
    case 1:
        data->timer++;
        if (data->timer == 40) {
            ov12_02263A00(data->battleSystem, data->battler);
            ov12_0226430C(data->battleSystem, data->battler, data->result);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}
#endif

#if W17_GROUP == 5
void ov12_02260B30(void *task, PokepicMoveTaskData *data)
{
    int value;

    switch (data->state) {
    case 0:
        if (data->counter == data->period) {
            data->counter = 0;
            value = Pokepic_GetAttr(data->pokepic, 0x28);
            if (value > data->target) {
                value--;
            } else if (value < data->target) {
                value++;
            }
            Pokepic_SetAttr(data->pokepic, 0x28, value);
            if (value == data->target) {
                data->state++;
            }
        } else {
            data->counter++;
        }
        break;
    case 1:
        ov12_0226430C(data->battleSystem, data->battler, data->result);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    }
}
#endif

#if W17_GROUP == 3
void ov12_0225FF80(void *task, PokepicMoveTaskData *data)
{
    if (data->battler < 6) {
        if (data->state == 0) {
            data->state = 2;
            data->battler++;
            Pokepic_SetAttr(data->pokepic, 6, 1 ^ Pokepic_GetAttr(data->pokepic, 6));
        } else {
            data->state--;
        }
    } else {
        Pokepic_SetAttr(data->pokepic, 6, 0);
        ov12_0226430C(data->battleSystem, data->result, 0x17);
        Heap_Free(data);
        SysTask_Destroy(task);
    }
}

void ov12_0225FFDC(void *task, MenuTaskData *data)
{
    switch (data->state) {
    case 0:
        ov12_02264DCC(data, data->menu);
        data->state++;
    case 1:
        if (ov12_02264E00(data) == -1) {
            data->state++;
        }
        break;
    default:
        ov12_0226430C(data->battleSystem, data->battler, data->result);
        data->taskHandle = NULL;
        SysTask_Destroy(task);
        break;
    }
}

void ov12_02260030(void *task, MenuTaskData *data)
{
    switch (data->state) {
    case 0:
        data->counter = 0;
        PlaySE(0x70B);
        ov12_02264E34(data, data->effect);
        data->state++;
    case 1:
        if (data->counter < 8) {
            data->counter++;
        }
        if (ov12_02264E68(data) == -1) {
            if (data->counter >= 8) {
                StopSE(0x70B, 0);
                data->state = 100;
            } else {
                data->state++;
            }
        }
        break;
    case 2:
        data->counter++;
        if (data->counter >= 8) {
            StopSE(0x70B, 0);
            data->state = 100;
        }
        break;
    default:
        ov12_0226430C(data->battleSystem, data->battler, data->result);
        data->taskHandle = NULL;
        SysTask_Destroy(task);
        break;
    }
}
#endif
