#pragma force_active on
#include "global.h"

#include "battle/battle_022378C0.h"
#include "battle/battle_input.h"
#include "battle/battle_setup.h"
#include "battle/battle_system.h"
#include "bag_cursor.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "message_printer.h"
#include "pokepic_animation_internal.h"
#include "render_window.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_020163E0.h"
#include "unk_020210A0.h"
#include "unk_02026E84.h"
#include "vram_transfer_manager.h"

/* These objects remain owned by their original assembly producer. */
extern const OamManagerParam ov12_0226C060;
extern const OamCharTransferParam ov12_0226C018;
extern SpriteResourceCountsListUnion ov12_0226C02C;
extern const u32 ov12_0226C1C8[23][3];

/* Exact canonical tag of the selected existing provider; no local layout. */
typedef struct Ov07Root Ov07Root;
extern Ov07Root *ov07_0221BEDC(enum HeapID heapId);

/* Preview of the independently approved scoped return repair. */
GF3DVramMan *ov12_02239644(void);
void ov12_022396F0(void);

/* Preview of the finite genuine SysTaskFunc provider repairs. */
void ov12_02239810(SysTask *task, void *context);
void ov12_02239854(SysTask *task, void *context);
void ov12_0223998C(SysTask *task, void *context);

#pragma force_active on
void ov12_02237F18(OverlayManager *manager) {
    BattleSystem *battleSystem = OverlayManager_GetData(manager);
    BattleSetup *setup = OverlayManager_GetArgs(manager);
    NARC *archive7;
    NARC *archive8;
    int paletteSize;
    int i;
    int colorIndex;
    u32 gender;

    paletteSize = sub_02026E8C();
    MIi_CpuClear16(0, GetMainBgPlttAddr(), paletteSize);
    paletteSize = sub_02026E9C();
    MIi_CpuClear16(0, GetSubBgPlttAddr(), paletteSize);
    battleSystem->unk0 = (void *)ov12_02239644();
    HBlankInterruptDisable();
    FontID_Alloc(4, HEAP_ID_BATTLE);
    battleSystem->hpNumPrinter = (void *)MessagePrinter_New(14, 2, 15, HEAP_ID_BATTLE);
    battleSystem->levelNumPrinter = battleSystem->hpNumPrinter;
    battleSystem->palette = PaletteData_Init(HEAP_ID_BATTLE);
    PaletteData_SetAutoTransparent(battleSystem->palette, TRUE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_MAIN_BG, 512, HEAP_ID_BATTLE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_SUB_BG, 512, HEAP_ID_BATTLE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_MAIN_OBJ, 448, HEAP_ID_BATTLE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_SUB_OBJ, 512, HEAP_ID_BATTLE);
    battleSystem->bgConfig = BgConfig_Alloc(HEAP_ID_BATTLE);
    battleSystem->window = AllocWindows(HEAP_ID_BATTLE, 3);
    for (i = 0; i < 4; i++) {
        battleSystem->unk1D0[i].unk0 = Heap_Alloc(HEAP_ID_BATTLE, 3200);
    }
    GF_CreateVramTransferManager(64, HEAP_ID_BATTLE);

    archive7 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
    archive8 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
    gender = BattleSystem_GetTrainerGender(battleSystem, ov12_0223BFC0(battleSystem));
    battleSystem->battleInput = BattleInput_NewInit(archive7, archive8, battleSystem, gender, battleSystem->unk1C0);
    NARC_Delete(archive7);
    NARC_Delete(archive8);

    ov12_022387AC(battleSystem, battleSystem->bgConfig);
    AddWindowParameterized(battleSystem->bgConfig, battleSystem->window, 1, 2, 19, 27, 4, 11, 31);
    FillWindowPixelBuffer(battleSystem->window, 255);
    DrawFrameAndWindow2(battleSystem->window, FALSE, 1, 10);
    battleSystem->spriteRenderer = SpriteSystem_Alloc(HEAP_ID_BATTLE);
    SpriteSystem_Init(battleSystem->spriteRenderer, &ov12_0226C060, &ov12_0226C018, 32);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, (GXOBJVRamModeChar)0x100010);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    battleSystem->gfxHandler = SpriteManager_New(battleSystem->spriteRenderer);
    SpriteSystem_InitSprites(battleSystem->spriteRenderer, battleSystem->gfxHandler, 128);
    SpriteSystem_InitManagerWithCapacities(battleSystem->spriteRenderer, battleSystem->gfxHandler, &ov12_0226C02C);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(battleSystem->spriteRenderer), 0, 0x110000);
    BattleInput_LoadDefaultResources(battleSystem->battleInput);

    archive7 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
    archive8 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
    BattleInput_ChangeMenu(archive7, archive8, battleSystem->battleInput, 0, 1, NULL);
    BattleInput_LoadBallGaugeResources(archive8, battleSystem->battleInput);
    NARC_Delete(archive7);
    NARC_Delete(archive8);

    battleSystem->pokepicManager = PokepicManager_Create(HEAP_ID_BATTLE);
    PokepicManager_SetPlttBaseAddrAndSize(battleSystem->pokepicManager, 0, 192);
    BattleSystem_HpBar_Init(battleSystem);
    ov12_022396F0();
    battleSystem->unk8C = (void *)ov07_0221BEDC(HEAP_ID_BATTLE);
    ov12_022389B8(battleSystem);
    sub_020210BC();
    sub_02021148(4);
    battleSystem->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 197, HEAP_ID_BATTLE);
    battleSystem->unk10 = (void *)NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 3, HEAP_ID_BATTLE);
    battleSystem->msgFormat = MessageFormat_New(HEAP_ID_BATTLE);
    battleSystem->msgBuffer = String_New(320, HEAP_ID_BATTLE);
    MIi_CpuCopy16(PaletteData_GetUnfadedBuf(battleSystem->palette, PLTTBUF_MAIN_BG), battleSystem->unk2228, 224);
    MIi_CpuCopy16(PaletteData_GetUnfadedBuf(battleSystem->palette, PLTTBUF_MAIN_OBJ), battleSystem->unk2308, 224);
    colorIndex = ov12_0223B52C(battleSystem);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, (u16)ov12_0226C1C8[battleSystem->backgroundId][colorIndex], 0, 112);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, (u16)ov12_0226C1C8[battleSystem->backgroundId][colorIndex], 192, 256);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_OBJ, PLTTSEL_BOTH, (u16)ov12_0226C1C8[battleSystem->backgroundId][colorIndex], 0, 223);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_BG, PLTTSEL_TRANSPARENT, 0, 160, 192);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_SUB_BG, PLTTSEL_TRANSPARENT, 0, 0, 255);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_SUB_OBJ, PLTTSEL_TRANSPARENT, 65535, 0, 255);
    battleSystem->msgIcon = sub_020163E0(battleSystem->palette, (PMLCDTarget)0, 11, HEAP_ID_BATTLE);
    sub_0201649C(battleSystem->msgIcon, 1);
    battleSystem->unk1C = (u32)SysTask_CreateOnMainQueue(ov12_02239810, battleSystem, 60000);
    battleSystem->unk20 = (u32)SysTask_CreateOnMainQueue(ov12_02239854, battleSystem, 50000);
    battleSystem->unk24 = (u32)SysTask_CreateOnVBlankQueue(ov12_0223998C, battleSystem, 1200);
    battleSystem->unk2438 = -51;
    ov12_0223A620(battleSystem);
    BagCursor_Battle_Init(BattleSystem_GetBagCursor(battleSystem));
    battleSystem->unk1C8 = sub_02016EDC(HEAP_ID_BATTLE, 4, 0);
    battleSystem->unk1CC = (void *)sub_02020654(4, HEAP_ID_BATTLE);
    if (battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING) {
        for (i = 0; i < 4; i++) {
            battleSystem->chatotVoiceParam[i] = setup->unk1BC[i];
        }
    }
}
