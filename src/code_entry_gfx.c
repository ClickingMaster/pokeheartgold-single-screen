#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0038.h"

#include "bg_window.h"
#include "code_entry_internal.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "render_window.h"
#include "sprite_system.h"
#include "text.h"
#include "touchscreen.h"

u32 GetUserFrameGfx2PaletteMember(u32 frame);

static void CodeEntry_SetButtonLabelPosition(TextOBJ *label, int x, int y);
static void CodeEntry_FreeButtonLabels(CodeEntryApp *app);
static void CodeEntry_LoadButtonLabelPalette(CodeEntryApp *app);
static void CodeEntry_CreateButtonLabel(CodeEntryApp *app, int button, int x, int y, int palette);
static void CodeEntry_PrintPrompt(Window *window, int msgNo);

static const OamCharTransferParam sCodeEntryOamCharTransferParam = {
    .maxTasks = 96,
    .sizeMain = 0x10000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_64K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const SpriteResourceCountsListUnion sCodeEntrySpriteResourceCounts = {
    .numChar = 96,
    .numPltt = 32,
    .numCell = 64,
    .numAnim = 64,
    .numMcel = 16,
    .numManm = 16,
};

static const OamManagerParam sCodeEntryOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

void CodeEntry_InitSpriteSystem(CodeEntryApp *app) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;

    app->spriteSystem = SpriteSystem_Alloc(HEAP_ID_108);
    oamManagerParam = sCodeEntryOamManagerParam;
    oamCharTransferParam = sCodeEntryOamCharTransferParam;
    SpriteSystem_Init(app->spriteSystem, &oamManagerParam, &oamCharTransferParam, 0x20);
    {
        SpriteResourceCountsListUnion counts = sCodeEntrySpriteResourceCounts;
        app->spriteManager = SpriteManager_New(app->spriteSystem);
        if (!SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 0x80)) {
            GF_AssertFail();
        }
        if (!SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->spriteManager, &counts)) {
            GF_AssertFail();
        }
    }
}

void CodeEntry_LoadGraphics(CodeEntryApp *app) {
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *paletteData;

    spriteSystem = app->spriteSystem;
    spriteManager = app->spriteManager;
    paletteData = app->paletteData;
    bgConfig = app->bgConfig;
    narc = app->narc;
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_108);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 14, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_108);
    PaletteData_LoadNarc(paletteData, NARC_a_1_9_0, 13, HEAP_ID_108, PLTTBUF_MAIN_BG, 0x20, 0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 15, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_108);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_108);
    PaletteData_LoadNarc(paletteData, NARC_a_1_9_0, 16, HEAP_ID_108, PLTTBUF_SUB_BG, 0x20, 0);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 1, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 2, FALSE, 1000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 3, FALSE, 1000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 5, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 4, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 6, FALSE, 1001);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 7, FALSE, 1001);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 9, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 1002);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 8, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1002);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 10, FALSE, 1002);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 11, FALSE, 1002);
    int frame = Options_GetFrame(app->args.options);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_0, 1, 10, (u8)frame, HEAP_ID_108);
    PaletteData_LoadNarc(paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_108, PLTTBUF_SUB_BG, 0x20, 0xB0);
    PaletteData_LoadNarc(paletteData, NARC_graphic_font, 8, HEAP_ID_108, PLTTBUF_SUB_BG, 0x20, 0xC0);
}

void CodeEntry_FreeGraphics(CodeEntryApp *app) {
    int i;

    for (i = 0; i < app->digitCount; i++) {
        Sprite_DeleteAndFreeResources(app->digits[i].sprite);
    }
    for (i = 0; i < app->separatorCount; i++) {
        Sprite_DeleteAndFreeResources(app->separators[i].sprite);
    }
    for (i = 0; i < 2; i++) {
        Sprite_DeleteAndFreeResources(app->buttons[i].sprite);
    }
    for (i = 0; i < 3; i++) {
        Sprite_DeleteAndFreeResources(app->cursors[i].sprite);
    }
    CodeEntry_FreeButtonLabels(app);
    RemoveWindow(&app->promptWindow);
}

