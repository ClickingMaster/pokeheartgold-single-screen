#include "frontier/frontier_brain_intro.h"

#include "global.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "brightness.h"
#include "filesystem.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "text.h"

// Cutscene played before a battle against a Frontier Brain (script command 71): the screen darkens, a horizontal
// band opens in the middle (hardware windows 0/1), four sprites scale in, the Brain's picture slides in from the
// right, the Brain's name is shown and the screen fades to white.

// One entry per Brain (gFrontierBrainIntroArgs).
typedef struct FrontierBrainIntroArgs {
    u32 nameMsgId; // trainer name, message bank 729
    u8 spritePalette;
    u8 spriteChar;
    u8 spriteCell;
    u8 spriteAnim;
    u8 bgPalette;
    u8 bgChar;
    u8 bgScreen;
    u8 unused;
} FrontierBrainIntroArgs;

typedef struct FrontierBrainIntroLerp {
    fx32 value;
    fx32 start;
    fx32 delta;
    s32 step;
    s32 duration;
} FrontierBrainIntroLerp;

typedef struct FrontierBrainIntroBurst {
    s16 timer;
    s16 count;
    ManagedSprite *sprites[4];
    FrontierBrainIntroLerp scales[4];
} FrontierBrainIntroBurst;

typedef struct FrontierBrainIntroText {
    TextOBJ *textObj;
    ObjCharSpaceReservation charTransfer;
    u16 width;
} FrontierBrainIntroText;

typedef struct FrontierBrainIntroTextCache {
    Window window;
    u16 charLength;
    u16 width;
} FrontierBrainIntroTextCache;

typedef struct FrontierBrainIntroSprites {
    u8 unk_00[0x14];
    FrontierBrainIntroBurst burst;
    int timer;
    FrontierBrainIntroText name;
    ManagedSprite *brainSprite;
} FrontierBrainIntroSprites; // size: 0x94

typedef struct FrontierBrainIntroWindowAnim {
    fx32 top;
    fx32 bottom;
    u8 finished;
    u8 state;
    u8 win0Left;
    u8 win0Top;
    u8 win0Right;
    u8 win0Bottom;
    u8 win1Left;
    u8 win1Top;
    u8 win1Right;
    u8 win1Bottom;
} FrontierBrainIntroWindowAnim;

typedef struct FrontierBrainIntroSlide {
    fx32 x;
    int state;
} FrontierBrainIntroSlide;

typedef struct FrontierBrainIntro {
    int state;
    u32 unk_04;
    u32 unk_08;
    FrontierBrainIntroSprites *sprites;
    BgConfig *bgConfig;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *paletteData;
    u16 *done;
    NARC *narc;
    s16 x;
    s16 y;
    u8 brain;
    FontSystem *fontSystem;
    SysTask *windowTask;
    s32 paletteTimer;
    s32 paletteFrame;
    u16 bgPalettes[8][16];
    SysTask *paletteTask;
    FrontierBrainIntroWindowAnim windowAnim;
    FrontierBrainIntroSlide slide;
    u32 brainPaletteSlot;
    u32 paletteMask;
} FrontierBrainIntro; // size: 0x168

extern const ManagedSpriteTemplate gFrontierBrainIntroBrainSpriteTemplate;
extern const ManagedSpriteTemplate gFrontierBrainIntroBurstSpriteTemplate;
extern const FrontierBrainIntroArgs gFrontierBrainIntroArgs[];

