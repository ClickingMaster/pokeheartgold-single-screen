#include "global.h"

#include "constants/sndseq.h"

#include "application/dress_up/dress_up_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0215.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "font_sprite.h"
#include "msgdata.h"
#include "options.h"
#include "render_window.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "text.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// Editor sub screen (message box, accessory count meter, an unused countdown with its pulse tween), message and
// sprite resource helpers shared by the buttons, and the backdrop animation of the finished design.

void TextOBJ_UpdatePosition(TextOBJ *object);

void DressUpButton_PlaySE(DressUpButton *button, u32 se, u32 trigger, u32 state) {
    if (state == trigger) {
        PlaySE((u16)se);
    }
}

// message centred in an off-screen window, for a text button
Window *DressUpButton_CreateTextBitmap(DressUpGfx *gfx, NarcId narcId, int bank, int msgId, int width, int height) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, bank, HEAP_ID_13);
    String *string;
    Window *window;
    int x;
    GF_ASSERT(msgData != NULL);
    string = NewString_ReadMsgData(msgData, msgId);
    window = AllocWindows(HEAP_ID_14, 1);
    InitWindow(window);
    AddTextWindowTopLeftCorner(gfx->bgConfig, window, width, height, 0, 0);
    x = FontID_String_GetCenterAlignmentX(2, string, 0, width * 8);
    AddTextPrinterParameterizedWithColor(window, 2, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 3), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    return window;
}

void DressUpButton_DeleteTextBitmap(Window *window) {
    WindowArray_Delete(window, 1);
}

void DressUpButton_MoveBy(DressUpButton *button, int dx, int dy) {
    VecFx32 pos = *Sprite_GetMatrixPtr(button->sprite);
    pos.x += dx << FX32_SHIFT;
    pos.y += dy << FX32_SHIFT;
    Sprite_SetMatrix(button->sprite, &pos);
}

void DressUpButton_MoveTextBy(DressUpTextButton *button, int dx, int dy) {
    DressUpButton_MoveBy(&button->button, dx, dy);
    TextOBJ_UpdatePosition(button->textObj);
}

#pragma force_active on

void DressUpHud_Init(DressUpHud *hud, const DressUpHudTemplate *template, u32 flags) {
    if (flags & DRESS_UP_HUD_CLEAR) {
        memset(hud, 0, sizeof(DressUpHud));
    }
    if (flags & DRESS_UP_HUD_SUB_BG) {
        DressUpHud_LoadSubBg(hud, template->bgConfig);
    }
    if (flags & DRESS_UP_HUD_MSG_WINDOW) {
        DressUpHud_AddWindow(&hud->msgWindow, template->bgConfig, 2, 19, 27, 4, 31, TRUE);
    }
    if (flags & DRESS_UP_HUD_COUNT_ICONS) {
        DressUpHud_CreateCountIcons(&hud->countIcons, template->spriteList, template->resMans, template->countMax, template->narc);
    }
    if (flags & DRESS_UP_HUD_COUNTDOWN) {
        DressUpCountdown_Create(&hud->countdown, template->spriteList, template->resMans, template->countdownSeconds, template->bgConfig, template->countdownOutput, template->narc);
    }
    if (flags & DRESS_UP_HUD_INFO_WINDOW) {
        DressUpHud_AddWindow(&hud->infoWindow, template->bgConfig, 2, 1, 27, 2, 139, TRUE);
        FillWindowPixelBuffer(hud->infoWindow, 15);
        DressUp_PrintMsg(hud->infoWindow, NARC_msgdata_msg, template->infoMsgBank, 6, 0, 0, MAKE_TEXT_COLOR(1, 2, 15), TEXT_SPEED_NOTRANSFER);
        DressUp_PrintMsg(hud->infoWindow, NARC_msgdata_msg, template->infoMsgBank, template->infoMsgId, 72, 0, MAKE_TEXT_COLOR(1, 2, 15), TEXT_SPEED_NOTRANSFER);
        DrawFrameAndWindow2(hud->infoWindow, FALSE, 1, 1);
    }
    hud->spriteList = template->spriteList;
    hud->resMans = template->resMans;
    DressUpHud_LoadFrameAndFontPal(template->bgConfig, template->options);
    hud->frame = Options_GetFrame(template->options);
    hud->textFrameDelay = Options_GetTextFrameDelay(template->options);
    hud->flags |= flags;
}

