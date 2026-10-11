#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "text.h"

// Course sprite helpers (OVY_96, not yet tidied).
Sprite *ov96_021EA2C4(void *resources, void *spriteList, int a2, enum HeapID heapId);
void ov96_021EB2BC(void *pool, int narc, int file, int id, int a4);
void ov96_021EB2F4(void *pool, int narc, int file, int id, int a4, int a5);
void ov96_021EB334(void *pool, int narc, int file, int id);
void ov96_021EB36C(void *pool, int narc, int file, int id);
void *ov96_021EB3E4(void *pool, int a1, int a2, int id, int anim);
void ov96_021EB52C(void *sprite, int a1, int show);
void ov96_021EB564(void *sprite, int anim);
void ov96_021EB588(void *sprite, VecFx32 *position);
VecFx32 *ov96_021EB594(void *sprite);
Sprite *ov96_021EB5B8(void *sprite);
void *ov96_021EB5E8(void *pool);
void ov96_021EB630(void *sprite, int priority);
// monsters.c / pokemon_icon_idx.c, declared here with the argument types the calls were built with
void *sub_0201457C(u16 charNarc, u16 charFile, u32 heapId, u32 personality, int a4, int a5, u16 species);
void *sub_02014450(u16 plttNarc, u16 plttFile, u32 heapId);
u32 sub_02074490(void);
// in the object linked between the two parts of the top screen code (picture sprites)
void ov96_02208914(void *pool, PokeathlonPennantCaptureDisplay *display);

extern const u8 ov96_0221CBC4[];
extern const u8 ov96_0221CBC8[];
extern const PokeathlonPennantCaptureSpriteIds ov96_0221CBCC[];
extern const VecFx32 ov96_0221CBF4;

// Several loops below walk a display-typed cursor by one array element per step instead of indexing
// (fields are read at their offset from the cursor); that is the form the code matches with.
#define DISPLAY_CURSOR_STEP(cursor, size) ((PokeathlonPennantCaptureDisplay *)((u8 *)(cursor) + (size)))

#pragma force_active on

PokeathlonPennantCaptureDisplay *PokeathlonPennantCaptureDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonPennantCaptureDisplay *display = Heap_Alloc(heapId, sizeof(PokeathlonPennantCaptureDisplay));
    int i;
    u8 *participant;

    MI_CpuFill8(display, 0, sizeof(PokeathlonPennantCaptureDisplay));
    display->heapId = heapId;
    display->bgConfig = bgConfig;
    display->course = course;
    PokeathlonPennantCaptureDisplay_InitWindow(display);
    display->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, heapId);
    display->msgFormat = MessageFormat_New(heapId);
    for (i = 0; i < 3; i++) {
        display->names[i] = String_New(11, heapId);
    }
    participant = PokeathlonCourse_GetParticipantUnk04(course, ov96_021E5F24(course));
    for (int j = 0; j < 3; j++) {
        CopyU16ArrayToString(display->names[j], (u16 *)(participant + 0x12));
        participant += 0x28;
    }
    PokeathlonPennantCaptureDisplay_PrintName(display, 1);
    return display;
}

#pragma force_active reset

void PokeathlonPennantCaptureDisplay_Delete(PokeathlonPennantCaptureDisplay *display) {
    int i;

    for (i = 0; i < 3; i++) {
        Heap_Free(display->monPics[i].character);
        Heap_Free(display->monPics[i].palette);
    }
    {
        PokeathlonPennantCaptureDisplay *cursor = display;
        int j = 0;
        for (; j < 12; j++, cursor = DISPLAY_CURSOR_STEP(cursor, sizeof(void *))) {
            Heap_Free(cursor->u.raw.iconFiles[0]);
        }
    }
    Heap_Free(display->scoreScreenFile);
    {
        PokeathlonPennantCaptureDisplay *cursor = display;
        int j = 0;
        for (; j < 3; j++, cursor = DISPLAY_CURSOR_STEP(cursor, sizeof(String *))) {
            String_Delete(cursor->names[0]);
        }
    }
    MessageFormat_Delete(display->msgFormat);
    DestroyMsgData(display->msgData);
    RemoveWindow(&display->window);
    Heap_Free(display);
}

