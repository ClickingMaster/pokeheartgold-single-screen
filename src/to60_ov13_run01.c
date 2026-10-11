#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef void (*StateFunc)(void);

extern int WifiSetupFade_IsActive(int a);
extern void WifiSetupFade_Start(int a, int b, int c, int d);
extern void WifiSetupDisplay_ShowPlanes(int a, int b);
extern void WifiSetupDisplay_HidePlanes(int a, int b);
extern void WifiSetupText_FreeBgCanvas(int a);
extern void WifiSetupFade_StartDelay(int a);
extern int WifiSetupInput_IsKeyPressed(int a);
extern void WifiSetupSignalIcon_Destroy(void);
extern void WifiSetupBusyIcon_Destroy(void);
extern void ov13_0223A840(void);
extern void ov13_0223AC20(void);
extern void ov13_0223ABB8(int a);
extern void WifiSetup_PrintMessage(int a);
extern void ov13_022279B4(int a);
extern int WifiSetupButtonBar_GetSelected(void);
extern void WifiSetupButtonBar_Unlock(void);
extern void WifiSetupButtonBar_SelectIfNone(int a);
extern void WifiSetupButtonBar_Lock(void);
extern void WifiSetupButtonBar_Close(void);
extern int ov13_022272A0(void);
extern void WifiSetupBackground_LoadHeader(int a);
extern int WifiSetupMessage_Show(int a, int b, int c);
extern void WifiSetup_LoadGraphics(void *a, void *b);
extern void WifiSetupMessage_Hide(void);

extern u8 ov13_0224F514;
extern u8 ov13_0224F518;
extern u8 ov13_0224F51C;
extern u8 ov13_0224F520[];
extern u8 ov13_0224F524[];
extern u8 ov13_0224F528;

struct UnkF52C {
    u8 flag;
    u16 counter;
    void *task;
};
extern struct UnkF52C ov13_0224F52C;
extern u8 ov13_0224F534;

struct UnkOam {
    u16 attr0;
    u16 attr1;
    u16 attr2;
};

struct UnkBig {
    u8 unk0[4];
    void *unk4;
    void *unk8;
    int unkC;
    struct UnkOam *unk10[5];
    struct UnkOam *unk24[5];
    u8 unk38[4];
    void *unk3C;
    u16 unk40;
    u16 unk42[3];
    u16 unk48[4];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
};

struct UnkF538 {
    u8 unk0;
    u16 unk2;
    struct UnkBig *big;
};
extern struct UnkF538 ov13_0224F538;

struct Str25 {
    char s[25];
};

struct Str22 {
    char s[22];
};
extern struct Str25 ov13_022453E3;
extern struct Str22 ov13_022453CD;
extern u8 ov13_02246358[];
extern u8 ov13_0224544C[];
extern u8 ov13_02245450[];
extern void ov13_02238440();
extern void *WifiSetupHeap_AllocZeroed(int size, int align);
extern u8 ov13_02239D04(void *p);
extern void ov13_02230C68();
extern void ov13_022307AC(void);
extern void ov13_02230CC4(void);
extern void *WifiSetup_LocalizePath(void *a);
extern void *WifiSetupFile_Load(void *a, void *b, int c);
extern void ov13_02239228(void *a);
extern void ov13_022392B4(void);
extern void ov13_0223943C(int a, int b, int c, int d, int e);
extern struct UnkOam *WifiSetupOamTemplate_AllocOam(int a, int b);
extern void WifiSetupOamTemplate_Copy(int a, int b, struct UnkOam *c);
extern void ov13_0222FF50(void);
extern void ov13_022301A4(void);
extern void ov13_02230414(void);
extern s32 FX_DivS32(s32 a, s32 b);
extern u8 ov13_02246300[];
extern u8 ov13_02246318[];
extern u8 ov13_02246330[];
extern u8 ov13_02246344[];
extern void ov13_02227E40(void);
extern void ov13_02227964(int a);
extern void ov13_0222B254(void);
extern void *WifiSetupTask_Create(int a, void *b, int c, int d);
extern int ov13_0222B3B4(void);
extern void WifiSetupTask_Destroy(int a, void *b);
extern void WifiSetupTask_DestroyLater(int a, void *b);
extern void ov13_0222B364(int a);
extern void ov13_02237618();
extern void WifiSetupInput_Update(void);
extern void WifiSetupTask_RunQueue(int a);
extern u8 ov13_022462D8[];
extern u8 ov13_022462EC[];
extern int WifiSetupText_InitBgCanvas(int a, int b);
extern void WifiSetupText_DrawInRect(int a, int b, int c, int d, int e, int f, int g, void *h);
extern void WifiSetupText_MarkDirty(int a);
extern void ov13_0223BE6C(int a, int b, int c, int d, int e);
extern int ov13_0223C1DC(void);
extern void ov13_0223C198(void);
extern int ov13_0223C1F0(void);
extern void WifiSetupBusyIcon_Create(int a);
extern u8 ov13_0224629C[];
extern u8 ov13_022462B0[];
extern u8 ov13_022462C4[];