void FrontierBrainIntro_Finish(FrontierBrainIntro *intro, SysTask *task);
void FrontierBrainIntro_UpdateWindows(SysTask *task, void *data);
void FrontierBrainIntro_MainTask(SysTask *task, void *data);
BOOL FrontierBrainIntro_Run(FrontierBrainIntro *intro, enum HeapID heapID, const FrontierBrainIntroArgs *args);
void FrontierBrainIntro_CreateText(FrontierBrainIntro *intro, FrontierBrainIntroText *text, String *string, u32 fontId, int color, int palette, int plttResourceId, int x, int y, int center, FrontierBrainIntroTextCache *cache);
void FrontierBrainIntro_DeleteText(FrontierBrainIntroText *text);
void FrontierBrainIntro_GetTextSize(String *string, u32 fontId, int *width, int *charLength);
void FrontierBrainIntro_CreateBurst(FrontierBrainIntro *intro, FrontierBrainIntroBurst *burst, fx32 x, fx32 y, enum HeapID heapID);
void FrontierBrainIntro_DeleteBurst(FrontierBrainIntroBurst *burst);
BOOL FrontierBrainIntro_UpdateBurst(FrontierBrainIntroBurst *burst);
void FrontierBrainIntroLerp_Init(FrontierBrainIntroLerp *lerp, fx32 start, fx32 end, s32 duration);
BOOL FrontierBrainIntroLerp_Step(FrontierBrainIntroLerp *lerp);
void FrontierBrainIntro_SetVec(VecFx32 *vec, fx32 x, fx32 y, fx32 z);
void FrontierBrainIntro_LoadBackground(FrontierBrainIntro *intro, const FrontierBrainIntroArgs *args);
void FrontierBrainIntro_CyclePaletteTask(SysTask *task, void *data);
void FrontierBrainIntro_StartWindowAnim(FrontierBrainIntro *intro, int close);
void FrontierBrainIntro_OpenWindowTask(SysTask *task, void *data);
void FrontierBrainIntro_CloseWindowTask(SysTask *task, void *data);
BOOL FrontierBrainIntro_SlideInBrain(FrontierBrainIntro *intro, FrontierBrainIntroSprites *sprites, FrontierBrainIntroSlide *slide);

