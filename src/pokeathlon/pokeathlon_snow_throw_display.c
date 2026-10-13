#include "global.h"

#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_snow_throw.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "text.h"
#include "char_tiles.h"

// NitroSDK MATH_QSort (no header declares it)
void MATH_QSort(void *base, u32 count, u32 width, int (*compare)(const void *, const void *), void *context);

// Sprite pool helpers (pokeathlon_course_objects.c), declared with untyped entries as the course objects use them.
void PokeathlonCourseSpritePool_LoadChar(void *pool, int narcId, int fileId, int resId, int vram);
void PokeathlonCourseSpritePool_LoadPltt(void *pool, int narcId, int fileId, int resId, int vram, int count);
void PokeathlonCourseSpritePool_LoadCell(void *pool, int narcId, int fileId, int resId);
void PokeathlonCourseSpritePool_LoadAnim(void *pool, int narcId, int fileId, int resId);
void PokeathlonCourseSpritePool_CreateSprite(void *pool, int priority, int whichScreen, int resId, int kind);
void *PokeathlonCourseSpritePool_Take(void *pool, int resId, int kind);
void **PokeathlonCourseSpritePool_GetResourceSlot(void *pool, u32 id, u32 kind);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *entry, int seq);
void PokeathlonCourseSpritePoolSprite_TryChangeAnim(void *entry, int seq);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(void *entry, int priority);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *entry, void *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(void *entry);
BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(void *entry);

// Which of two Pokemon at the same depth is drawn in front, by view (own team) and Pokemon.
extern const u8 ov96_0221C95C[][SNOW_THROW_MONS];
extern const WindowTemplate ov96_0221CA28;

// A snowball as sorted for drawing (8).
typedef struct PokeathlonSnowThrowDrawOrder {
    u16 own;
    u16 index;
    s32 y;
} PokeathlonSnowThrowDrawOrder;

// Host: two flying (or held) snowballs closer than 4 units both splash.
void PokeathlonSnowThrow_CollideBalls(PokeathlonSnowThrow *work) {
    u8 splash[SNOW_THROW_MONS];
    VecFx32 diff;
    int i;
    int j;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        splash[i] = FALSE;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        switch (work->balls[i].state) {
        case SNOW_THROW_BALL_FLYING:
        case SNOW_THROW_BALL_HELD:
            for (j = 0; j < SNOW_THROW_MONS; j++) {
                if (i == j) {
                    continue;
                }
                switch (work->balls[j].state) {
                case SNOW_THROW_BALL_FLYING:
                case SNOW_THROW_BALL_HELD:
                    VEC_Subtract(&work->balls[i].position, &work->balls[j].position, &diff);
                    if (VEC_Mag(&diff) < 4 * FX32_ONE) {
                        splash[i] = TRUE;
                        splash[j] = TRUE;
                    }
                    break;
                }
            }
            break;
        }
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        if (splash[i]) {
            work->balls[i].state = SNOW_THROW_BALL_SPLASHED;
            work->balls[i].size = 0;
            work->balls[i].taps = 0;
        }
    }
}

// The other players' names, in the order of the teams after the own one.
void PokeathlonSnowThrow_PrintNames(PokeathlonSnowThrow *work, PokeathlonCourseData *course) {
    int i;
    int participant;
    Window *window;
    String *name;
    u8 own;

    own = (u8)PokeathlonCourse_GetNetId(course);
    i = 0;
    participant = own + 1;
    window = work->nameWindows;
    for (; i < SNOW_THROW_TEAMS - 1; i++) {
        name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, (u8)(participant % SNOW_THROW_TEAMS)), work->heapId);
        AddTextPrinterParameterizedWithColor(window, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
        String_Delete(name);
        CopyWindowToVram(window);
        participant++;
        window++;
    }
}