struct UnkF4CC {
    u8 state;
    u8 flag1;
    u8 pad2[6];
    u32 lo : 4;
    u32 hi : 28;
    void *func;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
};
extern struct UnkF4CC ov13_0224F4CC;

struct UnkF4EC {
    u8 pad[0x60];
    void *unk60;
};
extern struct UnkF4EC *ov13_0224F4EC;

struct UnkRes {
    u32 *offsets;
    u8 *base;
    void *data;
};
extern struct UnkRes *ov13_0224F4F8;

struct UnkF4F0 {
    u8 pad[0x1E280];
    u8 unk1E280[0x18];
    void (*callback)(int);
    void *task;
    u8 unk1E2A0;
    u8 unk1E2A1;
};
extern struct UnkF4F0 *ov13_0224F4F0;

struct Unk1C {
    int unk0;
    void *unk4;
    void *unk8;
    int unkC;
    int unk10;
    int unk14;
    u8 unk18;
};
extern struct Unk1C ov13_02245F8C;

struct Unk0C {
    int state;
    int unk4;
    int unk8;
};
extern struct Unk0C *ov13_0224F4F4;

struct UnkE8 {
    u8 pad[0x20];
    int unk20;
    int unk24;
    u8 pad28[0xC0];
};

extern void GX_DispOff(void);
extern void ov13_0224230C(void);
extern void WifiSetupInput_Free(void);
extern void WifiSetup_RestoreVBlankIntr(void);
extern void ov13_0223AB8C(void);
extern void WifiSetupOamBlock_FreeManager(void);
extern void WifiSetupOam_Free(void);
extern void WifiSetupText_Free(void);
extern void WifiSetupFade_Free(void);
extern void WifiSetupArchive_Unload(void);
extern void WifiSetupTask_FreeScheduler(void);
extern void WifiSetupHeap_Destroy(void);
extern void WifiSetup_RestoreVramBanks(void);
extern void *WifiSetupPool_NewWithItems(int a, void *b, int c);
extern void WifiSetupHeap_FreeAndClear(void *pp);
extern void *WifiSetupPool_Pop(void *a);
extern void WifiSetupFile_Free(void *a);
extern void WifiSetupPool_Push(void *a, void *b);
extern void ov13_0223E80C(void *a, struct Unk1C *b);
extern int ov13_0223EA58(void);
extern void OS_Terminate(void);
extern int ov13_0223E8FC(void);
extern void ov13_0223EAA0(void);
extern int ov13_02226D74(int a, int b, void *c, void *d, void *e, int f);
extern int ov13_02226E5C(void);
extern int ov13_02226F28(struct UnkE8 *a);
extern void ov13_0223A5D0(struct UnkE8 *a);
extern void WifiSetupHeap_Free(void *a);
extern void WifiSetupTask_SetQueueEnabled(int a, int b);
extern void WifiSetupOamTemplate_Free(int a);
extern void WifiSetupBackground_Free(void);
extern void WifiSetupCursor_Free(void);
extern void ov13_02239FDC(void);
extern void ov13_02239F98(void);
extern void WifiSetupCursor_Init(void);
extern void WifiSetupBackground_Init(void);
extern void WifiSetupMessage_Reset(void);
extern void WifiSetupOamTemplate_Load(int a, void *b);
extern void *ov13_02246018[];
extern u8 ov13_02246034[];
extern u8 ov13_02246044[];
extern u8 ov13_02246058[];
extern u8 ov13_0224606C[];
extern u8 ov13_02246080[];
extern u8 ov13_02246094[];
extern u8 ov13_022460AC[];
extern u8 ov13_022460C4[];
extern u8 ov13_022460DC[];
extern u8 ov13_022460F4[];
extern u8 ov13_0224610C[];
extern u8 ov13_02246124[];
extern u8 ov13_02246138[];
extern u8 ov13_0224614C[];
extern u8 ov13_02246164[];
extern u8 ov13_0224617C[];
extern void ov13_022368D8();
extern void ov13_0223B6F4(int a);
extern void WifiSetupCursor_ShowCorners(int a, int b, int c, int d);
extern u16 ov13_0224533E[];
extern u16 ov13_02245340[];
extern u16 ov13_02245342[];
extern u16 ov13_02245344[];