void CodeEntry_CreateDigitSprites(CodeEntryApp *app) {
    int placed = 0;
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;
    int i, x;
    CodeEntryCell *separator, *digit;
    u32 value;
    ManagedSpriteTemplate template;

    template.x = placed;
    template.y = placed;
    template.z = placed;
    template.animation = placed;
    template.drawPriority = 10;
    template.bgPriority = placed;
    template.vramTransfer = placed;
    template.pal = placed;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;

    // the fixed digits show fixedValue, last digit first
    value = app->args.fixedValue;
    i = app->firstEditable - 1;
    if (i >= 0) {
        CodeEntryCell *fixed = &app->digits[i];
        do {
            fixed->value = value % 10 + 1;
            value /= 10;
            fixed--;
            i--;
        } while (i >= 0);
    }

    i = 0;
    if (app->digitCount + app->separatorCount > 0) {
        x = 76;
        separator = app->digits;
        digit = app->digits;
        do {
            // separators are counted from the start of digits[] (they follow it), so separator[16] is separators[0]
            if (app->separatorCount && i == placed + separator[16].value + 1) {
                separator[16].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
                ManagedSprite_SetPositionXY(separator[16].sprite, x, 24);
                ManagedSprite_SetAnim(separator[16].sprite, 22);
                ManagedSprite_TickFrame(separator[16].sprite);
                separator++;
                placed++;
            } else {
                digit->sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
                ManagedSprite_SetPositionXY(digit->sprite, x, 24);
                ManagedSprite_SetAnim(digit->sprite, CodeEntry_GetDigitAnim(digit->value, digit->active));
                ManagedSprite_SetAffineOverwriteMode(digit->sprite, 2);
                ManagedSprite_TickFrame(digit->sprite);
                digit++;
            }
            i++;
            x += 8;
        } while (i < app->digitCount + app->separatorCount);
    }
}

void CodeEntry_CreateCursorSprites(CodeEntryApp *app) {
    ManagedSpriteTemplate template;
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.pal = 0;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 1001;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 1001;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 1001;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 1001;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    app->cursors[0].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    app->cursors[1].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    app->cursors[2].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    CodeEntry_MoveDigitCursor(app, app->firstEditable);
    ManagedSprite_SetAnim(app->cursors[0].sprite, 0);
    ManagedSprite_TickFrame(app->cursors[0].sprite);
    app->cursors[1].velocityX = 0;
    app->cursors[1].velocityY = 0;
    app->cursors[1].value = 1;
    CodeEntry_MoveKeyCursor(app, 0);
    ManagedSprite_SetAnim(app->cursors[1].sprite, app->cursors[1].value);
    ManagedSprite_TickFrame(app->cursors[1].sprite);
    ManagedSprite_SetOamMode(app->cursors[1].sprite, GX_OAM_MODE_XLU);
    app->cursors[2].velocityX = 0;
    app->cursors[2].velocityY = 0;
    app->cursors[2].value = 1;
    CodeEntry_MoveKeyCursor(app, 0);
    ManagedSprite_SetAnim(app->cursors[2].sprite, app->cursors[2].value);
    ManagedSprite_TickFrame(app->cursors[2].sprite);
    ManagedSprite_SetOamMode(app->cursors[2].sprite, GX_OAM_MODE_XLU);
    CodeEntry_SetCursorVisible(app, 1, FALSE);
    CodeEntry_SetCursorVisible(app, 2, FALSE);
}

typedef struct CodeEntryButtonSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    volatile int pal; // volatile: stored twice, see below
    NNS_G2D_VRAM_TYPE vram;
    int resIdList[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    int vramTransfer;
} CodeEntryButtonSpriteTemplate;