void PokeathlonPennantCaptureDisplay_LoadBgGfx(PokeathlonPennantCaptureDisplay *display) {
    GfGfxLoader_LoadCharData((NarcId)0xE2, 5, display->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadCharData((NarcId)0xE2, 6, display->bgConfig, GF_BG_LYR_SUB_2, 0, 0x2000, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData((NarcId)0xE2, 7, display->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData((NarcId)0xE2, 8, display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, display->heapId);
    display->scoreScreenFile = GfGfxLoader_GetScrnData((NarcId)0xE2, 9, FALSE, &display->scoreScreen, display->heapId);
    GfGfxLoader_GXLoadPal((NarcId)0xE2, 4, (enum GFPalLoadLocation)4, (enum GFPalSlotOffset)0, 0x40, display->heapId);
    PokeathlonPennantCaptureDisplay_SetScore(display, 0);
}

void PokeathlonPennantCaptureDisplay_LoadSpriteResources(PokeathlonPennantCaptureDisplay *display, void *pool) {
    u32 ids[2];
    u32 *id = ids;
    int i;

#pragma unused(display)
    ids[0] = ov96_0221CBCC[1].ids[0];
    ids[1] = ov96_0221CBCC[1].ids[1];
    for (i = 0; i < 2; i++) {
        ov96_021EB2BC(pool, 0x5D, 9, *id, 2);
        ov96_021EB2F4(pool, 0x5D, 6, *id, 2, 1);
        ov96_021EB334(pool, 0x5D, 10, *id);
        ov96_021EB36C(pool, 0x5D, 10, *id);
        id++;
    }
    ov96_021EB2BC(pool, 0xE2, 13, 0x65, 2);
    ov96_021EB2F4(pool, 0xE2, 10, 0x65, 2, 8);
    ov96_021EB334(pool, 0xE2, 12, 0x65);
    ov96_021EB36C(pool, 0xE2, 11, 0x65);
}

void PokeathlonPennantCaptureDisplay_CreateSprites(PokeathlonPennantCaptureDisplay *display, void *pool, void *resources) {
    int participant;
    int i;
    PokeathlonPennantCaptureDisplay *row;
    void **status;
    void **point;
    VecFx32 digitPosition;
    VecFx32 initialPosition;

    participant = 0;
    row = display;
    status = display->statusSprites[0];
    point = display->pointSprites[0];
    for (; participant < 4; participant++) {
        row->u.mons[0].icon = ov96_021EB3E4(pool, 3, 2, 0x65, 2);
        row->u.mons[0].frame = ov96_021EB3E4(pool, 3, 2, 0x65, 3);
        row->u.mons[0].carryMark = ov96_021EB3E4(pool, 3, 2, 0x65, 10);
        status[2] = ov96_021EB3E4(pool, 2, 2, 0x65, 19);
        ov96_021EB564(status[2], 0);
        ov96_021EB52C(status[2], 1, 1);
        status[1] = ov96_021EB3E4(pool, 2, 2, 0x65, 21);
        ov96_021EB564(status[1], 9);
        ov96_021EB52C(status[1], 1, 1);
        status[0] = ov96_021EB3E4(pool, 2, 2, 0x65, 20);
        ov96_021EB564(row->u.mons[0].icon, participant + 19);
        ov96_021EB564(row->u.mons[0].frame, participant + 5);
        ov96_021EB564(status[0], 3);
        ov96_021EB52C(status[0], 1, 1);
        for (i = 0; i < 2; i++) {
            point[i] = ov96_021EB3E4(pool, 2, 2, 0x65, 24);
            ov96_021EB564(point[i], 2);
        }
        row = DISPLAY_CURSOR_STEP(row, sizeof(PokeathlonPennantCaptureDisplayMon));
        status += 4;
        point += 2;
    }
    initialPosition = ov96_0221CBF4;
    display->goalSprite = ov96_021EB3E4(pool, 3, 2, 0x65, 22);
    ov96_021EB564(display->goalSprite, 4);
    ov96_021EB52C(display->goalSprite, 1, 1);
    ov96_021EB588(display->goalSprite, &initialPosition);
    ov96_02208914(pool, display);
    for (i = 0; i < 2; i++) {
        display->timerDigits[i] = ov96_021EA2C4(resources, ov96_021EB5E8(pool), 3, display->heapId);
        Sprite_SetDrawFlag(display->timerDigits[i], TRUE);
        digitPosition.x = ov96_0221CBC4[i] << FX32_SHIFT;
        digitPosition.y = 0x2C4000;
        digitPosition.z = 0;
        Sprite_SetMatrix(display->timerDigits[i], &digitPosition);
        Sprite_SetAnimCtrlSeq(display->timerDigits[i], ov96_0221CBC8[i]);
    }
}

void PokeathlonPennantCaptureDisplay_LoadIcons(PokeathlonPennantCaptureDisplay *display, const PokeathlonPennantCaptureIconMon *mons) {
    NARC *volatile narc;
    volatile int x;
    PokeathlonPennantCaptureDisplay *volatile pointRow;
    u32 iconNarc;
    int i;
    PokeathlonPennantCaptureDisplay *fileRow;
    NNSG2dCharacterData **chars;
    PokeathlonPennantCaptureDisplay *monRow;
    PokeathlonPennantCaptureDisplay *statusRow;
    VecFx32 position;

    struct {
        u32 species;
        u32 form;
    } current;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, display->heapId);
    iconNarc = sub_02074490();
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, iconNarc, GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_6_OFFSET, 0x60, display->heapId);
    fileRow = display;
    i = 0;
    chars = display->iconChars;
    for (; i < 12; i++) {
        fileRow->u.raw.iconFiles[0] = Heap_AllocAtEnd(display->heapId, 0x1000);
        current.form = (u8)mons->form;
        current.species = mons->species;
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(current.species, FALSE, current.form), fileRow->u.raw.iconFiles[0]);
        NNS_G2dGetUnpackedBGCharacterData(fileRow->u.raw.iconFiles[0], chars);
        display->iconPalettes[i] = GetMonIconPaletteEx(current.species, current.form, FALSE) + 6;
        fileRow = DISPLAY_CURSOR_STEP(fileRow, sizeof(void *));
        mons++;
        chars++;
    }
    NARC_Delete(narc);

    i = 0;
    monRow = display;
    x = 0x70;
    pointRow = display;
    statusRow = display;
    for (; i < 4; i++) {
        PokeathlonPennantCaptureDisplay_SetIcon(display, (u8)i, 0);
        ov96_021EB52C(monRow->u.mons[0].icon, 1, 1);
        ov96_021EB52C(monRow->u.mons[0].frame, 1, 1);
        position.z = 0;
        position.x = x << FX32_SHIFT;
        position.y = 0x350000;
        ov96_021EB588(monRow->u.mons[0].icon, &position);
        ov96_021EB588(monRow->u.mons[0].frame, &position);
        ov96_021EB588(monRow->u.mons[0].carryMark, &position);
        {
            int j;
            PokeathlonPennantCaptureDisplay *point = pointRow;
            for (j = 0; j < 2; j++) {
                ov96_021EB588(point->pointSprites[0][0], &position);
                point = DISPLAY_CURSOR_STEP(point, sizeof(void *));
            }
        }
        position.y -= 0x20000;
        ov96_021EB588(statusRow->statusSprites[0][0], &position);
        ov96_021EB588(statusRow->statusSprites[0][1], &position);
        ov96_021EB588(statusRow->statusSprites[0][2], &position);
        monRow = DISPLAY_CURSOR_STEP(monRow, sizeof(PokeathlonPennantCaptureDisplayMon));
        x += 0x28;
        statusRow = DISPLAY_CURSOR_STEP(statusRow, sizeof(display->statusSprites[0]));
        pointRow = DISPLAY_CURSOR_STEP(pointRow, sizeof(display->pointSprites[0]));
    }
}

