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
    GXVRamBG bg;
    GXVRamOBJ obj;
    GXVRamBGExtPltt bgExt;
    GXVRamOBJExtPltt objExt;
    GXVRamTex tex;
    GXVRamTexPltt texPltt;
    GXVRamClearImage clearImage;
    GXVRamSubBG subBg;
    GXVRamSubOBJ subObj;
    GXVRamSubBGExtPltt subBgExt;
    GXVRamSubOBJExtPltt subObjExt;
    int arm7;
    GXVRamLCDC lcdc;
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

// @fn ov13_02241B98
List *ov13_02241B98(void) {
    List *l = ov13_0224128C(sizeof(List), 4);
    l->head.prev = NULL;
    l->head.next = &l->tail;
    l->tail.prev = &l->head;
    l->tail.next = NULL;
    return l;
}

// @fn ov13_02241BC4
void ov13_02241BC4(List *list) {
    ov13_022412B0(&list);
}

// @fn ov13_02241BE0
void ov13_02241BE0(void *node) {
    Node *n = node;
    int irq = OS_DisableIrqMask(1);
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->next = NULL;
    n->prev = NULL;
    OS_EnableIrqMask(irq);
}

// @fn ov13_02241C18
void ov13_02241C18(void *pos, void *node) {
    Node *p = pos;
    Node *n = node;
    int irq = OS_DisableIrqMask(1);
    p->prev->next = n;
    n->prev = p->prev;
    n->next = p;
    p->prev = n;
    OS_EnableIrqMask(irq);
}

// @fn ov13_02241C48
void ov13_02241C48(List *list, void *node) {
    ov13_02241C18(&list->tail, node);
}

// @fn ov13_02241C58
void ov13_02241C58(List *list, void *node) {
    ov13_02241C18(list->head.next, node);
}
// @fn ov13_02241C68
void ov13_02241C68(void) {
    OamBuf13 *m;
    int i;
    ov13_0224F608 = m = ov13_0224128C(sizeof(OamBuf13), 4);
    MIi_CpuClearFast(0x200, (void *)m, 0x800);
    for (i = 0; i < 2; i++) {
        ov13_0224F608->pools[i] = ov13_0224022C(0x40, ov13_0224F608->oam[i], 8);
    }
    ov13_0224F608->task = ov13_02242528(1, (void (*)(void *, void *))ov13_02241D04, NULL, 0xC8);
}

// @fn ov13_02241D04
void ov13_02241D04(void) {
    DC_FlushRange(ov13_0224F608, 0x800);
    GX_LoadOAM(ov13_0224F608->oam[0], 0, 0x400);
    GXS_LoadOAM(ov13_0224F608->oam[1], 0, 0x400);
}

// @fn ov13_02241D4C
void ov13_02241D4C(void) {
    ov13_022425F0(1, ov13_0224F608->task);
    ov13_022412B0(STATIC_ADDR(0x0224F608));
}

// @fn ov13_02241D78
Oam *ov13_02241D78(int a) {
    return ov13_022402E0(ov13_0224F608->pools[a]);
}

// @fn ov13_02241D98
void ov13_02241D98(Oam *o) {
    int idx;
    o->attr01 = (o->attr01 & 0xC1FFFCFF) | 0x200;
    idx = 0;
    if ((u32)o >= (u32)ov13_0224F608->oam[1]) {
        idx = 1;
    }
    ov13_02240288(ov13_0224F608->pools[idx], o);
}

// @fn ov13_02241DE4
Oam *ov13_02241DE4(int a, int b) {
    return &ov13_0224F608->oam[a][b];
}

// @fn ov13_02241DFC
void ov13_02241DFC(void) {
    int i;
    ov13_0224F60C = ov13_0224128C(0x340, 4);
    for (i = 0; i < 2; i++) {
        ov13_0224F60C[i].pool = ov13_0224022C(0x20, ov13_0224F60C[i].buf, 0xC);
        ov13_0224F60C[i].list = ov13_02241B98();
        ov13_0224F60C[i].first.start = 0x300;
        ov13_0224F60C[i].last.start = 0x400;
        ov13_02241C58(ov13_0224F60C[i].list, &ov13_0224F60C[i].first);
        ov13_02241C48(ov13_0224F60C[i].list, &ov13_0224F60C[i].last);
    }
}

