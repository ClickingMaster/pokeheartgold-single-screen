#include "global.h"

fx32 FX_Sqrt(fx32 value);
s32 LCRandom(void);
fx32 GF_SinDeg(u16 degree);
fx32 GF_CosDeg(u16 degree);
void *Sprite_GetMatrixPtr(void *sprite);
void Sprite_SetMatrix(void *sprite, const VecFx32 *matrix);
void Sprite_TryChangeAnimSeq(void *sprite, u32 sequence);
void Sprite_SetDrawFlag(void *sprite, BOOL draw);
void *SysTask_CreateOnMainQueue(void (*callback)(void *, void *), void *data, u32 priority);
BOOL ov74_0222B950(s32 x, s32 y, float targetX, float targetY, float *outX, float *outY, float scale, s16 limit);
void ov74_0222BA48(void *task, void *data);
void ov74_0222B5F8(void *work);
void ov74_0222B760(void *work);

#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct FloatPair {
    float x;
    float y;
} FloatPair;

typedef struct B760Work {
    u8 pad_0000[0x184C];
    s32 field_184C;
    u8 pad_1850[4];
    void *sprite;
    s32 x;
    s32 y;
    u8 pad_1860[4];
    s32 field_1864;
    s32 field_1868;
    u8 pad_186C[8];
    s32 field_1874;
    s32 field_1878;
    s32 field_187C;
    s32 field_1880;
    s32 field_1884;
    s32 field_1888;
    s32 field_188C;
    void *task;
} B760Work;

void ov74_0222B760(void *work) {
    u8 *taskData;
    B760Work *entry = work;
    int i;

    taskData = (u8 *)entry + 0x1848;
    i = 0;
    for (; i < 80; i++, entry = (B760Work *)((u8 *)entry + 0x4C), taskData += 0x4C) {
        VecFx32 position;

        entry->x = 0x80000;
        entry->y = 0x180000;
        entry->field_1864 = 0x4000;
        entry->field_1868 = 0x5000;
        entry->field_1874 = i % 2 + 4;
        entry->field_187C = 0;
        entry->field_184C = 0;
        entry->field_1884 = LCRandom() % 4 + 2;
        {
            s32 value = i / 30 + 4;
            float rounded;
            rounded = value > 0 ? (float)(value << FX32_SHIFT) + 0.5f : (float)(value << FX32_SHIFT) - 0.5f;
            entry->field_1864 = (s32)rounded;
        }
        if (i > 60) {
            entry->field_1880 = 5;
            entry->field_1878 = LCRandom() % 10 + 70;
        } else if (i > 50) {
            entry->field_1880 = 5;
            entry->field_1878 = LCRandom() % 10 + 60;
        } else if (i > 40) {
            entry->field_1880 = 5;
            entry->field_1878 = LCRandom() % 5 + 50;
        } else if (i > 30) {
            entry->field_1880 = 5;
            entry->field_1878 = LCRandom() % 10 + 40;
        } else if (i > 20) {
            entry->field_1880 = 5;
            entry->field_1878 = LCRandom() % 10 + 30;
        } else if (i > 10) {
            entry->field_1880 = 5;
            entry->field_1878 = LCRandom() % 5 + 20;
        } else {
            entry->field_1880 = 5;
            entry->field_1878 = LCRandom() % 5 + 10;
        }
        entry->field_1888 = entry->field_1880;
        entry->field_188C = entry->field_1880;
        position = *(VecFx32 *)Sprite_GetMatrixPtr(entry->sprite);
        position.x = 0x80000;
        position.y = 0x180000;
        Sprite_SetMatrix(entry->sprite, &position);
        Sprite_SetDrawFlag(entry->sprite, TRUE);
        entry->task = SysTask_CreateOnMainQueue(ov74_0222BA48, taskData, 6);
    }
}
