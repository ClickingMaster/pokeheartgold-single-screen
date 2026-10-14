#include "global.h"

#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_ring_drop.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "text.h"

// The UI's sprites (PokeathlonRingDropUi.sprites).
#define RING_DROP_UI_SPRITE_PLATE     0 // the own participant's plate
#define RING_DROP_UI_SPRITE_TIMER     1 // the timer label
#define RING_DROP_UI_SPRITE_ICONS     2 // the participants' Pokemon in the ring
#define RING_DROP_UI_SPRITE_SWITCHING 6 // replaces an icon while that Pokemon switches
#define RING_DROP_UI_SPRITE_RANKS     10
#define RING_DROP_UI_SPRITE_PICS      14 // the own next two team members (sub screen)
#define RING_DROP_UI_SPRITE_PIC_MARKS 16 // above them: ready (stamina at least 30)
#define RING_DROP_UI_SPRITE_COUNT     18

struct PokeathlonRingDropUi {
    PokeathlonCourseData *course;                      // 0x00
    enum HeapID heapId;                                // 0x04
    SpriteSystem *spriteSystem;                        // 0x08
    SpriteManager *spriteManager;                      // 0x0C
    PokeathlonCourseSpriteResources *resources;                                   // 0x10: the course sprite resources
    BgConfig *bgConfig;                                // 0x14
    MsgData *msgData;                                  // 0x18
    MessageFormat *msgFormat;                          // 0x1C
    u16 iconPalette;                                   // 0x20
    u8 participant;                                    // 0x22: the own one
    NARC *iconNarc;                                    // 0x24
    void *digitsBuffer;                                // 0x28
    NNSG2dScreenData *digits;                          // 0x2C: the score digits' tilemap
    Window nameWindow;                                 // 0x30
    Window nextNameWindow;                             // 0x40
    Window labelWindow;                                // 0x50
    ManagedSprite *sprites[RING_DROP_UI_SPRITE_COUNT]; // 0x60
    Sprite *timerDigits[2];                            // 0xA8
    String *memberNames[RING_DROP_TEAM_SIZE];          // 0xB0
    s8 switchDelays[RING_DROP_PARTICIPANTS];           // 0xBC
    u8 switchStep;                                     // 0xC0
    u16 score;                                         // 0xC2
    BOOL ranksHidden;                                  // 0xC4
}; // size: 0xC8

// ui->sprites[index], addressed from the start of the UI (as the code does it)
#define RING_DROP_UI_SPRITE(ui, index) (*(ManagedSprite **)((u8 *)(ui) + offsetof(PokeathlonRingDropUi, sprites) + (index) * sizeof(ManagedSprite *)))

// Course helpers (OVY_96).
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, u32 participant, u32 member, void *info);
void PokeathlonCourse_LoadMonIcon(void *sprite, void *info, u32 palette, u32 narc, u32 offset, u32 heapId);
void PokeathlonCourse_LoadMonPic(void *sprite, void *info, u32 mainScreen, u32 palette, u32 heapId);
Sprite *PokeathlonCourse_CreateTimerDigitSprite(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);

// Sprite templates (resources 10001 and 10000) and the windows: name, next name, label.
extern const ManagedSpriteTemplate gPokeathlonRingDropUiSpriteTemplate;
extern const ManagedSpriteTemplate gPokeathlonRingDropUiTimerSpriteTemplate;
extern const WindowTemplate gPokeathlonRingDropUiWindowTemplates[3];

#pragma force_active on