void CodeEntry_CreateButtonSprites(CodeEntryApp *app) {
    CodeEntryButtonSpriteTemplate template;
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 10;
    int noResource = (template.vram = NNS_G2D_VRAM_TYPE_2DMAIN) - 2;
    template.pal = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.pal = 0;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = noResource;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = noResource;
    app->buttons[0].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, (ManagedSpriteTemplate *)&template);
    template.pal = 1;
    app->buttons[1].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, (ManagedSpriteTemplate *)&template);
    ManagedSprite_SetPositionXY(app->buttons[0].sprite, (app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.left + app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.right) / 2, (app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.top + app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.bottom) / 2);
    ManagedSprite_SetAnim(app->buttons[0].sprite, 0);
    ManagedSprite_TickFrame(app->buttons[0].sprite);
    ManagedSprite_SetPositionXY(app->buttons[1].sprite, (app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.left + app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.right) / 2, (app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.top + app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.bottom) / 2);
    ManagedSprite_SetAnim(app->buttons[1].sprite, 0);
    ManagedSprite_TickFrame(app->buttons[1].sprite);
}

void CodeEntry_SetCursorVisible(CodeEntryApp *app, int cursor, int visible) {
    if (visible == TRUE) {
        ManagedSprite_SetDrawFlag(app->cursors[cursor].sprite, TRUE);
    } else {
        ManagedSprite_SetDrawFlag(app->cursors[cursor].sprite, FALSE);
    }
}

void CodeEntry_MoveDigitCursor(CodeEntryApp *app, int digit) {
    s16 x, y;
    if (digit >= app->firstEditable) {
        ManagedSprite *sprite = app->digits[digit].sprite;
        app->cursors[0].value = digit;
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        ManagedSprite_SetPositionXY(app->cursors[0].sprite, x, y + 0x10);
    }
}

void CodeEntry_MoveKeyCursor(CodeEntryApp *app, int key) {
    ManagedSprite_SetPositionXY(app->cursors[1].sprite, (app->hitboxes[key + 16].rect.left + app->hitboxes[key + 16].rect.right) / 2, (app->hitboxes[key + 16].rect.top + app->hitboxes[key + 16].rect.bottom) / 2);
}

void CodeEntry_MoveCursorToKey(CodeEntryApp *app, int key, int cursor) {
    ManagedSprite_SetPositionXY(app->cursors[cursor].sprite, (app->hitboxes[key + 16].rect.left + app->hitboxes[key + 16].rect.right) / 2, (app->hitboxes[key + 16].rect.top + app->hitboxes[key + 16].rect.bottom) / 2);
}

void CodeEntry_UpdateCursors(CodeEntryApp *app) {
    int i;

    ManagedSprite_TickFrame(app->cursors[0].sprite);
    ManagedSprite_TickFrame(app->cursors[1].sprite);
    ManagedSprite_TickFrame(app->cursors[2].sprite);
    for (i = 1; i < 3; i++) {
        int anim = ManagedSprite_GetActiveAnim(app->cursors[i].sprite);
        if (anim == 3) {
            if (!ManagedSprite_IsAnimated(app->cursors[i].sprite)) {
                ManagedSprite_SetAnim(app->cursors[i].sprite, app->cursors[i].value);
                if (app->touchActive == TRUE) {
                    CodeEntry_SetCursorVisible(app, 1, FALSE);
                } else {
                    CodeEntry_SetCursorVisible(app, 1, TRUE);
                }
                CodeEntry_SetCursorVisible(app, 2, FALSE);
            }
        } else {
            if (anim != app->cursors[i].value) {
                ManagedSprite_SetAnim(app->cursors[i].sprite, app->cursors[i].value);
            }
            if (ManagedSprite_GetActiveAnim(app->cursors[2].sprite) != 3) {
                if (app->touchActive == TRUE) {
                    CodeEntry_SetCursorVisible(app, 1, FALSE);
                } else {
                    CodeEntry_SetCursorVisible(app, 1, TRUE);
                }
            }
        }
    }
}

static void CodeEntry_SetButtonLabelPosition(TextOBJ *label, int x, int y) {
    if (label != NULL) {
        TextOBJ_SetPosition(label, x, y);
    }
}