void PokeathlonPennantCaptureDisplay_LoadMonPics(PokeathlonPennantCaptureDisplay *display, const PokeathlonPennantCapturePicMon *mons) {
    u16 ids[7];
    int i = 0;

    for (; i < 3; i++) {
        GetMonSpriteCharAndPlttNarcIdsEx((PokepicTemplate *)ids, mons[i].species, mons[i].gender, 2, mons[i].shiny, (u8)mons[i].form, mons[i].personality);
        display->monPics[i].character = sub_0201457C(ids[0], ids[1], display->heapId, mons[i].personality, 0, 2, mons[i].species);
        display->monPics[i].palette = sub_02014450(ids[0], ids[2], display->heapId);
    }
}

void PokeathlonPennantCaptureDisplay_LoadMonPicToVram(PokeathlonPennantCaptureDisplay *display, u32 slot, u32 member) {
    if (slot >= 2) {
        GF_AssertFail();
    }
    if (member >= 3) {
        GF_AssertFail();
    }
    DC_FlushRange(display->monPics[member].character, 0xC80);
    GXS_LoadOBJ(display->monPics[member].character, display->picCharOffsets[slot], 0xC80);
    DC_FlushRange(display->monPics[member].palette, 0x20);
    GXS_LoadOBJPltt(display->monPics[member].palette, display->picPlttOffsets[slot], 0x20);
}

