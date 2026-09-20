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

BOOL ov74_0222B950(s32 x, s32 y, float targetX, float targetY, float *outX, float *outY, float scale, s16 limit) {
    FloatPair result;
    FloatPair delta;
    FloatPair origin;
    float squared;
    float distance;

    origin.x = 0.0f;
    origin.y = 0.0f;
    delta.x = (float)x - targetX;
    delta.y = (float)y - targetY;
    result.x = 0.0f;
    result.y = 0.0f;
    squared = delta.x * delta.x + delta.y * delta.y;
    distance = (float)FX_Sqrt(FX_F32_TO_FX32(squared)) / FX32_ONE;

    if (distance < scale || (float)limit > distance || distance == 0.0f) {
        return FALSE;
    }

    result.x = delta.x * scale / distance;
    result.y = delta.y * scale / distance;
    *outX = result.x + origin.x;
    *outY = result.y + origin.y;
    return TRUE;
}