void FrontierBrainIntro_Start(int brain, BgConfig *bgConfig, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PaletteData *paletteData, u16 *done, s16 x, s16 y) {
    SysTask *task = CreateSysTaskAndEnvironment(FrontierBrainIntro_MainTask, sizeof(FrontierBrainIntro), 1000, HEAP_ID_101);
    FrontierBrainIntro *intro = SysTask_GetData(task);
    intro->bgConfig = bgConfig;
    intro->spriteSystem = spriteSystem;
    intro->spriteManager = spriteManager;
    intro->paletteData = paletteData;
    intro->done = done;
    intro->x = x;
    intro->y = y;
    intro->narc = NARC_New(NARC_a_1_0_9, HEAP_ID_101);
    if (intro->done != NULL) {
        *intro->done = 0;
    }
    intro->brain = brain - 1;
    MIi_CpuClear32(0, BgGetCharPtr(GF_BG_LYR_MAIN_1), 0x8000);
    ScheduleSetBgPosText(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
    ScheduleSetBgPosText(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
    FrontierBrainIntro_LoadBackground(intro, &gFrontierBrainIntroArgs[intro->brain]);
    intro->windowTask = SysTask_CreateOnVWaitQueue(FrontierBrainIntro_UpdateWindows, intro, 1);
}

void FrontierBrainIntro_Finish(FrontierBrainIntro *intro, SysTask *task) {
    SysTask_Destroy(intro->paletteTask);
    SysTask_Destroy(intro->windowTask);
    NARC_Delete(intro->narc);
    Heap_FreeExplicit(HEAP_ID_101, intro->sprites);
    DestroySysTaskAndEnvironment(task);
}

#pragma force_active on
void FrontierBrainIntro_UpdateWindows(SysTask *task, void *data) {
    FrontierBrainIntro *intro = data;
    G2_SetWnd0Position(intro->windowAnim.win0Left, intro->windowAnim.win0Top, intro->windowAnim.win0Right, intro->windowAnim.win0Bottom);
    G2_SetWnd1Position(intro->windowAnim.win1Left, intro->windowAnim.win1Top, intro->windowAnim.win1Right, intro->windowAnim.win1Bottom);
}
#pragma force_active reset

void FrontierBrainIntro_MainTask(SysTask *task, void *data) {
    FrontierBrainIntro *intro = data;
    if (FrontierBrainIntro_Run(data, HEAP_ID_101, &gFrontierBrainIntroArgs[intro->brain]) == TRUE) {
        FrontierBrainIntro_Finish(data, task);
    }
}

#pragma force_active on
BOOL FrontierBrainIntro_Run(FrontierBrainIntro *intro, enum HeapID heapID, const FrontierBrainIntroArgs *args) {
    FrontierBrainIntroSprites *sprites = intro->sprites;
    MsgData *msgData;
    String *string;
    f32 yf;
    f32 xf;
    fx32 x;
    fx32 y;
    int slot;

    switch (intro->state) {
    case 0:
        intro->sprites = Heap_Alloc(heapID, sizeof(FrontierBrainIntroSprites));
        memset(intro->sprites, 0, sizeof(FrontierBrainIntroSprites));
        sprites = intro->sprites;

        slot = SpriteSystem_LoadPaletteBufferFromOpenNarc(intro->paletteData, PLTTBUF_MAIN_OBJ, intro->spriteSystem, intro->spriteManager, intro->narc, 16, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 2004);
        intro->paletteMask |= 1 << slot;
        intro->fontSystem = FontSystem_NewInit(4, HEAP_ID_101);
        msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0729_bin, heapID);
        string = NewString_ReadMsgData(msgData, args->nameMsgId);
        FrontierBrainIntro_CreateText(intro, &sprites->name, string, 0, MAKE_TEXT_COLOR(1, 2, 0), 0, 2004, intro->x + 116, intro->y + 88, 0, NULL);
        TextOBJ_SetSpritesDrawFlag(sprites->name.textObj, FALSE);
        String_Delete(string);
        DestroyMsgData(msgData);

        intro->brainPaletteSlot = SpriteSystem_LoadPaletteBufferFromOpenNarc(intro->paletteData, PLTTBUF_MAIN_OBJ, intro->spriteSystem, intro->spriteManager, intro->narc, args->spritePalette, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 2002);
        intro->paletteMask |= 1 << intro->brainPaletteSlot;
        SpriteSystem_LoadCharResObjFromOpenNarc(intro->spriteSystem, intro->spriteManager, intro->narc, args->spriteChar, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 2010);
        SpriteSystem_LoadCellResObjFromOpenNarc(intro->spriteSystem, intro->spriteManager, intro->narc, args->spriteCell, FALSE, 2002);
        SpriteSystem_LoadAnimResObjFromOpenNarc(intro->spriteSystem, intro->spriteManager, intro->narc, args->spriteAnim, FALSE, 2002);
        PaletteData_BlendPalette(intro->paletteData, PLTTBUF_MAIN_OBJ, (u16)(intro->brainPaletteSlot << 4), 16, 14, RGB_BLACK);

        slot = SpriteSystem_LoadPaletteBufferFromOpenNarc(intro->paletteData, PLTTBUF_MAIN_OBJ, intro->spriteSystem, intro->spriteManager, intro->narc, 59, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 2003);
        intro->paletteMask |= 1 << slot;
        SpriteSystem_LoadCharResObjFromOpenNarc(intro->spriteSystem, intro->spriteManager, intro->narc, 204, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 2011);
        SpriteSystem_LoadCellResObjFromOpenNarc(intro->spriteSystem, intro->spriteManager, intro->narc, 205, FALSE, 2003);
        SpriteSystem_LoadAnimResObjFromOpenNarc(intro->spriteSystem, intro->spriteManager, intro->narc, 206, FALSE, 2003);
        sprites->brainSprite = SpriteSystem_NewSprite(intro->spriteSystem, intro->spriteManager, &gFrontierBrainIntroBrainSpriteTemplate);
        ManagedSprite_SetDrawFlag(sprites->brainSprite, FALSE);
        Sprite_TickFrame(sprites->brainSprite->sprite);

        if (intro->y > 0) {
            yf = 0.5f + (f32)(intro->y << FX32_SHIFT);
        } else {
            yf = (f32)(intro->y << FX32_SHIFT) - 0.5f;
        }
        if (intro->x > 0) {
            xf = 0.5f + (f32)(intro->x << FX32_SHIFT);
        } else {
            xf = (f32)(intro->x << FX32_SHIFT) - 0.5f;
        }
        x = (fx32)xf;
        y = (fx32)yf;
        FrontierBrainIntro_CreateBurst(intro, &sprites->burst, x + FX32_CONST(72), y + FX32_CONST(82), heapID);
        intro->state++;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 3, 1, heapID);
        intro->state++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            intro->state++;
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 3, 1, heapID);
        intro->state++;
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            intro->state++;
        }
        break;
    case 5:
        FrontierBrainIntro_StartWindowAnim(intro, FALSE);
        intro->state++;
        break;
    case 6:
        if (intro->windowAnim.finished == TRUE) {
            intro->state++;
            sprites->timer = 10;
        }
        break;
    case 7:
        if (--sprites->timer < 0 && FrontierBrainIntro_UpdateBurst(&sprites->burst) == TRUE) {
            intro->state++;
        }
        break;
    case 8:
        intro->state++;
        break;
    case 9:
        if (FrontierBrainIntro_SlideInBrain(intro, sprites, &intro->slide) == TRUE) {
            intro->state++;
        }
        break;
    case 10:
        sprites->timer = 10;
        intro->state++;
        break;
    case 11:
        if (--sprites->timer < 0) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 3, 1, heapID);
            intro->state++;
        }
        break;
    case 12:
        if (IsPaletteFadeFinished()) {
            PaletteData_BlendPalettes(intro->paletteData, PLTTBUF_MAIN_OBJ, (u16)(0x3FFF ^ intro->paletteMask), 14, RGB_BLACK);
            PaletteData_BlendPalette(intro->paletteData, PLTTBUF_MAIN_OBJ, (u16)(intro->brainPaletteSlot << 4), 16, 0, RGB_BLACK);
            SetBlendBrightness(-14, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD), SCREEN_MASK_MAIN);
            TextOBJ_SetSpritesDrawFlag(sprites->name.textObj, TRUE);
            intro->state++;
        }
        break;
    case 13:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 3, 1, heapID);
        intro->state++;
        break;
    case 14:
        if (IsPaletteFadeFinished()) {
            sprites->timer = 26;
            intro->state++;
        }
        break;
    case 15:
        if (--sprites->timer < 0) {
            intro->state++;
        }
        break;
    case 16:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 15, 1, HEAP_ID_101);
        intro->state++;
        break;
    case 17:
        if (IsPaletteFadeFinished()) {
            intro->state++;
        }
        break;
    case 18:
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_WHITE);
        if (intro->done) {
            *intro->done = 1;
        }
        FrontierBrainIntro_DeleteText(&sprites->name);
        FontSystem_Delete(intro->fontSystem);
        Sprite_DeleteAndFreeResources(sprites->brainSprite);
        FrontierBrainIntro_DeleteBurst(&sprites->burst);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void FrontierBrainIntro_CreateText(FrontierBrainIntro *intro, FrontierBrainIntroText *text, String *string, u32 fontId, int color, int palette, int plttResourceId, int x, int y, int center, FrontierBrainIntroTextCache *cache) {
    TextOBJTemplate textObjTemplate;
    Window window;
    ObjCharSpaceReservation charTransfer;
    int width;
    int charLength;
    TextOBJ *textObj;
    BgConfig *bgConfig;
    SpriteManager *spriteManager;

    GF_ASSERT(text->textObj == NULL);
    bgConfig = intro->bgConfig;
    spriteManager = intro->spriteManager;

    if (cache == NULL) {
        FrontierBrainIntro_GetTextSize(string, fontId, &width, &charLength);
    } else {
        width = cache->width;
        charLength = cache->charLength;
    }

    if (cache == NULL) {
        InitWindow(&window);
        AddTextWindowTopLeftCorner(bgConfig, &window, charLength, 2, 0, 0);
        AddTextPrinterParameterizedWithColorAndSpacing(&window, fontId, string, 0, 0, TEXT_SPEED_NOTRANSFER, color, 0, 0, NULL);
    } else {
        window = cache->window;
    }

    ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_101), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &charTransfer);
    if (center == TRUE) {
        x -= width / 2;
    }

    textObjTemplate.fontSystem = intro->fontSystem;
    textObjTemplate.window = &window;
    textObjTemplate.spriteList = SpriteManager_GetSpriteList(spriteManager);
    textObjTemplate.plttResourceProxy = SpriteManager_FindPlttResourceProxy(spriteManager, plttResourceId);
    textObjTemplate.sprite = NULL;
    textObjTemplate.offset = charTransfer.offset;
    textObjTemplate.x = x;
    textObjTemplate.y = y - 8;
    textObjTemplate.unk_20 = 0;
    textObjTemplate.unk_24 = 11;
    textObjTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    textObjTemplate.heapID = HEAP_ID_101;

    textObj = FontOAM_Create(&textObjTemplate);
    TextOBJ_SetPalOffsetRespectVramOffset(textObj, palette);
    TextOBJ_SetPosition(textObj, x, y - 8);

    if (cache == NULL) {
        RemoveWindow(&window);
    }
    text->textObj = textObj;
    text->charTransfer = charTransfer;
    text->width = width;
}

