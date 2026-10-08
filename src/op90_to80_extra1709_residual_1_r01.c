#pragma force_active on
#include "global.h"
#include "pokepic.h"
typedef struct Ov07MosaicWork {
    int counter;
    int y1;
    int x1;
    int y2;
    int step;
    int toggle;
    int savedAttr;
    Pokepic *pic;
    u8 state;
    u8 pad21[3];
    void *owner;
    u8 pad28[0x28];
} Ov07MosaicWork;
void *ov07_022324D8(void *owner, u32 size);
void ov07_02231FE4(void *owner, void *work);
u32 ov07_0221C470(void *owner);
Pokepic *ov07_0221FA48(void *owner, u32 battler);
int ov07_02231924(void *owner, u32 battler);
int ov07_0221FAE8(void *owner);
void SetBgPriority(u8 bgId, u16 priority);
void ov07_0221C410(void *owner, void *func, void *data);
void ov07_0222BAF4(void *task, Ov07MosaicWork *work);
void ov07_0222BC7C(void *owner);

// @fn ov07_0222BC7C
void ov07_0222BC7C(void *owner) {
    Ov07MosaicWork *work = ov07_022324D8(owner, sizeof(Ov07MosaicWork));
    int kind;
    s16 x, y;
    ov07_02231FE4(owner, &work->state);
    work->pic = ov07_0221FA48(owner, ov07_0221C470(owner));
    work->counter = 0;
    kind = ov07_02231924(owner, ov07_0221C470(owner));
    switch (kind) {
    case 3:
    case 4:
        SetBgPriority(2, (u8)(ov07_0221FAE8(owner) - 1));
        break;
    }
    x = Pokepic_GetAttr(work->pic, 0);
    y = Pokepic_GetAttr(work->pic, 1);
    y = y - Pokepic_GetAttr(work->pic, 41);
    work->savedAttr = Pokepic_GetAttr(work->pic, 23);
    Pokepic_SetAttr(work->pic, 23, 8);
    work->x1 = x - 40;
    work->y1 = y + 40;
    work->step = 0;
    work->y2 = work->y1;
    work->toggle = 0;
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(work->x1, work->y1, work->x1 + 80, work->y2);
    ov07_0221C410(work->owner, ov07_0222BAF4, work);
}