void DressUpHud_Free(DressUpHud *hud) {
    if (hud->flags & DRESS_UP_HUD_SUB_BG) {
        DressUpHud_UnloadSubBg(hud);
        hud->flags &= ~DRESS_UP_HUD_SUB_BG;
    }
    if (hud->flags & DRESS_UP_HUD_MSG_WINDOW) {
        DressUpHud_DeleteWindow(hud->msgWindow);
        hud->flags &= ~DRESS_UP_HUD_MSG_WINDOW;
    }
    if (hud->flags & DRESS_UP_HUD_COUNT_ICONS) {
        DressUpHud_DestroyCountIcons(&hud->countIcons, hud->resMans);
        hud->flags &= ~DRESS_UP_HUD_COUNT_ICONS;
    }
    if (hud->flags & DRESS_UP_HUD_COUNTDOWN) {
        DressUpCountdown_Destroy(&hud->countdown, hud->resMans);
        hud->flags &= ~DRESS_UP_HUD_COUNTDOWN;
    }
    if (hud->flags & DRESS_UP_HUD_INFO_WINDOW) {
        DressUpHud_DeleteWindow(hud->infoWindow);
        hud->flags &= ~DRESS_UP_HUD_INFO_WINDOW;
    }
    memset(hud, 0, sizeof(DressUpHud));
}

#pragma force_active reset

void DressUpHud_UpdateCountdown(DressUpHud *hud) {
    if (hud->flags & DRESS_UP_HUD_COUNTDOWN) {
        DressUpCountdown_Update(&hud->countdown);
    }
}

void DressUpHud_PrintMsgInstant(DressUpHud *hud, NarcId narcId, int bank, int msgId) {
    GF_ASSERT(hud->flags & DRESS_UP_HUD_MSG_WINDOW);
    DressUp_PrintMsgFramed(hud->msgWindow, narcId, bank, msgId, 0, 0, TEXT_SPEED_NOTRANSFER);
}

u8 DressUpHud_PrintMsg(DressUpHud *hud, NarcId narcId, int bank, int msgId) {
    GF_ASSERT(hud->flags & DRESS_UP_HUD_MSG_WINDOW);
    return DressUp_PrintMsgFramedKeepString(hud->msgWindow, narcId, bank, msgId, 0, 0, hud->textFrameDelay, &hud->msgString);
}

void DressUpHud_FreeMsgString(DressUpHud *hud) {
    String_Delete(hud->msgString);
    hud->msgString = NULL;
}

void DressUpHud_SetAccessoryCount(DressUpHud *hud, int count) {
    DressUpHud_SetCountIcons(&hud->countIcons, count);
}

void DressUpHud_LoadSubBg(DressUpHud *hud, BgConfig *bgConfig) {
    DressUpBgScreenTemplate template;

    template.bgConfig = bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = 0xE0;
    template.plttFileId = 0xE1;
    template.screenFileId = 0xE2;
    template.x = 0;
    template.y = 0;
    template.layer = GF_BG_LYR_SUB_0;
    template.plttCount = 1;
    template.plttSlot = 0;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    DressUpBgScreen_LoadFromTemplate(&hud->subBg, &template);
}

void DressUpHud_LoadFrameAndFontPal(BgConfig *bgConfig, Options *options) {
    u8 frame = Options_GetFrame(options);

    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_1, 1, 1, frame, HEAP_ID_14);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_14);
}

