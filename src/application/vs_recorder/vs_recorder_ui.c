#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"
#include "msgdata/msg.naix"

#include "assert.h"
#include "fashion_case.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "math_util.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "touch_hitbox_controller.h"

// VS Recorder screen framework, part 1: screen / step helpers, the menu tree (stack, actions, touch input),
// shared BG / sprite resources, the drifting decor sprites and the menu row sprites.

typedef struct VsRecorderS8x6 {
    s8 v[6];
} VsRecorderS8x6;

typedef struct VsRecorderS16x6 {
    s16 v[6];
} VsRecorderS16x6;

typedef struct VsRecorderF32x6 {
    f32 v[6];
} VsRecorderF32x6;

typedef struct VsRecorderAngles {
    int values[6];
} VsRecorderAngles;

BOOL FashionEntry_IsSaved(SaveFashionDataSub *entry);
void TextOBJ_SetParentSprite(TextOBJ *textOBJ, const Sprite *sprite);

extern const u8 ov40_02244DA8[];
extern const u8 ov40_02244DC0[];
extern const VsRecorderAngles ov40_02244DD8;
extern const VsRecorderF32x6 ov40_02244DF0;

void VsRecorder_SetScreenIfDone(VsRecorderWork *work, int nextScreen, BOOL done, int *screen) {
    if (done) {
        *screen = nextScreen;
        VsRecorder_SetSubState(work, 0);
        VsRecorder_SetStep(work, 0);
    }
}

void VsRecorder_SetSubState(VsRecorderWork *work, int subState) {
    work->subState = subState;
    work->step = 0;
    work->counter = 0;
}

void VsRecorder_SetStep(VsRecorderWork *work, u32 step) {
    work->step = step;
    work->counter = 0;
}

void VsRecorder_RefreshMenu(VsRecorderWork *work) {
    sub_02088030(work);
    VsRecorder_RefreshChoiceRows(work);
    work->menuCursor = 0;
}

BOOL VsRecorder_MenuEnterChild(VsRecorderWork *work) {
    work->menuStack[work->menuDepth] = work->menu;
    work->menuPath[work->menuDepth] = &work->menu[work->menuCursor];
    work->menu = work->menu[work->menuCursor].child;
    work->menuDepth++;
    work->menuCursor = 0;
    if (work->menu == NULL) {
        return FALSE;
    }
    VsRecorder_RefreshMenu(work);
    return TRUE;
}

BOOL VsRecorder_MenuRestore(VsRecorderWork *work) {
    u32 depth = work->menuDepth;
    work->menu = work->menuStack[depth];
    VsRecorder_RefreshMenu(work);
    return TRUE;
}

void VsRecorder_MenuRunAction(VsRecorderWork *work) {
    u32 index = work->menuCursor;
    const VsRecorderMenuEntry *entry = &work->menu[index];

    switch (entry->action) {
    case VS_RECORDER_MENU_ACTION_CANCEL:
        sub_02087A84(work->result, 0, 0);
        VsRecorder_SetSubState(work, 2);
        break;
    case 4:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_SAVED_VIDEO, TRUE, work->screenPtr);
        break;
    case 5:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_FRONTIER, TRUE, work->screenPtr);
        break;
    case 8:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_DRESS_UP_SEND, TRUE, work->screenPtr);
        break;
    case 9:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_DRESS_UP_VIEW, TRUE, work->screenPtr);
        break;
    case 10:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_BOX_DATA_SEND, TRUE, work->screenPtr);
        break;
    case 11:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_BOX_DATA_VIEW, TRUE, work->screenPtr);
        break;
    case 12:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_RANKINGS, TRUE, work->screenPtr);
        break;
    case 13:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_RANKINGS_LAST, TRUE, work->screenPtr);
        break;
    case 14:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_COLOSSEUM_TOP, TRUE, work->screenPtr);
        break;
    case 15:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_FRONTIER_TOP, TRUE, work->screenPtr);
        break;
    case 16:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_NEWEST, TRUE, work->screenPtr);
        break;
    case 17:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_SEARCH, TRUE, work->screenPtr);
        break;
    case 18:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_SEARCH_NUMBER, TRUE, work->screenPtr);
        break;
    case 19:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_SEND_VIDEO, TRUE, work->screenPtr);
        break;
    case 7:
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_DELETE, TRUE, work->screenPtr);
        break;
    }
}