void FrontierBrainIntro_DeleteText(FrontierBrainIntroText *text) {
    FontOAM_Delete(text->textObj);
    ObjCharTransfer_FreeCharSpace(&text->charTransfer);
}

void FrontierBrainIntro_GetTextSize(String *string, u32 fontId, int *width, int *charLength) {
    int textWidth = FontID_String_GetWidth(fontId, string, 0);
    int tiles = textWidth / 8;
    if (FX_ModS32(textWidth, 8) != 0) {
        tiles++;
    }
    *width = textWidth;
    *charLength = tiles;
}

void FrontierBrainIntro_CreateBurst(FrontierBrainIntro *intro, FrontierBrainIntroBurst *burst, fx32 x, fx32 y, enum HeapID heapID) {
    ManagedSpriteTemplate template = gFrontierBrainIntroBurstSpriteTemplate;
    int i;

    template.x = x >> FX32_SHIFT;
    template.y = y >> FX32_SHIFT;
    burst->timer = 0;
    burst->count = 0;
    for (i = 0; i < 4; i++) {
        burst->sprites[i] = SpriteSystem_NewSprite(intro->spriteSystem, intro->spriteManager, &template);
        ManagedSprite_SetDrawFlag(burst->sprites[i], FALSE);
        if (i != 3) {
            Sprite_SetAffineOverwriteMode(burst->sprites[i]->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
            Sprite_SetAnimCtrlSeq(burst->sprites[i]->sprite, 1);
            FrontierBrainIntroLerp_Init(&burst->scales[i], FX32_CONST(2), FX32_ONE, 6);
        } else {
            FrontierBrainIntroLerp_Init(&burst->scales[i], FX32_ONE, FX32_ONE, 6);
        }
    }
}

void FrontierBrainIntro_DeleteBurst(FrontierBrainIntroBurst *burst) {
    int i;
    for (i = 0; i < 4; i++) {
        Sprite_DeleteAndFreeResources(burst->sprites[i]);
    }
}

#pragma force_active on
BOOL FrontierBrainIntro_UpdateBurst(FrontierBrainIntroBurst *burst) {
    int i;
    BOOL done = TRUE;
    BOOL finished;
    VecFx32 scale;

    if (burst->count < 4) {
        done = FALSE;
        burst->timer--;
        if (burst->timer <= 0) {
            burst->timer = 3;
            burst->count++;
        }
    }
    for (i = 0; i < burst->count; i++) {
        finished = FrontierBrainIntroLerp_Step(&burst->scales[i]);
        FrontierBrainIntro_SetVec(&scale, burst->scales[i].value, burst->scales[i].value, burst->scales[i].value);
        Sprite_SetAffineScale(burst->sprites[i]->sprite, &scale);
        Sprite_SetDrawFlag(burst->sprites[i]->sprite, TRUE);
        if (finished == FALSE) {
            done = FALSE;
        }
    }
    return done;
}
#pragma force_active reset

void FrontierBrainIntroLerp_Init(FrontierBrainIntroLerp *lerp, fx32 start, fx32 end, s32 duration) {
    lerp->value = start;
    lerp->start = start;
    lerp->delta = end - start;
    lerp->duration = duration;
    lerp->step = 0;
}

BOOL FrontierBrainIntroLerp_Step(FrontierBrainIntroLerp *lerp) {
    fx64 value = (fx64)lerp->delta * (lerp->step << FX32_SHIFT);
    fx32 result;
    s32 step;
    s32 duration;

    value = (value + 0x800) >> FX32_SHIFT;
    result = FX_Div((fx32)value, lerp->duration << FX32_SHIFT);
    result += lerp->start;
    lerp->value = result;
    step = lerp->step;
    duration = lerp->duration;
    step++;
    if (step <= duration) {
        lerp->step = step;
        return FALSE;
    }
    lerp->step = duration;
    return TRUE;
}

void FrontierBrainIntro_SetVec(VecFx32 *vec, fx32 x, fx32 y, fx32 z) {
    VecFx32 v = { x, y, z };
    *vec = v;
}

#pragma force_active on
#pragma opt_common_subs off
#pragma opt_propagation off
void FrontierBrainIntro_LoadBackground(FrontierBrainIntro *intro, const FrontierBrainIntroArgs *args) {
    NNSG2dPaletteData *plttData;
    void *buffer;

    GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd1InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);
    PaletteData_LoadNarc(intro->paletteData, NARC_a_1_0_9, args->bgPalette, HEAP_ID_101, PLTTBUF_MAIN_BG, 0x20, 0xC0);
    GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, args->bgChar, intro->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_101);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, args->bgScreen, intro->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_101);
    BgTilemapRectChangePalette(intro->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 32, 32, 12);
    ScheduleBgTilemapBufferTransfer(intro->bgConfig, GF_BG_LYR_MAIN_1);
    buffer = GfGfxLoader_GetPlttData(NARC_a_1_0_9, args->bgPalette, &plttData, HEAP_ID_101);
    MIi_CpuCopy16(plttData->pRawData, intro->bgPalettes[0], sizeof(intro->bgPalettes));
    Heap_Free(buffer);
    intro->paletteTask = SysTask_CreateOnMainQueue(FrontierBrainIntro_CyclePaletteTask, intro, 1100);
}
#pragma opt_propagation reset
#pragma opt_common_subs reset
#pragma force_active reset