void DressUpHud_AddWindow(Window **window, BgConfig *bgConfig, int x, int y, int width, int height, int baseTile, BOOL drawFrame) {
    *window = AllocWindows(HEAP_ID_14, 1);
    InitWindow(*window);
    AddWindowParameterized(bgConfig, *window, GF_BG_LYR_SUB_1, x, y, width, height, 2, baseTile);
    FillWindowPixelBuffer(*window, 15);
    if (drawFrame != FALSE) {
        DrawFrameAndWindow2(*window, FALSE, 1, 1);
    }
    CopyWindowToVram(*window);
}

void DressUpHud_UnloadSubBg(DressUpHud *hud) {
    DressUpBgScreen_Unload(&hud->subBg);
}

void DressUpHud_DeleteWindow(Window *window) {
    WindowArray_Delete(window, 1);
}

u8 DressUp_PrintMsgFramed(Window *window, NarcId narcId, int bank, int msgId, int x, int y, int textSpeed) {
    FillWindowPixelBuffer(window, 15);
    u8 printerId = DressUp_PrintMsg(window, narcId, bank, msgId, x, y, MAKE_TEXT_COLOR(1, 2, 15), textSpeed);
    DrawFrameAndWindow2(window, FALSE, 1, 1);
    return printerId;
}

u8 DressUp_PrintMsgFramedKeepString(Window *window, NarcId narcId, int bank, int msgId, int x, int y, int textSpeed, String **string) {
    FillWindowPixelBuffer(window, 15);
    u8 printerId = DressUp_PrintMsgKeepString(window, narcId, bank, msgId, x, y, MAKE_TEXT_COLOR(1, 2, 15), textSpeed, string);
    DrawFrameAndWindow2(window, FALSE, 1, 1);
    return printerId;
}

u8 DressUp_PrintMsg(Window *window, NarcId narcId, int bank, int msgId, int x, int y, u32 color, int textSpeed) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, bank, HEAP_ID_13);
    GF_ASSERT(msgData);
    String *string = NewString_ReadMsgData(msgData, msgId);
    u8 printerId = AddTextPrinterParameterizedWithColor(window, 1, string, x, y, textSpeed, color, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    return printerId;
}

// as DressUp_PrintMsg, but the String is kept in *string until the printer is done with it
u8 DressUp_PrintMsgKeepString(Window *window, NarcId narcId, int bank, int msgId, int x, int y, u32 color, int textSpeed, String **string) {
    GF_ASSERT(!*string);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, bank, HEAP_ID_13);
    GF_ASSERT(msgData);
    *string = NewString_ReadMsgData(msgData, msgId);
    u8 printerId = AddTextPrinterParameterizedWithColor(window, 1, *string, x, y, textSpeed, color, NULL);
    DestroyMsgData(msgData);
    return printerId;
}

#pragma force_active on

void DressUpHud_CreateCountIcons(DressUpCountIcons *icons, SpriteList *spriteList, GF_2DGfxResMan **resMans, int count, NARC *narc) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;

    struct {
        int y, row;
    } loop;

    int col, index, x;

    DressUp_LoadSpriteRes(icons->resources, resMans, HEAP_ID_14, narc, 103, 225, 102, 101, 2, 2000);
    DressUp_MakeSpriteHeader(icons->resources, resMans, &header, 0);
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    loop.row = 0;
    template.priority = 0;
    template.heapID = HEAP_ID_14;
    template.spriteList = spriteList;
    loop.y = 104;
    index = loop.row;
    for (; loop.row < 2; loop.y += 18, loop.row++) {
        template.position.y = loop.y;
        template.position.y = (s32)((u32) template.position.y << FX32_SHIFT);
        template.position.y += 512 * FX32_ONE; // sub screen
        for (col = 0, x = 38; col < 10; x += 18, col++) {
            template.position.x = x;
            template.position.x = (s32)((u32) template.position.x << FX32_SHIFT);
            icons->sprites[index + col] = Sprite_Create(&template);
            Sprite_SetAnimCtrlSeq(icons->sprites[index + col], 1);
            if (col + index >= count) {
                Sprite_SetDrawFlag(icons->sprites[index + col], FALSE);
            }
        }
        index += 10;
    }
}