void PokeathlonPennantCaptureDisplay_SetIcon(PokeathlonPennantCaptureDisplay *display, int participant, int member) {
    Sprite *sprite;
    int slot;

    sprite = ov96_021EB5B8(display->u.mons[participant].icon);
    slot = member + participant * 3;
    DC_FlushRange(display->iconChars[slot]->pRawData, 0x200);
    GXS_LoadOBJ(display->iconChars[slot]->pRawData, 0x1800 + participant * 0x200, 0x200);
    Sprite_SetPaletteOverride(sprite, display->iconPalettes[slot]);
}

void PokeathlonPennantCaptureDisplay_SetScore(PokeathlonPennantCaptureDisplay *display, int score) {
    u8 x[3];
    u8 y[3];
    u8 digit;
    u8 i;

    digit = score / 100;
    x[0] = (digit % 5) * 4;
    y[0] = (digit / 5) * 8;
    digit = (score - digit * 100) / 10;
    x[1] = (digit % 5) * 4;
    y[1] = (digit / 5) * 8;
    digit = score % 10;
    x[2] = (digit % 5) * 4;
    y[2] = (digit / 5) * 8;

    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(display->bgConfig, 5, 14 + i * 4, 3, 4, 8, display->scoreScreen->rawData, x[i], y[i], 20, 16);
    }
    ScheduleBgTilemapBufferTransfer(display->bgConfig, 5);
}

#pragma force_active on