PokeathlonRingDropUi *PokeathlonRingDropUi_New(SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *resources, BgConfig *bgConfig, MsgData *msgData, MessageFormat *msgFormat, u8 participant, enum HeapID heapId, PokeathlonCourseData *course) {
    PokeathlonRingDropUi *ui;
    u8 *member;
    int i;

    GF_ASSERT(spriteSystem);
    GF_ASSERT(spriteManager);
    GF_ASSERT(resources);
    GF_ASSERT(bgConfig);
    GF_ASSERT(msgData);
    GF_ASSERT(msgFormat);
    ui = Heap_Alloc(heapId, sizeof(PokeathlonRingDropUi));
    MI_CpuFill8(ui, 0, sizeof(PokeathlonRingDropUi));
    ui->spriteSystem = spriteSystem;
    ui->spriteManager = spriteManager;
    ui->resources = resources;
    ui->participant = participant;
    ui->heapId = heapId;
    ui->bgConfig = bgConfig;
    ui->msgData = msgData;
    ui->msgFormat = msgFormat;
    ui->course = course;
    ui->iconNarc = NARC_New((NarcId)20, ui->heapId);
    ui->digitsBuffer = GfGfxLoader_GetScrnData((NarcId)242, 7, TRUE, &ui->digits, ui->heapId);
    ui->iconPalette = SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, (NarcId)20, GetMonIconPlttNaix(), FALSE, 3, 2, 10004);
    member = PokeathlonCourse_GetParticipantMons(course, PokeathlonCourse_GetNetId(course));
    i = 0;
    do {
        ui->memberNames[i] = String_New(11, ui->heapId);
        CopyU16ArrayToString(ui->memberNames[i], (const u16 *)(member + 18));
        i++;
        member += 40;
    } while (i < RING_DROP_TEAM_SIZE);
    PokeathlonRingDropUi_AddWindows(ui);
    return ui;
}

#pragma force_active reset

void PokeathlonRingDropUi_Init(PokeathlonRingDropUi *ui) {
    PokeathlonRingDropUi_CreateSprites(ui);
    PokeathlonRingDropUi_SetTimer(ui, 60);
    PokeathlonRingDropUi_DrawScore(ui, 0);
    PokeathlonRingDropUi_PrintNames(ui);
}

void PokeathlonRingDropUi_Free(PokeathlonRingDropUi *ui) {
    if (ui == NULL) {
        GF_AssertFail();
    }
    NARC_Delete(ui->iconNarc);
    {
        int i = 0;
        PokeathlonRingDropUi *cursor = ui;
        do {
            String_Delete(cursor->memberNames[0]);
            i++;
            cursor = (PokeathlonRingDropUi *)((u8 *)cursor + sizeof(String *));
        } while (i < RING_DROP_TEAM_SIZE);
    }
    Heap_Free(ui->digitsBuffer);
    {
        u8 *cursor = (u8 *)ui;
        int i = 0;
        cursor += offsetof(PokeathlonRingDropUi, nameWindow);
        do {
            RemoveWindow((Window *)cursor);
            i++;
            cursor += sizeof(Window);
        } while (i < 3);
    }
    {
        PokeathlonRingDropUi *cursor;
        int i;
        i = 0;
        cursor = ui;
        do {
            if (cursor->sprites[0] == NULL) {
                GF_AssertFail();
            }
            Sprite_DeleteAndFreeResources(cursor->sprites[0]);
            i++;
            cursor = (PokeathlonRingDropUi *)((u8 *)cursor + sizeof(ManagedSprite *));
        } while (i < RING_DROP_UI_SPRITE_COUNT);
    }
    Heap_Free(ui);
}

void PokeathlonRingDropUi_StopAnimations(PokeathlonRingDropUi *ui) {
    int i = 0;
    PokeathlonRingDropUi *cursor = ui;
    int zero = i;
    do {
        ManagedSprite_SetAnimateFlag(cursor->sprites[RING_DROP_UI_SPRITE_PIC_MARKS], zero);
        i++;
        cursor = (PokeathlonRingDropUi *)((u8 *)cursor + sizeof(ManagedSprite *));
    } while (i < 2);
}

// sorted: the participants by score, (score, participant) pairs. Ties share a rank. Redraws the own score.
#pragma force_active on

void PokeathlonRingDropUi_SetRanks(PokeathlonRingDropUi *ui, u16 participant, u16 score, const u8 *sorted) {
    int rank;
    int i;

    GF_ASSERT(ui != NULL);
    GF_ASSERT(participant < RING_DROP_PARTICIPANTS);
    rank = 0;
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++, sorted += 2) {
        ManagedSprite_SetAnimNoRestart(ui->sprites[RING_DROP_UI_SPRITE_RANKS + sorted[1]], rank + 6);
        if (i != 3 && sorted[0] != sorted[2]) {
            rank = i + 1;
        }
    }
    if (participant == PokeathlonCourse_GetNetId(ui->course) && score != ui->score) {
        ((void (*)(PokeathlonRingDropUi *, u8))PokeathlonRingDropUi_DrawScore)(ui, score);
        ui->score = score;
    }
}

#pragma force_active reset