#pragma push
#pragma opt_lifetimes off

// icons below count show as used (seq 0), the others as free (seq 1)
void DressUpHud_SetCountIcons(DressUpCountIcons *icons, int count) {
    int i = icons->count;
    if (i < count) {
        for (; i < count; i++) {
            Sprite_SetAnimCtrlSeq(icons->sprites[i], 0);
        }
    } else if (i > count) {
        for (int j = i - 1; j >= count; j--) {
            Sprite_SetAnimCtrlSeq(icons->sprites[j], 1);
        }
    }
    icons->count = count;
}

#pragma pop
#pragma force_active reset

void DressUpHud_DestroyCountIcons(DressUpCountIcons *icons, GF_2DGfxResMan **resMans) {
    int i = 0;
    DressUpCountIcons *cursor = icons;
    do {
        Sprite_Delete(cursor->sprites[0]);
        i++;
        cursor = (DressUpCountIcons *)((u8 *)cursor + sizeof(Sprite *));
    } while (i < DRESS_UP_COUNT_ICON_NUM);
    DressUp_FreeSpriteRes(icons->resources, resMans);
}

void DressUp_LoadSpriteRes(SpriteResource **resources, GF_2DGfxResMan **resMans, enum HeapID heapID, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttCount, int baseId) {
    resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CHAR], narc, charFileId, FALSE, baseId + charFileId, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources[GF_GFX_RES_TYPE_CHAR]);
    GF2DGfxResObj_FreeData(resources[GF_GFX_RES_TYPE_CHAR]);

    resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_PLTT], narc, plttFileId, FALSE, baseId + plttFileId, NNS_G2D_VRAM_TYPE_2DSUB, plttCount, heapID);
    SpriteTransfer_CreatePlttTransferTask(resources[GF_GFX_RES_TYPE_PLTT]);
    GF2DGfxResObj_FreeData(resources[GF_GFX_RES_TYPE_PLTT]);

    resources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CELL], narc, cellFileId, FALSE, baseId + cellFileId, GF_GFX_RES_TYPE_CELL, heapID);
    resources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_ANIM], narc, animFileId, FALSE, baseId + animFileId, GF_GFX_RES_TYPE_ANIM, heapID);
}

void DressUp_FreeSpriteRes(SpriteResource **resources, GF_2DGfxResMan **resMans) {
    SpriteTransfer_DeleteCharTransferTask(resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(resources[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_CHAR], resources[GF_GFX_RES_TYPE_CHAR]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_PLTT], resources[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_CELL], resources[GF_GFX_RES_TYPE_CELL]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_ANIM], resources[GF_GFX_RES_TYPE_ANIM]);
}

#pragma force_active on

void DressUp_MakeSpriteHeader(SpriteResource **resources, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority) {
    int charId = GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_CHAR]);
    int plttId = GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_PLTT]);
    int cellId = GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_CELL]);
    CreateSpriteResourcesHeader(header, charId, plttId, cellId, GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_ANIM]), -1, -1, FALSE, priority, resMans[GF_GFX_RES_TYPE_CHAR], resMans[GF_GFX_RES_TYPE_PLTT], resMans[GF_GFX_RES_TYPE_CELL], resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