BOOL VsRecorder_MenuCanSelect(VsRecorderWork *work) {
    u32 index = work->menuCursor;
    const VsRecorderMenuEntry *entry = &work->menu[index];
    u32 selection = entry->selection;
    BOOL result = TRUE;

    switch (work->mode) {
    default:
        if (selection <= 3 && work->savedVideos[selection] == NULL) {
            VsRecorder_ShowNotice(work, 0x10D);
            result = FALSE;
        }
        break;
    case 5:
        if (selection == 0) {
            SaveFashionData *fashionData = Save_FashionData_Get(work->saveData);
            result = FashionEntry_IsSaved(Save_FashionData_GetEntry(fashionData, selection));
            if (!result) {
                VsRecorder_ShowNotice(work, 0x122);
            }
        }
        break;
    case 6:
        if (selection == 100 && ((BOOL(*)(void))ov40_0222DD94)()) {
            VsRecorder_ShowNotice(work, 0x111);
            result = FALSE;
        }
        break;
    case 2:
        if (selection == 9999 && work->savedVideos[0] == NULL) {
            VsRecorder_ShowNotice(work, 0x81);
            result = FALSE;
        }
        break;
    case 3:
    case 4:
        break;
    }
    if (!result) {
        PlaySE(SEQ_SE_PL_BREC58);
    } else {
        PlaySE(SEQ_SE_PL_BREC57);
    }
    return result;
}

#pragma force_active on

// touch callback of the menu choice hitboxes
void VsRecorder_OnMenuTouch(u32 index, u32 event, void *arg) {
    struct {
        s16 y, x;
    } pos;

    VsRecorderWork *work = arg;

    if (event != 0) {
        return;
    }
    work->menuCursor = index;
    if (!VsRecorder_MenuCanSelect(work)) {
        return;
    }
    if (index > 4) {
        return;
    }
    VsRecorder_GetSpritePos(work->choiceRows[index].sprite, &pos.x, &pos.y);
    OrbitCursor_SetPosition(work->orbitCursors[0], (s16)(pos.x + 16), pos.y);
    OrbitCursor_SetTarget(work->orbitCursors[0], (s16)(pos.x + 16), pos.y);
    work->selection = work->menu[work->menuCursor].selection;
    VsRecorder_SetSubState(work, work->menu[work->menuCursor].nextSubState);
    if (work->menu[work->menuCursor].nextSubState == VS_RECORDER_MENU_ACTION_CANCEL) {
        sub_02087A84(work->result, 0, 0);
    } else {
        VsRecorder_ClearMenuHitboxes(work);
        OrbitCursor_SetVisible(work->orbitCursors[0], 1);
    }
    if (work->selection == 999) {
        VsRecorder_ShowMenuNotice(work, 193);
        return;
    }
    if (work->selection == 9999) {
        VsRecorder_ShowMenuNotice(work, 301);
        return;
    }
    if (work->menuNoticeShown) {
        VsRecorder_HideNotice(work);
        work->menuNoticeShown = 0;
    }
}

#pragma force_active reset

void VsRecorder_InitMenuHitboxes(VsRecorderWork *work) {
    s32 i = 0;
    TouchscreenHitbox *hitbox = work->menuHitboxes;
    u8 *row = (u8 *)work; // VsRecorderMenuRow stride over the work

    do {
        i++;
        ((VsRecorderWork *)row)->choiceRows[0].hitbox = hitbox;
        hitbox++;
        row += sizeof(VsRecorderMenuRow);
    } while (i < 5);
    work->hitboxController = TouchHitboxController_Create(work->menuHitboxes, 5, VsRecorder_OnMenuTouch, work, HEAP_ID_109);
}

