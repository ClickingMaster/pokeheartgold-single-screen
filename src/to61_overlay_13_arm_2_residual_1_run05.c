#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern s32 FX_DivS32(s32, s32);
extern int WM_GetAllowedChannel(void);
extern int WM_GetLinkLevel(void);

struct Sprite {
    u32 bits;
    u16 attr;
};

static inline void ReadPos(struct Sprite *s, int *x, int *y) {
    *x = (s->bits & 0x01ff0000) >> 16;
    *y = (s->bits & 255) - 12;
}

struct A {
    void *task;
    struct Sprite *sprite;
    u8 type, tick;
};

struct B {
    void *sprite[3];
    void *task, *task2;
    u16 tick;
    s8 selected;
    u8 type, flag, ending;
};

struct C {
    struct Sprite *sprite;
    void *task;
    s8 type;
};

struct D {
    void *win[12];
    struct Sprite *sprite[47], *extra[4];
    void *other[2], *handle[4], *last, *task;
    u8 selected, page;
    s8 down;
    s8 hover;
    u8 pad, index, hold, flag1, flag2;
};
extern struct A *ov13_0224F464[1];

extern struct B *ov13_0224F468;
extern struct C *ov13_0224F4B8[1];
extern struct D *ov13_0224F4BC;
extern u8 ov13_0224F46C;
extern u8 ov13_02244EB0[];
extern u8 ov13_02244EE0[];
extern u8 ov13_02244EC4[];
extern u8 ov13_02244ED2[];
extern u8 ov13_02244F30[];
extern u8 ov13_02244F38[];
extern u8 ov13_02244F44[];
extern u16 ov13_02244EBC[];
extern u16 ov13_02244EBE[];
extern u16 ov13_02244F14[];
extern u16 ov13_02244F16[];
extern void *ov13_0224F4B0;
extern void *ov13_0224F4B4;
extern void *ov13_0224F4F8;
extern char ov13_0224F470[];
extern char ov13_02245B20[];
extern char ov13_02245D54[];
extern char *ov13_02245D04[];
extern u32 ov13_02244EF8[];
extern void *ov13_0224128C(int, int);
extern void ov13_022412B0(void *);
extern struct Sprite *ov13_02240184(int, int);
extern void *ov13_022401B0(int, int, int);
extern void ov13_02240008(void *, int, int);
extern void ov13_022400F0(int, int, void *);
extern void ov13_0223FF18(void *, int, int, int);
extern void ov13_02240068(void *, int, int *, int *);
extern void *ov13_02242528(int, void (*)(), int, int);
extern void ov13_022425C8(void *, void (*)());
extern void ov13_022425D0(int, void *);
extern void ov13_022425F0(int, void *);
extern void ov13_02241D98(void *);
extern void ov13_0223FD60(void *);
extern int ov13_0222B970(void);
extern int ov13_0222B980(void);
extern void *ov13_022408A0(int, int);
extern void ov13_02240B04(int);
extern void ov13_02240CC0(void *, int, int, int, int, int, u32, void *);
extern void ov13_02240B28(void *, int, int, int, int, void *);
extern void ov13_02240E2C(void *);
extern void *ov13_0222BAF4(void *, int, int, int);
extern void *ov13_0222BADC(void *, int);
extern void *ov13_0223F95C(char *, u32 *, int);
extern void ov13_0223FA50(void *);
extern void ov13_0222B90C(int *, int *);
extern void ov13_0223AC4C(int);
extern void ov13_0223B1E0(int);
extern u8 *ov13_02239FF0(void);
extern void ov13_0222936C(int);
extern void ov13_0223ABB8(int);
extern void ov13_022293F0(int, int);
extern void ov13_022290F0(int, int, int);
extern void ov13_022294B8(void);
extern void ov13_02228738(void);
extern void ov13_02228920(void);
extern void ov13_02228C68(void);
extern void ov13_02228EC0(void);
extern void ov13_02227064(void);
extern void ov13_02227390(void *);
extern void ov13_022273FC(void *);
extern void ov13_0222744C(void);
extern void ov13_022275D8(void *);
extern void ov13_0222763C(void *);
extern void ov13_022276B4(void *);
extern void ov13_02227FAC(void);
extern void ov13_022299CC(void);
extern void ov13_022284D8(void *);
extern void ov13_0222856C(void *);
extern void ov13_02228600(void *);
extern void ov13_02228694(void *);
extern void ov13_02228720(void);
extern void ov13_02227E70(void *);
void ov13_022272FC(int);
void ov13_02227130(int);
u32 ov13_02227B1C(void);
void ov13_02227D18(void);
char *ov13_022278C4(char *);
void ov13_02227924(char *, void (*)(const void *, u32, u32));
void ov13_02227DE4(int);

void ov13_02226F58(int type);
void ov13_02227028(void);
void ov13_02227228(void);
int ov13_02227258(void);
void ov13_0222726C(int a);
void ov13_0222728C(int a);
int ov13_022272A0(void);
void ov13_022272CC(void);
void ov13_022272E4(void);
void ov13_022277A4(void);
int ov13_022277B8(int a, int b, int c);
int ov13_0222788C(void);
void ov13_02227964(int a);
void ov13_022279B4(int a);
void ov13_022279EC(int a);
void ov13_02227A60(void *a, int b);
void ov13_02227B34(void);
void ov13_02227B54(void);
void ov13_02227B6C(int a, int b, int c, int y);
void ov13_02227C28(int a, int b, int c, int d);
void ov13_02227D58(void);
void ov13_02227DD0(void);
void ov13_02227E40(void);
void ov13_02227E58(void);
void ov13_02227EB0(int a);
void ov13_02227F68(void);
void ov13_022283B8(void);
int ov13_022283EC(void);
void ov13_02228400(int a);
void ov13_02228414(int a);
int ov13_02228428(void);

void ov13_02228444(void *task);
void ov13_02228C08(void);
void ov13_02228C38(void);
void ov13_02228E6C(int a);

extern u16 ov13_02244EB8[];
extern u16 ov13_02245958[];
extern u16 ov13_02244F68[];
extern u16 ov13_02244F64[];
extern u16 ov13_02244FA4[];
extern u16 ov13_02244F84[];
extern u16 ov13_02244F94[];
extern u16 ov13_02244F7C[];
extern u16 ov13_02244F9C[];
extern u16 ov13_02244F8C[];
extern int ov13_02245D80[];
extern void ov13_02240E74(void *, void *, void *);
extern int ov13_022417DC(void *);
extern int ov13_02241754(void *);
extern void *ov13_0223FDE0(void *, int);
void ov13_02228E6C(int);

void ov13_02228694(void *task) {
    int x, y;
    ov13_02240068(ov13_0224F4BC->other[0], 0, &x, &y);
    y -= 12;
    if (y > 170) {
        ov13_022290F0(ov13_0224F4BC->page, 4, y);
        return;
    }
    ov13_022290F0(ov13_0224F4BC->page, 4, 170);
    ov13_022294B8();
    ov13_022425C8(task, ov13_02228720);
}

void ov13_02228720(void) {
    ov13_02228738();
    ov13_02228920();
    ov13_02228C68();
    ov13_02228EC0();
}