void DressUpCountdown_Create(DressUpCountdown *countdown, SpriteList *spriteList, GF_2DGfxResMan **resMans, int seconds, BgConfig *bgConfig, int *output, NARC *narc) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;
    int i, x;

    DressUp_LoadSpriteRes(countdown->resources, resMans, HEAP_ID_14, narc, 229, 230, 228, 227, 2, 3000);
    DressUp_MakeSpriteHeader(countdown->resources, resMans, &header, 0);
    template.spriteList = spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_14;
    template.position.y = 58 << FX32_SHIFT;
    template.priority = 0;
    template.position.y += 512 * FX32_ONE; // sub screen
    for (i = 0, x = 103; i < 2; x += 24, i++) {
        template.position.x = x;
        template.position.x <<= FX32_SHIFT;
        countdown->digits[i] = Sprite_Create(&template);
    }
    countdown->value = seconds;
    countdown->frames = seconds * 30;
    countdown->output = output;
    countdown->output[0] = seconds;
    countdown->output[2] = seconds;
    countdown->warning = FALSE;
    DressUpCountdown_DrawDigits(countdown);
    DressUpHud_AddWindow(&countdown->window, bgConfig, 10, 8, 14, 4, 193, FALSE);
    FillWindowPixelBuffer(countdown->window, 0);
    DressUp_PrintMsg(countdown->window, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00004, 0, 4, MAKE_TEXT_COLOR(1, 2, 0), TEXT_SPEED_NOTRANSFER);
    DressUp_PrintMsg(countdown->window, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00005, 72, 4, MAKE_TEXT_COLOR(1, 2, 0), TEXT_SPEED_NOTRANSFER);
    CopyWindowToVram(countdown->window);
}

#pragma force_active reset

void DressUpCountdown_Destroy(DressUpCountdown *countdown, GF_2DGfxResMan **resMans) {
    if (countdown->tasks[0]) {
        SysTask_Destroy(countdown->tasks[0]);
    }
    if (countdown->tasks[1]) {
        SysTask_Destroy(countdown->tasks[1]);
    }
    DressUp_FreeSpriteRes(countdown->resources, resMans);
    DressUpHud_DeleteWindow(countdown->window);
    memset(countdown, 0, sizeof(DressUpCountdown));
}

void DressUpCountdown_Update(DressUpCountdown *countdown) {
    DressUpCountdown_DecFrames(countdown);
    DressUpCountdown_Sync(countdown);
    DressUpCountdown_DrawDigits(countdown);
    DressUpCountdown_UpdatePulse(&countdown->pulse);
}

// follows the owner's seconds counter; each change in the last ten seconds pulses the digits
void DressUpCountdown_Sync(DressUpCountdown *countdown) {
    s32 seconds = countdown->output[0];
    if (countdown->value != seconds) {
        countdown->value = seconds;
        if (seconds <= 10) {
            DressUpCountdown_StartPulse(countdown, &countdown->pulse);
            PlaySE(SEQ_SE_DP_HYUN2);
        }
    }
}

#pragma force_active on
#pragma push
#pragma opt_treetransformation off

void DressUpCountdown_DrawDigits(DressUpCountdown *countdown) {
    struct {
        volatile int value, digit;
    } state;

    state.value = countdown->value;
    int divisor = 1;
    int i;
    DressUpCountdown *cursor;
    divisor *= 10;
    i = 0;
    cursor = countdown;
    for (; i < 2; i++) {
        int quotient = state.value / divisor;
        state.digit = quotient;
        if (quotient > 10) {
            GF_AssertFail();
        }
        Sprite_SetAnimCtrlSeq(cursor->digits[0], state.digit);
        u32 consumed = (u32)state.digit * (u32)divisor;
        state.value = (s32)((u32)state.value - consumed);
        divisor /= 10;
        if (countdown->warning == FALSE && countdown->value <= 10) {
            Sprite_SetPalIndexRespectVramOffset(cursor->digits[0], 1);
            if (i == 1) {
                countdown->warning = TRUE;
            }
        }
        cursor = (DressUpCountdown *)((u8 *)cursor + sizeof(Sprite *));
    }
}

#pragma pop
#pragma force_active reset

void DressUpCountdown_DecFrames(DressUpCountdown *countdown) {
    s32 frames = countdown->frames - 1;
    if (frames >= 0) {
        countdown->frames = frames;
    }
}