void VsRecorder_UpdateMenuHitboxes(VsRecorderWork *work) {
    s16 pos[2]; // y, x
    u32 offset;
    s32 i;
    u32 zero;
    u8 *row; // VsRecorderMenuRow stride over the work

    if (work->menu == NULL) {
        return;
    }
    offset = 0;
    row = (u8 *)work;
    i = offset;
    zero = offset;
    do {
        if (*(int *)((u8 *)work->menu + offset) == 0) {
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.top = zero;
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.bottom = zero;
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.left = zero;
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.right = zero;
        } else {
            ManagedSprite_GetPositionXY(((VsRecorderWork *)row)->choiceRows[0].sprite, &pos[1], &pos[0]);
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.top = pos[0] - 16;
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.bottom = pos[0] + 16;
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.left = pos[1];
            ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.right = pos[1] + 160;
        }
        offset += sizeof(VsRecorderMenuEntry);
        i++;
        row += sizeof(VsRecorderMenuRow);
    } while (i < 5);
}

void VsRecorder_ClearMenuHitboxes(VsRecorderWork *work) {
    s16 pos[2];
    s32 i = 0;
    u32 zero = i;
    u8 *row = (u8 *)work; // VsRecorderMenuRow stride over the work

    do {
        ManagedSprite_GetPositionXY(((VsRecorderWork *)row)->choiceRows[0].sprite, &pos[1], &pos[0]);
        ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.top = zero;
        ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.bottom = zero;
        ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.left = zero;
        ((VsRecorderWork *)row)->choiceRows[0].hitbox->rect.right = zero;
        i++;
        row += sizeof(VsRecorderMenuRow);
    } while (i < 5);
}

void VsRecorder_UpdateMenuTouch(VsRecorderWork *work) {
    TouchHitboxController_IsTriggered(work->hitboxController);
}

#pragma force_active on

void VsRecorder_MenuPop(VsRecorderWork *work) {
    if (work->menuDepth > 0) {
        work->menuDepth--;
    } else if (work->menuDepth != 0) {
        work->menuDepth = 0;
        GF_AssertFail();
    }
    work->menu = work->menuStack[work->menuDepth];
}

void VsRecorder_ClearResumeState(VsRecorderWork *work) {
    int i;
    work->hasResumeScreen = FALSE;
    work->resumeScreen = VS_RECORDER_SCREEN_NONE;
    for (i = 0; i < 8; i++) {
        work->unk_0840[i] = 0;
    }
}

#pragma force_active reset

u32 VsRecorder_HasResumeScreen(VsRecorderWork *work) {
    return work->hasResumeScreen;
}

void VsRecorder_SetResumeScreen(VsRecorderWork *work, u32 screen) {
    work->hasResumeScreen = TRUE;
    work->resumeScreen = screen;
}

#pragma force_active on

// BG 1 / 5 frame: the VS Recorder's own (palettes by colour scheme) or the Global Terminal's
void VsRecorder_LoadFrameBg(VsRecorderWork *work) {
    NARC *narc = work->narc;
    BgConfig *bgConfig = work->bgConfig;
    PaletteData *plttData = work->plttData;

    if (work->mode == 0) {
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 67, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 67, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 69, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 3), HEAP_ID_109, PLTTBUF_MAIN_BG, 0x120, 0);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 3), HEAP_ID_109, PLTTBUF_SUB_BG, 0x120, 0);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 0), HEAP_ID_109, PLTTBUF_MAIN_BG, 0x20, 0xE0);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 0), HEAP_ID_109, PLTTBUF_SUB_BG, 0x20, 0xE0);
    } else {
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 54, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 54, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 55, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, 53, HEAP_ID_109, PLTTBUF_MAIN_BG, 0x120, 0);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, 53, HEAP_ID_109, PLTTBUF_SUB_BG, 0x120, 0);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, 52, HEAP_ID_109, PLTTBUF_MAIN_BG, 0x20, 0xE0);
        PaletteData_LoadNarc(plttData, NARC_a_1_9_1, 52, HEAP_ID_109, PLTTBUF_SUB_BG, 0x20, 0xE0);
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 70, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 70, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_109);
    PaletteData_BlendPalettes(plttData, PLTTBUF_SUB_BG, 2, 16, (u16)work->blendColor);
}