struct UnkF4FC {
    u8 sel;
    void *unk4;
};
extern struct UnkF4FC ov13_0224F4FC;
extern struct Str22 ov13_0224534E;

struct Unk8 {
    u8 data[8];
};
extern struct Unk8 ov13_0224532E[];
extern int WifiSetupInput_IsTouchPressed(struct Unk8 *a);
extern int WifiSetupInput_IsKeyRepeated(int a);
extern void WifiSetupCursor_Hide(void);

struct Unk2 {
    u8 data[2];
};
extern struct Unk2 ov13_0224532C;
extern void ov13_02239378(void *a, int b, int c);
extern u8 ov13_0224F504;
extern u8 ov13_02246190[];
extern u8 ov13_022461A4[];
extern u8 ov13_022461B8[];

struct UnkF508 {
    u8 sel;
    u8 unk1;
    void *unk4;
    struct UnkOam *unk8;
};
extern struct UnkF508 ov13_0224F508;
extern u16 ov13_022453B2[];
extern u16 ov13_022453B4[];
extern u16 ov13_022453B6[];
extern u16 ov13_022453B8[];
extern struct Unk8 ov13_0224539A[];
extern u8 ov13_02245364[];
extern struct Str22 ov13_0224536C;

struct Str23 {
    char s[23];
};
extern struct Str23 ov13_02245382;
extern u8 ov13_02246258[];
extern u8 ov13_02246270[];
extern u8 ov13_02246288[];

struct WiFiInfo {
    u64 id;
    u8 pad[0xC];
};

extern void DWCi_BM_GetWiFiInfo(struct WiFiInfo *info);
extern void WifiSetupButtonBar_SetSelected(int a);
extern void WifiSetupOam_Release(void *a);
extern s32 FX_ModS32(s32 a, s32 b);
extern void WifiSetupSignalIcon_Create(int a);
extern void ov13_0222E220();
extern void ov13_0222DC24();
extern u16 ov13_022461CC[];
extern u16 ov13_02246208[];
extern u16 ov13_02246230[];
extern int swprintf(u16 *buf, u32 n, const u16 *fmt, ...);
extern void ov13_0223EAD4(void);
extern void ov13_0223EE58(u8 *type, u8 *flag);
extern void *ov13_0223EE9C(void);

void ov13_0222B898();
void ov13_0222B8E8();
void ov13_0222B8F8();
void ov13_0222B90C();
void ov13_0222B934();
void ov13_0222B948();
u8 ov13_0222B970();
u32 ov13_0222B980();
BOOL ov13_0222B998();
void ov13_0222B9B4();
u8 ov13_0222B9C8();
void ov13_0222BA08();
void ov13_0222BA44();
struct UnkRes *ov13_0222BA58();
void ov13_0222BAB0();
u8 *ov13_0222BADC(struct UnkRes *res, int idx);
void ov13_0222BAF4(struct UnkRes *res, int idx, int pos, int ch);
void ov13_0222BB20();
void ov13_0222BC0C();
void ov13_0222BC2C();
BOOL ov13_0222BC68();
void ov13_0222BC84();
void ov13_0222BC9C();
u8 *ov13_0222BCA8();
void ov13_0222BCC0();
void ov13_0222BF60();
void ov13_0222BFCC();
int ov13_0222BFF0();
void ov13_0222C0AC();
void ov13_0222C0D8();
void *ov13_0222C0F0();
void ov13_0222C100();
void ov13_0222C10C();
void ov13_0222C148();
void ov13_0222C1BC();
void ov13_0222C39C();
void ov13_0222C3D8();
void ov13_0222C44C();
void ov13_0222C4C8();
void ov13_0222C5B4();
void ov13_0222C608();
void ov13_0222C644();
void ov13_0222C66C();
void ov13_0222C680();
void ov13_0222C7AC();
void ov13_0222C7B0();
void ov13_0222C800();
void ov13_0222C820();
void ov13_0222C86C();
void ov13_0222C914();
void ov13_0222C984();
void ov13_0222C9C4();
void ov13_0222CA0C();
void ov13_0222CA8C();
void ov13_0222CAE0();
void ov13_0222CB1C();
void ov13_0222CB44();
void ov13_0222CB58();
void ov13_0222CB90();
void ov13_0222CB94();
void ov13_0222CBFC();
void ov13_0222CC1C();
void ov13_0222CC80();
void ov13_0222CD38();
void ov13_0222CD88();
void ov13_0222CDA8();
void ov13_0222CDCC();
void ov13_0222CE4C();
void ov13_0222CE80();
void ov13_0222CEAC();
void ov13_0222CED4();
void ov13_0222CEE8();
void ov13_0222CF08();
void ov13_0222CF0C();
void ov13_0222CF34();
void ov13_0222CF54();
void ov13_0222CFA0();
void ov13_0222CFD4();
void ov13_0222D008();
void ov13_0222D088();
void ov13_0222D25C();
void ov13_0222D2B0();
void ov13_0222D2EC();
void ov13_0222D314();
void ov13_0222D328();
void ov13_0222D348();
void ov13_0222D34C();
void ov13_0222D374();
void ov13_0222D394();
void ov13_0222D3E4();
void ov13_0222D454();
void ov13_0222D51C();
void ov13_0222D6B8();
void ov13_0222D70C();
void ov13_0222D748();
void ov13_0222D770();
void ov13_0222D784();
void ov13_0222D878();
void ov13_0222D87C();
void ov13_0222D920();
void ov13_0222D940();
void ov13_0222D9A4();
void ov13_0222DAB0();
void ov13_0222DB40();
void ov13_0222DB6C();
void ov13_0222DBA4();