void DressUpTween_Init(DressUpTween *tween, fx32 start, fx32 end, int frames) {
    tween->value = start;
    tween->start = start;
    tween->distance = end - start;
    tween->frames = frames;
    tween->frame = 0;
}

BOOL DressUpTween_Step(DressUpTween *tween) {
    int next;
    fx32 value;
    value = FX_Div((fx32)(((s64)tween->distance * (tween->frame << FX32_SHIFT) + FX32_HALF) >> FX32_SHIFT), tween->frames << FX32_SHIFT);
    tween->value = value + tween->start;
    next = tween->frame + 1;
    if (next <= tween->frames) {
        tween->frame = next;
        return FALSE;
    }
    tween->frame = tween->frames;
    return TRUE;
}

// digits grow by 0.15 per second below ten, then shrink back over 16 frames around their centre
void DressUpCountdown_StartPulse(DressUpCountdown *countdown, DressUpCountdownPulse *pulse) {
    VecFx32 pos;
    fx32 scale;
    int i;

    pos.y = 58 << FX32_SHIFT;
    pos.y += 512 * FX32_ONE; // sub screen
    for (i = 0; i < 2; i++) {
        pulse->digits[i] = countdown->digits[i];
        Sprite_SetAffineOverwriteMode(pulse->digits[i], 2);
        pos.x = 103 + 24 * i;
        pos.x <<= FX32_SHIFT;
        pulse->positions[i] = pos;
    }

    i = 10 - countdown->value;
    if (i > 0) {
        scale = FX_Mul(i << FX32_SHIFT, 0x266) + FX32_ONE;
    } else {
        scale = FX32_ONE;
    }
    DressUpTween_Init(&pulse->scale, scale, FX32_ONE, 16);
    scale = FX_Mul(scale, 0x18000) - 0x18000;
    DressUpTween_Init(&pulse->x, scale, 0, 16);
    DressUpTween_Init(&pulse->y, scale, 0, 16);
    pulse->active = TRUE;
}

void DressUpCountdown_UpdatePulse(DressUpCountdownPulse *pulse) {
    VecFx32 scale;
    VecFx32 pos;
    BOOL done;
    if (pulse->active == FALSE) {
        return;
    }
    done = DressUpTween_Step(&pulse->scale);
    DressUpTween_Step(&pulse->x);
    DressUpTween_Step(&pulse->y);
    scale.x = pulse->scale.value;
    scale.y = pulse->scale.value;
    scale.z = pulse->scale.value;
    Sprite_SetAffineScale(pulse->digits[0], &scale);
    Sprite_SetAffineScale(pulse->digits[1], &scale);
    pos = pulse->positions[0];
    pos.x -= pulse->x.value;
    pos.y -= pulse->y.value;
    Sprite_SetMatrix(pulse->digits[0], &pos);
    pos = pulse->positions[1];
    pos.y -= pulse->y.value;
    Sprite_SetMatrix(pulse->digits[1], &pos);
    if (done) {
        pulse->active = FALSE;
    }
}

// the backdrop alternates with its second screen (file backdrop * 4 + 0x89) every 16 ticks
void DressUp_StartBackdropAnim(DressUpBackdropAnim *anim, DressUpCanvas *canvas, enum HeapID heapID) {
    DressUpBgScreenAnimTemplate template;
    anim->canvas = canvas;
    template.screen = &canvas->backdrop;
    template.altScreenFileId = canvas->backdropId * 4 + 0x89;
    template.delay = 16;
    template.heapID = heapID;
    DressUpBgScreen_InitAltAnim(&anim->anim, &template);
}

void DressUp_UpdateBackdropAnim(DressUpBackdropAnim *anim) {
    DressUpBgScreen_UpdateAltAnim(&anim->anim, 1);
}

void DressUp_StopBackdropAnim(DressUpBackdropAnim *anim) {
    DressUpBgScreen_StopAltAnim(&anim->anim);
    memset(anim, 0, sizeof(DressUpBackdropAnim));
}