void PokeathlonSnowThrow_SetTimerDigits(PokeathlonSnowThrow *work, u32 seconds) {
    u8 value = (u8)seconds;

    Sprite_SetAnimCtrlSeq(work->timerDigits[0], (u8)(value / 10) + 1);
    Sprite_SetAnimCtrlSeq(work->timerDigits[1], (u8)(value % 10) + 1);
}

// Finish callback: the own team's hits.
void PokeathlonSnowThrow_ShowOwnScore(PokeathlonCourseData *course) {
    u16 *teams;
    PokeathlonSnowThrow *work;
    u16 score;

    teams = ((PokeathlonSnowThrowPacket *)PokeathlonCourse_ToBytes((u8 *)PokeathlonCourse_GetDataCopyArea(course) + offsetof(PokeathlonSnowThrowLink, packet)))->teams;
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    score = (u16)(teams[(u8)PokeathlonCourse_GetNetId(course)] & 0x3FF);
    if (score > 999) {
        score = 999;
    }
    PokeathlonCourse_SetRecordScore(course, score);
    PokeathlonSnowThrow_PrintScore(work, score);
}

// MATH_QSort: lower on the screen first, else by the view's order of the Pokemon.
int PokeathlonSnowThrow_CompareDrawOrder(const void *a, const void *b) {
    const PokeathlonSnowThrowDrawOrder *orderA = a;
    const PokeathlonSnowThrowDrawOrder *orderB = b;
    u8 own = (u8)orderA->own;
    s32 result;

    if (orderA->y > orderB->y) {
        result = -1;
    } else if (orderA->y < orderB->y) {
        result = 1;
    } else if (ov96_0221C95C[own][orderA->index] < ov96_0221C95C[own][orderB->index]) {
        result = -1;
    } else if (ov96_0221C95C[own][orderA->index] > ov96_0221C95C[own][orderB->index]) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

// The snowballs and their shadows overlap in the order of their y on the screen.
void PokeathlonSnowThrow_SortSprites(PokeathlonSnowThrow *work, u16 own) {
    PokeathlonSnowThrowDrawOrder orders[SNOW_THROW_MONS];
    u8 i;
    u8 ball;
    VecFx32 *position;

    for (i = 0; i < SNOW_THROW_MONS; i++) {
        position = PokeathlonCourseSpritePoolSprite_GetPosition(work->balls[i].shadowSprite);
        orders[i].index = work->balls[i].index;
        orders[i].y = position->y;
        orders[i].own = own;
    }
    MATH_QSort(orders, SNOW_THROW_MONS, sizeof(PokeathlonSnowThrowDrawOrder), PokeathlonSnowThrow_CompareDrawOrder, work->sortBuffer);
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        ball = (u8)orders[i].index;
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[ball].sprite, i + 20);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->balls[ball].splashSprite, i + 5);
    }
}

PokeathlonSnowThrowDisplay *PokeathlonSnowThrowDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonSnowThrowDisplay *display;

    display = Heap_Alloc(heapId, sizeof(PokeathlonSnowThrowDisplay));
    MI_CpuFill8(display, 0, sizeof(PokeathlonSnowThrowDisplay));
    display->heapId = heapId;
    display->bgConfig = bgConfig;
    display->course = course;
    PokeathlonSnowThrowDisplay_InitWindow(display);
    PokeathlonSnowThrowDisplay_PrintName(display);
    return display;
}

void PokeathlonSnowThrowDisplay_Delete(PokeathlonSnowThrowDisplay *display) {
    int i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        Heap_Free(display->monPics[i].character);
        Heap_Free(display->monPics[i].palette);
    }
    Heap_Free(display->scoreScreenFile);
    RemoveWindow(&display->nameWindow);
    Heap_Free(display);
}