#pragma force_active reset

void ov40_0222C6C8(VsRecorderWork *work, GFBgLayer layer, BOOL alternate) {
    NARC *narc = work->narc;
    BgConfig *bgConfig = work->bgConfig;
    int screen = 2;

    if (alternate != FALSE) {
        screen = 0;
    }

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 45, bgConfig, layer, 0, 0, FALSE, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, screen, bgConfig, layer, 0, 0, FALSE, HEAP_ID_109);
}

void ov40_0222C710(VsRecorderWork *work, GFBgLayer layer) {
    NARC *narc = work->narc;
    BgConfig *bgConfig = work->bgConfig;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 45, bgConfig, layer, 0, 0, FALSE, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 1, bgConfig, layer, 0, 0, FALSE, HEAP_ID_109);
}

void VsRecorder_LoadDecorGfx(VsRecorderWork *work) {
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *plttData;

    narc = work->narc;
    spriteSystem = work->spriteSystem;
    spriteManager = work->spriteManager;
    plttData = work->plttData;

    if (work->mode == 0) {
        SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, VsRecorder_GetSchemeFileId(work, 2), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 9999);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_SUB_OBJ, spriteSystem, spriteManager, narc, VsRecorder_GetSchemeFileId(work, 2), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 10000);
    } else {
        SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 51, FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 9999);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_SUB_OBJ, spriteSystem, spriteManager, narc, 51, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 10000);
    }
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 65, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 9999);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 65, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 10000);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 46, FALSE, 9999);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 47, FALSE, 9999);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 46, FALSE, 10000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 47, FALSE, 10000);
}

#pragma force_active on

void VsRecorder_CreateDecorSprites(VsRecorderWork *work) {
    ManagedSpriteTemplate template;
    VsRecorderS16x6 ys;
    VsRecorderF32x6 scales;
    VsRecorderS8x6 xs;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int i, j;
    BOOL busy;

    spriteSystem = work->spriteSystem;
    spriteManager = work->spriteManager;
    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.vramTransfer = 0;
    template.pal = 0;
    template.bgPriority = 2;
    template.resIdList[0] = 9999;
    template.resIdList[1] = 9999;
    template.resIdList[2] = 9999;
    template.resIdList[3] = 9999;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    ys = *(const VsRecorderS16x6 *)(ov40_02244DA8 + 0x22);
    scales = ov40_02244DF0;
    xs = *(const VsRecorderS8x6 *)ov40_02244DA8;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
                template.resIdList[0] = 9999;
                template.resIdList[1] = 9999;
                template.resIdList[2] = 9999;
                template.resIdList[3] = 9999;
            } else {
                template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
                template.resIdList[0] = 10000;
                template.resIdList[1] = 10000;
                template.resIdList[2] = 10000;
                template.resIdList[3] = 10000;
            }
            work->decor[i].sprites[j] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
            VsRecorder_SetSpritePos(work->decor[i].sprites[j], ys.v[i], 0x60);
            ManagedSprite_TickFrame(work->decor[i].sprites[j]);
            ManagedSprite_SetAffineOverwriteMode(work->decor[i].sprites[j], 2);
            ManagedSprite_SetAffineScale(work->decor[i].sprites[j], scales.v[i], 1.0f);
            work->decor[i].counter = 0;
            work->decor[i].phase = i % 3;
            work->decor[i].state = 0;
            work->decor[i].scale = scales.v[i];
            work->decor[i].duration = xs.v[i] * 2 + 10;
            if (i < 3) {
                work->decor[i].angle = 0;
                work->decor[i].targetX = xs.v[i] + 8;
                work->decor[i].direction = -1;
            } else {
                work->decor[i].angle = 0;
                work->decor[i].targetX = 255 - (xs.v[i] + 8);
                work->decor[i].direction = 1;
            }
        }
        work->decor[i].task = SysTask_CreateOnVBlankQueue(VsRecorder_DecorTask, &work->decor[i], 5);
    }
    if (work->menuDepth != 0) {
        // coming back from a sub-screen: skip the fly-in
        do {
            busy = FALSE;
            for (i = 0; i < 6; i++) {
                if (work->decor[i].state != 3) {
                    VsRecorder_DecorTask(work->decor[i].task, &work->decor[i]);
                    busy = TRUE;
                }
            }
        } while (busy);
    }
}

