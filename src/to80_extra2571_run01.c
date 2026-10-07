#pragma force_active on
#include "global.h"
#include "overlay_manager.h"
#include "sprite.h"
#pragma force_active on
#include "../.local-tools/to90/w18-callback272-review/canonical/include/overlay_37_work.h"


extern const Ov37StateEntry ov37_021E7D20[24];
extern int sub_0203769C(void);
extern u16 sub_02033250(void);
extern u32 ov37_021E75C4(void);
extern BOOL IsPaletteFadeFinished(void);
extern int ov37_021E76F0(void *);
extern void ov37_021E784C(Ov37Work *,u32);
extern int sub_02037030(int,void *,int);
extern void ov37_021E7478(u8 *,int,u32,u8 *);
extern void ov37_021E741C(u16 *);
u32 ov37_021E5A84(void *,u32 *);
u32 ov37_021E5A84(void *manager,u32 *state) {
    Ov37Work *w=OverlayManager_GetData(manager);
    if (!sub_0203769C() && w->unk_93B4) w->unk_93B4 &= sub_02033250();
    w->curState=*state;
    ov37_021E784C(w,*state);
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            if (sub_0203769C()) {
                if ((s32)ov37_021E75C4()>=2) { sub_02037030(128,0,0);*state=1; }
            } else *state=1;
        }
        break;
    case 1:
        if (ov37_021E7D20[w->prevState].main)
            *state=ov37_021E7D20[w->prevState].main(w,*state);
        ov37_021E7478((u8 *)w+632,0,0xE0D0F,(u8 *)w);
        if (!sub_0203769C()) {
            int next=ov37_021E76F0(w);
            if (*state==1) *state=next;
        }
        ov37_021E741C(&w->unk_4374);
        break;
    case 2:
        if (ov37_021E7D20[w->prevState].main && ov37_021E7D20[w->prevState].unk_04)
            *state=ov37_021E7D20[w->prevState].main(w,*state);
        break;
    case 3:
        if (IsPaletteFadeFinished()) return 1;
        break;
    }
    SpriteList_RenderAndAnimateSprites(w->spriteList);
    return 0;
}