// The ranks are hidden in the first and the last ten seconds.
void PokeathlonRingDropUi_SetTimer(PokeathlonRingDropUi *ui, s32 seconds) {
    u8 rest = (u8)(seconds % 100);
    u8 tens = (u8)(rest / 10);
    u8 ones = (u8)(rest % 10);
    int i;
    ui->ranksHidden = seconds < 10 || seconds > 55;
    i = 0;
    {
        PokeathlonRingDropUi *cursor = ui;
        int one = 1;
        do {
            int draw;
            if (ui->ranksHidden == FALSE) {
                draw = one;
            } else {
                draw = 0;
            }
            ManagedSprite_SetDrawFlag(cursor->sprites[RING_DROP_UI_SPRITE_RANKS], draw);
            i++;
            cursor = (PokeathlonRingDropUi *)((u8 *)cursor + sizeof(ManagedSprite *));
        } while (i < RING_DROP_PARTICIPANTS);
    }
    Sprite_SetAnimCtrlSeq(ui->timerDigits[0], tens + 1);
    Sprite_SetAnimCtrlSeq(ui->timerDigits[1], ones + 1);
}

void PokeathlonRingDropUi_ShowNextReady(PokeathlonRingDropUi *ui, BOOL next, BOOL afterNext) {
    ManagedSprite_SetDrawFlag(ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS], next);
    ManagedSprite_SetDrawFlag(ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS + 1], afterNext);
}

u16 PokeathlonRingDropUi_GetScore(PokeathlonRingDropUi *ui) {
    return ui->score;
}

// The own switch: the next team member's picture moves down to the front, the one after it comes in at the back.
// TRUE when done.
#pragma force_active on

BOOL PokeathlonRingDropUi_RunSwitch(PokeathlonRingDropUi *ui, u8 participant, u8 member) {
    u8 own = PokeathlonCourse_GetNetId(ui->course);
    u8 next = (member + 1) % RING_DROP_TEAM_SIZE;

    switch (ui->switchStep) {
    case 0: {
        s16 x, y;
        int distance;
        ManagedSprite_GetPositionXYWithSubscreenOffset(ui->sprites[RING_DROP_UI_SPRITE_PICS + 1], &x, &y, FX32_CONST(480));
        if (y < 216) {
            distance = 272 - y;
            if (distance < 0) {
                distance = -distance;
            }
            y += (s16)(distance >> 2);
            if (y > 216) {
                y = 216;
            }
            PokeathlonRingDropUi_SetPicPosition(ui, RING_DROP_UI_SPRITE_PICS + 1, x, y);
        } else {
            x = 40;
            y = -40;
            PokeathlonRingDropUi_SetPicPosition(ui, RING_DROP_UI_SPRITE_PICS + 1, x, y);
            PokeathlonRingDropUi_LoadMonPic(ui->course, ui->heapId, ui->sprites[RING_DROP_UI_SPRITE_PICS + 1], own, member);
            PokeathlonRingDropUi_PrintNextName(ui, next);
            ui->switchStep++;
        }
        break;
    }
    case 1: {
        s16 x, y;
        s16 step;
        int distance;
        ManagedSprite_GetPositionXYWithSubscreenOffset(ui->sprites[RING_DROP_UI_SPRITE_PICS], &x, &y, FX32_CONST(480));
        distance = 136 - y;
        if (distance < 0) {
            distance = -distance;
        }
        step = distance * 3 / 4;
        if (y <= 136) {
            if (step == 0) {
                step = 1;
            }
            y += step;
            PokeathlonRingDropUi_SetPicPosition(ui, RING_DROP_UI_SPRITE_PICS, x, y);
            ManagedSprite_GetPositionXYWithSubscreenOffset(ui->sprites[RING_DROP_UI_SPRITE_PICS + 1], &x, &y, FX32_CONST(480));
            y += step;
            PokeathlonRingDropUi_SetPicPosition(ui, RING_DROP_UI_SPRITE_PICS + 1, x, y);
        } else {
            ManagedSprite *tmp = ui->sprites[RING_DROP_UI_SPRITE_PICS];
            ui->sprites[RING_DROP_UI_SPRITE_PICS] = ui->sprites[RING_DROP_UI_SPRITE_PICS + 1];
            ui->sprites[RING_DROP_UI_SPRITE_PICS + 1] = tmp;
            tmp = ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS];
            ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS] = ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS + 1];
            ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS + 1] = tmp;
            PokeathlonRingDropUi_SetPicPosition(ui, RING_DROP_UI_SPRITE_PICS, 40, 48);
            PokeathlonRingDropUi_SetPicPosition(ui, RING_DROP_UI_SPRITE_PICS + 1, 40, 136);
            ui->switchStep++;
        }
        break;
    }
    case 2:
        ui->switchStep = 0;
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

