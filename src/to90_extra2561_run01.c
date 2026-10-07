#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_window.h"
#include "sprite_system.h"
#include "gf_gfx_planes.h"
#pragma force_active on

typedef struct {
    u8 unused000[0xEC];
    Window window0;
    u8 unused0FC[0x11C - 0xFC];
    Window window1;
    u8 unused12C[0x320 - 0x12C];
    ManagedSprite *sprite;
    u8 unused324[0x404 - 0x324];
    int state;
    u8 unused408[8];
    int timer;
    u8 unused414[0x460 - 0x414];
} Work;

extern void ov57_02239BCC(void);
extern void ov57_0223A034(void *, u32);
extern void ov57_02239240(u8 *, BOOL);
extern void ov57_022387C0(u8 *, BOOL);
extern void ov57_02239728(Window *, int, u32, BOOL);
extern void ov57_0223866C(void *, int);
extern void ov57_02238AF0(void *, s32);
extern void ov57_0223B948(u8 *, void *);
extern void ov57_02239BAC(void);
struct Work;
extern void ov57_0223B828(struct Work *, u32, u32, u32);
extern void ov57_02239B0C(void *);
extern int IsPaletteFadeFinished(void);
extern void *ov57_0223B940(u8 *);
BOOL ov57_0223A7DC(void *);

BOOL ov57_0223A7DC(void *parent) {
    Work *w = parent;
    switch (w->state) {
    case 0:
        if (w->timer > 0) {
            w->timer--;
            return FALSE;
        }
        w->timer = 0;
        ov57_02239BCC();
        w->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished() != TRUE) break;
        GfGfx_EngineATogglePlanes(16, TRUE);
        SetBgPriority(3, 3);
        SetBgPriority(7, 3);
        GfGfx_EngineBTogglePlanes(4, FALSE);
        ov57_0223A034(w, FALSE);
        ov57_02239240((u8 *)w, FALSE);
        ov57_022387C0((u8 *)w, FALSE);
        sub_0200E5D4(&w->window0, TRUE);
        ov57_02239728(&w->window1, 3, 7, FALSE);
        CopyWindowToVram(&w->window1);
        ov57_0223866C(w, TRUE);
        *(vu16 *)0x04000304 &= ~0x8000u;
        ov57_02238AF0(w, -1);
        ov57_0223B948((u8 *)w, NULL);
        ov57_02239BAC();
        w->state++;
        break;
    case 2:
        if (ov57_0223B940((u8 *)w) == (void *)1) break;
        if (IsPaletteFadeFinished() != TRUE) break;
        ManagedSprite_SetDrawFlag(w->sprite, FALSE);
        ov57_0223B828((struct Work *)parent, 0, 255, 0);
        ov57_02239B0C(w);
        w->state = 0;
        return TRUE;
    }
    return FALSE;
}
