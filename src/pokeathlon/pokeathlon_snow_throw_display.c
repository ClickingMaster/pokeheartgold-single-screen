#include "pokeathlon/pokeathlon_snow_throw.h"

#include "global.h"

#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "assert.h"
#include "bg_window.h"
#include "char_tiles.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"

void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *mtx, VecFx32 *result);
void PokeathlonCourseSpritePool_LoadChar(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId, int vram);
void PokeathlonCourseSpritePool_LoadPltt(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId, int vram, int count);
void PokeathlonCourseSpritePool_LoadCell(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId);
void PokeathlonCourseSpritePool_LoadAnim(PokeathlonCourseSpritePool *pool, int narcId, int fileId, int resId);
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonCourseSpritePoolSprite *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite *entry, int priority);
void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePool_CreateSprite(PokeathlonCourseSpritePool *pool, int priority, int whichScreen, int resId, int kind);
PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_Take(PokeathlonCourseSpritePool *pool, int resId, int kind);
void **PokeathlonCourseSpritePool_GetResourceSlot(PokeathlonCourseSpritePool *pool, u32 id, u32 kind);
void PokeathlonCourseSpritePoolSprite_TryChangeAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(PokeathlonCourseSpritePoolSprite *entry);

static void PokeathlonSnowThrowDisplay_CreatePicSprites(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons);
static void PokeathlonSnowThrowDisplay_TransferMonPics(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_GetPicVramOffsets(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_InitWindow(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowDisplay_PrintName(PokeathlonSnowThrowDisplay *display);
static void PokeathlonSnowThrowCpu_Think(PokeathlonSnowThrowCpu *cpu, u8 team, u8 mon, int elapsed);

static const WindowTemplate sSnowThrowDisplayNameWindowTemplate = {
    .bgId = GF_BG_LYR_SUB_0,
    .left = 5,
    .top = 19,
    .width = 8,
    .height = 2,
    .palette = 15,
    .baseTile = 1,
};

// Where each Pokemon stands on the field (fx32 field units; the CPU's copy of the table).
static const VecFx32 sSnowThrowCpuMonPositions[SNOW_THROW_MONS] = {
    { FX32_CONST(25), 0, FX32_CONST(61) },
    { FX32_CONST(33), 0, FX32_CONST(61) },
    { FX32_CONST(41), 0, FX32_CONST(61) },
    { FX32_CONST(61), 0, FX32_CONST(41) },
    { FX32_CONST(61), 0, FX32_CONST(33) },
    { FX32_CONST(61), 0, FX32_CONST(25) },
    { FX32_CONST(41), 0, FX32_CONST(5) },
    { FX32_CONST(33), 0, FX32_CONST(5) },
    { FX32_CONST(25), 0, FX32_CONST(5) },
    { FX32_CONST(5), 0, FX32_CONST(25) },
    { FX32_CONST(5), 0, FX32_CONST(33) },
    { FX32_CONST(5), 0, FX32_CONST(41) },
};

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

void PokeathlonSnowThrowDisplay_LoadSpriteGraphics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool) {
#pragma unused(display)
    PokeathlonCourseSpritePool_LoadChar(pool, NARC_a_2_1_7, 13, 101, 2);
    PokeathlonCourseSpritePool_LoadPltt(pool, NARC_a_2_1_7, 10, 101, 2, 4);
    PokeathlonCourseSpritePool_LoadCell(pool, NARC_a_2_1_7, 12, 101);
    PokeathlonCourseSpritePool_LoadAnim(pool, NARC_a_2_1_7, 11, 101);
}

void PokeathlonSnowThrowDisplay_CreateSprites(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseMonInfo *mons) {
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

void PokeathlonSnowThrowDisplay_SetMonShadows(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseSpriteResources *monResources) {
    PokeathlonCoursePoint positions[SNOW_THROW_MEMBERS];
    int i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        VecFx32 *position = PokeathlonCourseSpritePoolSprite_GetPosition(display->picSprites[i]);
        positions[i].x = position->x / FX32_ONE;
        positions[i].y = position->y / FX32_ONE;
        PokeathlonCourse_CreateStartSpritesInPool(display->course, 0, monResources, pool, NNS_G2D_VRAM_TYPE_2DSUB, 3, positions);
    }
}

static void PokeathlonSnowThrowDisplay_LoadMonPics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons) {
    int i = 0;
    u8 *pic = (u8 *)display;
    PokepicTemplate template;

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

static void PokeathlonSnowThrowDisplay_CreatePicSprites(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons) {
    VecFx32 position;
    int i;
    volatile int x;
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

static void PokeathlonSnowThrowDisplay_TransferMonPics(PokeathlonSnowThrowDisplay *display) {
    u8 i;

    for (i = 0; i < SNOW_THROW_MEMBERS; i++) {
        DC_FlushRange(display->monPics[i].character, 3200);
        GXS_LoadOBJ(display->monPics[i].character, display->picCharOffsets[i], 3200);
        DC_FlushRange(display->monPics[i].palette, 32);
        GXS_LoadOBJPltt(display->monPics[i].palette, display->picPlttOffsets[i], 32);
    }
}

// Each picture has its own part of the resource's VRAM and its own palette (after the first).
static void PokeathlonSnowThrowDisplay_GetPicVramOffsets(PokeathlonCourseSpritePool *pool, PokeathlonSnowThrowDisplay *display) {
    SpriteResource *charResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, 101, 0);
    SpriteResource *plttResource = *PokeathlonCourseSpritePool_GetResourceSlot(pool, 101, 1);
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

static void PokeathlonSnowThrowDisplay_InitWindow(PokeathlonSnowThrowDisplay *display) {
    AddWindow(display->bgConfig, &display->nameWindow, &sSnowThrowDisplayNameWindowTemplate);
    BG_FillCharDataRange(display->bgConfig, GF_BG_LYR_SUB_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, display->heapId);
    FillWindowPixelBuffer(&display->nameWindow, 0);
}

static void PokeathlonSnowThrowDisplay_PrintName(PokeathlonSnowThrowDisplay *display) {
    u8 own = (u8)PokeathlonCourse_GetNetId(display->course);
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(display->course, own);
    String *name = PlayerProfile_GetPlayerName_NewString(profile, display->heapId);

    AddTextPrinterParameterizedWithColor(&display->nameWindow, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(name);
    CopyWindowToVram(&display->nameWindow);
}

// Touch screen pixels (y + 288: the touch screen is under the top screen) to field units, through the field's
// perspective.

void PokeathlonSnowThrow_ScreenToField(int screenX, int screenY, float *fieldX, float *fieldZ) {
    float x = (float)screenX, y = (float)screenY;
    float z = (float)(22.53846153846154 + y * 5.0 / 52.0);
    float xOut = (float)((x * 0.19375 + y / 8.0 - 15.5 - 10.0) / (0.39402173913043476 + y / 512.0));
    *fieldZ = z;
    *fieldX = xOut;
}

// Field units to touch screen pixels (the inverse of PokeathlonSnowThrow_ScreenToField).

void PokeathlonSnowThrow_FieldToScreen(float fieldX, float fieldZ, int *screenX, int *screenY) {
    float y = (float)(4.0 / (1.0 + (fieldZ - 57.0) * (fieldZ - 57.0)) + (11.0 + (fieldZ * fieldZ * fieldZ) / 7600.0 - (24.0 - fieldZ) * (24.0 - fieldZ) * (24.0 - fieldZ) / 4400.0 - 1.0));
    y *= 8.0;
    *screenY = (int)y;
    float x = (float)((10.0 + 0.34782608695652173 * fieldX) - (0.6451612903225806 * (y / 8.0 - 10.0) * (64.0 - fieldX) / 64.0));
    x *= 8.0;
    *screenX = (int)x;
}

// Turns a direction by a random angle of at most `degrees` either way.
void PokeathlonSnowThrow_AddAimSpread(int degrees, VecFx32 *direction) {
    MtxFx43 rotation;
    u16 angle = (u16)(0xB6 * (degrees - (LCRandom() % (degrees * 2))));
    MTX_RotY43(&rotation, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_MultVec43(direction, &rotation, direction);
}

PokeathlonSnowThrowCpu *PokeathlonSnowThrowCpu_New(enum HeapID heapId, int cpuCount, int mode) {
    PokeathlonSnowThrowCpu *cpu = Heap_Alloc(heapId, sizeof(PokeathlonSnowThrowCpu));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonSnowThrowCpu));
    cpu->cpuCount = cpuCount;
    cpu->humanCount = SNOW_THROW_TEAMS - cpuCount;
    cpu->firstCpu = cpu->humanCount;
    cpu->interval = 40;
    cpu->timer = cpu->interval;
    return cpu;
}

void PokeathlonSnowThrowCpu_SetMon(PokeathlonSnowThrowCpu *cpu, u8 index, PokeathlonSnowThrowMon *mon, PokeathlonSnowThrowBall *ball) {
    cpu->mons[index] = mon;
    cpu->balls[index] = ball;
}

void PokeathlonSnowThrowCpu_Delete(PokeathlonSnowThrowCpu *cpu) {
    Heap_Free(cpu);
}

// Called by the host every frame with the frames gone by. Every `interval` frames it looks at one member of each
// CPU team in turn (the next member the next time). Returns the Pokemon looked at, SNOW_THROW_NONE if none.
int PokeathlonSnowThrowCpu_Update(PokeathlonSnowThrowCpu *cpu, int elapsed) {
    int result = SNOW_THROW_NONE;
    int team;
    int mon;

    if (cpu->humanCount == SNOW_THROW_TEAMS) {
        return 0;
    }

    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->thinking = TRUE;
        cpu->timer = 0;
    }

    if (cpu->thinking) {
        team = cpu->firstCpu + cpu->cursor;
        mon = cpu->member + team * SNOW_THROW_MEMBERS;
        if (mon < cpu->firstCpu) {
            GF_AssertFail();
        }
        if ((cpu->balls[mon]->state == SNOW_THROW_BALL_NONE || cpu->balls[mon]->state == SNOW_THROW_BALL_HELD) && cpu->mons[mon]->condition != SNOW_THROW_STUNNED) {
            PokeathlonSnowThrowCpu_Think(cpu, (u8)team, (u8)mon, elapsed);
        }
        result = (u8)mon;
        cpu->cursor++;
        if (cpu->cursor >= cpu->cpuCount) {
            cpu->cursor = 0;
            cpu->member = (cpu->member + 1) % SNOW_THROW_MEMBERS;
            cpu->thinking = FALSE;
        }
    }

    return result;
}

// A CPU Pokemon with enough energy (by chance) makes its snowball bigger, up to 4 taps; otherwise it throws at one
// of the other teams' Pokemon that are best to hit (many hits, a stun, low energy; by chance), with its aim spread
// and at a speed of 1 to 2.
static void PokeathlonSnowThrowCpu_Think(PokeathlonSnowThrowCpu *cpu, u8 team, u8 mon, int elapsed) {
    int i;
    u8 count;
    u8 pick;
    f32 speed;
    PokeathlonSnowThrowCpuTarget *targets[SNOW_THROW_MONS];
    PokeathlonSnowThrowCpuTarget *sorted[SNOW_THROW_MONS - SNOW_THROW_MEMBERS];
    VecFx32 direction;
    const VecFx32 *home;

    if (cpu->mons[mon]->energy + LCRandom() % 51 <= SNOW_THROW_MAX_ENERGY && cpu->balls[mon]->taps < 4) {
        if (cpu->balls[mon]->state == SNOW_THROW_BALL_NONE) {
            cpu->balls[mon]->taps = 1;
            cpu->balls[mon]->state = SNOW_THROW_BALL_HELD;
            cpu->balls[mon]->position = sSnowThrowCpuMonPositions[mon];
            return;
        }
        GF_ASSERT(cpu->balls[mon]->state == SNOW_THROW_BALL_HELD);
        cpu->balls[mon]->taps++;
        if (cpu->balls[mon]->taps <= 1) {
            cpu->balls[mon]->size = 0;
        } else if (cpu->balls[mon]->taps <= 2) {
            cpu->balls[mon]->size = 1;
        } else if (cpu->balls[mon]->taps <= 3) {
            cpu->balls[mon]->size = 2;
        } else {
            cpu->balls[mon]->size = 3;
        }
        return;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        cpu->targets[i].mon = cpu->mons[i];
        cpu->targets[i].index = i;
        cpu->targets[i].score = cpu->mons[i]->hits / 20 + LCRandom() % 21;
        if (cpu->mons[i]->condition == SNOW_THROW_STUNNED) {
            cpu->targets[i].score++;
        }
        cpu->targets[i].score += (SNOW_THROW_MAX_ENERGY - cpu->mons[i]->energy) / 20;
    }
    for (i = 0; i < SNOW_THROW_MONS; i++) {
        targets[i] = &cpu->targets[i];
    }
    count = PokeathlonSnowThrowCpu_SortTargets(team, targets, sorted);
    pick = LCRandom() % count;
    {
        VecFx32 zero = { 0, 0, 0 };
        home = &sSnowThrowCpuMonPositions[mon];
        VEC_Subtract(&sSnowThrowCpuMonPositions[(u8)sorted[pick]->index], home, &direction);
        VEC_Normalize(&direction, &direction);
        PokeathlonSnowThrow_AddAimSpread(3, &direction);
        PokeathlonSnowThrow_AddAimSpread((u8)cpu->mons[mon]->aimSpread, &direction);
        // (always at least 2: the random part is not scaled down)
        speed = (f32)elapsed / 40.0 + ((f32)cpu->mons[mon]->energy / 50.0 + (f32)(LCRandom() % 51));
        if (speed > 2.0) {
            speed = 2.0f;
        } else if (speed < 1.0) {
            speed = 1.0f;
        }
        VEC_MultAdd((fx32)(4096.0f * speed), &direction, &zero, &cpu->balls[mon]->velocity);
        cpu->balls[mon]->state = SNOW_THROW_BALL_FLYING;
        cpu->balls[mon]->position = *home;
    }
}

// Insertion sort of the other teams' nine Pokemon by score, best first (into a linked list). Returns how many share
// the best score.
u8 PokeathlonSnowThrowCpu_SortTargets(u32 ownTeam, PokeathlonSnowThrowCpuTarget **targets, PokeathlonSnowThrowCpuTarget **sorted) {
    u8 count;
    u8 i;
    u8 bestCount;
    PokeathlonSnowThrowCpuTargetNode nodes[SNOW_THROW_MONS - SNOW_THROW_MEMBERS + 1];

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].target = NULL;
    bestCount = 0;
    count = bestCount;

    for (i = count; i < SNOW_THROW_MONS; i++) {
        if (i / SNOW_THROW_MEMBERS != ownTeam) {
            u8 j;
            PokeathlonSnowThrowCpuTarget *target = targets[i];
            PokeathlonSnowThrowCpuTargetNode *node = &nodes[count + 1];
            PokeathlonSnowThrowCpuTargetNode *cur = &nodes[0];

            node->target = target;
            node->next = NULL;
            node->prev = NULL;
            j = 0;
            if (count > bestCount) {
                u16 score = node->target->score;
                do {
                    cur = cur->next;
                    if (cur->target->score < score) {
                        cur->prev->next = node;
                        node->prev = cur->prev;
                        node->next = cur;
                        cur->prev = node;
                        break;
                    }
                    j++;
                } while (j < count);
            }
            if (j == count) {
                cur->next = node;
                node->prev = cur;
            }
            count++;
        }
    }

    {
        PokeathlonSnowThrowCpuTargetNode *cur = nodes[0].next;
        u16 best = cur->target->score;
        cur = &nodes[0];
        for (i = 0; i < SNOW_THROW_MONS - SNOW_THROW_MEMBERS; i++) {
            cur = cur->next;
            sorted[i] = cur->target;
            if (best == sorted[i]->score) {
                bestCount++;
            }
        }
    }
    if (bestCount == 0) {
        GF_AssertFail();
    }
    return bestCount;
}