#pragma force_active reset

void VsRecorder_DeleteDecorSprites(VsRecorderWork *work) {
    int i;
    int j;
    int offset = offsetof(VsRecorderWork, decor);
    u8 *row;
    u8 *item;

    row = (u8 *)work;
    for (i = 0; i < 6; i++, row += sizeof(VsRecorderDecor)) {
        j = 0;
        item = row;
        while (j < 2) {
            Sprite_DeleteAndFreeResources(*(ManagedSprite **)(item + offset));
            j++;
            item += sizeof(ManagedSprite *);
        }
    }
}

void VsRecorder_DestroyDecorTasks(VsRecorderWork *work) {
    int i;
    u8 *row = (u8 *)work; // VsRecorderDecor stride over the work

    for (i = 0; i < 6; i++, row += sizeof(VsRecorderDecor)) {
        SysTask_Destroy(((VsRecorderWork *)row)->decor[0].task);
    }
}

#pragma force_active on

// sprite resources of the menu rows (main / sub screen), per app mode
void VsRecorder_LoadMenuRowGfx(VsRecorderWork *work) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    int chars, cells, anims;

    switch (work->mode) {
    case 0:
        chars = 63;
        cells = 24;
        anims = 25;
        break;
    case 2:
        chars = 17;
        cells = 18;
        anims = 19;
        break;
    case 5:
    case 6:
        chars = 8;
        cells = 9;
        anims = 10;
        break;
    case 4:
        chars = 11;
        cells = 12;
        anims = 13;
        break;
    case 3:
        chars = 14;
        cells = 15;
        anims = 16;
        break;
    default:
        chars = 48;
        cells = 22;
        anims = 23;
        break;
    }
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, chars, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 10001);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, chars, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 10002);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, cells, FALSE, 10001);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, cells, FALSE, 10002);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, anims, FALSE, 10001);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, anims, FALSE, 10002);
}

#pragma force_active reset

void VsRecorder_UnloadMenuRowChars(VsRecorderWork *work) {
    SpriteManager_UnloadCharObjById(work->spriteManager, 10001);
    SpriteManager_UnloadCharObjById(work->spriteManager, 10002);
}