// A pressed button shows its pressed frames (the label moves up by one pixel), then springs back.
void CodeEntry_UpdateButtons(CodeEntryApp *app) {
    int i;

    for (i = 0; i < 2; i++) {
        s16 x = (s16)((app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.left + app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.right) / 2) - 40;
        s16 y = (s16)((app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.top + app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.bottom) / 2) - 7;
        switch (app->buttons[i].value) {
        case 0:
            app->buttons[i].moveFrames = 0;
            break;
        case 1:
            app->buttons[i].moveFrames++;
            if (app->buttons[i].moveFrames == 1) {
                ManagedSprite_SetAnim(app->buttons[i].sprite, 1);
                CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y);
            } else if (app->buttons[i].moveFrames == 2) {
                ManagedSprite_SetAnim(app->buttons[i].sprite, 2);
                CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y - 1);
            } else if (app->buttons[i].moveFrames == 10) {
                ManagedSprite_SetAnim(app->buttons[i].sprite, 0);
                CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y);
                app->buttons[i].value++;
            }
            break;
        default:
            ManagedSprite_SetAnim(app->buttons[i].sprite, 0);
            CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y);
            app->buttons[i].value = 0;
            break;
        }
    }
}

int CodeEntry_GetDigitAnim(int value, int active) {
    int add = 0;
    if (active == FALSE) {
        add = 11;
    }
    return add + value;
}

void CodeEntry_MarkSelectedDigits(CodeEntryApp *app) {
    int i = 0;
    if (app->digitCount > 0) {
        CodeEntryCell *digit = app->digits;
        do {
            if (i >= app->selectedStart && i < app->selectedEnd) {
                digit->active = TRUE;
            } else {
                digit->active = FALSE;
            }
            i++;
            digit++;
        } while (i < app->digitCount);
    }
}

// Places the digits and separators: the selected group is spread out (32 px per digit), the others are packed
// (8 px). With animate, sets the sprites moving there over 2 frames instead.
void CodeEntry_LayoutDigits(CodeEntryApp *app, int animate) {
    s16 x, y;
    int i;
    int separator;
    s16 pos;

    pos = app->groupX[app->selectedGroup];
    separator = 0;
    for (i = 0; i < app->digitCount; i++) {
        if (i >= app->selectedStart && i < app->selectedEnd) {
            if (i == app->selectedStart) {
                pos += 20;
            } else {
                pos += 32;
            }
        } else {
            if (i == 0) {
                pos += 20;
            } else {
                pos += 8;
            }
        }
        ManagedSprite_GetPositionXY(app->digits[i].sprite, &x, &y);
        if (!animate) {
            ManagedSprite_SetPositionXY(app->digits[i].sprite, pos, y);
        } else {
            app->digits[i].velocityX = (pos - x) / 2;
            app->digits[i].velocityY = 0;
            app->digits[i].moveFrames = 2;
            app->digits[i].animStep = 0;
        }
        if (i == app->separators[separator].value && separator != app->separatorCount) {
            ManagedSprite_GetPositionXY(app->separators[separator].sprite, &x, &y);
            if (app->selectedStart == app->selectedEnd) {
                pos += 8;
            } else if (i > app->selectedStart && i < app->selectedEnd) {
                pos += 20;
            } else {
                pos += 8;
            }
            if (!animate) {
                ManagedSprite_SetPositionXY(app->separators[separator].sprite, pos, y);
            } else {
                app->separators[separator].velocityX = (pos - x) / 2;
                app->separators[separator].velocityY = 0;
                app->separators[separator].moveFrames = 2;
            }
            separator++;
        }
    }
}

void CodeEntry_UpdateDigitHitboxes(CodeEntryApp *app) {
    int i = 0;
    if (app->digitCount > 0) {
        CodeEntryCell *digit = app->digits;
        do {
            int halfWidth;
            int halfHeight;
            s16 y, x;
            if (i >= app->selectedStart && i < app->selectedEnd) {
                halfWidth = 16;
                halfHeight = 16;
            } else {
                halfWidth = 4;
                halfHeight = 8;
            }
            ManagedSprite_GetPositionXY(digit->sprite, &x, &y);
            digit->hitbox->rect.top = y - halfHeight;
            digit->hitbox->rect.left = x - halfWidth;
            digit->hitbox->rect.bottom = y + halfHeight;
            digit->hitbox->rect.right = x + halfWidth;
            i++;
            digit++;
        } while (i < app->digitCount);
    }
}