#define REG16(a)    (*(vu16 *)(a))
#define SET_PRI3(a) REG16(a) = (u16)((REG16(a) & ~3) | 3)

void ov13_0222B898(void) {
    GX_DispOff();
    *(vu32 *)0x04001000 &= ~0x10000;
    ov13_0224230C();
    WifiSetupInput_Free();
    WifiSetup_RestoreVBlankIntr();
    ov13_0223AB8C();
    WifiSetupOamBlock_FreeManager();
    WifiSetupOam_Free();
    WifiSetupText_Free();
    WifiSetupFade_Free();
    WifiSetupArchive_Unload();
    WifiSetupTask_FreeScheduler();
    WifiSetupHeap_Destroy();
    WifiSetup_RestoreVramBanks();
}

void ov13_0222B8E8(void *func) {
    ov13_0224F4CC.func = func;
}

void ov13_0222B8F8(int a, int b) {
    ov13_0224F4CC.unk10 = a;
    ov13_0224F4CC.unk14 = b;
}

void ov13_0222B90C(int *a, int *b) {
    if (a != NULL) {
        *a = ov13_0224F4CC.unk10;
    }
    if (b != NULL) {
        *b = ov13_0224F4CC.unk14;
    }
}

void ov13_0222B934(int a, int b) {
    ov13_0224F4CC.unk18 = a;
    ov13_0224F4CC.unk1C = b;
}

void ov13_0222B948(int *a, int *b) {
    if (a != NULL) {
        *a = ov13_0224F4CC.unk18;
    }
    if (b != NULL) {
        *b = ov13_0224F4CC.unk1C;
    }
}

u8 ov13_0222B970(void) {
    return ov13_0224F4CC.state;
}

u32 ov13_0222B980(void) {
    return ov13_0224F4CC.lo;
}

BOOL ov13_0222B998(u32 mask) {
    return (mask & ov13_0224F4CC.hi) ? TRUE : FALSE;
}

void ov13_0222B9B4(void) {
    ov13_0224F4CC.flag1 = 1;
}

u8 ov13_0222B9C8(void) {
    u8 state = ov13_0224F4CC.state;
    if (state == 6) {
        return 0x38;
    }
    if (state == 1 && !(ov13_0224F4CC.hi & 2)) {
        return 0x37;
    }
    return state + 0x31;
}

void ov13_0222BA08(void) {
    ov13_0224F4EC = WifiSetupHeap_AllocZeroed(0x64, 4);
    ov13_0224F4EC->unk60 = WifiSetupPool_NewWithItems(8, ov13_0224F4EC, 0xc);
}

void ov13_0222BA44(void) {
    WifiSetupHeap_FreeAndClear(&ov13_0224F4EC);
}

struct UnkRes *ov13_0222BA58(void *src) {
    u32 size;
    struct UnkRes *res = WifiSetupPool_Pop(ov13_0224F4EC->unk60);
    u8 *data = WifiSetupFile_Load(src, &size, 4);
    u8 *hdr;
    res->data = data;
    hdr = data + 0x20;
    res->offsets = (u32 *)(hdr + 0x10);
    res->base = hdr + *(u32 *)(hdr + 4) + 8;
    return res;
}

void ov13_0222BAB0(struct UnkRes *res) {
    WifiSetupFile_Free(res->data);
    WifiSetupPool_Push(ov13_0224F4EC->unk60, res);
}

u8 *ov13_0222BADC(struct UnkRes *res, int idx) {
    return res->base + res->offsets[(u16)idx];
}