void FrontierBrainIntro_CyclePaletteTask(SysTask *task, void *data) {
    FrontierBrainIntro *intro = data;
    if (++intro->paletteTimer >= 0) {
        intro->paletteTimer = 0;
        if (++intro->paletteFrame >= 8) {
            intro->paletteFrame = 0;
        }
        PaletteData_LoadPalette(intro->paletteData, intro->bgPalettes[intro->paletteFrame], PLTTBUF_MAIN_BG, 0xC0, 0x20);
    }
}

void FrontierBrainIntro_StartWindowAnim(FrontierBrainIntro *intro, int close) {
    FrontierBrainIntroWindowAnim *anim = &intro->windowAnim;
    MI_CpuFill8(anim, 0, sizeof(FrontierBrainIntroWindowAnim));
    if (close == FALSE) {
        anim->top = 0x5000;
        anim->bottom = 0x5000;
        SysTask_CreateOnMainQueue(FrontierBrainIntro_OpenWindowTask, anim, 1000);
    } else {
        anim->top = 0x2E00;
        anim->bottom = 0x7200;
        SysTask_CreateOnMainQueue(FrontierBrainIntro_CloseWindowTask, anim, 1000);
    }
}

void FrontierBrainIntro_OpenWindowTask(SysTask *task, void *data) {
    FrontierBrainIntroWindowAnim *anim = data;
    if (anim->state == 0) {
        anim->top -= 0x800;
        anim->bottom += 0x800;
        if (anim->top <= 0x2E00) {
            anim->top = 0x2E00;
            anim->bottom = 0x7200;
            anim->state++;
        }
        anim->win0Left = 0;
        anim->win0Top = anim->top >> 8;
        anim->win0Right = 0xFF;
        anim->win0Bottom = anim->bottom >> 8;
        anim->win1Left = 1;
        anim->win1Top = anim->top >> 8;
        anim->win1Right = 0;
        anim->win1Bottom = anim->bottom >> 8;
    } else {
        anim->finished = TRUE;
        SysTask_Destroy(task);
    }
}

