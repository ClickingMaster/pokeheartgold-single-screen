#include "global.h"

#include "bg_window.h"
#include "easy_chat_app_internal.h"
#include "filesystem.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_0200FA24.h"
#include "unk_0203A3B0.h"

typedef struct EasyChatSentenceView EasyChatSentenceView;
typedef struct EasyChatMenuView EasyChatMenuView;
typedef struct EasyChatCategoryView EasyChatCategoryView;
typedef struct EasyChatWordListView EasyChatWordListView;
typedef struct EasyChatPageButtonsView EasyChatPageButtonsView;

typedef struct EasyChatButtonText {
    TextOBJ *textObj;             // 0x00
    UnkStruct_02021AC8 charAlloc; // 0x04
} EasyChatButtonText;             // size: 0x10

struct EasyChatAppView {
    SysTask *mainTask;                         // 0x000
    SysTask *vblankTask;                       // 0x004
    SysTask *commandTasks[4];                  // 0x008
    EasyChatApp *app;                          // 0x018
    ECMenu *menu;                              // 0x01C
    BgConfig *bgConfig;                        // 0x020
    SpriteList *spriteList;                    // 0x024
    G2dRenderer renderer;                      // 0x028
    NNSG2dImageProxy imageProxies[2];          // 0x150
    NNSG2dImagePaletteProxy paletteProxies[2]; // 0x198
    NNSG2dCellDataBank *cellBanks[2];          // 0x1C0
    NNSG2dAnimBankData *animBanks[2];          // 0x1C8
    void *cellFiles[2];                        // 0x1D0
    void *animFiles[2];                        // 0x1D8
    EasyChatSentenceView *sentence;            // 0x1E0
    EasyChatMenuView *menuView;                // 0x1E4
    EasyChatCategoryView *categories;          // 0x1E8
    EasyChatWordListView *wordList;            // 0x1EC
    EasyChatPageButtonsView *pageButtons;      // 0x1F0
    u8 onMenu;                                 // 0x1F4
    MenuInputState *menuInputState;            // 0x1F8
    FontSystem *fontSystem;                    // 0x1FC
    TextOBJLayout *textLayout;                 // 0x200
    Window buttonWindow;                       // 0x204
    EasyChatButtonText buttonTexts[2];         // 0x214
}; // size: 0x234

typedef struct EasyChatViewCommand {
    EasyChatAppView *view; // 0x00
    EasyChatApp *app;      // 0x04
    ECMenu *menu;          // 0x08
    u32 command;           // 0x0C
    volatile u32 slot;     // 0x10
    u32 state;             // 0x14
} EasyChatViewCommand;     // size: 0x18

typedef void (*EasyChatViewCommandFunc)(SysTask *task, EasyChatViewCommand *command);

extern const ObjCharTransferTemplate ov102_021EC698;
extern const GraphicsModes ov102_021EC6A8;
extern const BgTemplate ov102_021EC6B8;
extern const BgTemplate ov102_021EC6D4;
extern const BgTemplate ov102_021EC6F0;
extern const BgTemplate ov102_021EC70C;
extern const BgTemplate ov102_021EC728;
extern const BgTemplate ov102_021EC744;
extern const GraphicsBanks ov102_021EC760;
extern const EasyChatViewCommandFunc ov102_021EC788[];