#pragma force_active reset

// The computer participants' switches take 13 frames. TRUE when done.
BOOL PokeathlonRingDropUi_WaitSwitch(PokeathlonRingDropUi *ui, u8 participant, u8 member) {
    if (participant == 0 || participant >= RING_DROP_PARTICIPANTS) {
        GF_AssertFail();
    }
    ui->switchDelays[participant]++;
    if (ui->switchDelays[participant] > 12) {
        ui->switchDelays[participant] = 0;
        return TRUE;
    }
    return FALSE;
}

// A participant's icon while it switches (and the new member's icon after it).
void PokeathlonRingDropUi_SetSwitching(PokeathlonRingDropUi *ui, u8 participant, u32 member, BOOL switching) {
    if (ui == NULL) {
        GF_AssertFail();
    }
    if (participant >= RING_DROP_PARTICIPANTS) {
        GF_AssertFail();
    }
    if (member >= RING_DROP_TEAM_SIZE) {
        GF_AssertFail();
    }
    if (switching == FALSE) {
        PokeathlonRingDropUi_LoadMonIcon(ui, participant, member);
    }
    PokeathlonRingDropUi_ShowSwitching(ui, participant, switching);
}

#pragma force_active on

ManagedSprite *PokeathlonRingDropUi_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 animation, u16 priority) {
    ManagedSpriteTemplate template = gPokeathlonRingDropUiSpriteTemplate;
    ManagedSprite *sprite;
    GF_ASSERT(spriteSystem);
    GF_ASSERT(spriteManager);
    template.x = x;
    template.y = y;
    template.animation = animation;
    template.drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(480));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

#pragma force_active reset

void PokeathlonRingDropUi_AddWindows(PokeathlonRingDropUi *ui) {
    u8 i;
    if (ui == NULL) {
        GF_AssertFail();
    }
    if (ui->bgConfig == NULL) {
        GF_AssertFail();
    }
    for (i = 0; i < 3; i++) {
        AddWindow(ui->bgConfig, &(&ui->nameWindow)[i], &gPokeathlonRingDropUiWindowTemplates[i]);
    }
    BG_FillCharDataRange(ui->bgConfig, GF_BG_LYR_SUB_1, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, ui->heapId);
}

// The own score, three digits from the digits tilemap (5 digits per row, 4x8 tiles each).
void PokeathlonRingDropUi_DrawScore(PokeathlonRingDropUi *ui, s32 score) {
    u8 tileX[3];
    u8 tileY[3];
    u8 digit;
    u8 i;
    digit = (u8)(score / 100);
    tileX[0] = (digit % 5) * 4;
    tileY[0] = (digit / 5) * 8;
    digit = (u8)((score - digit * 100) / 10);
    tileX[1] = (digit % 5) * 4;
    tileY[1] = (digit / 5) * 8;
    digit = (u8)(score % 10);
    tileX[2] = (digit % 5) * 4;
    tileY[2] = (digit / 5) * 8;
    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(ui->bgConfig, GF_BG_LYR_SUB_2, (u8)(16 + i * 4), 11, 4, 8, ui->digits->rawData, tileX[i], tileY[i], 20, 16);
    }
    ScheduleBgTilemapBufferTransfer(ui->bgConfig, GF_BG_LYR_SUB_2);
}

void PokeathlonRingDropUi_PrintNames(PokeathlonRingDropUi *ui) {
    String *name;
    FillWindowPixelBuffer(&ui->nameWindow, 0);
    name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(ui->course, ui->participant), ui->heapId);
    AddTextPrinterParameterizedWithColor(&ui->nameWindow, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    PokeathlonRingDropUi_PrintMessageAt(&ui->labelWindow, ui->msgFormat, ui->msgData, msg_0309_00304, ui->heapId, 2, 2);
    String_Delete(name);
    CopyWindowToVram(&ui->nameWindow);
    BufferString(ui->msgFormat, 0, ui->memberNames[1], 2, 1, 2);
    PokeathlonRingDropUi_PrintMessage(&ui->nextNameWindow, ui->msgFormat, ui->msgData, msg_0309_00154, ui->heapId);
}

