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
extern struct A *gWifiSetupBusyIcon[1];

extern struct B *ov13_0224F468;
extern struct C *gWifiSetupSignalIcon[1];
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
extern void *WifiSetupHeap_AllocZeroed(int, int);
extern void WifiSetupHeap_FreeAndClear(void *);
extern struct Sprite *WifiSetupOamTemplate_AllocOam(int, int);
extern void *WifiSetupOamTemplate_AllocBlock(int, int, int);
extern void WifiSetupOamBlock_SetPriority(void *, int, int);
extern void WifiSetupOamTemplate_Copy(int, int, void *);
extern void WifiSetupOamBlock_SetPosition(void *, int, int, int);
extern void WifiSetupOamBlock_GetPosition(void *, int, int *, int *);
extern void *WifiSetupTask_Create(int, void (*)(), int, int);
extern void WifiSetupTask_SetFunc(void *, void (*)());
extern void WifiSetupTask_DestroyLater(int, void *);
extern void WifiSetupTask_Destroy(int, void *);
extern void WifiSetupOam_Release(void *);
extern void WifiSetupOamBlock_Free(void *);
extern int ov13_0222B970(void);
extern int ov13_0222B980(void);
extern void *WifiSetupText_InitBgCanvas(int, int);
extern void WifiSetupText_FreeBgCanvas(int);
extern void WifiSetupText_DrawInRect(void *, int, int, int, int, int, u32, void *);
extern void WifiSetupText_Draw(void *, int, int, int, int, void *);
extern void WifiSetupText_MarkDirty(void *);
extern void *ov13_0222BAF4(void *, int, int, int);
extern void *ov13_0222BADC(void *, int);
extern void *WifiSetupFile_Load(char *, u32 *, int);
extern void WifiSetupFile_Free(void *);
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
extern void WifiSetupBusyIcon_Task(void);
extern void WifiSetupButtonBar_SlideInTask(void *);
extern void WifiSetupButtonBar_WaitTask(void *);
extern void WifiSetupButtonBar_InputTask(void);
extern void WifiSetupButtonBar_SlideOutTask(void *);
extern void WifiSetupButtonBar_DestroyTask(void *);
extern void WifiSetupButtonBar_PressTask(void *);
extern void WifiSetupSignalIcon_Task(void);
extern void ov13_022299CC(void);
extern void ov13_022284D8(void *);
extern void ov13_0222856C(void *);
extern void ov13_02228600(void *);
extern void ov13_02228694(void *);
extern void ov13_02228720(void);
extern void WifiSetupBackground_TransferTask(void *);
void WifiSetupButtonBar_SetY(int);
void WifiSetupButtonBar_Create(int);
u32 WifiSetup_GetTextFlags(void);
void WifiSetupCursor_Hide(void);
char *WifiSetup_LocalizePath(char *);
void WifiSetup_LoadGraphics(char *, void (*)(const void *, u32, u32));
void WifiSetupBackground_LoadHeader(int);

void WifiSetupBusyIcon_Create(int type);
void WifiSetupBusyIcon_Destroy(void);
void WifiSetupButtonBar_Close(void);
int WifiSetupButtonBar_GetSelected(void);
void WifiSetupButtonBar_SelectIfNone(int a);
void WifiSetupButtonBar_SetSelected(int a);
int ov13_022272A0(void);
void WifiSetupButtonBar_Unlock(void);
void WifiSetupButtonBar_Lock(void);
void WifiSetupMessage_Reset(void);
int WifiSetupMessage_Show(int a, int b, int c);
int WifiSetupMessage_Hide(void);
void ov13_02227964(int a);
void ov13_022279B4(int a);
void WifiSetup_PrintMessage(int a);
void WifiSetup_PrintMessageWithText(void *a, int b);
void WifiSetupCursor_Init(void);
void WifiSetupCursor_Free(void);
void WifiSetupCursor_ShowPair(int a, int b, int c, int y);
void WifiSetupCursor_ShowCorners(int a, int b, int c, int d);
void WifiSetupBackground_Init(void);
void WifiSetupBackground_Free(void);
void ov13_02227E40(void);
void ov13_02227E58(void);
void WifiSetupSignalIcon_Create(int a);
void WifiSetupSignalIcon_Destroy(void);
void WifiSetupKeyboard_Close(void);
int WifiSetupKeyboard_GetSelected(void);
void ov13_02228400(int a);
void ov13_02228414(int a);
int WifiSetupKeyboard_Exists(void);

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
extern void WifiSetup_RectFromPosSize(void *, void *, void *);
extern int WifiSetupInput_IsTouchPressed(void *);
extern int WifiSetupInput_IsTouchHeld(void *);
extern void *WifiSetupOamBlock_GetOam(void *, int);
void ov13_02228E6C(int);

void ov13_02228C08(void) {
    ov13_0222936C(ov13_0224F4BC->page == 2 ? 0 : 2);
    ov13_0223ABB8(1);
}

void ov13_02228C38(void) {
    ov13_0222936C(ov13_0224F4BC->page != 1 ? 1 : 0);
    ov13_0223ABB8(1);
}
