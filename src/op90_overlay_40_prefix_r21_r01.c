#pragma force_active on
#include "global.h"
#include "system.h"
#include "gf_gfx_planes.h"
#include "vram_transfer_manager.h"
#include "filesystem.h"
#include "bg_window.h"
#include "palette.h"
#include "gf_3d_vramman.h"
#include "pokepic.h"
#include "touchpad.h"
#include "render_text.h"
#include "unk_02009D48.h"
#include "unk_0203A3B0.h"
#include "heap.h"
#include "font_sprite.h"
#include "msgdata.h"

typedef struct {
    int count;
    enum HeapID heapId;
    int resBase;
    int unk1C;
    fx32 yOffset;
} Ov40MotionArgs;

typedef struct {
    int unk0;
    u8 unk4[0x14 - 0x4];
    NARC *narc;
    void *spriteSys;
    void *spriteMan;
    u8 unk20[4];
    BgConfig *bgConfig;
    PaletteData *plttData;
    u8 unk2C[0x44 - 0x2C];
    int unk44;
    MsgData *msgData1;
    MsgData *msgData2;
    void *fontSystem;
    u8 unk54[0x5C - 0x54];
    u8 unk5C;
    u8 unk5D[3];
    GF3DVramMan *vramMan;
    PokepicManager *pokepicMan;
    u8 unk68[0x6F0 - 0x68];
    void *motion[2];
} Ov40Work;

void *sub_02087284(Ov40MotionArgs args, void *spriteSys, void *spriteMan, PaletteData *plttData);
BOOL sub_02087878(void *p, int a1);
BOOL sub_020878B0(void *p, int a1);
BOOL sub_020879E0(void *p, int a1);
void sub_0203A948(int a0, enum HeapID a1);
void *sub_0203A4AC(enum HeapID heapId);
void sub_02088030(Ov40Work *work);
void ov40_0222BA90(BgConfig *bgConfig);
void ov40_0222BC68(Ov40Work *work);
void ov40_0222C360(Ov40Work *work);
void ov40_0222BD04(void *work);
void ov40_0223D544(Ov40Work *work);
void ov40_0222FCCC(Ov40Work *work);
void ov40_0222C4F8(Ov40Work *work);
void ov40_0222FBF8(Ov40Work *work);
void ov40_0222B6E0(Ov40Work *work);

// @fn ov40_0222B6E0
void ov40_0222B6E0(Ov40Work *work) {
    Ov40MotionArgs args;
    NNSG2dPaletteData *pltt;
    void *file;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GF_CreateVramTransferManager(4, HEAP_ID_109);
    work->narc = NARC_New(NARC_a_1_9_1, HEAP_ID_109);
    work->bgConfig = BgConfig_Alloc(HEAP_ID_109);
    work->plttData = PaletteData_Init(HEAP_ID_109);
    work->vramMan = GF_3DVramMan_Create(HEAP_ID_109, 0, 1, 0, 4, NULL);
    work->pokepicMan = PokepicManager_Create(HEAP_ID_109);
    NNS_G2dSetupSoftwareSpriteCamera();
    PaletteData_SetAutoTransparent(work->plttData, TRUE);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_109);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_109);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_109);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_109);
    ov40_0222BA90(work->bgConfig);
    ov40_0222BC68(work);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    work->unk44 = 1;
    ov40_0222C360(work);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    Main_SetVBlankIntrCB(ov40_0222BD04, work);
    if (work->unk0 != 0) {
        ov40_0223D544(work);
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DSUB, GX_OBJVRAMMODE_CHAR_1D_128K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DSUB);
        WirelessStatusIcon_Create();
        sub_0203A948(1, HEAP_ID_109);
        file = sub_0203A4AC(HEAP_ID_109);
        NNS_G2dGetUnpackedPaletteData(file, &pltt);
        PaletteData_LoadPalette(work->plttData, pltt->pRawData, PLTTBUF_MAIN_OBJ, 0xE0, 0x20);
        PaletteData_LoadPalette(work->plttData, pltt->pRawData, PLTTBUF_SUB_OBJ, 0xE0, 0x20);
        Heap_Free(file);
    }
    args.count = 12;
    args.heapId = HEAP_ID_109;
    args.resBase = 1;
    args.unk1C = work->unk0;
    args.yOffset = FX32_CONST(256);
    work->motion[0] = sub_02087284(args, work->spriteSys, work->spriteMan, work->plttData);
    sub_02087878(work->motion[0], work->unk5C);
    sub_020878B0(work->motion[0], 0);
    sub_020879E0(work->motion[0], 0);
    args.resBase = 2;
    args.yOffset = FX32_CONST(256);
    work->motion[1] = sub_02087284(args, work->spriteSys, work->spriteMan, work->plttData);
    sub_02087878(work->motion[1], work->unk5C);
    sub_020878B0(work->motion[1], 0);
    sub_020879E0(work->motion[1], 0);
    work->fontSystem = FontSystem_NewInit(20, HEAP_ID_109);
    work->msgData1 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 13, HEAP_ID_109);
    work->msgData2 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 26, HEAP_ID_109);
    ov40_0222FCCC(work);
    sub_02088030(work);
    ov40_0222C4F8(work);
    ov40_0222FBF8(work);
}