void PokeathlonRingDropUi_PrintMessage(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId) {
    PokeathlonRingDropUi_PrintMessageAt(window, msgFormat, msgData, msgId, heapId, 0, 0);
}

void PokeathlonRingDropUi_PrintMessageAt(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId, s16 x, s16 y) {
    String *string;
    FillWindowPixelBuffer(window, 0);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, heapId);
    AddTextPrinterParameterizedWithColor(window, 0, string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(window);
}

// The code calls these with s16 coordinates and a u8 participant.
#define PokeathlonRingDropUi_NewSpriteS16(spriteSystem, spriteManager, x, y, animation, priority) (((ManagedSprite * (*)(SpriteSystem *, SpriteManager *, s16, s16, u16, u16)) PokeathlonRingDropUi_NewSprite)((spriteSystem), (spriteManager), (x), (y), (animation), (priority)))
#define PokeathlonRingDropUi_LoadMonIconU8(ui, participant, member)                               (((void (*)(PokeathlonRingDropUi *, u8, u32))PokeathlonRingDropUi_LoadMonIcon)((ui), (participant), (member)))

#pragma force_active on

void PokeathlonRingDropUi_CreateSprites(PokeathlonRingDropUi *ui) {
    ManagedSpriteTemplate template = { 0 };
    int i;
    s16 x;

    ui->sprites[RING_DROP_UI_SPRITE_PLATE] = PokeathlonRingDropUi_NewSpriteS16(ui->spriteSystem, ui->spriteManager, 134, 160, ui->participant + 12, 6);
    ManagedSprite_SetPaletteOverride(ui->sprites[RING_DROP_UI_SPRITE_PLATE], 1);
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        x = i * 32 + 120;
        ui->sprites[RING_DROP_UI_SPRITE_ICONS + i] = PokeathlonRingDropUi_NewSpriteS16(ui->spriteSystem, ui->spriteManager, x, 36, i + 1, 5);
        PokeathlonRingDropUi_LoadMonIconU8(ui, i, 0);
        ui->sprites[RING_DROP_UI_SPRITE_SWITCHING + i] = PokeathlonRingDropUi_NewSpriteS16(ui->spriteSystem, ui->spriteManager, x, 36, 10, 4);
        ManagedSprite_SetDrawFlag(ui->sprites[RING_DROP_UI_SPRITE_SWITCHING + i], FALSE);
        ui->sprites[RING_DROP_UI_SPRITE_RANKS + i] = PokeathlonRingDropUi_NewSpriteS16(ui->spriteSystem, ui->spriteManager, i * 32 + 120, 24, 6, 0);
        ManagedSprite_SetDrawFlag(ui->sprites[RING_DROP_UI_SPRITE_RANKS + i], FALSE);
    }
    for (i = 0; i < 2; i++) {
        template.resIdList[0] = 10002 + i;
        template.resIdList[1] = 10002 + i;
        template.resIdList[2] = 10002;
        template.resIdList[3] = 10002;
        template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        template.x = 40;
        template.y = i * 88 + 48;
        template.drawPriority = i + 2;
        template.bgPriority = 2;
        ui->sprites[RING_DROP_UI_SPRITE_PICS + i] = SpriteSystem_NewSpriteWithYOffset(ui->spriteSystem, ui->spriteManager, &template, FX32_CONST(480));
        ManagedSprite_SetAnimateFlag(ui->sprites[RING_DROP_UI_SPRITE_PICS + i], TRUE);
    }
    for (i = 0; i < 2; i++) {
        PokeathlonRingDropUi_LoadMonPic(ui->course, ui->heapId, ui->sprites[RING_DROP_UI_SPRITE_PICS + i], ui->participant, 2 - i);
        ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS + i] = PokeathlonRingDropUi_NewSpriteS16(ui->spriteSystem, ui->spriteManager, 40, i * 88 + 24, 5, 1);
        ManagedSprite_SetDrawFlag(ui->sprites[RING_DROP_UI_SPRITE_PIC_MARKS + i], FALSE);
    }
    PokeathlonRingDropUi_CreateTimerSprites(ui);
}

#pragma force_active reset