void VsRecorder_LayoutMenuRows(VsRecorderWork *work) {
    s16 x;
    s16 y;
    int i;
    VsRecorderMenuRow *pathRow;
    u8 *row; // VsRecorderMenuRow stride over the work
    u8 *pathRow2;
    int j;

    for (i = 0, pathRow = work->pathRows, row = (u8 *)work; i < 5; i++, pathRow++, row += sizeof(VsRecorderMenuRow)) {
        VsRecorder_SetSpritePos(pathRow->sprite, 0x32, (5 - work->choiceCount) * 16 + 0xD9);
        TextOBJ_SetPosition(((VsRecorderWork *)row)->pathRows[0].text, 0x24, 0x24 - 0x2C);
    }

    j = 0;
    pathRow2 = (u8 *)work;
    do {
        if (work->menuDepth == 0) {
            break;
        }
        VsRecorder_GetSpritePos(((VsRecorderWork *)pathRow2)->pathRows[0].sprite, &x, &y);
        if (work->menuDepth - 1 == j) {
            y = 0xA9;
            ManagedSprite_SetPaletteOverrideOffset(((VsRecorderWork *)pathRow2)->pathRows[0].sprite, 1);
        } else {
            x = x - (work->menuDepth - j) * 4;
            y = (work->menuDepth - j - 1) * 16 + (0x24 * (5 - (work->menuDepth - j)) + 25);
            ManagedSprite_SetPaletteOverrideOffset(((VsRecorderWork *)pathRow2)->pathRows[0].sprite, 2);
        }
        VsRecorder_SetSpritePos(((VsRecorderWork *)pathRow2)->pathRows[0].sprite, x, y);
        j++;
        pathRow2 += sizeof(VsRecorderMenuRow);
    } while (j < work->menuDepth);
}

#pragma force_active on

void VsRecorder_CreateMenuRowSprites(VsRecorderWork *work) {
    s16 x;
    s16 y;
    ManagedSpriteTemplate template;
    VsRecorderMenuRow *rows = work->pathRows;
    int i;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *plttData;

    spriteSystem = work->spriteSystem;
    spriteManager = work->spriteManager;
    plttData = work->plttData;
    template.x = 42;
    template.y = (5 - work->choiceCount) * 16 + 201;
    if (work->choiceCount == 2) {
        template.y = (4 - work->choiceCount) * 16 + 201;
    }
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = 3;
    template.vramTransfer = 0;
    template.resIdList[0] = 10001;
    template.pal = 1;
    template.resIdList[1] = 9999;
    template.resIdList[2] = 10001;
    template.resIdList[3] = 10001;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    for (i = 0; i < 5; i++) {
        template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        template.resIdList[0] = 10002;
        template.resIdList[1] = 10000;
        template.resIdList[2] = 10002;
        template.resIdList[3] = 10002;
        template.drawPriority = ov40_02244DC0[i + 5];
        rows[i].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
        ManagedSprite_TickFrame(rows[i].sprite);
        ManagedSprite_SetDrawFlag(rows[i].sprite, TRUE);
        VsRecorder_SetSpritePos(rows[i].sprite, template.x + 8, template.y);
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.resIdList[0] = 10001;
        template.resIdList[1] = 9999;
        template.resIdList[2] = 10001;
        template.resIdList[3] = 10001;
        template.drawPriority = ov40_02244DC0[i];
        rows[i + 5].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
        ManagedSprite_TickFrame(rows[i + 5].sprite);
        VsRecorder_SetSpritePos(rows[i + 5].sprite, template.x, template.y);
        ManagedSprite_SetDrawFlag(rows[i + 5].sprite, work->menu[i].enabled);
    }
    i = 0;
    do {
        if (work->menuDepth == 0) {
            break;
        }
        VsRecorder_GetSpritePos(work->pathRows[i].sprite, &x, &y);
        if (work->menuDepth - 1 == i) {
            y = 169;
            ManagedSprite_SetPaletteOverrideOffset(work->pathRows[i].sprite, 1);
        } else {
            x -= (work->menuDepth - i) * 4;
            y = (work->menuDepth - i - 1) * 16 + ((5 - (work->menuDepth - i)) * 36 + 25);
            ManagedSprite_SetPaletteOverrideOffset(work->pathRows[i].sprite, 2);
        }
        VsRecorder_SetSpritePos(work->pathRows[i].sprite, x, y);
        i++;
    } while (i < work->menuDepth);
    PaletteData_BlendPalettes(plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, work->blendColor);
}

