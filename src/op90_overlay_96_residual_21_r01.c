#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "sys_task_api.h"
typedef struct {
    u16 id;
    u16 x2;
    u16 big;
    u16 x6;
    int anim;
    int xc;
} Record;
typedef struct {
    int x0;
    int x4;
    u8 pad8[8];
    int x10;
    int types[4];
} Template;
typedef struct {
    int x0;
    Sprite *sprite;
    Sprite *subSprite;
    int subPal;
    int pal;
    int width;
    int height;
    u8 pad1c[0x20];
    int x58;
    BOOL hidden;
} Slot;
typedef struct {
    int x0;
    int count;
    u8 pad8[0xc];
    void *x14;
    u8 *x18;
    Slot slots[1];
} Work;
u8 ov96_021E9CF4(u8 *p, u32 mask);
Sprite *ov96_021EAA88(Work *work, int a, int b, int c);
void ov96_021E8B1C(void *a, int b, Record *records, Template *tmpl, int c, int d);
void ov96_021EAA24(SysTask *task, void *data);
void ov96_021EA8A8(Work *work, int a1, Record *records, Template *tmpl, u32 priority, int a5);
// @fn ov96_021EA8A8
void ov96_021EA8A8(Work *work, int a1, Record *records, Template *tmpl, u32 priority, int a5) {
    int i;
    int delay = priority;
    for (i = 0; i < work->count; i++) {
        work->slots[i].height = work->slots[i].width = records[i].big ? 0x20 : 0x10;
        switch (tmpl->types[i]) {
        case 1:
            work->slots[i].pal = 3;
            break;
        case 2:
            work->slots[i].pal = 4;
            break;
        case 3:
            work->slots[i].pal = 5;
            break;
        default:
            GF_AssertFail();
            work->slots[i].pal = 3;
            break;
        }
        if (ov96_021E9CF4(work->x18, 2)) {
            work->slots[i].sprite = ov96_021EAA88(work, tmpl->x4, tmpl->x10, 0x16);
            if (records[i].big) {
                Sprite_SetAnimCtrlSeq(work->slots[i].sprite, 1);
            } else {
                Sprite_SetAnimCtrlSeq(work->slots[i].sprite, 0);
            }
            if (records[i].id == 50 || records[i].id == 51) {
                work->slots[i].hidden = TRUE;
                Sprite_SetDrawFlag(work->slots[i].sprite, FALSE);
            } else {
                work->slots[i].hidden = FALSE;
            }
        }
        if (ov96_021E9CF4(work->x18, 4)) {
            work->slots[i].subSprite = ov96_021EAA88(work, tmpl->x4, tmpl->x10, 0x15);
            GF_ASSERT(records[i].anim < 4);
            Sprite_SetAnimCtrlSeq(work->slots[i].subSprite, records[i].anim);
            switch (tmpl->types[i]) {
            case 1:
                work->slots[i].subPal = 3;
                break;
            case 2:
                work->slots[i].subPal = 4;
                break;
            case 3:
                work->slots[i].subPal = 5;
                break;
            default:
                GF_AssertFail();
                work->slots[i].subPal = 3;
                break;
            }
        }
        work->slots[i].x58 = 12;
    }
    if (delay <= 0) {
        delay = 1;
    }
    ov96_021E8B1C(work->x14, a1, records, tmpl, delay - 1, a5);
    SysTask_CreateOnMainQueue(ov96_021EAA24, work, delay);
}
