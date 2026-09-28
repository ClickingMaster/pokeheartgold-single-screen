#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

typedef struct Node {
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct List {
    Node head;
    Node tail;
} List;

typedef struct Pool {
    u16 size;
    u8 head;
    u8 tail;
    void *items[1];
} Pool;

typedef struct Oam {
    u32 attr01;
    u16 attr2;
    u16 affine;
} Oam;

typedef struct YX13 {
    int y;
    int x;
} YX13;

static inline void OamGetPos13(Oam *o, int *x, int *y) {
    *x = (o->attr01 & 0x01FF0000) >> 16;
    *y = o->attr01 & 0xFF;
}

static inline void OamSetPos13(Oam *o, int x, int y) {
    o->attr01 = (o->attr01 & 0xFE00FF00) | (y & 0xFF) | ((u32)(x << 23) >> 7);
}

typedef struct OamBlk {
    Node node;
    Oam *oam;
    u8 count;
} OamBlk;

typedef struct OamMgr {
    u8 buf[0x20][0x10];
    OamBlk first;
    OamBlk last;
    List *list;
    Pool *pool;
} OamMgr;

typedef struct Ctx13 {
    void *buf;
    u8 unk4[0x80];
    Pool *pool;
    FSArchive arc;
    u8 unkX[0xE4 - 0x88 - sizeof(FSArchive)];
    u16 lockId;
} Ctx13;

/* address of a TU-static global: forces a separate literal-pool slot like the original */
#define STATIC_ADDR(addr) ((void *)(ov13_0224F5C8 + ((addr) - 0x0224F5C8)))

extern char ov13_02247424[];
extern char ov13_0224F5C8[];
extern char ov13_0224742C[];
extern Ctx13 *ov13_0224F5CC;
extern OamMgr *ov13_0224F5D0;
extern void *ov13_0224F5D4[];

typedef struct Fade {
    void *task;
    s16 cnt;
    u16 dur;
    u8 kind;
    u8 active;
} Fade;

typedef struct B4 {
    s8 v[4];
} B4;

typedef struct OamHdr {
    u16 n;
    u16 pad;
    u32 off;
} OamHdr;

extern Fade *ov13_0224F5DC;

typedef struct FontInf {
    u8 pad[7];
    u8 dir;
} FontInf;

typedef struct FontRes {
    u8 pad0[2];
    u16 alterChar;
    u8 pad4[4];
    FontInf *inf;
} FontRes;

typedef struct Font {
    FontRes *res;
    void *unk4;
} Font;

typedef struct CharWidths {
    s8 left;
    u8 glyphWidth;
    s8 charWidth;
} CharWidths;

typedef struct CanvasVT {
    void (*drawChar)(void);
    void (*clear)(void *cc, int color);
} CanvasVT;

typedef struct TextCanvas {
    void *canvas;
    Font *font;
    int hSpace;
    int vSpace;
} TextCanvas;

typedef struct Dir {
    s8 x;
    s8 y;
} Dir;

typedef struct Canvas {
    u8 cc[0x14];
    CanvasVT *vt;
    TextCanvas tc;
    void *buf;
    union {
        void *task;
        struct {
            u16 charNo;
            u8 w;
            u8 h;
        } obj;
    } u;
} Canvas;

typedef struct Text13 {
    Font fonts[2];
    Canvas objCanvas[0x20];
    Canvas bg[2];
    Pool *pool;
    void *fontData[2];
    u8 dirty[2];
} Text13;

typedef struct Pos16 {
    u16 x;
    u16 y;
} Pos16;

typedef struct Rect16 {
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
} Rect16;

extern Text13 *ov13_0224F5E0;

typedef struct OamBuf13 {
    Oam oam[2][0x80];
    Pool *pools[2];
    void *task;
} OamBuf13;

typedef struct ChrBlk {
    Node node;
    u16 start;
    u16 size;
} ChrBlk;

typedef struct ChrMgr {
    u8 buf[0x20][0xC];
    ChrBlk first;
    ChrBlk last;
    List *list;
    Pool *pool;
} ChrMgr;

typedef struct VramSave13 {
    int bg;
    int obj;
    int bgExt;
    int objExt;
    int tex;
    int texPltt;
    int clearImage;
    int subBg;
    int subObj;
    int subBgExt;
    int subObjExt;
    int arm7;
    int lcdc;
} VramSave13;

typedef struct Task13 {
    Node node;
    void (*func)(void *task, void *arg);
    void *arg;
    u8 prio;
    u8 owned;
} Task13;

typedef struct Sched13 {
    Pool *taskPool;
    Pool *queue;
    List *list;
    Task13 head;
    Task13 tail;
    int count;
    u8 enabled;
    void *buf;
} Sched13;

extern OamBuf13 *ov13_0224F608;
extern ChrMgr *ov13_0224F60C;
extern VramSave13 ov13_0224F610;
extern Sched13 *ov13_0224F644;
extern u8 ov13_02245960[];
int GX_DisableBankForARM7(void);
void GX_SetBankForARM7(int bank);
int PM_GetLEDPattern(u32 *pattern);
void PMi_SendLEDPatternCommand(int pattern);
void ov13_02241D04(void);
void ov13_0224225C(void);
Pool *ov13_022401F8(int n);
void ov13_022425FC(int queue, Task13 *t);
Task13 *ov13_0224253C(int queue, void (*func)(void *, void *), void *arg, u32 prio, u8 owned);

typedef struct Input13 {
    TPData samples[5];
    Pos16 touch;
    Pos16 prevTouch;
    u16 cont;
    u16 trig;
    u16 repeat;
    u16 release;
    u8 touching : 1;
    u8 touchTrig : 1;
    u8 touchRepeat : 1;
    u8 touchRelease : 1;
} Input13;

typedef struct InputCtl13 {
    u8 lcdOff;
    u8 touchCnt;
    u8 pad[2];
    Input13 *input;
} InputCtl13;

typedef struct IrqSave13 {
    OSIrqFunction fn;
    OSIrqMask mask;
} IrqSave13;

extern NNSFndHeapHandle ov13_0224F5E4;
extern InputCtl13 ov13_0224F5E8;
extern u8 ov13_0224F5F0[];
extern IrqSave13 ov13_0224F600;
void TP_RequestAutoSamplingStartAsync(u16 vcount, u16 freq, TPData *bufs, u16 size);
void TP_RequestAutoSamplingStopAsync(void);
void TP_WaitBusy(u32 flag);
u32 TP_CheckBusy(u32 flag);
u16 TP_GetLatestIndexInAuto(void);
BOOL PM_SetLCDPower(int on);
void ov13_022413E4(void);
void ov13_022413F4(void);
void ov13_022414F8(void);
BOOL ov13_022417DC(Rect16 *r);
void ov13_02241B70(void);
void ov13_02242478(int queue);
void ov13_02240E50(u16 x, u16 y, Pos16 *p);

static inline int GX_GetVisiblePlane13(void) {
    return (int)((reg_GX_DISPCNT & 0x1F00) >> 8);
}

static inline int GXS_GetVisiblePlane13(void) {
    return (int)((reg_GXS_DB_DISPCNT & 0x1F00) >> 8);
}

static inline void TextCanvasInit(TextCanvas *tc, void *cc, Font *font, int hSpace, int vSpace) {
    tc->canvas = cc;
    tc->font = font;
    tc->hSpace = hSpace;
    tc->vSpace = vSpace;
}
extern char *ov13_02247430[];
extern char *ov13_02247438[];
extern u16 ov13_02245950[];
extern u16 ov13_02245952[];
extern u16 ov13_0224594C[];
int ov13_0222B970(void);
void NNS_G2dFontInitUTF16(Font *font, void *data);
int NNSi_G2dCalcRequiredOBJ(int w, int h);
void NNS_G2dCharCanvasInitForOBJ1D(void *cc, void *base, int w, int h, int bpp);
void NNS_G2dCharCanvasInitForBG(void *cc, void *base, int w, int h, int bpp);
void NNS_G2dMapScrToCharText(void *scr, int w, int h, int left, int top, int scrW, int charNo, int pal);
void NNSi_G2dTextCanvasDrawText(TextCanvas *tc, int x, int y, int cl, u32 flags, const u16 *txt, Dir dir);
void NNSi_G2dTextCanvasDrawTextRect(TextCanvas *tc, int x, int y, int w, int h, int cl, u32 flags, const u16 *txt, Dir dir);
void NNS_G2dCharCanvasDrawChar(void *cc, Font *font, int x, int y, int cl, u16 ch);
u16 NNS_G2dFontFindGlyphIndex(Font *font, u16 ch);
CharWidths *NNS_G2dFontGetCharWidthsFromIndex(Font *font, u16 idx);
void NNS_G2dArrangeOBJ1D(Oam *oam, int w, int h, int x, int y, int color, int charNo, int vramMode);
void *ov13_02241EBC(int screen, int size, int fromFront, u32 *charNo);
void ov13_02242034(void *blk);
void ov13_02240D80(Canvas *c, int color);
void ov13_02240A18(void *task, u8 *dirty);
void ov13_022425F0(int queue, void *task);
void ov13_02240AA0(Canvas *c);
void ov13_02240BD8(void *cc, int x, int y, int cl, u16 ch, int fontIdx);
void ov13_0223FE98(OamBlk *blk, int idx, u32 mode, u32 pal);
void ov13_02240008(OamBlk *blk, int idx, u32 prio);
void ov13_0223FDF4(OamBlk *blk, int idx, u32 mode, u32 param);
extern u8 ov13_02245940[];
fx32 FX_DivS32(fx32 a, fx32 b);
void *ov13_02242528(int queue, void (*func)(void *, void *), void *arg, u32 prio);
void ov13_022425D0(int queue, void *task);
void ov13_02240490(void *task, Fade *f);
void ov13_02240610(void *task, Fade *f);

void ov13_022412B0(void *pp);
void *ov13_0224128C(u32 size, int align);
void *ov13_02240288(Pool *pool, void *item);
void *ov13_022402E0(Pool *pool);
Pool *ov13_0224022C(int n, void *base, int stride);
void ov13_0224026C(Pool *pool);
int ov13_0223FA84(const char *a, const char *b, int n);
List *ov13_02241B98(void);
void ov13_02241BC4(List *list);
void ov13_02241BE0(void *node);
void ov13_02241C18(void *pos, void *node);
void ov13_02241C48(List *list, void *node);
void ov13_02241C58(List *list, void *node);
Oam *ov13_02241DE4(int a, int b);
Oam *ov13_02241D78(int a);
void ov13_0223F944(void *arg);
void ov13_0223FA50(void *p);
Oam *ov13_0223FDE0(OamBlk *blk, int idx);
void ov13_022400F0(int a, int b, Oam *dst);
OamBlk *ov13_0223FC04(int which, int count, int fromFront);

/* prototypes for functions defined below */
void ov13_0223F814(void);
int ov13_0223F8A0(FSArchive *arc, int cmd);
int ov13_0223F904(FSArchive *arc, void *dst, u32 pos, u32 len);
int ov13_0223F954(void);
void *ov13_0223F95C(const char *path, u32 *outSize, int align);
void ov13_0223FADC(void);
void ov13_0223FBB0(void);
void ov13_0223FD60(OamBlk *blk);
u8 ov13_0223FDEC(OamBlk *blk);
void ov13_0223FF18(OamBlk *blk, int idx, int x, int y);
void ov13_02240068(OamBlk *blk, int idx, int *x, int *y);
void ov13_02240098(int slot, const char *path);
void ov13_022400C8(int slot);
Oam *ov13_02240184(int slot, int idx);
OamBlk *ov13_022401B0(int slot, int idx, int fromFront);
void ov13_02240338(void);
void ov13_02240380(void);
u8 ov13_02240394(int screen);
int ov13_022403B8(int kind, int screen, int plane, int dur);
int ov13_022405B8(int dur);
void ov13_02240648(void);
void ov13_0224074C(void);
Canvas *ov13_02240798(int screen, int w, int h, int fromFront, int *nObj, int fontIdx);
void ov13_02240874(Canvas *c);
Canvas *ov13_022408A0(int screen, int fontIdx);
void ov13_02240B04(int screen);
void ov13_02240B28(Canvas *c, int x, int y, int cl, u32 flags, const u16 *txt);
void ov13_02240C14(void *cc, int x, int y, int cl, int adv, const u16 *str, int fontIdx);
void ov13_02240CC0(Canvas *c, int x, int y, int w, int h, int cl, u32 flags, const u16 *txt);
void ov13_02240D94(Canvas *c, int x, int y, OamBlk *blk, u32 prio);
void ov13_02240E2C(Canvas *c);
void ov13_02240E5C(u16 x1, u16 y1, u16 x2, u16 y2, Rect16 *r);
void ov13_02240E74(Pos16 *pos, Pos16 *size, Rect16 *r);
void ov13_02240EA8(int screen, int plane);
void ov13_02240EFC(int screen, int plane);
void ov13_02240F58(int screen, int which, Rect16 *r);
void ov13_02241070(int screen, int which, int planes, BOOL effect);
void ov13_022411DC(void *mem);
void ov13_0224121C(void);
void *ov13_02241240(u32 size, int align);
void ov13_022412F4(void *p);
void ov13_02241330(void);
void ov13_022413AC(void);
BOOL ov13_02241694(u16 mask);
BOOL ov13_022416D4(u16 mask);
BOOL ov13_02241714(u16 mask);
BOOL ov13_02241754(Rect16 *r);
BOOL ov13_02241864(Rect16 *r);
BOOL ov13_022418EC(Rect16 *r);
BOOL ov13_02241974(Rect16 *r);
BOOL ov13_022419DC(Pos16 *out);
void ov13_02241A44(void);
void ov13_02241AC0(void);
void ov13_02241B34(void);
void ov13_02241C68(void);
void ov13_02241D4C(void);
void ov13_02241D98(Oam *o);
void ov13_02241DFC(void);
int ov13_02242074(const u8 *s, int max);
void ov13_022420A0(void);
void ov13_02242154(void);
void ov13_022422E0(void);
void ov13_0224230C(void);
void ov13_0224231C(void);
void ov13_02242430(void);
void ov13_022425C8(Task13 *t, void (*func)(void *, void *));
void ov13_02242640(int q, u8 enabled);

// @fn ov13_02240A18
void ov13_02240A18(void *task, u8 *dirty) {
    if (*dirty == 0) {
        return;
    }
    if (dirty == &ov13_0224F5E0->dirty[0]) {
        DC_FlushRange(ov13_0224F5E0->bg[0].buf, 0x6000);
        GX_LoadBG0Char(ov13_0224F5E0->bg[0].buf, 0, 0x6000);
    } else {
        DC_FlushRange(ov13_0224F5E0->bg[1].buf, 0x3000);
        GXS_LoadBG0Char(ov13_0224F5E0->bg[1].buf, 0x3000, 0x3000);
    }
    *dirty = 0;
}

// @fn ov13_02240AA0
void ov13_02240AA0(Canvas *c) {
    ov13_022425F0(1, c->u.task);
    if (c == &ov13_0224F5E0->bg[0]) {
        MIi_CpuClear16(0, G2_GetBG0CharPtr(), 0x6000);
    } else {
        MIi_CpuClear16(0, G2_GetBG0CharPtr(), 0x3000);
    }
    ov13_022412B0(&c->buf);
}

// @fn ov13_02240B04
void ov13_02240B04(int screen) {
    ov13_02240AA0(&ov13_0224F5E0->bg[screen]);
}

// @fn ov13_02240B28
void ov13_02240B28(Canvas *c, int x, int y, int cl, u32 flags, const u16 *txt) {
    Font *font = c->tc.font;
    Dir d = {0, 0};
    switch (font->res->inf->dir) {
    case 0:
    case 7:
        d.x = 1;
        break;
    case 1:
    case 2:
        d.y = 1;
        break;
    case 3:
    case 4:
        d.x = -1;
        break;
    case 5:
    case 6:
        d.y = -1;
        break;
    }
    NNSi_G2dTextCanvasDrawText(&c->tc, x, y, cl, flags, txt, d);
}

// @fn ov13_02240BD8
void ov13_02240BD8(void *cc, int x, int y, int cl, u16 ch, int fontIdx) {
    NNS_G2dCharCanvasDrawChar(cc, &ov13_0224F5E0->fonts[fontIdx], x, y, cl, ch);
}

// @fn ov13_02240C14
void ov13_02240C14(void *cc, int x, int y, int cl, int adv, const u16 *str, int fontIdx) {
    Font *font;
    u16 idx;
    CharWidths *w;
    while (*str != 0) {
        font = &ov13_0224F5E0->fonts[fontIdx];
        idx = NNS_G2dFontFindGlyphIndex(font, *str);
        if (idx == 0xFFFF) {
            idx = font->res->alterChar;
        }
        w = NNS_G2dFontGetCharWidthsFromIndex(font, idx);
        ov13_02240BD8(cc, x + ((adv - w->charWidth) >> 1), y, cl, *str, fontIdx);
        str++;
        x += adv;
    }
}

// @fn ov13_02240CC0
void ov13_02240CC0(Canvas *c, int x, int y, int w, int h, int cl, u32 flags, const u16 *txt) {
    Font *font = c->tc.font;
    Dir d = {0, 0};
    switch (font->res->inf->dir) {
    case 0:
    case 7:
        d.x = 1;
        break;
    case 1:
    case 2:
        d.y = 1;
        break;
    case 3:
    case 4:
        d.x = -1;
        break;
    case 5:
    case 6:
        d.y = -1;
        break;
    }
    NNSi_G2dTextCanvasDrawTextRect(&c->tc, x, y, w, h, cl, flags, txt, d);
}

// @fn ov13_02240D80
void ov13_02240D80(Canvas *c, int color) {
    c->vt->clear(c, color);
}

// @fn ov13_02240D94
void ov13_02240D94(Canvas *c, int x, int y, OamBlk *blk, u32 prio) {
    Oam *o = ov13_0223FDE0(blk, 0);
    ov13_0223FDF4(blk, -1, 0, 0);
    ov13_0223FE98(blk, -1, 0, 0xF);
    ov13_02240008(blk, -1, prio);
    NNS_G2dArrangeOBJ1D(o, c->u.obj.w, c->u.obj.h, x, y, 0, c->u.obj.charNo, 2);
}

// @fn ov13_02240E2C
void ov13_02240E2C(Canvas *c) {
    if (c == &ov13_0224F5E0->bg[0]) {
        ov13_0224F5E0->dirty[0] = 1;
    } else {
        ov13_0224F5E0->dirty[1] = 1;
    }
}

// @fn ov13_02240E50
void ov13_02240E50(u16 x, u16 y, Pos16 *p) {
    p->x = x;
    p->y = y;
}

// @fn ov13_02240E5C
void ov13_02240E5C(u16 x1, u16 y1, u16 x2, u16 y2, Rect16 *r) {
    r->x1 = x1;
    r->y1 = y1;
    r->x2 = x2;
    r->y2 = y2;
}

// @fn ov13_02240E74
void ov13_02240E74(Pos16 *pos, Pos16 *size, Rect16 *r) {
    r->x1 = pos->x;
    r->y1 = pos->y;
    r->x2 = pos->x + size->x;
    r->y2 = pos->y + size->y;
}

// @fn ov13_02240EA8
void ov13_02240EA8(int screen, int plane) {
    if (screen == 1) {
        GXS_SetVisiblePlane(plane | GXS_GetVisiblePlane13());
    } else {
        GX_SetVisiblePlane(plane | GX_GetVisiblePlane13());
    }
}

// @fn ov13_02240EFC
void ov13_02240EFC(int screen, int plane) {
    if (screen == 1) {
        GXS_SetVisiblePlane(~plane & GXS_GetVisiblePlane13());
    } else {
        GX_SetVisiblePlane(~plane & GX_GetVisiblePlane13());
    }
}

// @fn ov13_02240F58
void ov13_02240F58(int screen, int which, Rect16 *r) {
    if (screen == 1) {
        if (which == 0) {
            G2S_SetWnd0Position(r->x1, r->y1, r->x2, r->y2);
        } else {
            G2S_SetWnd1Position(r->x1, r->y1, r->x2, r->y2);
        }
    } else {
        if (which == 0) {
            G2_SetWnd0Position(r->x1, r->y1, r->x2, r->y2);
        } else {
            G2_SetWnd1Position(r->x1, r->y1, r->x2, r->y2);
        }
    }
}

// @fn ov13_02241070
void ov13_02241070(int screen, int which, int planes, BOOL effect) {
    switch (which) {
    case 0:
        if (screen == 1) {
            G2S_SetWnd0InsidePlane(planes, effect);
        } else {
            G2_SetWnd0InsidePlane(planes, effect);
        }
        break;
    case 1:
        if (screen == 1) {
            G2S_SetWnd1InsidePlane(planes, effect);
        } else {
            G2_SetWnd1InsidePlane(planes, effect);
        }
        break;
    case 2:
        if (screen == 1) {
            u32 tmp = (reg_G2S_DB_WINOUT & ~0x3F00) | (planes << 8);
            if (effect) {
                tmp |= 0x2000;
            }
            reg_G2S_DB_WINOUT = tmp;
        } else {
            u32 tmp = (reg_G2_WINOUT & ~0x3F00) | (planes << 8);
            if (effect) {
                tmp |= 0x2000;
            }
            reg_G2_WINOUT = tmp;
        }
        break;
    case 3:
        if (screen == 1) {
            G2S_SetWndOutsidePlane(planes, effect);
        } else {
            G2_SetWndOutsidePlane(planes, effect);
        }
        break;
    }
}
// @fn ov13_022411DC
void ov13_022411DC(void *mem) {
    MI_CpuFill8(mem, 0, 0x40000);
    ov13_0224F5E4 = NNS_FndCreateExpHeapEx(mem, 0x40000, 0);
    if (ov13_0224F5E4 == NULL) {
        OS_Terminate();
    }
}

// @fn ov13_0224121C
void ov13_0224121C(void) {
    NNS_FndDestroyExpHeap(ov13_0224F5E4);
    ov13_0224F5E4 = NULL;
}

// @fn ov13_02241240
void *ov13_02241240(u32 size, int align) {
    void *p;
    int irq;
    irq = OS_DisableIrqMask(1);
    p = NNS_FndAllocFromExpHeapEx(ov13_0224F5E4, size, align);
    if (p == NULL) {
        OS_Terminate();
    }
    OS_EnableIrqMask(irq);
    return p;
}

// @fn ov13_0224128C
void *ov13_0224128C(u32 size, int align) {
    void *p = ov13_02241240(size, align);
    MI_CpuFill8(p, 0, size);
    return p;
}

// @fn ov13_022412B0
void ov13_022412B0(void *pp) {
    int irq = OS_DisableIrqMask(1);
    if (*(void **)pp == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(ov13_0224F5E4, *(void **)pp);
    OS_EnableIrqMask(irq);
    *(void **)pp = NULL;
}

// @fn ov13_022412F4
void ov13_022412F4(void *p) {
    int irq = OS_DisableIrqMask(1);
    if (p == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(ov13_0224F5E4, p);
    OS_EnableIrqMask(irq);
}

// @fn ov13_02241330
void ov13_02241330(void) {
    TPCalibrateParam cp;
    ov13_0224F5E8.input = ov13_0224128C(0x3A, 4);
    if (!TP_GetUserInfo(&cp)) {
        OS_Terminate();
    }
    TP_SetCalibrateParam(&cp);
    TP_RequestAutoSamplingStartAsync(0, 4, ov13_0224F5E8.input->samples, 5);
    TP_WaitBusy(2);
    if (TP_CheckBusy(2)) {
        OS_Terminate();
    }
    ov13_022413E4();
}

// @fn ov13_022413AC
void ov13_022413AC(void) {
    do {
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(4);
    } while (TP_CheckBusy(4));
    ov13_022412B0(&ov13_0224F5E8.input);
}

// @fn ov13_022413E4
void ov13_022413E4(void) {
    ov13_022413F4();
    ov13_022414F8();
}

// @fn ov13_022413F4
void ov13_022413F4(void) {
    int i;
    u8 *cnt;
    Input13 *in = ov13_0224F5E8.input;
    u16 pad = PAD_Read();
    ov13_0224F5E8.input->trig = (in->cont ^ pad) & pad;
    ov13_0224F5E8.input->release = in->cont & (in->cont ^ pad);
    ov13_0224F5E8.input->cont = pad;
    ov13_0224F5E8.input->repeat = ov13_0224F5E8.input->trig;
    cnt = ov13_0224F5F0;
    for (i = 0; i < 14; i++) {
        u16 bit = 1 << i;
        if ((pad & bit) == 0) {
            *cnt = 0;
        } else {
            (*cnt)++;
            if (*cnt == 40) {
                ov13_0224F5E8.input->repeat |= bit;
            } else if (*cnt == 47) {
                ov13_0224F5E8.input->repeat |= bit;
                *cnt = 40;
            }
        }
        cnt++;
    }
}
// @fn ov13_022414F8
void ov13_022414F8(void) {
    int n;
    int idx;
    int wasTouching;
    u8 touched;
    int diff;
    TPData cal;

    wasTouching = ov13_0224F5E8.input->touching ? 1 : 0;
    touched = 0;
    idx = TP_GetLatestIndexInAuto();
    ov13_0224F5E8.input->prevTouch = ov13_0224F5E8.input->touch;
    n = 0;
    do {
        TPData *d = &ov13_0224F5E8.input->samples[idx];
        if (d->touch == 1 && d->validity == 0) {
            touched = 1;
            TP_GetCalibratedPoint(&cal, d);
            ov13_02240E50(cal.x, cal.y, &ov13_0224F5E8.input->touch);
            break;
        }
        n++;
        idx = FX_ModS32(idx + 4, 5);
    } while (n < 4);
    diff = touched ^ wasTouching;
    ov13_0224F5E8.input->touchTrig = touched & diff;
    ov13_0224F5E8.input->touchRelease = wasTouching & diff;
    ov13_0224F5E8.input->touching = touched;
    ov13_0224F5E8.input->touchRepeat = ov13_0224F5E8.input->touchTrig;
    if (!touched) {
        ov13_0224F5E8.touchCnt = 0;
        return;
    }
    ov13_0224F5E8.touchCnt++;
    if (ov13_0224F5E8.touchCnt == 40) {
        ov13_0224F5E8.input->touchRepeat = 1;
    } else if (ov13_0224F5E8.touchCnt == 47) {
        ov13_0224F5E8.input->touchRepeat = 1;
        ov13_0224F5E8.touchCnt = 40;
    }
}

// @fn ov13_02241694
BOOL ov13_02241694(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & ov13_0224F5E8.input->trig);
}

// @fn ov13_022416D4
BOOL ov13_022416D4(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & ov13_0224F5E8.input->repeat);
}

// @fn ov13_02241714
BOOL ov13_02241714(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & ov13_0224F5E8.input->release);
}