void PokeathlonSnowThrowDisplay_LoadBgGraphics(PokeathlonSnowThrowDisplay *display, u32 own) {
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 4, display->bgConfig, GF_BG_LYR_SUB_3, 0, 0x2000, FALSE, display->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_1_7, 5, display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 6, display->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 7, display->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, display->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_1_7, 8, display->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, display->heapId);
    display->scoreScreenFile = GfGfxLoader_GetScrnData(NARC_a_2_1_7, 9, FALSE, &display->scoreScreen, display->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_1_7, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x80, display->heapId);
    // the own team's colour
    CopyRectToBgTilemapRect(display->bgConfig, GF_BG_LYR_SUB_1, 0, 18, 4, 4, display->scoreScreen->rawData, own * 4, 16, 20, 20);
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_1);
}

void PokeathlonSnowThrowDisplay_LoadSpriteGraphics(PokeathlonSnowThrowDisplay *display, void *pool) {
#pragma unused(display)
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_1_7, 13, 101, 2);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_1_7, 10, 101, 2, 4);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_1_7, 12, 101);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_1_7, 11, 101);
}

void PokeathlonSnowThrowDisplay_CreateSprites(PokeathlonSnowThrowDisplay *display, void *pool, PokeathlonCourseMonInfo *mons) {
    int i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        PokeathlonCourseSpritePool_CreateSprite(pool, 3, 2, 101, 5);
    }
    PokeathlonSnowThrowDisplay_CreatePicSprites(pool, display, mons);
}

void PokeathlonSnowThrowDisplay_ShowTired(PokeathlonSnowThrowDisplay *display, u8 member, BOOL show) {
    PokeathlonCourseSpritePoolSprite_Show(display->tiredSprites[member], TRUE, show);
}

// The own team's hits in big digits (from the score screen: 5 x 2 digits of 4 x 8 tiles).
#pragma force_active on

void PokeathlonSnowThrowDisplay_SetScore(PokeathlonSnowThrowDisplay *display, u16 score) {
    u8 srcY[3], srcX[3];
    u16 digit;
    u8 i;

    digit = score / 100;
    srcY[0] = (digit % 5) * 4;
    srcX[0] = (digit / 5) * 8;
    digit = (u16)(score - 100 * digit) / 10u;
    srcY[1] = (digit % 5) * 4;
    srcX[1] = (digit / 5) * 8;
    digit = score % 10;
    srcY[2] = (digit % 5) * 4;
    srcX[2] = (digit / 5) * 8;
    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(display->bgConfig, GF_BG_LYR_SUB_1, 19 + i * 4, 16, 4, 8, display->scoreScreen->rawData, srcY[i], srcX[i], 20, 20);
    }
    ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_1);
}

void PokeathlonSnowThrowDisplay_SetPicAnim(PokeathlonSnowThrowDisplay *display, u8 member, int anim) {
    if (member >= SNOW_THROW_MEMBERS) {
        GF_AssertFail();
    }
    PokeathlonCourseSpritePoolSprite_TryChangeAnim(display->picSprites[member], anim + member);
}

#pragma force_active reset

BOOL PokeathlonSnowThrowDisplay_IsPicAnimated(PokeathlonSnowThrowDisplay *display, u8 member) {
    return PokeathlonCourseSpritePoolSprite_IsAnimated(display->picSprites[member]);
}

// The top screen shakes when an own Pokemon is hit.
void PokeathlonSnowThrowDisplay_Shake(PokeathlonSnowThrowDisplay *display) {
    display->shaking = TRUE;
    display->shakeTimer = 0;
}

void PokeathlonSnowThrowDisplay_UpdateShake(PokeathlonSnowThrowDisplay *display) {
    int offset;

    if (display->shaking) {
        display->shakeTimer++;
        offset = -((display->shakeTimer % 2) << 3);
        if (display->shakeTimer >= 10) {
            display->shaking = FALSE;
            display->shakeTimer = 0;
            offset = 0;
        }
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, offset);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, offset);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, offset);
    } else {
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(display->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 0);
    }
}

// The shadows under the own team's pictures.
#pragma force_active on

