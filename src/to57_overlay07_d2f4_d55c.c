#include "to57_overlay07_d2f4_d55c_internal.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct BrightnessTaskData {
    void *owner;
    u32 screen : 8;
    u32 reverse : 1;
    u32 : 23;
} BrightnessTaskData;

typedef struct OwnerData {
    u8 pad[0x164];
    void *entries[6];
} OwnerData;

typedef struct MotionData {
    void *target;
    u8 counter;
    u8 interval;
    u8 pad[2];
    void *pokepic;
} MotionData;

void ov07_0221D2F4(BrightnessTaskData *data, int reverse) {
    if (reverse) {
        StartBrightnessTransition(data->screen, 16, 0, 60, 1);
    } else {
        StartBrightnessTransition(data->screen, 0, 16, 60, 1);
    }
}

void ov07_0221D330(SysTask *task, BrightnessTaskData *data) {
    DoAllScreenBrightnessTransitionStep();
    if (IsBrightnessTransitionActive(1)) {
        if (data->reverse) {
            Heap_Free(data);
            SysTask_Destroy(task);
            ov07_0221C69C();
        } else {
            ov07_0221D2F4(data, 0);
            data->reverse = 1;
        }
    }
}

void ov07_0221D374(void *p) {
    BrightnessTaskData *data;
    data = Heap_Alloc(U32_AT(p, 0), sizeof(BrightnessTaskData));
    MI_CpuFill8(data, 0, sizeof(BrightnessTaskData));
    data->owner = p;
    PTR_AT(p, 0x18) = (u8 *)PTR_AT(p, 0x18) + 4;
    data->screen = (u8)U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    ScreenBrightnessData_InitAll();
    ov07_0221D2F4(data, 1);
    SysTask_CreateOnMainQueue((void (*)(SysTask *, void *))ov07_0221D330, data, 0x1001);
}

int ov07_0221D3CC(void *p, u32 type) {
    int result;
    int i;
    switch (type) {
    case 0:
        result = U16_AT(PTR_AT(p, 0xc0), 0x14);
        break;
    case 1:
        result = U16_AT(PTR_AT(p, 0xc0), 0x16);
        break;
    case 2:
        result = ov07_0223197C(p, U16_AT(PTR_AT(p, 0xc0), 0x14));
        break;
    case 3:
        result = ov07_0223197C(p, U16_AT(PTR_AT(p, 0xc0), 0x16));
        break;
    case 4:
        result = 0xff;
        for (i = 0; i < 4; i++) {
            int v = ov07_0221FA04(p, i);
            if (v == 0 || v == 2) {
                result = i;
                break;
            }
        }
        if (result == 0xff) {
            result = 0;
        }
        break;
    case 5:
        result = 0xff;
        for (i = 0; i < 4; i++) {
            int v = ov07_0221FA04(p, i);
            if (v == 1 || v == 3) {
                result = i;
                break;
            }
        }
        if (result == 0xff) {
            result = 0;
        }
        break;
    case 6:
        result = 0xff;
        for (i = 0; i < 4; i++) {
            if (ov07_0221FA04(p, i) == 4) {
                result = i;
                break;
            }
        }
        if (result == 0xff) {
            result = 0;
        }
        break;
    case 7:
        result = 0xff;
        for (i = 0; i < 4; i++) {
            if (ov07_0221FA04(p, i) == 5) {
                result = i;
                break;
            }
        }
        if (result == 0xff) {
            result = 0;
        }
        break;
    }
    return result;
}

void ov07_0221D4B0(SysTask *task, void *argument) {
    MotionData *data = argument;
    s16 x, y;
    if (data->interval != 0) {
        data->counter++;
        if (data->counter != data->interval) {
            return;
        }
        data->counter = 0;
    }
    x = Pokepic_GetAttr(data->pokepic, 0);
    y = Pokepic_GetAttr(data->pokepic, 1);
    y -= Pokepic_GetAttr(data->pokepic, 0x29);
    ManagedSprite_SetPositionXY(data->target, x, y);
}

void ov07_0221D4FC(SysTask *task, MotionData *data) {
    s16 x, y;
    if (data->interval != 0) {
        data->counter++;
        if (data->counter != data->interval) {
            return;
        }
        data->counter = 0;
    }
    x = Pokepic_GetAttr(data->pokepic, 0);
    y = Pokepic_GetAttr(data->pokepic, 1);
    y -= Pokepic_GetAttr(data->pokepic, 0x29);
    BgSetPosTextAndCommit(data->target, 2, 0, -(x - 40));
    BgSetPosTextAndCommit(data->target, 2, 3, -(y - 40));
}

void ov07_0221D55C(void *arg, u32 index) {
    OwnerData *p = arg;
    if (index == 4) {
        void *entry = p->entries[5];
        if (entry != NULL) {
            SysTask_Destroy(PTR_AT(entry, 0xc));
            Heap_Free(p->entries[5]);
            p->entries[5] = NULL;
        }
    } else {
        if (p->entries[index] != NULL) {
            SysTask_Destroy(PTR_AT(p->entries[index], 0xc));
            Heap_Free(p->entries[index]);
            p->entries[index] = NULL;
        }
    }
}