// Slides the picture of the outgoing team member out and the incoming one in (two sprites in turn).
BOOL PokeathlonPennantCaptureDisplay_SlideMonPic(PokeathlonPennantCaptureDisplay *display, int member) {
    int *state = &display->picState;

    switch (*state) {
    case 0: {
        VecFx32 position = *ov96_021EB594(display->picSprites[display->picSlot]);
        position.y += 0x20000;
        ov96_021EB588(display->picSprites[display->picSlot], &position);
        if (position.y >= 0x39C000) {
            position.y = 0x294000;
            ov96_021EB588(display->picSprites[display->picSlot], &position);
            (*state)++;
        }
        break;
    }
    case 1:
        PokeathlonPennantCaptureDisplay_LoadMonPicToVram(display, (u8)display->picSlot, member);
        display->picSlot = (display->picSlot + 1) % 2;
        (*state)++;
        break;
    case 2: {
        VecFx32 position = *ov96_021EB594(display->picSprites[display->picSlot]);
        position.y += 0x20000;
        ov96_021EB588(display->picSprites[display->picSlot], &position);
        if (position.y >= 0x344000) {
            position.y = 0x344000;
            ov96_021EB588(display->picSprites[display->picSlot], &position);
            display->picSlot = (display->picSlot + 1) % 2;
            (*state)++;
        }
        break;
    }
    case 3: {
        VecFx32 position = *ov96_021EB594(display->picSprites[display->picSlot]);
        position.y += 0x20000;
        ov96_021EB588(display->picSprites[display->picSlot], &position);
        if (position.y >= 0x2EC000) {
            position.y = 0x2EC000;
            ov96_021EB588(display->picSprites[display->picSlot], &position);
            display->picSlot = (display->picSlot + 1) % 2;
            PokeathlonPennantCaptureDisplay_PrintName(display, (u8)((member + 2) % 3));
            *state = 0;
            return TRUE;
        }
        break;
    }
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonPennantCaptureDisplay_StepSwapWait(PokeathlonPennantCaptureDisplay *display, u32 participant) {
    if (participant == 0 || participant >= 4) {
        GF_AssertFail();
    }
    participant--;
    display->counters[participant].x += 0x20;
    if (display->counters[participant].x >= 0x58) {
        display->counters[participant].x = 0;
        display->counters[participant].y++;
    }
    if (display->counters[participant].y >= 3) {
        display->counters[participant].y = 0;
        return TRUE;
    }
    return FALSE;
}

void PokeathlonPennantCaptureDisplay_UpdateMon(PokeathlonPennantCaptureDisplay *display, u8 participant, u16 member, BOOL hidden, int active) {
    if (display->shownMember[participant] != member) {
        display->shownMember[participant] = member;
        PokeathlonPennantCaptureDisplay_SetIcon(display, participant, member);
    }
    if (hidden) {
        VecFx32 position;
        position.z = 0;
        position.x = (participant * 0x28 + 0x70) << 12;
        position.y = 0x350004;
        ov96_021EB588(display->u.mons[participant].icon, &position);
        ov96_021EB588(display->u.mons[participant].frame, &position);
        PokeathlonPennantCaptureDisplay_ShowStatus(display->statusSprites[participant], 0);
    } else {
        VecFx32 position;
        position.z = 0;
        position.x = (participant * 0x28 + 0x70) << 12;
        position.y = 0x350000;
        ov96_021EB588(display->u.mons[participant].icon, &position);
        ov96_021EB588(display->u.mons[participant].frame, &position);
        PokeathlonPennantCaptureDisplay_ShowStatus(display->statusSprites[participant], 1);
        if (active == 1) {
            if (display->u.mons[participant].active != active) {
                ov96_021EB52C(display->u.mons[participant].carryMark, 1, 1);
                ov96_021EB564(display->u.mons[participant].carryMark, 1);
            }
        } else {
            ov96_021EB52C(display->u.mons[participant].carryMark, 1, 0);
        }
    }
    display->u.mons[participant].active = active;
}

void PokeathlonPennantCaptureDisplay_SetTimer(PokeathlonPennantCaptureDisplay *display, int frames) {
    u8 seconds = frames / 30;
    u8 anim = seconds / 10;
    Sprite_SetAnimCtrlSeq(display->timerDigits[0], anim + 1);
    anim = seconds % 10;
    Sprite_SetAnimCtrlSeq(display->timerDigits[1], anim + 1);
}

// Draws the given participant's top-screen sprites above the others'.
void PokeathlonPennantCaptureDisplay_BringToFront(PokeathlonPennantCaptureDisplay *display, int chosen) {
    u8 order[4];
    int i;
    u8 next;

    for (i = 0; i < 2; i++) {
        Sprite_SetDrawPriority(display->timerDigits[i], 2);
    }
    for (i = 0; i < 2; i++) {
        Sprite_SetDrawPriority(ov96_021EB5B8(display->picSprites[i]), 2);
    }
    next = 1;
    for (i = 0; i < 4; i++) {
        if (i == chosen) {
            order[0] = i;
        }
    }
    for (i = 0; i < 4; i++) {
        if (order[0] != i) {
            order[next++] = i;
        }
    }
    for (i = 0; i < 4; i++) {
        ov96_021EB630(display->u.mons[order[i]].carryMark, 3 + i * 2);
        ov96_021EB630(display->u.mons[order[i]].icon, 4 + i * 2);
        ov96_021EB630(display->u.mons[order[i]].frame, 11 + i);
    }
}

void PokeathlonPennantCaptureDisplay_SetPennants(PokeathlonPennantCaptureDisplay *display, int participant, u32 pennants) {
    if (pennants > 9) {
        GF_AssertFail();
    }
    ov96_021EB564(display->statusSprites[participant][1], pennants + 9);
}

void PokeathlonPennantCaptureDisplay_ShowPoint(PokeathlonPennantCaptureDisplay *display, int participant) {
    ov96_021EB564(display->pointSprites[participant][display->nextPointSprite], 2);
    ov96_021EB52C(display->pointSprites[participant][display->nextPointSprite], 1, 1);
    display->nextPointSprite = (display->nextPointSprite + 1) % 2;
}

void PokeathlonPennantCaptureDisplay_PrintName(PokeathlonPennantCaptureDisplay *display, u32 member) {
    String *string;
    FillWindowPixelBuffer(&display->window, 0);
    BufferString(display->msgFormat, 0, display->names[member], 2, 1, 2);
    string = ReadMsgData_ExpandPlaceholders(display->msgFormat, display->msgData, msg_0309_00154, display->heapId);
    AddTextPrinterParameterizedWithColor(&display->window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(&display->window);
}
