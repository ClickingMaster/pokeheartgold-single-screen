#include "global.h"
#include "touch_sprite_motion_internal.h"
#include "touch_motion_vector_internal.h"
#include "math_util.h"
fx32 GF_SinDeg(u16 deg);
fx32 GF_CosDeg(u16 deg);
u16 FX_Atan2Idx(fx32 y, fx32 x);
#pragma force_active on
// @fn sub_020873D4
void sub_020873D4(SysTask *task, void *data) {
    UnkStruct02087284 *p = data;
    fx32 posX, posY;
    float outX, outY;
    s16 pos[2];
    int i;
    int done;
    float fx, fy;
    if (p->unk_F0 == 0) {
        return;
    }
    done = 0;
    for (i = 0; i < p->count; i++) {
        if (i != 0) {
            p->sprites[i].x = p->sprites[i - 1].x;
            p->sprites[i].y = p->sprites[i - 1].y;
        }
        ManagedSprite_GetPositionFxXYWithSubscreenOffset(p->sprites[i].sprite, &posX, &posY, p->yOffset);
        fx = (float)posX / 4096.0f;
        fy = (float)posY / 4096.0f;
        if (sub_020872DC(p->sprites[i].x, p->sprites[i].y, fx, fy, &outX, &outY, (8.0f - 0.5f * (float)(i - done)) / 2.0f, 16) && p->sprites[i].unk_08 == 0) {
            float ry = outY > 0.0f ? 0.5f + 4096.0f * outY : 4096.0f * outY - 0.5f;
            ManagedSprite_AddSpritePrecisePositionXY(p->sprites[i].sprite, (fx32)(outX > 0.0f ? 0.5f + 4096.0f * outX : 4096.0f * outX - 0.5f), (fx32)ry);
            continue;
        }
        switch (p->sprites[i].unk_08) {
        case 0:
            ManagedSprite_GetPositionXYWithSubscreenOffset(p->sprites[i].sprite, &pos[1], &pos[0], p->yOffset);
            if (p->unk_E8 == 0xFF) {
                int d;
                float ax, ay;
                p->unk_E8 = i;
                d = p->sprites[0].x - pos[1];
                ax = d > 0 ? 0.5f + (float)(d << 12) : (float)(d << 12) - 0.5f;
                d = p->sprites[0].y - pos[0];
                ay = d > 0 ? 0.5f + (float)(d << 12) : (float)(d << 12) - 0.5f;
                p->sprites[i].unk_0C = FX_Atan2Idx((fx32)ay, (fx32)ax);
            } else {
                p->sprites[i].unk_0C = p->sprites[p->unk_E8].unk_0C - (720 / p->count) * p->unk_EC;
            }
            p->sprites[i].unk_0C %= 720;
            p->unk_EC++;
            p->sprites[i].unk_08++;
            break;
        case 1:
            p->sprites[i].unk_0C += 4;
            p->sprites[i].unk_0C %= 720;
            {
                fx32 x = (p->sprites[0].x << 12) + GF_SinDeg(p->sprites[i].unk_0C) * p->unk_FC;
                fx32 y = (p->sprites[0].y << 12) + GF_CosDeg(p->sprites[i].unk_0C) * p->unk_100;
                ManagedSprite_SetPositionFxXYWithSubscreenOffset(p->sprites[i].sprite, x, y, p->yOffset);
            }
            break;
        }
        done++;
    }
}