// right column: one row per entry of the current menu
void VsRecorder_RefreshChoiceRows(VsRecorderWork *work) {
    int i;

    for (i = 0; i < 5; i++) {
        ManagedSprite_SetDrawFlag(work->choiceRows[i].sprite, work->menu[i].enabled);
        if (work->menu[i].enabled != 0) {
            ManagedSprite_SetAnim(work->choiceRows[i].sprite, work->menu[i].anim);
            VsRecorder_SetMenuRowText(work, &work->choiceRows[i], work->menu[i].msgId, &work->menu[i]);
        }
    }
}

#pragma force_active reset

void VsRecorder_AttachMenuRowText(VsRecorderWork *work) {
    int i;
    u8 *row = (u8 *)work; // VsRecorderMenuRow stride over the work

    for (i = 0; i < 5; i++, row += sizeof(VsRecorderMenuRow)) {
        TextOBJ_SetParentSprite(((VsRecorderWork *)row)->choiceRows[0].text, ((VsRecorderWork *)row)->choiceRows[0].sprite->sprite);
        TextOBJ_SetParentSprite(((VsRecorderWork *)row)->pathRows[0].text, ((VsRecorderWork *)row)->pathRows[0].sprite->sprite);
    }
}

#pragma force_active on

// left column: the entries chosen on the way down
void VsRecorder_RefreshPathRows(VsRecorderWork *work) {
    int i;

    if (work->menuDepth != 0) {
        for (i = 0; i < work->menuDepth; i++) {
            ManagedSprite_SetAnim(work->pathRows[i].sprite, work->menuPath[i]->anim);
            VsRecorder_SetMenuRowText(work, &work->pathRows[i], work->menuPath[i]->msgId, work->menuPath[i]);
            TextOBJ_SetPosition(work->pathRows[i].text, 36, -8);
            TextOBJ_SetSpritesDrawFlag(work->pathRows[i].text, TRUE);
            ManagedSprite_SetDrawFlag(work->pathRows[i].sprite, TRUE);
        }
    }
}

#pragma force_active reset

void VsRecorder_DeleteMenuRowSprites(VsRecorderWork *work) {
    int i;
    VsRecorderMenuRow *row = work->pathRows;

    for (i = 0; i < 5; i++, row++) {
        Sprite_DeleteAndFreeResources(row[0].sprite);
        Sprite_DeleteAndFreeResources(row[5].sprite);
    }
}

void VsRecorder_SetMenuRowsDrawFlag(VsRecorderWork *work, BOOL draw) {
    int i;
    u8 *row = (u8 *)work;

    for (i = 0; i < 5; i++, row += sizeof(VsRecorderMenuRow)) {
        if (((VsRecorderWork *)row)->pathRows[0].enabled == 0) {
            TextOBJ_SetSpritesDrawFlag(((VsRecorderWork *)row)->pathRows[0].text, FALSE);
            ManagedSprite_SetDrawFlag(((VsRecorderWork *)row)->pathRows[0].sprite, FALSE);
        } else {
            TextOBJ_SetSpritesDrawFlag(((VsRecorderWork *)row)->pathRows[0].text, draw);
            ManagedSprite_SetDrawFlag(((VsRecorderWork *)row)->pathRows[0].sprite, draw);
        }
        if (((VsRecorderWork *)row)->choiceRows[0].enabled == 0) {
            TextOBJ_SetSpritesDrawFlag(((VsRecorderWork *)row)->choiceRows[0].text, FALSE);
            ManagedSprite_SetDrawFlag(((VsRecorderWork *)row)->choiceRows[0].sprite, FALSE);
        } else {
            TextOBJ_SetSpritesDrawFlag(((VsRecorderWork *)row)->choiceRows[0].text, draw);
            ManagedSprite_SetDrawFlag(((VsRecorderWork *)row)->choiceRows[0].sprite, draw);
        }
    }
}

#pragma force_active on

