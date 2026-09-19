#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"

typedef struct OverlayManager OverlayManager;

typedef struct Ov65Args {
    u8 pad0[4];
    void *playerProfile;
    void *party;
    void *saveData;
    void *unk10;
    void *unk14;
} Ov65Args;

#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

extern void *OverlayManager_GetArgs(OverlayManager *manager);
extern u32 SaveArray_Party_sizeof(void);
extern void *Heap_Alloc(enum HeapID heapID, u32 size);
extern void Party_InitWithMaxSize(void *party, int count);
extern void *memset(void *dst, int value, u32 size);
extern void *PlayerProfile_GetPlayerName_NewString(void *profile, enum HeapID heapID);
extern void *NewString_ReadMsgData(void *msgData, int msgNo);
extern void sub_02039F68(void *);
extern void ov65_0221E050(void);
extern const GraphicsModes ov65_0221FD58;
extern const BgTemplate ov65_0221FDF0;
extern const BgTemplate ov65_0221FDB8;
extern const BgTemplate ov65_0221FE0C;
extern const BgTemplate ov65_0221FE28;
extern const BgTemplate ov65_0221FD80;
extern const BgTemplate ov65_0221FD9C;
extern const BgTemplate ov65_0221FDD4;
void ov65_0221CEB8(BgConfig *bgConfig);
void ov65_0221D0EC(void *work, OverlayManager *manager);

void ov65_0221CEB8(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;
    BgTemplate bg4;
    BgTemplate bg5;
    BgTemplate bg6;
    int i;

    modes = ov65_0221FD58;
    SetBothScreensModesAndDisable(&modes);

    bg0 = ov65_0221FDF0;
    InitBgFromTemplate(bgConfig, 0, &bg0, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 0);
    bg1 = ov65_0221FDB8;
    InitBgFromTemplate(bgConfig, 1, &bg1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 1);
    bg2 = ov65_0221FE0C;
    InitBgFromTemplate(bgConfig, 2, &bg2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 2);
    bg3 = ov65_0221FE28;
    InitBgFromTemplate(bgConfig, 3, &bg3, 0);
    bg4 = ov65_0221FD80;
    InitBgFromTemplate(bgConfig, 4, &bg4, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 4);
    bg5 = ov65_0221FD9C;
    InitBgFromTemplate(bgConfig, 5, &bg5, 0);
    bg6 = ov65_0221FDD4;
    InitBgFromTemplate(bgConfig, 6, &bg6, 0);

    BG_ClearCharDataRange(0, 0x20, 0, (enum HeapID)0x1a);
    BG_ClearCharDataRange(1, 0x20, 0, (enum HeapID)0x1a);
    BG_ClearCharDataRange(4, 0x20, 0, (enum HeapID)0x1a);
    for (i = 0; i < 4; i++) {
        BgSetPosTextAndCommit(bgConfig, (u8)i, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, (u8)i, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(bgConfig, (u8)(i + 4), BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, (u8)(i + 4), BG_POS_OP_SET_Y, 0);
    }
    ToggleBgLayer(0, FALSE);
    ToggleBgLayer(1, FALSE);
    ToggleBgLayer(2, FALSE);
    ToggleBgLayer(3, FALSE);
    ToggleBgLayer(4, FALSE);
    ToggleBgLayer(5, FALSE);
    ToggleBgLayer(6, FALSE);

    reg_GX_DISPCNT &= 0xFFFF1FFF;
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(0xF0, 0, 0, 0x10);
    *(vu16 *)0x04000304 &= 0xFFFF7FFF;
}

void ov65_0221D0EC(void *work, OverlayManager *manager) {
    Ov65Args *args = OverlayManager_GetArgs(manager);
    u32 size;

    PTR_AT(work, 8) = args;
    U32_AT(work, 0x674) = 4;
    U32_AT(work, 0x50) = 0;
    U32_AT(work, 0x670) = 0;
    U32_AT(work, 0x178) = 6;
    U32_AT(work, 0x17c) = 6;
    U32_AT(work, 0x94) = 0;
    U32_AT(work, 0x98) = 6;
    U32_AT(work, 0x14c) = 0;
    U32_AT(work, 0x54) = 0;
    U32_AT(work, 0x58) = 0;
    PTR_AT(work, 0x2220) = ov65_0221E050;
    U32_AT(work, 0x6c) = 0;
    U32_AT(work, 0x70) = 0;
    U32_AT(work, 0x68) = 0;
    U32_AT(work, 0x211c) = 0;
    U32_AT(work, 0x36ac) = -1;
    U32_AT(work, 0x36b0) = -1;
    U32_AT(work, 0x22cc) = 0;
    U32_AT(work, 0x36b8) = 0;
    U32_AT(work, 0x36bc) = 0;
    PTR_AT(work, 0x2224) = args->party;
    PTR_AT(work, 0x2230) = args->saveData;
    PTR_AT(work, 4) = args->unk10;

    size = SaveArray_Party_sizeof();
    PTR_AT(work, 0x2228) = Heap_Alloc((enum HeapID)0x1a, size);
    Party_InitWithMaxSize(PTR_AT(work, 0x2228), 6);
    size = SaveArray_Party_sizeof();
    memset(PTR_AT(work, 0x2228), 0xff, size);
    PTR_AT(work, 0x198) = PlayerProfile_GetPlayerName_NewString(args->playerProfile, (enum HeapID)0x1a);
    PTR_AT(work, 0x19c) = NewString_ReadMsgData(PTR_AT(work, 0x190), 0x2a);
    sub_02039F68(args->unk14);
}