// @fn ov13_02241EBC
void *ov13_02241EBC(int which, int size, int fromFront, u32 *out) {
    ChrBlk *blk;
    ChrBlk *cur;
    int irq;
    int n;

    blk = ov13_022402E0(ov13_0224F60C[which].pool);
    n = ((size + 3) & ~3) >> 2;
    blk->size = n;
    irq = OS_DisableIrqMask(1);
    if (fromFront) {
        cur = &ov13_0224F60C[which].first;
        while (cur != &ov13_0224F60C[which].last) {
            ChrBlk *next = (ChrBlk *)cur->node.next;
            int start = cur->start + cur->size;
            if (start + n <= next->start) {
                blk->start = start;
                ov13_02241C18(next, blk);
                break;
            }
            cur = next;
        }
        if (cur == &ov13_0224F60C[which].last) {
            OS_Terminate();
        }
    } else {
        cur = &ov13_0224F60C[which].last;
        while (cur != &ov13_0224F60C[which].first) {
            ChrBlk *prev = (ChrBlk *)cur->node.prev;
            int start = cur->start - n;
            if (start >= prev->start + prev->size) {
                blk->start = start;
                ov13_02241C18(cur, blk);
                break;
            }
            cur = prev;
        }
        if (cur == &ov13_0224F60C[which].first) {
            OS_Terminate();
        }
    }
    *out = blk->start;
    OS_EnableIrqMask(irq);
    return blk;
}

// @fn ov13_02242034
void ov13_02242034(void *blk) {
    int idx = 0;
    ov13_02241BE0(blk);
    if ((u32)blk >= (u32)&ov13_0224F60C[1]) {
        idx = 1;
    }
    ov13_02240288(ov13_0224F60C[idx].pool, blk);
}

// @fn ov13_02242074
int ov13_02242074(const u8 *s, int max) {
    int i;
    for (i = 0; i < max; i++) {
        if (s[i] == 0) {
            break;
        }
    }
    return i;
}

// @fn ov13_022420A0
void ov13_022420A0(void) {
    ov13_0224F610.bg = GX_DisableBankForBG();
    ov13_0224F610.obj = GX_DisableBankForOBJ();
    ov13_0224F610.bgExt = GX_DisableBankForBGExtPltt();
    ov13_0224F610.objExt = GX_DisableBankForOBJExtPltt();
    ov13_0224F610.tex = GX_DisableBankForTex();
    ov13_0224F610.texPltt = GX_DisableBankForTexPltt();
    ov13_0224F610.clearImage = GX_DisableBankForClearImage();
    ov13_0224F610.subBg = GX_DisableBankForSubBG();
    ov13_0224F610.subObj = GX_DisableBankForSubOBJ();
    ov13_0224F610.subBgExt = GX_DisableBankForSubBGExtPltt();
    ov13_0224F610.subObjExt = GX_DisableBankForSubOBJExtPltt();
    ov13_0224F610.arm7 = GX_DisableBankForARM7();
    ov13_0224F610.lcdc = GX_DisableBankForLCDC();
    GX_SetBankForARM7(ov13_0224F610.arm7);
    ov13_0224225C();
}

// @fn ov13_02242154
void ov13_02242154(void) {
    OSIntrMode irq;
    GX_DisableBankForBG();
    GX_DisableBankForOBJ();
    GX_DisableBankForSubBG();
    GX_DisableBankForSubOBJ();
    ov13_0224225C();
    GX_SetBankForBG(ov13_0224F610.bg);
    GX_SetBankForOBJ(ov13_0224F610.obj);
    GX_SetBankForBGExtPltt(ov13_0224F610.bgExt);
    GX_SetBankForOBJExtPltt(ov13_0224F610.objExt);
    GX_SetBankForTex(ov13_0224F610.tex);
    GX_SetBankForTexPltt(ov13_0224F610.texPltt);
    GX_SetBankForClearImage(ov13_0224F610.clearImage);
    GX_SetBankForSubBG(ov13_0224F610.subBg);
    GX_SetBankForSubOBJ(ov13_0224F610.subObj);
    GX_SetBankForSubBGExtPltt(ov13_0224F610.subBgExt);
    GX_SetBankForSubOBJExtPltt(ov13_0224F610.subObjExt);
    GX_SetBankForLCDC(ov13_0224F610.lcdc);
    *(vu16 *)0x04000050 = 0;
    *(vu16 *)0x04001050 = 0;
    *(vu32 *)0x04000010 = 0;
    *(vu32 *)0x04000014 = 0;
    *(vu32 *)0x04000018 = 0;
    *(vu32 *)0x0400001C = 0;
    *(vu32 *)0x04001010 = 0;
    *(vu32 *)0x04001014 = 0;
    *(vu32 *)0x04001018 = 0;
    *(vu32 *)0x0400101C = 0;
    irq = OS_DisableInterrupts();
    PM_SetLCDPower(1);
    OS_RestoreInterrupts(irq);
}