void PokeathlonSnowThrowDisplay_SetMonShadows(PokeathlonSnowThrowDisplay *display, void *pool, void *monResources) {
    PokeathlonCoursePoint positions[SNOW_THROW_MEMBERS];
    int i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        VecFx32 *position = PokeathlonCourseSpritePoolSprite_GetPosition(display->picSprites[i]);
        positions[i].x = position->x / FX32_ONE;
        positions[i].y = position->y / FX32_ONE;
        PokeathlonCourse_CreateStartSpritesInPool(display->course, 0, monResources, pool, NNS_G2D_VRAM_TYPE_2DSUB, 3, positions);
    }
}

#pragma force_active reset

void PokeathlonSnowThrowDisplay_LoadMonPics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons) {
    int i = 0;
    u8 *pic = (u8 *)display;
    PokepicTemplate template;

    // (steps a byte pointer through monPics, as built)
    do {
        GetMonSpriteCharAndPlttNarcIdsEx(&template, mons->species, mons->gender, 0, mons->isShiny, (u8)mons->form, mons->personality);
        ((PokeathlonSnowThrowMonPic *)(pic + offsetof(PokeathlonSnowThrowDisplay, monPics)))->character = CharTiles_AllocAndLoadMonPicAsObjFromNarc((NarcId) template.narcID, template.charDataID, display->heapId, mons->personality, FALSE, 0, mons->species);
        ((PokeathlonSnowThrowMonPic *)(pic + offsetof(PokeathlonSnowThrowDisplay, monPics)))->palette = AllocAndLoadPltt16FromNarc((NarcId) template.narcID, template.palDataID, display->heapId);
        mons++;
        pic += sizeof(PokeathlonSnowThrowMonPic);
        i++;
    } while (i < SNOW_THROW_MEMBERS);
}

// The own team's pictures (slid in from the left, 50 pixels apart) and the tired marks above them.
#pragma force_active on

void PokeathlonSnowThrowDisplay_CreatePicSprites(void *pool, PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons) {
    VecFx32 position;
    int i;
    volatile int x; // (kept in memory, as built)
    u8 *slot;
    PokeathlonCourseMonInfo *mon;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        PokeathlonCourseSpritePool_CreateSprite(pool, 3, 2, 101, 2);
    }
    mon = mons;
    slot = (u8 *)display;
    i = 0;
    x = 16;
    for (; i < SNOW_THROW_MEMBERS; i++) {
        void **picSprite = (void **)(slot + offsetof(PokeathlonSnowThrowDisplay, picSprites));
        void **tiredSprite = (void **)(slot + offsetof(PokeathlonSnowThrowDisplay, tiredSprites));

        *picSprite = PokeathlonCourseSpritePool_Take(pool, 101, 2);
        position.z = 0;
        position.x = x << FX32_SHIFT;
        position.y = (GetMonPicHeightBySpeciesGenderForm(mon->species, mon->gender, 0, mon->form, mon->personality) + 376) << FX32_SHIFT;
        PokeathlonCourseSpritePoolSprite_SetPosition(*picSprite, &position);
        PokeathlonCourseSpritePoolSprite_SetAnim(*picSprite, i + 1);
        PokeathlonCourseSpritePoolSprite_Show(*picSprite, TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(*picSprite, 19 - i);
        *tiredSprite = PokeathlonCourseSpritePool_Take(pool, 101, 5);
        PokeathlonCourseSpritePoolSprite_SetAnim(*tiredSprite, 0);
        position.y -= FX32_CONST(16);
        PokeathlonCourseSpritePoolSprite_SetPosition(*tiredSprite, &position);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(*tiredSprite, 4);
        {
            int next = x + 50;
            mon++;
            slot += sizeof(void *);
            x = next;
        }
    }
    PokeathlonSnowThrowDisplay_LoadMonPics(display, mons);
    PokeathlonSnowThrowDisplay_GetPicVramOffsets(pool, display);
    PokeathlonSnowThrowDisplay_TransferMonPics(display);
}