// @fn ov13_02241754
BOOL ov13_02241754(Rect16 *r) {
    Input13 *in;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    in = ov13_0224F5E8.input;
    if (!in->touching) {
        return FALSE;
    }
    if (r->x1 > in->touch.x) {
        return FALSE;
    }
    if (r->x2 < in->touch.x) {
        return FALSE;
    }
    if (r->y1 > in->touch.y) {
        return FALSE;
    }
    return r->y2 >= in->touch.y;
}

// @fn ov13_022417DC
BOOL ov13_022417DC(Rect16 *r) {
    Input13 *in;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    in = ov13_0224F5E8.input;
    if (!in->touchTrig) {
        return FALSE;
    }
    if (r->x1 > in->touch.x) {
        return FALSE;
    }
    if (r->x2 < in->touch.x) {
        return FALSE;
    }
    if (r->y1 > in->touch.y) {
        return FALSE;
    }
    return r->y2 >= in->touch.y;
}

// @fn ov13_02241864
BOOL ov13_02241864(Rect16 *r) {
    Input13 *in;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    in = ov13_0224F5E8.input;
    if (!in->touchRepeat) {
        return FALSE;
    }
    if (r->x1 > in->touch.x) {
        return FALSE;
    }
    if (r->x2 < in->touch.x) {
        return FALSE;
    }
    if (r->y1 > in->touch.y) {
        return FALSE;
    }
    return r->y2 >= in->touch.y;
}