// V-blank task of one decor pair: wait, wobble in, slide to the target x, then sway in place
void VsRecorder_DecorTask(SysTask *task, void *data) {
    struct {
        struct {
            s16 y2, y1, x, pad;
        } p;

        fx32 y2, x2, y1, x1, b_y2, b_x2, b_y1, b_x1;
        VsRecorderAngles angles;
    } f;

    VsRecorderDecor *decor = data;

    decor->tick = (decor->tick + 1) % 2;
    if (decor->tick != 0) {
        return;
    }
    switch (decor->state) {
    case 0:
        if (decor->counter == 3) {
            decor->state++;
            decor->counter = 0;
        } else {
            decor->counter++;
        }
        break;
    case 1:
        if (decor->counter == 4) {
            decor->state++;
            decor->counter = 0;
        } else {
            ManagedSprite_GetSpritePositionFxXY(decor->sprites[0], &f.b_x1, &f.b_y1);
            ManagedSprite_GetSpritePositionFxXY(decor->sprites[1], &f.b_x2, &f.b_y2);
            f.b_x1 += GF_SinDeg((u16)(decor->angle * 65535 / 360)) * (-decor->direction);
            f.b_x2 = f.b_x1;
            decor->angle += 32;
            decor->angle %= 360;
            ManagedSprite_SetPositonFxXY(decor->sprites[0], f.b_x1, f.b_y1);
            ManagedSprite_SetPositonFxXY(decor->sprites[1], f.b_x2, f.b_y2);
            decor->counter++;
        }
        break;
    case 2:
        f.angles = ov40_02244DD8;
        ManagedSprite_GetPositionXY(decor->sprites[0], &f.p.x, &f.p.y1);
        ManagedSprite_GetPositionXY(decor->sprites[1], &f.p.x, &f.p.y2);
        if (decor->counter == 0) {
            decor->stepX = decor->targetX - f.p.x;
            decor->stepX = decor->stepX / decor->duration;
            decor->counter++;
        } else if (decor->counter == decor->duration + 1) {
            f.p.x = decor->targetX;
            decor->angle = f.angles.values[decor->phase];
            decor->state++;
            decor->counter = 0;
        } else {
            f.p.x += decor->stepX;
            decor->counter++;
        }
        ManagedSprite_SetPositionXY(decor->sprites[0], f.p.x, f.p.y1);
        ManagedSprite_SetPositionXY(decor->sprites[1], f.p.x, f.p.y2);
        break;
    case 3:
        if (decor->counter == 0) {
            ManagedSprite_GetSpritePositionFxXY(decor->sprites[0], &decor->startX, &f.y1);
            decor->counter++;
        }
        ManagedSprite_GetSpritePositionFxXY(decor->sprites[0], &f.x1, &f.y1);
        ManagedSprite_GetSpritePositionFxXY(decor->sprites[1], &f.x2, &f.y2);
        f.x1 = decor->startX + GF_SinDeg((u16)(decor->angle * 65535 / 360)) * 3 * (-decor->direction);
        f.x2 = f.x1;
        if (decor->phase % 2) {
            decor->angle -= 4;
        } else {
            decor->angle += 4;
        }
        decor->angle %= 360;
        ManagedSprite_SetPositonFxXY(decor->sprites[0], f.x1, f.y1);
        ManagedSprite_SetPositonFxXY(decor->sprites[1], f.x2, f.y2);
        break;
    }
    if (decor->state >= 2 && decor->scale > 0.1f) {
        decor->scale -= 0.1f;
        ManagedSprite_SetAffineScale(decor->sprites[0], decor->scale, 1.0f);
        ManagedSprite_SetAffineScale(decor->sprites[1], decor->scale, 1.0f);
    }
}

#pragma force_active reset

// sprite position on the combined (main + sub) screen space
void VsRecorder_SetSpritePos(ManagedSprite *sprite, s16 x, s16 y) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(sprite, x, y, 1 << 20);
}

void VsRecorder_GetSpritePos(ManagedSprite *sprite, s16 *x, s16 *y) {
    ManagedSprite_GetPositionXYWithSubscreenOffset(sprite, x, y, 1 << 20);
}