EasyChatSentenceView *ov102_021EA314(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void ov102_021EA380(EasyChatSentenceView *sentence);
void ov102_021EA41C(EasyChatSentenceView *sentence, NARC *narc);
void ov102_021EA71C(EasyChatSentenceView *sentence, int a1);
BOOL ov102_021EA754(EasyChatSentenceView *sentence);
void ov102_021EAA3C(EasyChatSentenceView *sentence);
void ov102_021EAB10(EasyChatSentenceView *sentence, TouchscreenHitbox *hitbox, u8 slot);
u32 ov102_021EADF0(EasyChatSentenceView *sentence);
void ov102_021EADF8(EasyChatSentenceView *sentence, int a1);
void ov102_021EAE40(EasyChatSentenceView *sentence, int a1);
void ov102_021EAF44(EasyChatSentenceView *sentence);
void ov102_021EAF50(EasyChatSentenceView *sentence);
void ov102_021EAF5C(EasyChatSentenceView *sentence, int a1);
void ov102_021EAF7C(EasyChatSentenceView *sentence);
void ov102_021EAFAC(EasyChatSentenceView *sentence);
Sprite *ov102_021EAFEC(EasyChatSentenceView *sentence);
void ov102_021EAFF0(EasyChatSentenceView *sentence, u16 slot);
void ov102_021EB0C0(EasyChatSentenceView *sentence, u32 a1);
int ov102_021EB130(EasyChatSentenceView *sentence);
void ov102_021EB170(EasyChatSentenceView *sentence, u8 a1);
EasyChatMenuView *ov102_021EB1A4(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void ov102_021EB1E0(EasyChatMenuView *menuView);
void ov102_021EB1F8(EasyChatMenuView *menuView, NARC *narc);
void ov102_021EB2EC(EasyChatMenuView *menuView, int a1, int a2);
void ov102_021EB50C(EasyChatMenuView *menuView);
void ov102_021EB518(EasyChatMenuView *menuView);
void ov102_021EB524(EasyChatMenuView *menuView, int a1);
void ov102_021EB530(EasyChatMenuView *menuView, u16 cursor);
void ov102_021EB624(EasyChatMenuView *menuView);
BOOL ov102_021EB654(EasyChatMenuView *menuView);
void ov102_021EB664(EasyChatMenuView *menuView);
void ov102_021EB67C(EasyChatMenuView *menuView);
EasyChatCategoryView *ov102_021EB694(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void ov102_021EB6C8(EasyChatCategoryView *categories);
void ov102_021EB6E0(EasyChatCategoryView *categories, NARC *narc);
void ov102_021EB98C(EasyChatCategoryView *categories, int a1);
void ov102_021EB9C0(EasyChatCategoryView *categories, u16 category);
void ov102_021EBA44(EasyChatCategoryView *categories);
BOOL ov102_021EBA5C(EasyChatCategoryView *categories);
void ov102_021EBAAC(EasyChatCategoryView *categories);
BOOL ov102_021EBAD0(EasyChatCategoryView *categories);
void ov102_021EBB1C(EasyChatCategoryView *categories);
void ov102_021EBB2C(EasyChatCategoryView *categories);
BOOL ov102_021EBB44(EasyChatCategoryView *categories);
void ov102_021EBB50(EasyChatCategoryView *categories);
BOOL ov102_021EBB6C(EasyChatCategoryView *categories);
void ov102_021EBB88(EasyChatCategoryView *categories);
BOOL ov102_021EBBC4(EasyChatCategoryView *categories);
void ov102_021EBBD0(EasyChatCategoryView *categories);
void ov102_021EBBF4(EasyChatCategoryView *categories);
BOOL ov102_021EBC28(EasyChatCategoryView *categories);
EasyChatWordListView *ov102_021EBC34(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void ov102_021EBCBC(EasyChatWordListView *wordList);
void ov102_021EBD00(EasyChatWordListView *wordList);
void ov102_021EBDEC(EasyChatWordListView *wordList);
BOOL ov102_021EBE3C(EasyChatWordListView *wordList);
BOOL ov102_021EBE80(EasyChatWordListView *wordList);
void ov102_021EBEA4(EasyChatWordListView *wordList);
BOOL ov102_021EBEC8(EasyChatWordListView *wordList);
void ov102_021EBEF4(EasyChatWordListView *wordList, int a1);
void ov102_021EBF38(EasyChatWordListView *wordList, int cursor);
void ov102_021EBFA0(EasyChatWordListView *wordList, int scroll);
BOOL ov102_021EC05C(EasyChatWordListView *wordList);
EasyChatPageButtonsView *ov102_021EC20C(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void ov102_021EC240(EasyChatPageButtonsView *pageButtons);
void ov102_021EC248(EasyChatPageButtonsView *pageButtons, NARC *narc);
void ov102_021EC290(EasyChatPageButtonsView *pageButtons, int a1);
void ov102_021EC294(EasyChatPageButtonsView *pageButtons, int a1, int a2);

void EasyChatAppView_Delete(EasyChatAppView *view);
void EasyChatAppView_InitCharTransfer(EasyChatAppView *view);
void EasyChatAppView_FreeCharTransfer(EasyChatAppView *view);
void EasyChatAppView_CreateButtonTexts(EasyChatAppView *view);
void EasyChatAppView_DeleteButtonTexts(EasyChatAppView *view);
SysTask *EasyChatAppView_CreateVBlankTask(SysTaskFunc func, void *data, u32 priority);
void EasyChatAppView_MainTask(SysTask *task, void *data);
void EasyChatAppView_VBlankTask(SysTask *task, void *data);
void EasyChatAppView_EndCommand(EasyChatViewCommand *command);
void EasyChatAppView_Cmd_Load(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_LoadSpriteGraphics(EasyChatViewCommand *command, NARC *narc);
void EasyChatAppView_Cmd_Unload(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_InitBgs(EasyChatViewCommand *command);
void EasyChatAppView_Cmd_FadeIn(SysTask *task, EasyChatViewCommand *command);
EasyChatAppView *EasyChatAppView_Create(EasyChatApp *app, ECMenu *menu);
void EasyChatAppView_StartCommand(EasyChatAppView *view, u32 command);
BOOL EasyChatAppView_AreCommandsDone(EasyChatAppView *view);
BOOL EasyChatAppView_IsCommandDone(EasyChatAppView *view, u32 command);
void EasyChatAppView_Cmd_RedrawSentence(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_Nop(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ShowCategoryCursor(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ShowWordListCursor(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_FocusMenu(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_FocusSentence(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_OpenCategories(SysTask *task, EasyChatViewCommand *command);
void ov102_021E9A24(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_SwitchListMode(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_CloseCategories(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_OpenWordList(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_WordListToCategories(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_CloseWordList(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_WordListToMenu(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_MoveMenuCursor(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_SetMenuCursor(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_SetCategoryCursor(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_SetWordListCursor(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ScrollWordList(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_HideMessage(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ConfirmFinish(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ConfirmQuit(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ShowIncompleteMessage(SysTask *task, EasyChatViewCommand *command);
void ov102_021EA144(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_CloseYesNo(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_PressPageUp(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_PressPageDown(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ReleasePageUp(SysTask *task, EasyChatViewCommand *command);
void EasyChatAppView_Cmd_ReleasePageDown(SysTask *task, EasyChatViewCommand *command);
Sprite *EasyChatAppView_GetSliderSprite(EasyChatAppView *view);
u32 EasyChatAppView_GetNumWordSlots(EasyChatAppView *view);
void EasyChatAppView_GetWordSlotHitbox(EasyChatAppView *view, TouchscreenHitbox *hitbox, u8 slot);
int EasyChatAppView_GetYesNoResult(EasyChatAppView *view);
BgConfig *EasyChatAppView_GetBgConfig(EasyChatAppView *view);
SpriteList *EasyChatAppView_GetSpriteList(EasyChatAppView *view);
void EasyChatAppView_InitSpriteHeader(EasyChatAppView *view, SpriteResourcesHeader *header, u32 index, u8 priority);
Sprite *EasyChatAppView_CreateSprite(EasyChatAppView *view, SpriteResourcesHeader *header, s32 x, s32 y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen);

EasyChatAppView *EasyChatAppView_Create(EasyChatApp *app, ECMenu *menu) {
    EasyChatAppView *view = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatAppView));
    int i;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    view->app = app;
    view->menu = menu;
    view->menuInputState = EasyChatApp_GetMenuInputStatePtr(app);
    EasyChatAppView_InitCharTransfer(view);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(1, 0x7F, 0, 0x20, 0, 0x7F, 0, 0x20, HEAP_ID_35);
    view->spriteList = G2dRenderer_Init(0x80, &view->renderer, HEAP_ID_35);
    view->bgConfig = BgConfig_Alloc(HEAP_ID_35);
    view->mainTask = SysTask_CreateOnMainQueue(EasyChatAppView_MainTask, view, 2);
    view->vblankTask = EasyChatAppView_CreateVBlankTask(EasyChatAppView_VBlankTask, view, 1);
    for (i = 0; i < 4; i++) {
        view->commandTasks[i] = NULL;
    }
    FontID_Alloc(4, HEAP_ID_35);
    return view;
}

void EasyChatAppView_Delete(EasyChatAppView *view) {
    int i;

    if (view == NULL) {
        return;
    }
    Main_SetVBlankIntrCB(NULL, NULL);
    FontID_Release(4);
    for (i = 0; i < 4; i++) {
        if (view->commandTasks[i] != NULL) {
            SysTask_Destroy(view->commandTasks[i]);
        }
    }
    SysTask_Destroy(view->mainTask);
    SysTask_Destroy(view->vblankTask);
    OamManager_Free();
    SpriteList_Delete(view->spriteList);
    EasyChatAppView_FreeCharTransfer(view);
    Heap_Free(view->bgConfig);
    Heap_Free(view);
}

void EasyChatAppView_InitCharTransfer(EasyChatAppView *view) {
#pragma unused(view)
    ObjCharTransferTemplate template = ov102_021EC698;

    ObjCharTransfer_Init(&template);
    ObjCharTransfer_ClearBuffers();
}

void EasyChatAppView_FreeCharTransfer(EasyChatAppView *view) {
#pragma unused(view)
    ObjCharTransfer_Destroy();
}

#pragma force_active on

void EasyChatAppView_CreateButtonTexts(EasyChatAppView *view) {
    TextOBJTemplate template;
    int i;
    u32 charSize;
    String *string;
    MsgData *msgData;
    String *line;
    u32 numLines;

    view->fontSystem = FontSystem_NewInit(2, HEAP_ID_35);
    InitWindow(&view->buttonWindow);
    AddTextWindowTopLeftCorner(view->bgConfig, &view->buttonWindow, 9, 4, 0, 0);
    FillWindowPixelBufferText_AssumeTileSize32(&view->buttonWindow, 0);
    view->textLayout = TextOBJLayout_New(&view->buttonWindow, HEAP_ID_35);
    charSize = TextOBJLayout_GetCharSize(view->textLayout, NNS_G2D_VRAM_TYPE_2DMAIN);
    template.fontSystem = view->fontSystem;
    template.window = &view->buttonWindow;
    template.spriteList = view->spriteList;
    template.plttResourceProxy = &view->paletteProxies[0];
    template.unk_20 = 3;
    template.unk_24 = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.x = 0;
    template.y = 0;
    template.heapID = HEAP_ID_35;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 283, HEAP_ID_35);
    string = String_New(21, HEAP_ID_35);
    line = String_New(21, HEAP_ID_35);
    for (i = 0; i < 2; i++) {
        u32 y;
        u32 j;

        sub_02021AC8(charSize, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &view->buttonTexts[i].charAlloc);
        template.offset = view->buttonTexts[i].charAlloc.offset;
        template.sprite = NULL;
        ReadMsgDataIntoString(msgData, 11 + i, string);
        numLines = String_CountLines(string);
        FillWindowPixelBufferText_AssumeTileSize32(&view->buttonWindow, 0);
        y = (32 - (numLines << 4)) >> 1;
        for (j = 0; j < numLines; j++, y += 16) {
            String_GetLineN(line, string, j);
            AddTextPrinterParameterizedWithColor(&view->buttonWindow, 4, line, FontID_String_GetCenterAlignmentX(4, line, 0, 72), y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 15, 0), NULL);
        }
        view->buttonTexts[i].textObj = TextOBJ_Create(&template, view->textLayout);
        TextOBJ_SetSpritesDrawFlag(view->buttonTexts[i].textObj, TRUE);
        TextOBJ_SetPaletteNum(view->buttonTexts[i].textObj, 0);
        TextOBJ_SetPosition(view->buttonTexts[i].textObj, 24 + i * 136, 6);
    }
    String_Delete(line);
    String_Delete(string);
    DestroyMsgData(msgData);
}

#pragma force_active reset

void EasyChatAppView_DeleteButtonTexts(EasyChatAppView *view) {
    int i;

    for (i = 0; i < 2; i++) {
        TextOBJ_Destroy(view->buttonTexts[i].textObj);
        sub_02021B5C(&view->buttonTexts[i].charAlloc);
        view->buttonTexts[i].textObj = NULL;
    }
    TextOBJLayout_Delete(view->textLayout);
    RemoveWindow(&view->buttonWindow);
    FontSystem_Delete(view->fontSystem);
}

SysTask *EasyChatAppView_CreateVBlankTask(SysTaskFunc func, void *data, u32 priority) {
    return SysTask_CreateOnVBlankQueue(func, data, priority);
}

void EasyChatAppView_MainTask(SysTask *task, void *data) {
}

void EasyChatAppView_VBlankTask(SysTask *task, void *data) {
    EasyChatAppView *view = data;
    volatile u32 *dtcm = (volatile u32 *)0x027E0000; // OS_SetIrqCheckFlag with the DTCM address as a constant

    DoScheduledBgGpuUpdates(view->bgConfig);
    SpriteList_RenderAndAnimateSprites(view->spriteList);
    OamManager_ApplyAndResetBuffers();
    dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
}

#pragma force_active on

void EasyChatAppView_StartCommand(EasyChatAppView *view, u32 command) {
    if (command < 32) {
        EasyChatViewCommand *cmd = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatViewCommand));

        if (cmd != NULL) {
            int i;

            cmd->state = 0;
            cmd->command = command;
            cmd->view = view;
            cmd->app = view->app;
            cmd->menu = view->menu;
            for (i = 0; i < 4; i++) {
                if (view->commandTasks[i] == NULL) {
                    cmd->slot = i;
                    view->commandTasks[i] = SysTask_CreateOnMainQueue((SysTaskFunc)ov102_021EC788[command], cmd, 1);
                    return;
                }
            }
        }
    } else {
        GF_AssertFail();
    }
}

#pragma force_active reset

BOOL EasyChatAppView_AreCommandsDone(EasyChatAppView *view) {
    int i;

    for (i = 0; i < 4; i++) {
        if (view->commandTasks[i] != NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL EasyChatAppView_IsCommandDone(EasyChatAppView *view, u32 command) {
    int i;
    EasyChatViewCommand *running;

    for (i = 0; i < 4; i++) {
        if (view->commandTasks[i] != NULL) {
            running = SysTask_GetData(view->commandTasks[i]);
            if (running->command == command) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void EasyChatAppView_EndCommand(EasyChatViewCommand *command) {
    SysTask_Destroy(command->view->commandTasks[command->slot]);
    command->view->commandTasks[command->slot] = NULL;
    Heap_Free(command);
}

void EasyChatAppView_Cmd_Load(SysTask *task, EasyChatViewCommand *command) {
    NARC *narc;

    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= ~0x1F00;
    reg_GXS_DB_DISPCNT &= ~0x1F00;
    reg_GX_DISPCNT = (reg_GX_DISPCNT & ~0x300010) | 0x100010;
    reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~0x300010) | 0x10;
    narc = NARC_New(NARC_a_0_7_6, HEAP_ID_35);
    EasyChatAppView_InitBgs(command);
    EasyChatAppView_LoadSpriteGraphics(command, narc);
    EasyChatAppView_CreateButtonTexts(command->view);
    command->view->sentence = ov102_021EA314(command->view, command->app, command->menu);
    ov102_021EA41C(command->view->sentence, narc);
    command->view->menuView = ov102_021EB1A4(command->view, command->app, command->menu);
    ov102_021EB1F8(command->view->menuView, narc);
    command->view->categories = ov102_021EB694(command->view, command->app, command->menu);
    ov102_021EB6E0(command->view->categories, narc);
    command->view->wordList = ov102_021EBC34(command->view, command->app, command->menu);
    ov102_021EBD00(command->view->wordList);
    command->view->pageButtons = ov102_021EC20C(command->view, command->app, command->menu);
    ov102_021EC248(command->view->pageButtons, narc);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GX_DispOn();
    NARC_Delete(narc);
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_LoadSpriteGraphics(EasyChatViewCommand *command, NARC *narc) {
    EasyChatAppView *view = command->view;
    int i;

    for (i = 0; i < 2; i++) {
        NNS_G2dInitImagePaletteProxy(&view->paletteProxies[i]);
        NNS_G2dInitImageProxy(&view->imageProxies[i]);
        GfGfxLoader_LoadImageMappingFromOpenNarc(narc, 9, TRUE, GF_BG_LYR_MAIN_0, 0, (NNS_G2D_VRAM_TYPE)(u32)(i + 1), 0, HEAP_ID_35, &view->imageProxies[i]);
        GfGfxLoader_PartiallyLoadPaletteFromOpenNarc(narc, 10, (NNS_G2D_VRAM_TYPE)(int)(i + 1), 0, HEAP_ID_35, &view->paletteProxies[i]);
        view->cellFiles[i] = GfGfxLoader_GetCellBankFromOpenNarc(narc, 7, TRUE, &view->cellBanks[i], HEAP_ID_35);
        view->animFiles[i] = GfGfxLoader_GetAnimBankFromOpenNarc(narc, 8, TRUE, &view->animBanks[i], HEAP_ID_35);
    }
    sub_0203A964();
}

void EasyChatAppView_Cmd_Unload(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;
    int i;

    switch (command->state) {
    case 0:
        BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 5, 1, HEAP_ID_35);
        command->state++;
        return;
    case 1:
        if (!IsPaletteFadeFinished()) {
            return;
        }
        ov102_021EA380(command->view->sentence);
        ov102_021EB1E0(command->view->menuView);
        ov102_021EB6C8(command->view->categories);
        ov102_021EBCBC(command->view->wordList);
        ov102_021EC240(command->view->pageButtons);
        EasyChatAppView_DeleteButtonTexts(command->view);
        for (i = 0; i < 2; i++) {
            if (view->cellFiles[i] != NULL) {
                Heap_Free(view->cellFiles[i]);
            }
            if (view->animFiles[i] != NULL) {
                Heap_Free(view->animFiles[i]);
            }
        }
        FontID_Release(2);
        FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_0);
        FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_1);
        FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_2);
        FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_MAIN_3);
        FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_SUB_0);
        FreeBgTilemapBuffer(view->bgConfig, GF_BG_LYR_SUB_1);
        reg_GX_POWCNT |= (u32)&reg_GX_POWCNT >> 11;
        EasyChatAppView_EndCommand(command);
        return;
    }
}

void EasyChatAppView_InitBgs(EasyChatViewCommand *command) {
    int i;
    EasyChatAppView *view = command->view;

    reg_GX_POWCNT &= ~0x8000;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GfGfx_SetBanks(&ov102_021EC760);
    SetBothScreensModesAndDisable(&ov102_021EC6A8);
    InitBgFromTemplate(view->bgConfig, GF_BG_LYR_MAIN_0, &ov102_021EC728, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(view->bgConfig, GF_BG_LYR_MAIN_1, &ov102_021EC6D4, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(view->bgConfig, GF_BG_LYR_MAIN_2, &ov102_021EC6B8, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(view->bgConfig, GF_BG_LYR_MAIN_3, &ov102_021EC70C, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(view->bgConfig, GF_BG_LYR_SUB_0, &ov102_021EC744, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(view->bgConfig, GF_BG_LYR_SUB_1, &ov102_021EC6F0, GF_BG_TYPE_TEXT);
    for (i = 0; i < 6; i++) {
        BgClearTilemapBufferAndCommit(view->bgConfig, i);
    }
    FontID_Alloc(2, HEAP_ID_35);
}

void EasyChatAppView_Cmd_FadeIn(SysTask *task, EasyChatViewCommand *command) {
    switch (command->state) {
    case 0:
        BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 5, 1, HEAP_ID_35);
        command->state++;
        return;
    case 1:
        if (IsPaletteFadeFinished()) {
            command->state++;
        }
        return;
    default:
        EasyChatAppView_EndCommand(command);
        return;
    }
}

void EasyChatAppView_Cmd_RedrawSentence(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EAA3C(view->sentence);
    ov102_021EAFF0(view->sentence, EasyChatApp_GetWordSlot(command->app));
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_Nop(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_ShowCategoryCursor(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EB98C(command->view->categories, 1);
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_ShowWordListCursor(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EBEF4(command->view->wordList, 1);
    EasyChatAppView_EndCommand(command);
}

#pragma force_active on

void EasyChatAppView_Cmd_FocusMenu(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    view->onMenu = TRUE;
    ov102_021EAF5C(view->sentence, 0);
    ov102_021EAF7C(view->sentence);
    ov102_021EB524(view->menuView, 1);
    ov102_021EB530(view->menuView, EasyChatApp_GetMenuCursor(view->app));
    EasyChatAppView_EndCommand(command);
}

#pragma force_active reset

void EasyChatAppView_Cmd_FocusSentence(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    view->onMenu = FALSE;
    ov102_021EB524(view->menuView, 0);
    ov102_021EAFAC(view->sentence);
    ov102_021EAF5C(view->sentence, 1);
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_OpenCategories(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EAF44(view->sentence);
        ov102_021EAF7C(view->sentence);
        ov102_021EADF8(view->sentence, 1);
        ov102_021EAE40(view->sentence, 0);
        ov102_021EB664(view->menuView);
        ov102_021EBA44(view->categories);
        ov102_021EA71C(view->sentence, 0);
        command->state++;
        break;
    case 1: {
        BOOL categoriesDone = ov102_021EBA5C(view->categories);
        BOOL sentenceDone = ov102_021EA754(view->sentence);

        if (categoriesDone && sentenceDone) {
            ov102_021EB9C0(view->categories, EasyChatApp_GetCategory(view->app));
            ov102_021EB98C(view->categories, 1);
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
    }
}

void ov102_021E9A24(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EB624(view->menuView);
        command->state++;
        break;
    case 1:
        ov102_021EBBF4(view->categories);
        command->state++;
        break;
    case 2:
        if (ov102_021EBC28(view->categories) && ov102_021EB654(view->menuView)) {
            ov102_021EBBD0(view->categories);
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
}

void EasyChatAppView_Cmd_SwitchListMode(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EB624(view->menuView);
        command->state++;
        break;
    case 1:
        ov102_021EB98C(view->categories, 0);
        ov102_021EBBF4(view->categories);
        command->state++;
        break;
    case 2:
        if (ov102_021EBC28(view->categories)) {
            ov102_021EBBD0(view->categories);
            command->state++;
        }
        break;
    case 3:
        if (ov102_021EB654(view->menuView)) {
            ov102_021EB9C0(view->categories, EasyChatApp_GetCategory(view->app));
            ov102_021EB98C(view->categories, 1);
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
}

void EasyChatAppView_Cmd_CloseCategories(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EB67C(view->menuView);
        ov102_021EB98C(view->categories, 0);
        ov102_021EBAAC(view->categories);
        ov102_021EADF8(view->sentence, 0);
        ov102_021EAE40(view->sentence, 0);
        ov102_021EA71C(view->sentence, 1);
        command->state++;
        break;
    case 1: {
        BOOL categoriesDone = ov102_021EBAD0(view->categories);
        BOOL sentenceDone = ov102_021EA754(view->sentence);

        if (categoriesDone && sentenceDone) {
            ov102_021EAFAC(view->sentence);
            ov102_021EAF50(view->sentence);
            view->onMenu = FALSE;
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
    }
}

void EasyChatAppView_Cmd_OpenWordList(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EBDEC(view->wordList);
        ov102_021EB98C(view->categories, 0);
        ov102_021EBB50(view->categories);
        command->state++;
        break;
    case 1:
        if (ov102_021EBB6C(view->categories)) {
            ov102_021EBE3C(view->wordList);
            command->state++;
        }
        break;
    case 2:
        if (ov102_021EBE80(view->wordList)) {
            ov102_021EBF38(view->wordList, EasyChatApp_GetWordListCursor(view->app));
            ov102_021EBEF4(view->wordList, 1);
            ov102_021EC290(view->pageButtons, 1);
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
}

void EasyChatAppView_Cmd_WordListToCategories(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EB624(view->menuView);
        command->state++;
        break;
    case 1:
        ov102_021EBEF4(view->wordList, 0);
        ov102_021EC290(view->pageButtons, 0);
        ov102_021EBEA4(view->wordList);
        command->state++;
        break;
    case 2:
        if (ov102_021EBEC8(view->wordList)) {
            ov102_021EBBD0(view->categories);
            command->state++;
        }
        break;
    case 3:
        ov102_021EBB88(view->categories);
        command->state++;
        break;
    case 4:
        if (ov102_021EBBC4(view->categories) && ov102_021EB654(view->menuView)) {
            ov102_021EB9C0(view->categories, EasyChatApp_GetCategory(view->app));
            ov102_021EB98C(view->categories, 1);
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
}

void EasyChatAppView_Cmd_CloseWordList(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EB67C(view->menuView);
        ov102_021EBEF4(view->wordList, 0);
        ov102_021EC290(view->pageButtons, 0);
        ov102_021EBEA4(view->wordList);
        ov102_021EADF8(view->sentence, 0);
        ov102_021EAE40(view->sentence, 0);
        ov102_021EA71C(view->sentence, 1);
        command->state++;
        break;
    case 1: {
        BOOL wordListDone = ov102_021EBEC8(view->wordList);
        BOOL sentenceDone = ov102_021EA754(view->sentence);

        if (wordListDone && sentenceDone) {
            ov102_021EBB1C(view->categories);
            ov102_021EBB88(view->categories);
            command->state++;
        }
        break;
    }
    case 2:
        if (ov102_021EBBC4(view->categories)) {
            ov102_021EBB2C(view->categories);
            command->state++;
        }
        break;
    case 3:
        if (ov102_021EBB44(view->categories)) {
            ov102_021EAA3C(view->sentence);
            ov102_021EAF5C(view->sentence, 1);
            ov102_021EAFAC(view->sentence);
            ov102_021EAF50(view->sentence);
            view->onMenu = FALSE;
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
}

void EasyChatAppView_Cmd_WordListToMenu(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EB67C(view->menuView);
        ov102_021EBEF4(view->wordList, 0);
        ov102_021EC290(view->pageButtons, 0);
        ov102_021EBEA4(view->wordList);
        command->state++;
        break;
    case 1:
        if (ov102_021EBEC8(view->wordList)) {
            ov102_021EBB1C(view->categories);
            ov102_021EBB88(view->categories);
            command->state++;
        }
        break;
    case 2:
        if (ov102_021EBBC4(view->categories)) {
            ov102_021EBB2C(view->categories);
            command->state++;
        }
        break;
    case 3:
        if (ov102_021EBB44(view->categories)) {
            ov102_021EAA3C(view->sentence);
            ov102_021EAF5C(view->sentence, 0);
            ov102_021EB524(view->menuView, 1);
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
}

void EasyChatAppView_Cmd_MoveMenuCursor(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;
    u8 cursor = EasyChatApp_GetMenuCursor(view->app);

    if (cursor == 0) {
        view->onMenu = FALSE;
        // called without its second argument
        ((void (*)(EasyChatMenuView *))ov102_021EB524)(view->menuView);
        ov102_021EAF5C(view->sentence, 1);
        ov102_021EAFF0(view->sentence, EasyChatApp_GetWordSlot(view->app));
    } else {
        view->onMenu = TRUE;
        ov102_021EAF5C(view->sentence, 0);
        ov102_021EB524(view->menuView, 1);
        ov102_021EB530(view->menuView, cursor);
    }
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_SetMenuCursor(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EB530(view->menuView, EasyChatApp_GetMenuCursor(view->app));
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_SetCategoryCursor(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EB9C0(view->categories, EasyChatApp_GetCategory(view->app));
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_SetWordListCursor(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EBF38(view->wordList, EasyChatApp_GetWordListCursor(view->app));
    EasyChatAppView_EndCommand(command);
}

#pragma force_active on

void EasyChatAppView_Cmd_ScrollWordList(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    switch (command->state) {
    case 0:
        ov102_021EBFA0(view->wordList, EasyChatApp_GetWordListScroll(view->app));
        command->state++;
        break;
    case 1:
        if (ov102_021EC05C(view->wordList)) {
            ov102_021EC290(view->pageButtons, 1);
            EasyChatAppView_EndCommand(command);
        }
        break;
    }
}

#pragma force_active reset

void EasyChatAppView_Cmd_HideMessage(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EAE40(command->view->sentence, 0);
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_ConfirmFinish(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EAE40(view->sentence, 2);
    ov102_021EB2EC(command->view->menuView, 0, 0);
    if (view->onMenu == TRUE) {
        ov102_021EB50C(view->menuView);
    } else {
        ov102_021EAF44(view->sentence);
    }
    ov102_021EB0C0(view->sentence, ov102_021E9090(command->app));
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_ConfirmQuit(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EAE40(view->sentence, 3);
    ov102_021EB2EC(command->view->menuView, 0, 0);
    if (view->onMenu == TRUE) {
        ov102_021EB50C(view->menuView);
    } else {
        ov102_021EAF44(view->sentence);
    }
    ov102_021EB0C0(view->sentence, ov102_021E9090(command->app));
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_ShowIncompleteMessage(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EAE40(command->view->sentence, 1);
    EasyChatAppView_EndCommand(command);
}

void ov102_021EA144(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EB170(view->sentence, ov102_021E9090(command->app));
    EasyChatAppView_EndCommand(command);
}

#pragma force_active on

void EasyChatAppView_Cmd_CloseYesNo(SysTask *task, EasyChatViewCommand *command) {
    EasyChatAppView *view = command->view;

    ov102_021EAE40(view->sentence, 0);
    if (view->onMenu == TRUE) {
        ov102_021EB2EC(view->menuView, 1, 1);
        ov102_021EB518(view->menuView);
    } else {
        ov102_021EB2EC(view->menuView, 1, 0);
        ov102_021EAF50(view->sentence);
    }
    EasyChatAppView_EndCommand(command);
}

#pragma force_active reset

void EasyChatAppView_Cmd_PressPageUp(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EC294(command->view->pageButtons, 0, 1);
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_PressPageDown(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EC294(command->view->pageButtons, 1, 1);
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_ReleasePageUp(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EC294(command->view->pageButtons, 0, 0);
    EasyChatAppView_EndCommand(command);
}

void EasyChatAppView_Cmd_ReleasePageDown(SysTask *task, EasyChatViewCommand *command) {
    ov102_021EC294(command->view->pageButtons, 1, 0);
    EasyChatAppView_EndCommand(command);
}

Sprite *EasyChatAppView_GetSliderSprite(EasyChatAppView *view) {
    return ov102_021EAFEC(view->sentence);
}

u32 EasyChatAppView_GetNumWordSlots(EasyChatAppView *view) {
    return ov102_021EADF0(view->sentence);
}

void EasyChatAppView_GetWordSlotHitbox(EasyChatAppView *view, TouchscreenHitbox *hitbox, u8 slot) {
    ov102_021EAB10(view->sentence, hitbox, slot);
}

int EasyChatAppView_GetYesNoResult(EasyChatAppView *view) {
    return ov102_021EB130(view->sentence);
}

BgConfig *EasyChatAppView_GetBgConfig(EasyChatAppView *view) {
    return view->bgConfig;
}

SpriteList *EasyChatAppView_GetSpriteList(EasyChatAppView *view) {
    return view->spriteList;
}

// Sprite helpers shared by the screen pieces: the resource header of OBJ set `index` (0 main, 1 sub screen) and
// a sprite made from it
void EasyChatAppView_InitSpriteHeader(EasyChatAppView *view, SpriteResourcesHeader *header, u32 index, u8 priority) {
    header->imageProxy = &view->imageProxies[index];
    header->plttProxy = &view->paletteProxies[index];
    header->cellData = view->cellBanks[index];
    header->cellAnim = view->animBanks[index];
    header->priority = priority;
    header->charData = NULL;
    header->multiCellData = NULL;
    header->multiCellAnim = NULL;
    header->flag = 0;
}

Sprite *EasyChatAppView_CreateSprite(EasyChatAppView *view, SpriteResourcesHeader *header, s32 x, s32 y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen) {
    SimpleSpriteTemplate template;
    OSIntrMode intrMode;
    Sprite *sprite;

    template.spriteList = view->spriteList;
    template.header = header;
    template.position.x = x << FX32_SHIFT;
    template.position.y = y << FX32_SHIFT;
    template.position.z = 0;
    template.priority = priority;
    template.whichScreen = whichScreen;
    template.heapID = HEAP_ID_35;
    if (whichScreen == NNS_G2D_VRAM_TYPE_2DSUB) {
        template.position.y += FX32_CONST(192);
    }
    intrMode = OS_DisableInterrupts();
    sprite = Sprite_Create(&template);
    OS_RestoreInterrupts(intrMode);
    if (sprite != NULL) {
        Sprite_SetAnimActiveFlag(sprite, TRUE);
        Sprite_SetAnimSpeed(sprite, FX32_ONE);
    }
    return sprite;
}