// @fn ov13_022418EC
BOOL ov13_022418EC(Rect16 *r) {
    Input13 *in;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    in = ov13_0224F5E8.input;
    if (!in->touchRelease) {
        return FALSE;
    }
    if (r->x1 > in->touch.x) {
        return FALSE;
    }
    if (r->x2 < in->touch.x) {
        return FALSE;
    }
    if (r->y1 > in->touch.y) {
        return FALSE;
    }
    return r->y2 >= in->touch.y;
}

// @fn ov13_02241974
BOOL ov13_02241974(Rect16 *r) {
    Rect16 tmp;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    tmp.x1 = r->x1;
    tmp.y1 = r->y1;
    tmp.x2 = r->x1 + r->x2;
    tmp.y2 = r->y1 + r->y2;
    return ov13_022417DC(&tmp);
}

// @fn ov13_022419DC
BOOL ov13_022419DC(Pos16 *out) {
    Input13 *in;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    in = ov13_0224F5E8.input;
    if (!in->touching) {
        *out = in->prevTouch;
        return FALSE;
    }
    *out = in->touch;
    return TRUE;
}

// @fn ov13_02241A44
void ov13_02241A44(void) {
    if (ov13_0224F5E8.lcdOff) {
        if (PAD_DetectFold()) {
            return;
        }
        if (!PM_SetLCDPower(1)) {
            return;
        }
        ov13_0224F5E8.lcdOff = 0;
    } else {
        if (!PAD_DetectFold()) {
            return;
        }
        if (PM_SetLCDPower(0)) {
            ov13_0224F5E8.lcdOff = 1;
        }
    }
}

// @fn ov13_02241AC0
void ov13_02241AC0(void) {
    ov13_0224F600.mask = reg_OS_IE;
    OS_SetIrqMask(0x40018);
    OS_EnableIrqMask(1);
    ov13_0224F600.fn = OS_GetIrqFunction(1);
    OS_SetIrqFunction(1, ov13_02241B70);
    OS_ResetRequestIrqMask(1);
    OS_EnableIrq();
    OS_EnableInterrupts();
}

// @fn ov13_02241B34
void ov13_02241B34(void) {
    OS_DisableIrq();
    OS_SetIrqMask(ov13_0224F600.mask);
    OS_SetIrqFunction(1, ov13_0224F600.fn);
}
