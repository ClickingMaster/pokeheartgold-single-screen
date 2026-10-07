#pragma force_active on
#include "global.h"
#include "touchscreen.h"
#include "sprite_system.h"
void PlaySE(u16 seq);

typedef struct Swipe {
    u32 ticks : 8;
    u32 startY : 8;
    u32 curY : 8;
    u32 active : 1;
    u32 pad : 7;
} Swipe;

typedef struct View {
    u8 unk0[0x28];
    ManagedSprite *sprite;
    u8 unk2C[0x20];
    void *ctrl;
    u8 unk50[0x214];
    Swipe swipe;
} View;

extern const TouchscreenHitbox ov96_0221CC04[];
u8 ov96_0220B730(void *);
void ov96_0220B6EC(void *);
BOOL ov96_0220B744(void *);
BOOL ov96_0220B788(void *);
void ov96_0220A298(View *view, BOOL anywhere);

// @fn ov96_0220A298
void ov96_0220A298(View *view, BOOL anywhere) {
    u32 x, y, hx, hy;
    int anim;
    Swipe *swipe;

    ov96_0220B730(view->ctrl);
    swipe = &view->swipe;
    if (System_GetTouchNew()) {
        BOOL inside, inTop, inX;
        System_GetTouchNewCoords(&x, &y);
        inside = FALSE;
        inTop = FALSE;
        inX = FALSE;
        if (x >= 96 && x <= 176) {
            inX = TRUE;
        }
        if (inX && y >= 72) {
            inTop = TRUE;
        }
        if (inTop && y <= 152) {
            inside = TRUE;
        }
        if (anywhere || inside) {
            swipe->startY = y;
            swipe->curY = y;
            swipe->ticks = 1;
            swipe->active = TRUE;
        }
    } else if (swipe->active && !System_GetTouchHeld()) {
        if ((int)(swipe->startY - swipe->curY) >= 32 && ov96_0220B744(view->ctrl)) {
            ov96_0220B6EC(view->ctrl);
        }
        swipe->active = FALSE;
    } else if (swipe->active) {
        System_GetTouchHeldCoords(&hx, &hy);
        swipe->curY = hy;
        swipe->ticks++;
        if (swipe->ticks >= 10) {
            swipe->active = FALSE;
        }
    }
    if (System_GetTouchNew() || System_GetTouchHeld() <= 0) {
        if (TouchscreenHitbox_FindRectAtTouchNew(ov96_0221CC04) != -1) {
            if (ov96_0220B744(view->ctrl)) {
                anim = 20;
                PlaySE(2203);
                ov96_0220B6EC(view->ctrl);
            } else {
                anim = 22;
                PlaySE(2204);
            }
        } else if (!ov96_0220B744(view->ctrl)) {
            anim = 21;
        } else if (ov96_0220B788(view->ctrl)) {
            anim = 23;
        } else {
            anim = 19;
        }
        ManagedSprite_SetAnimNoRestart(view->sprite, anim);
    }
}
