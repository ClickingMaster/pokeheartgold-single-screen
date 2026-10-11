#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[4];
    u16 flags;
} Widget;

typedef struct {
    void *task, *unused, *image, *palette, *subimage;
    void *menu, *widgets[7];
    Widget *extra;
    u8 q[4];
    void *callback;
    u8 r[2];
    u8 count, flag;
} UI;

typedef struct {
    u8 p, state;
    u16 q;
    u16 position, pad;
    UI *ui;
} Global;

extern Global ov13_0224F54C;
extern s8 ov13_022455D4[];
extern u8 *ov13_02239FF0(void);
extern void *WifiSetupHeap_AllocZeroed(int, int);
extern void *WifiSetupText_InitBgCanvas(int, int);
extern void *WifiSetupOamTemplate_AllocOam(int, int);
extern void *WifiSetupTask_Create(int, void *, int, int);
extern void ov13_02233214(void);
extern void WifiSetupMessage_Show(int, int, int);
extern void ov13_02227964(int);
extern void ov13_02227E58(void);
extern void ov13_0223943C(int, int, int, int, int);
extern void ov13_02233DCC(void);
extern void ov13_02234BCC(void);
extern void WifiSetupApp_SetStateFunc(void *);
extern void ov13_02234B48(void);
extern void ov13_022350A8(void);
extern void ov13_02233250(void);
extern int getMode(void);
void ov13_02232E04(void), ov13_02233000(void);
extern int WifiSetupApp_GetLanguage(void);

typedef struct {
    u8 bytes[23];
} Text23;

typedef struct {
    u8 bytes[21];
} Text21;

typedef struct {
    u8 bytes[22];
} Text22;

extern Text23 ov13_02245573;
extern Text21 ov13_02245548;
extern Text22 ov13_0224555D;
extern u8 ov13_02246474[];
extern u8 ov13_02246488[];
extern u8 ov13_022464A0[];
extern u8 ov13_022464B8[];
extern void WifiSetup_LoadGraphics(const void *, void *);
extern void ov13_02239228(void *);
extern void ov13_022392B4(void);
extern void *WifiSetup_LocalizePath(void *);
extern void *WifiSetupFile_Load(void *, int, int);

static inline void setPriority(volatile u16 *r, int p) {
    *r = (*r & ~3) | p;
}

void ov13_02232E04(void) {
    u8 *user = ov13_02239FF0();
    int i;
    ov13_0224F54C.ui = WifiSetupHeap_AllocZeroed(72, 4);
    ov13_0224F54C.ui->flag = user[246];
    ov13_0224F54C.ui->count = 12;
    ov13_02233214();
    ov13_02233000();
    if (!ov13_0224F54C.state) {
        WifiSetupMessage_Show(48, ov13_022455D4[WifiSetupApp_GetLanguage()], user[244] + 1);
    } else {
        WifiSetupMessage_Show(69, -1, 0);
    }
    ov13_02227964(1);
    ov13_02227E58();
    ov13_0223943C(2, 85, 241, 65, (ov13_0224F54C.position * 55) / 145);
    ov13_0224F54C.ui->menu = WifiSetupText_InitBgCanvas(0, 1);
    for (i = 0; i < 7; i++) {
        ov13_0224F54C.ui->widgets[i] = WifiSetupOamTemplate_AllocOam(0, 41);
    }
    switch (user[231]) {
    case 1:
        ov13_0224F54C.ui->extra = WifiSetupOamTemplate_AllocOam(0, 80);
        ov13_0224F54C.ui->extra->flags = (ov13_0224F54C.ui->extra->flags & ~0xc00) | 0xc00;
        break;
    case 2:
        ov13_0224F54C.ui->extra = WifiSetupOamTemplate_AllocOam(0, 81);
        ov13_0224F54C.ui->extra->flags = (ov13_0224F54C.ui->extra->flags & ~0xc00) | 0xc00;
        break;
    }
    ov13_0224F54C.ui->callback = WifiSetupTask_Create(1, ov13_02234B48, 0, 110);
    ov13_0224F54C.ui->task = WifiSetupTask_Create(0, ov13_022350A8, 0, 120);
    ov13_02233DCC();
    ov13_02234BCC();
    WifiSetupApp_SetStateFunc(ov13_02233250);
}