void CodeEntry_InitFont(CodeEntryApp *app) {
    app->fontSystem = FontSystem_NewInit(2, HEAP_ID_108);
    FontID_Alloc(2, HEAP_ID_108);
}

static void CodeEntry_FreeButtonLabels(CodeEntryApp *app) {
    FontID_Release(2);
    FontOAM_Delete(app->buttonLabels[0]);
    ObjCharTransfer_FreeCharSpace(&app->buttonLabelAllocs[0].charAlloc);
    FontOAM_Delete(app->buttonLabels[1]);
    ObjCharTransfer_FreeCharSpace(&app->buttonLabelAllocs[1].charAlloc);
    FontSystem_Delete(app->fontSystem);
}

static void CodeEntry_LoadButtonLabelPalette(CodeEntryApp *app) {
    SpriteSystem_LoadPaletteBuffer(app->paletteData, PLTTBUF_MAIN_OBJ, app->spriteSystem, app->spriteManager, NARC_graphic_font, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1003);
}

void CodeEntry_CreateButtonLabels(CodeEntryApp *app) {
    CodeEntry_LoadButtonLabelPalette(app);
    CodeEntry_CreateButtonLabel(app, 0, 0x4E, 0xA5, 0);
    CodeEntry_CreateButtonLabel(app, 1, 0xAC, 0xA5, 0);
}

static void CodeEntry_CreateButtonLabel(CodeEntryApp *app, int button, int x, int y, int palette) {
    TextOBJTemplate template;
    Window window;
    MsgData *msgData;
    String *string;
    s16 centerX, centerY;
    int size;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0038_bin, HEAP_ID_108);
    string = NewString_ReadMsgData(msgData, button + msg_0038_00002);
    InitWindow(&window);
    AddTextWindowTopLeftCorner(app->bgConfig, &window, 10, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&window, 2, string, FontID_String_GetCenterAlignmentX(2, string, 0, 80), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 2), NULL);
    size = FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_108);
    ObjCharTransfer_ReserveCharSpace(size, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &app->buttonLabelAllocs[button].charAlloc);
    centerX = (app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.left + app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.right) / 2;
    centerY = (app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.top + app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.bottom) / 2;
    template.fontSystem = app->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(app->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(app->spriteManager, 1003);
    template.sprite = NULL;
    template.offset = app->buttonLabelAllocs[button].charAlloc.offset;
    template.x = (s16)(centerX - 40);
    template.y = (s16)(centerY - 7);
    template.bgPriority = 0;
    template.drawPriority = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_108;
    app->buttonLabels[button] = FontOAM_Create(&template);
    TextOBJ_SetPalOffsetRespectVramOffset(app->buttonLabels[button], palette);
    String_Delete(string);
    DestroyMsgData(msgData);
    RemoveWindow(&window);
}

void CodeEntry_CreatePromptWindow(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int msgNo) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, (u8)bgId, (u8)x, (u8)y, (u8)width, height, 12, (u16)baseTile);
    DrawFrameAndWindow2(window, TRUE, 1, 11);
    FillWindowPixelBuffer(window, 15);
    CopyWindowToVram(window);
    CodeEntry_PrintPrompt(window, msgNo);
}

static void CodeEntry_PrintPrompt(Window *window, int msgNo) {
    MsgData *msgData;
    String *string;

    FillWindowPixelBuffer(window, 15);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0038_bin, HEAP_ID_108);
    string = NewString_ReadMsgData(msgData, msgNo);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterized(window, 1, string, 0, 0, TEXT_SPEED_INSTANT, NULL);
    CopyWindowToVram(window);
    String_Delete(string);
    DestroyMsgData(msgData);
}