void PokeathlonRingDropUi_LoadMonPic(PokeathlonCourseData *course, enum HeapID heapId, ManagedSprite *sprite, u32 participant, u8 member) {
    u32 info[4];
    PokeathlonCourse_GetMonInfo(course, participant, member, info);
    PokeathlonCourse_LoadMonPic(sprite, info, 0, 0, heapId);
}

void PokeathlonRingDropUi_LoadMonIcon(PokeathlonRingDropUi *ui, s32 participant, u32 member) {
    u32 info[4];
    ManagedSprite *sprite = RING_DROP_UI_SPRITE(ui, participant + RING_DROP_UI_SPRITE_ICONS);
    PokeathlonCourse_GetMonInfo(ui->course, participant, member, info);
    PokeathlonCourse_LoadMonIcon(sprite, info, ui->iconPalette, (u32)ui->iconNarc, participant << 9, ui->heapId);
}

void *PokeathlonRingDropUi_NewTimerSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 animation, u16 priority) {
    ManagedSpriteTemplate template = gPokeathlonRingDropUiTimerSpriteTemplate;
    void *sprite;
    if (spriteSystem == NULL) {
        GF_AssertFail();
    }
    if (spriteManager == NULL) {
        GF_AssertFail();
    }
    template.x = x;
    template.y = y;
    template.animation = animation;
    template.drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(480));
    ManagedSprite_SetAnimateFlag(sprite, 1);
    return sprite;
}

void PokeathlonRingDropUi_CreateTimerSprites(PokeathlonRingDropUi *ui) {
    s32 i;
    PokeathlonRingDropUi *cursor;
    s32 x;
    ManagedSprite *sprite;

    sprite = PokeathlonRingDropUi_NewTimerSprite(ui->spriteSystem, ui->spriteManager, 114, 180, 20, 1);
    ui->sprites[RING_DROP_UI_SPRITE_TIMER] = sprite;
    ManagedSprite_SetPaletteOverride(sprite, 14);
    i = 0;
    cursor = ui;
    x = 134;
    for (; i < 2; i++) {
        s32 matrix[3] = { 0, 0, 0 };
        s32 position;
        cursor->timerDigits[0] = PokeathlonCourse_CreateTimerDigitSprite(ui->resources, SpriteManager_GetSpriteList(ui->spriteManager), 1, ui->heapId);
        position = x;
        position <<= FX32_SHIFT;
        matrix[0] = position;
        matrix[1] = FX32_CONST(180);
        matrix[2] = 0;
        Sprite_SetMatrix(cursor->timerDigits[0], (VecFx32 *)matrix);
        Sprite_SetDrawFlag(cursor->timerDigits[0], TRUE);
        cursor = (PokeathlonRingDropUi *)((u8 *)cursor + sizeof(Sprite *));
        x += 16;
    }
}

void PokeathlonRingDropUi_PrintNextName(PokeathlonRingDropUi *ui, s32 member) {
    s32 index;
    index = (member + 1) % RING_DROP_TEAM_SIZE;
    index = (u8)index;
    BufferString(ui->msgFormat, 0, ui->memberNames[index], 2, 1, 2);
    PokeathlonRingDropUi_PrintMessage(&ui->nextNameWindow, ui->msgFormat, ui->msgData, msg_0309_00154, ui->heapId);
}

void PokeathlonRingDropUi_SetPicPosition(PokeathlonRingDropUi *ui, s32 sprite, s32 x, s32 y) {
    // (the code passes x and y without narrowing them)
    ((void (*)(ManagedSprite *, s32, s32, fx32))ManagedSprite_SetPositionXYWithSubscreenOffset)(ui->sprites[sprite], x, y, FX32_CONST(480));
    ((void (*)(ManagedSprite *, s32, s32, fx32))ManagedSprite_SetPositionXYWithSubscreenOffset)(ui->sprites[sprite + 2], x, (s16)(y - 24), FX32_CONST(480));
}

void PokeathlonRingDropUi_ShowSwitching(PokeathlonRingDropUi *ui, s32 participant, BOOL switching) {
    ManagedSprite_SetDrawFlag(RING_DROP_UI_SPRITE(ui, participant + RING_DROP_UI_SPRITE_ICONS), switching == FALSE);
    ManagedSprite_SetDrawFlag(RING_DROP_UI_SPRITE(ui, participant + RING_DROP_UI_SPRITE_SWITCHING), switching);
}
