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

void ov74_0222B5F8(void *work) {
    u8 *entry = work;
    u8 *taskData = entry;
    int i;

    i = 0;
    taskData += 0x88;
    for (; i < 80; i++, entry += 0x4C, taskData += 0x4C) {
        VecFx32 position;
        u32 angle;
        s32 radius;
        if (PTR_AT(entry, 0x94) == NULL) {
            continue;
        }
        S32_AT(entry, 0x98) = 0x80000;
        S32_AT(entry, 0x9C) = 0x60000;
        S32_AT(entry, 0xA4) = 0xC000;
        S32_AT(entry, 0xA8) = 0x1000;
        S32_AT(entry, 0xB4) = 0xFF;
        S32_AT(entry, 0xBC) = 0;
        S32_AT(entry, 0x8C) = 0;
        S32_AT(entry, 0x90) = 0;
        if (i > 30) {
            S32_AT(entry, 0xB8) = (s32)(LCRandom() % 10) + 15;
        } else if (i > 20) {
            S32_AT(entry, 0xB8) = (s32)(LCRandom() % 10) + 10;
        } else if (i > 10) {
            S32_AT(entry, 0xB8) = (s32)(LCRandom() % 5) + 10;
        } else {
            S32_AT(entry, 0xB8) = (s32)(LCRandom() % 5) + 5;
        }
        S32_AT(entry, 0xB8) = 0;
        S32_AT(entry, 0xC8) = S32_AT(entry, 0xC0);
        S32_AT(entry, 0xCC) = S32_AT(entry, 0xC0);
        angle = LCRandom() % 360;
        radius = (s32)(LCRandom() % 32) + 64;
        position.x = S32_AT(entry, 0x98) + GF_SinDeg((u16)angle) * radius;
        position.y = S32_AT(entry, 0x9C) + GF_CosDeg((u16)angle) * radius;
        Sprite_SetMatrix(PTR_AT(entry, 0x94), &position);
        Sprite_TryChangeAnimSeq(PTR_AT(entry, 0x94), 1);
        Sprite_SetDrawFlag(PTR_AT(entry, 0x94), FALSE);
        PTR_AT(entry, 0xD0) = SysTask_CreateOnMainQueue(ov74_0222BA48, taskData, 6);
    }
}
