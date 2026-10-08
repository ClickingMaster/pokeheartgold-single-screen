#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "palette.h"
#include "heap.h"
#include "math_util.h"
#include "unk_02013534.h"

void ManagedSprite_SetPositonFxXY(ManagedSprite *managedSprite, fx32 x, fx32 y);
void ManagedSprite_GetSpritePositionFxXY(ManagedSprite *managedSprite, fx32 *x, fx32 *y);
void sub_020136B4(TextOBJ *textOBJ, int a1, int a2);
BOOL sub_02087948(void *p, s16 x, s16 y);
BOOL sub_020878B8(void *p, s16 x, s16 y);
BOOL sub_020879E0(void *p, int flag);
void ov40_0222D288(ManagedSprite *sprite, s16 x, s16 y);
void ov40_0222D294(ManagedSprite *sprite, s16 *x, s16 *y);
void ov40_0222C39C(void *work);

typedef struct {
    int delay;
    int angle;
    s16 targetY;
    u8 unkA[2];
    fx32 baseX;
} Ov40SlideAnim;

typedef struct {
    Ov40SlideAnim anims[5];
    u8 fade;
} Ov40SlideData;

typedef struct {
    ManagedSprite *sprite;
    u8 unk4[0x10];
    TextOBJ *textObj;
    u8 unk18[0x10];
} Ov40Choice;

typedef struct {
    u8 unk0[8];
    int state;
    u8 unkC[0x28 - 0xC];
    PaletteData *plttData;
    u8 unk2C[0x58 - 0x2C];
    int unk58;
    u8 unk5C[0x534 - 0x5C];
    Ov40Choice choices[5];
    Ov40Choice subChoices[5];
    u8 unk6C4[0x6E0 - 0x6C4];
    int count;
    int selected;
    u8 unk6E8[0x6F0 - 0x6E8];
    void *motion;
    u8 unk6F4[0x860 - 0x6F4];
    Ov40SlideData *slideData;
} Ov40Work;

BOOL ov40_0223142C(Ov40Work *work);

// @fn ov40_0223142C
BOOL ov40_0223142C(Ov40Work *work) {
    fx32 posY;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    Ov40SlideData *data;
    int i;
    int j;
    int done;
    fx32 y;
    fx32 x;

    switch (work->state) {
    case 0:
        data = Heap_Alloc(HEAP_ID_109, sizeof(Ov40SlideData));
        MI_CpuFill8(data, 0, sizeof(Ov40SlideData));
        work->slideData = data;
        data->fade = 16;
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, data->fade, work->unk58);
        for (i = 0; i < work->count; i++) {
            data->anims[i].delay = i * 4;
            data->anims[i].angle = 90;
            data->anims[i].targetY = 25 + i * 36 + (5 - work->count) * 16;
            ov40_0222D288(work->subChoices[i].sprite, 42, (5 - work->count) * 16 + 169);
            ManagedSprite_GetSpritePositionFxXY(work->subChoices[i].sprite, &data->anims[i].baseX, &posY);
            ManagedSprite_SetPositonFxXY(work->subChoices[i].sprite, data->anims[i].baseX, ((5 - work->count) * 16 + 169) * FX32_ONE);
            sub_020136B4(work->subChoices[i].textObj, 36, -8);
            TextOBJ_SetSpritesDrawFlag(work->subChoices[i].textObj, TRUE);
        }
        work->state++;
        break;
    case 1:
        data = work->slideData;
        if (data->fade != 0) {
            data->fade--;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, data->fade, work->unk58);
        }
        done = 0;
        for (j = 0; j < work->count; j++) {
            if (data->anims[j].delay != 0) {
                data->anims[j].delay--;
                continue;
            }
            ov40_0222D294(work->subChoices[j].sprite, &x1, &y1);
            if (y1 == data->anims[j].targetY) {
                done++;
                continue;
            }
            if (y1 - 8 > data->anims[j].targetY) {
                y1 -= 8;
            } else {
                y1 = data->anims[j].targetY;
            }
            y = y1 * FX32_ONE;
            x = data->anims[j].baseX + GF_SinDeg(0xFFFF * data->anims[j].angle / 360) * 16;
            data->anims[j].angle -= 4;
            ManagedSprite_SetPositonFxXY(work->subChoices[j].sprite, x, y);
            sub_020136B4(work->subChoices[j].textObj, 36, -8);
        }
        if (done == work->count) {
            work->state++;
        }
        break;
    default:
        data = work->slideData;
        ov40_0222D294(work->subChoices[work->selected].sprite, &x2, &y2);
        sub_02087948(work->motion, x2 + 16, y2);
        sub_020878B8(work->motion, x2 + 16, y2);
        sub_020879E0(work->motion, 0);
        ov40_0222C39C(work);
        Heap_Free(data);
        return TRUE;
    }
    return FALSE;
}