void PokeathlonSnowThrowDisplay_TransferMonPics(PokeathlonSnowThrowDisplay *display) {
    u8 i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        DC_FlushRange(display->monPics[i].character, 3200);
        GXS_LoadOBJ(display->monPics[i].character, display->picCharOffsets[i], 3200);
        DC_FlushRange(display->monPics[i].palette, 32);
        GXS_LoadOBJPltt(display->monPics[i].palette, display->picPlttOffsets[i], 32);
    }
}

#pragma force_active reset

// Each picture has its own 0xD00 bytes (after 0x300) of the resource's VRAM and its own palette (after the first).
void PokeathlonSnowThrowDisplay_GetPicVramOffsets(void *pool, PokeathlonSnowThrowDisplay *display) {
    void *charResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, 101, 0);
    void *plttResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, 101, 1);
    NNSG2dImageProxy *image = SpriteTransfer_GetCharProxy(charResource);
    NNSG2dImagePaletteProxy *palette = SpriteTransfer_GetPaletteProxy(plttResource, image);
    u8 i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        u32 *offsets = (u32 *)((u8 *)display + i * 4);
        u32 offset = i;
        offset *= 0xD00;
        *(u32 *)((u8 *)offsets + offsetof(PokeathlonSnowThrowDisplay, picCharOffsets)) = offset + 0x300 + NNS_G2dGetImageLocation(image, NNS_G2D_VRAM_TYPE_2DSUB);
        {
            u32 offset = i << 5;
            offset += 0x20;
            *(u32 *)((u8 *)offsets + offsetof(PokeathlonSnowThrowDisplay, picPlttOffsets)) = offset + NNS_G2dGetImagePaletteLocation(palette, NNS_G2D_VRAM_TYPE_2DSUB);
        }
    }
}

void PokeathlonSnowThrowDisplay_InitWindow(PokeathlonSnowThrowDisplay *display) {
    AddWindow(display->bgConfig, &display->nameWindow, &ov96_0221CA28);
    BG_FillCharDataRange(display->bgConfig, GF_BG_LYR_SUB_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, display->heapId);
    FillWindowPixelBuffer(&display->nameWindow, 0);
}

void PokeathlonSnowThrowDisplay_PrintName(PokeathlonSnowThrowDisplay *display) {
    u8 own = (u8)PokeathlonCourse_GetNetId(display->course);
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(display->course, own);
    String *name = PlayerProfile_GetPlayerName_NewString(profile, display->heapId);

    AddTextPrinterParameterizedWithColor(&display->nameWindow, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(name);
    CopyWindowToVram(&display->nameWindow);
}

// Touch screen pixels (y + 288: the touch screen is under the top screen) to field units, through the field's
// perspective.
#pragma force_active on

void PokeathlonSnowThrow_ScreenToField(int screenX, int screenY, float *fieldX, float *fieldZ) {
    float x = (float)screenX, y = (float)screenY;
    float z = (float)(22.53846153846154 + y * 5.0 / 52.0);
    float xOut = (float)((x * 0.19375 + y / 8.0 - 15.5 - 10.0) / (0.39402173913043476 + y / 512.0));
    *fieldZ = z;
    *fieldX = xOut;
}

#pragma force_active reset

// Field units to touch screen pixels (the inverse of PokeathlonSnowThrow_ScreenToField).
#pragma force_active on

void PokeathlonSnowThrow_FieldToScreen(float fieldX, float fieldZ, int *screenX, int *screenY) {
    float y = (float)(4.0 / (1.0 + (fieldZ - 57.0) * (fieldZ - 57.0)) + (11.0 + (fieldZ * fieldZ * fieldZ) / 7600.0 - (24.0 - fieldZ) * (24.0 - fieldZ) * (24.0 - fieldZ) / 4400.0 - 1.0));
    y *= 8.0;
    *screenY = (int)y;
    float x = (float)((10.0 + 0.34782608695652173 * fieldX) - (0.6451612903225806 * (y / 8.0 - 10.0) * (64.0 - fieldX) / 64.0));
    x *= 8.0;
    *screenX = (int)x;
}

#pragma force_active reset