// @fn ov13_0224225C
void ov13_0224225C(void) {
    GX_SetBankForLCDC((GXVRamLCDC)0x1F3);
    MIi_CpuClearFast(0, (void *)0x06800000, 0x40000);
    MIi_CpuClearFast(0, (void *)0x06880000, 0x24000);
    GX_DisableBankForLCDC();
    MIi_CpuClearFast(0x200, (void *)0x07000000, 0x400);
    MIi_CpuClearFast(0, (void *)0x05000000, 0x400);
    MIi_CpuClearFast(0x200, (void *)0x07000400, 0x400);
    MIi_CpuClearFast(0, (void *)0x05000400, 0x400);
}

// @fn ov13_022422E0
void ov13_022422E0(void) {
    u32 pat;
    if (PM_GetLEDPattern(&pat) != 0) {
        return;
    }
    if (pat == 0xF) {
        return;
    }
    PMi_SendLEDPatternCommand(0xF);
}

// @fn ov13_0224230C
void ov13_0224230C(void) {
    PMi_SendLEDPatternCommand(1);
}

// @fn ov13_0224231C
void ov13_0224231C(void) {
    int i;
    u8 *cnt;
    ov13_0224F644 = ov13_0224128C(0x80, 4);
    cnt = ov13_02245960;
    for (i = 0; i < 2; i++) {
        ov13_0224F644[i].count = *cnt;
        ov13_0224F644[i].buf = ov13_0224128C(*cnt * 0x14, 4);
        ov13_0224F644[i].taskPool = ov13_0224022C(*cnt, ov13_0224F644[i].buf, 0x14);
        ov13_0224F644[i].queue = ov13_022401F8(*cnt++);
        ov13_0224F644[i].list = ov13_02241B98();
        ov13_0224F644[i].head.prio = 0;
        ov13_0224F644[i].tail.prio = 0xFF;
        ov13_02241C58(ov13_0224F644[i].list, &ov13_0224F644[i].head);
        ov13_02241C48(ov13_0224F644[i].list, &ov13_0224F644[i].tail);
        ov13_0224F644[i].enabled = 1;
    }
}

// @fn ov13_02242430
void ov13_02242430(void) {
    int i;
    for (i = 0; i < 2; i++) {
        ov13_02241BC4(ov13_0224F644[i].list);
        ov13_0224026C(ov13_0224F644[i].taskPool);
    }
    ov13_022412B0(STATIC_ADDR(0x0224F644));
}

// @fn ov13_02242478
void ov13_02242478(int q) {
    Task13 *t;
    int i;
    if (!ov13_0224F644[q].enabled) {
        return;
    }
    for (t = (Task13 *)ov13_0224F644[q].head.node.next; t != &ov13_0224F644[q].tail; t = (Task13 *)t->node.next) {
        t->func(t, t->arg);
    }
    for (i = 0; i < ov13_0224F644[q].count; i++) {
        t = ov13_022402E0(ov13_0224F644[q].queue);
        if (t == NULL) {
            return;
        }
        ov13_022425FC(q, t);
    }
}

// @fn ov13_02242528
void *ov13_02242528(int q, void (*func)(void *, void *), void *arg, u32 prio) {
    return ov13_0224253C(q, func, arg, prio, 0);
}

// @fn ov13_0224253C
Task13 *ov13_0224253C(int q, void (*func)(void *, void *), void *arg, u32 prio, u8 owned) {
    Task13 *t;
    Task13 *cur;
    int irq;
    t = ov13_022402E0(ov13_0224F644[q].taskPool);
    t->func = func;
    t->arg = arg;
    t->prio = prio;
    t->owned = owned;
    irq = OS_DisableIrqMask(1);
    for (cur = (Task13 *)ov13_0224F644[q].head.node.next;; cur = (Task13 *)cur->node.next) {
        if (prio < cur->prio) {
            ov13_02241C18(cur, t);
            break;
        }
    }
    OS_EnableIrqMask(irq);
    return t;
}

// @fn ov13_022425C8
void ov13_022425C8(Task13 *t, void (*func)(void *, void *)) {
    t->func = func;
}

// @fn ov13_022425D0
void ov13_022425D0(int q, void *t) {
    ov13_02240288(ov13_0224F644[q].queue, t);
}

// @fn ov13_022425F0
void ov13_022425F0(int q, void *t) {
    ov13_022425FC(q, t);
}

// @fn ov13_022425FC
void ov13_022425FC(int q, Task13 *t) {
    if (t->owned) {
        ov13_022412B0(&t->arg);
    }
    ov13_02241BE0(t);
    ov13_02240288(ov13_0224F644[q].taskPool, t);
}

// @fn ov13_02242640
void ov13_02242640(int q, u8 enabled) {
    ov13_0224F644[q].enabled = enabled;
}