void FrontierBrainIntro_CloseWindowTask(SysTask *task, void *data) {
    FrontierBrainIntroWindowAnim *anim = data;
    if (anim->state == 0) {
        anim->top += 0x800;
        anim->bottom -= 0x800;
        if (anim->top >= 0x5000) {
            anim->top = 0x5000;
            anim->bottom = 0x5000;
            anim->state++;
        }
        anim->win0Left = 0;
        anim->win0Top = anim->top >> 8;
        anim->win0Right = 0xFF;
        anim->win0Bottom = anim->bottom >> 8;
        anim->win1Left = 1;
        anim->win1Top = anim->top >> 8;
        anim->win1Right = 0;
        anim->win1Bottom = anim->bottom >> 8;
    } else {
        anim->finished = TRUE;
        SysTask_Destroy(task);
    }
}

BOOL FrontierBrainIntro_SlideInBrain(FrontierBrainIntro *intro, FrontierBrainIntroSprites *sprites, FrontierBrainIntroSlide *slide) {
    switch (slide->state) {
    case 0:
        ManagedSprite_SetPositionXYWithSubscreenOffset(sprites->brainSprite, (s16)(intro->x + 256), (s16)(intro->y + 80), FX32_CONST(512));
        ManagedSprite_SetDrawFlag(sprites->brainSprite, TRUE);
        slide->x = 0x10000;
        slide->state++;
        break;
    case 1:
        slide->x -= 0xF00;
        if (slide->x <= 0xD000) {
            slide->x = 0xD000;
            slide->state++;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(sprites->brainSprite, (s16)(slide->x >> 8), (s16)(intro->y + 80), FX32_CONST(512));
        break;
    default:
        return TRUE;
    }
    return FALSE;
}
