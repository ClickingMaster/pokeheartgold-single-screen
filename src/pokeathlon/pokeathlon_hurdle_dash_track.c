#include "pokeathlon/pokeathlon_hurdle_dash_race.h"

#include "global.h"

#include "constants/species.h"

#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "touchscreen.h"

PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_Take(PokeathlonCourseSpritePool *pool, int resId, int kind);
PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_CreateAndTake(PokeathlonCourseSpritePool *pool, int priority, int whichScreen, int resId, u8 kind);
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonCourseSpritePoolSprite *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonCourseSpritePoolSprite *entry, const VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(PokeathlonCourseSpritePoolSprite *entry);
Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite *entry, int priority);

static void PokeathlonHurdleDashStandings_LoadResources(PokeathlonHurdleDashStandings *standings);
static void PokeathlonHurdleDashStandings_InitResourcesHeader(PokeathlonHurdleDashStandings *standings);
static void PokeathlonHurdleDashStandings_CreateSprites(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons);
static void PokeathlonHurdleDashStandings_InitSpriteTemplate(SpriteTemplate *template, PokeathlonHurdleDashStandings *standings, const SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen);
static void PokeathlonHurdleDashDust_Remove(PokeathlonHurdleDashDust *dust);

// Where a new hurdle goes before its lane and row are added.
static const VecFx32 sHurdleDashNewHurdlePos = { 0, -FX32_CONST(8), 0 };

static PokeathlonHurdle *PokeathlonHurdleDash_AllocHurdles(u32 count, enum HeapID heapId) {
    u32 size = count * sizeof(PokeathlonHurdle);
    PokeathlonHurdle *hurdles = Heap_Alloc(heapId, size);
    MIi_CpuClearFast(0, (u32 *)hurdles, size);
    return hurdles;
}

static void PokeathlonHurdleDash_FreeHurdles(PokeathlonHurdle *hurdles) {
    Heap_Free(hurdles);
}

PokeathlonHurdle *PokeathlonHurdleDash_GetHurdle(PokeathlonHurdle *hurdles, int index) {
    return &hurdles[index];
}

static void PokeathlonHurdle_SetSprites(PokeathlonHurdle *hurdle, void *standingSprite, void *knockedSprite) {
    hurdle->standingSprite = standingSprite;
    hurdle->knockedSprite = knockedSprite;
}

static void PokeathlonHurdle_SetActive(PokeathlonHurdle *hurdle, BOOL active) {
    if (active) {
        hurdle->visible = TRUE;
        hurdle->standing = TRUE;
        PokeathlonCourseSpritePoolSprite_Show(hurdle->standingSprite, TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_Show(hurdle->knockedSprite, TRUE, FALSE);
        Sprite_SetAnimActiveFlag(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite), FALSE);
    } else {
        hurdle->visible = FALSE;
        hurdle->standing = FALSE;
        PokeathlonCourseSpritePoolSprite_Show(hurdle->standingSprite, FALSE, FALSE);
        PokeathlonCourseSpritePoolSprite_Show(hurdle->knockedSprite, FALSE, FALSE);
    }
}

static BOOL PokeathlonHurdle_IsVisible(PokeathlonHurdle *hurdle) {
    return hurdle->visible;
}

BOOL PokeathlonHurdle_IsStanding(PokeathlonHurdle *hurdle) {
    return hurdle->standing;
}

void PokeathlonHurdle_KnockDown(PokeathlonHurdle *hurdle) {
    Sprite *sprite;
    hurdle->standing = FALSE;
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite);
    PokeathlonCourseSpritePoolSprite_Show(hurdle->standingSprite, TRUE, FALSE);
    PokeathlonCourseSpritePoolSprite_Show(hurdle->knockedSprite, TRUE, TRUE);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 1);
}

static void PokeathlonHurdle_SetPosition(PokeathlonHurdle *hurdle, VecFx32 *position) {
    Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->standingSprite), position);
    Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite), position);
}

// Moves the hurdle down with the track; it is removed once it leaves the screen.
void PokeathlonHurdle_Scroll(PokeathlonHurdle *hurdle, int distance) {
    Sprite *standing;
    Sprite *knocked;
    VecFx32 position;
    if (hurdle->visible) {
        standing = PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->standingSprite);
        knocked = PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite);
        position = *Sprite_GetMatrixPtr(standing);
        position.y += distance << FX32_SHIFT;
        Sprite_SetMatrix(standing, &position);
        Sprite_SetMatrix(knocked, &position);
        if (position.y >= 0x208000) {
            PokeathlonHurdle_SetActive(hurdle, FALSE);
        }
    }
}

void PokeathlonHurdle_GetRect(PokeathlonHurdle *hurdle, PokeathlonHurdleDashRect *rect) {
    const VecFx32 *position = Sprite_GetMatrixPtr(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->standingSprite));
    rect->width = 64;
    rect->height = 16;
    rect->x = (s16)(position->x / FX32_ONE - 32);
    rect->y = (s16)(position->y / FX32_ONE - 16);
}

// Places the next of the lane's four hurdles y pixels below the top of the screen.

static PokeathlonHurdle *PokeathlonHurdleDashTrack_PlaceHurdle(PokeathlonHurdleDashTrack *track, int lane, int y) {
    PokeathlonHurdle *hurdle;
    VecFx32 position;
    hurdle = PokeathlonHurdleDash_GetHurdle(track->lanes[lane].hurdles, (u8)track->lanes[lane].next);
    if (PokeathlonHurdle_IsVisible(hurdle)) {
        GF_AssertFail();
        return NULL;
    }
    track->lanes[lane].next = (track->lanes[lane].next + 1) % 4;
    position = sHurdleDashNewHurdlePos;
    position.y += y << FX32_SHIFT;
    switch (lane) {
    case 0:
        position.x = 48 * FX32_ONE;
        break;
    case 1:
        position.x = 128 * FX32_ONE;
        break;
    case 2:
        position.x = 208 * FX32_ONE;
        break;
    }
    PokeathlonHurdle_SetPosition(hurdle, &position);
    PokeathlonHurdle_SetActive(hurdle, TRUE);
    return hurdle;
}

PokeathlonHurdleDashTrack *PokeathlonHurdleDashTrack_New(enum HeapID heapId) {
    int i;
    PokeathlonHurdleDashLane *lane;
    PokeathlonHurdleDashTrack *track = Heap_Alloc(heapId, sizeof(PokeathlonHurdleDashTrack));
    MI_CpuFill8(track, 0, sizeof(PokeathlonHurdleDashTrack));
    lane = track->lanes;
    i = 0;
    do {
        lane->hurdles = PokeathlonHurdleDash_AllocHurdles(4, heapId);
        i++;
        lane++;
    } while (i < 3);
    return track;
}

void PokeathlonHurdleDashTrack_Free(PokeathlonHurdleDashTrack *track) {
    PokeathlonHurdleDashTrack *base;
    int i;
    PokeathlonHurdleDashLane *lane;
    base = track;
    i = 0;
    lane = base->lanes;
    do {
        PokeathlonHurdleDash_FreeHurdles(lane->hurdles);
        i++;
        lane++;
    } while (i < 3);
    Heap_Free(base);
}

void PokeathlonHurdleDashTrack_CreateHurdleSprites(PokeathlonHurdleDashLane *lane, PokeathlonCourseSpritePool *spritePool) {
    int i = 0;
    int j;
    do {
        j = 0;
        do {
            PokeathlonHurdle *hurdle = PokeathlonHurdleDash_GetHurdle(lane->hurdles, j);
            PokeathlonCourseSpritePoolSprite *standing = PokeathlonCourseSpritePool_Take(spritePool, 101, 2);
            PokeathlonCourseSpritePoolSprite *knocked = PokeathlonCourseSpritePool_Take(spritePool, 103, 6);
            PokeathlonHurdle_SetSprites(hurdle, standing, knocked);
            j++;
        } while (j < 4);
        lane++;
        i++;
    } while (i < 3);
}

PokeathlonHurdle *PokeathlonHurdleDashTrack_GetLaneHurdles(PokeathlonHurdleDashTrack *track, u32 lane) {
    if (lane >= 3) {
        GF_AssertFail();
    }
    return track->lanes[lane].hurdles;
}

void *PokeathlonHurdleDashTrack_GetLayoutToSend(PokeathlonHurdleDashTrack *track) {
    return track->layoutToSend;
}

void *PokeathlonHurdleDashTrack_GetLayout(PokeathlonHurdleDashTrack *track) {
    return track->layout;
}

int PokeathlonHurdleDashTrack_GetLayoutSize(PokeathlonHurdleDashTrack *track) {
    return sizeof(track->layout);
}

// Places a hurdle once the flag of the layout at position (8 pixels each) comes into view.

void PokeathlonHurdleDashTrack_RevealHurdles(PokeathlonHurdleDashTrack *track, int position, int lane) {
    int flag = position / 8;
    int y;
    if (flag != 0) {
        y = position - flag * 8;
        flag--;
        if (flag < 512 && track->layout[lane][flag] != 0 && track->lastHurdle[lane] < flag) {
            track->lastHurdle[lane] = flag;
            PokeathlonHurdleDashTrack_PlaceHurdle(track, lane, y);
        }
    }
}

// The host picks one of the three hurdle layouts.
void PokeathlonHurdleDashTrack_LoadRandomLayout(PokeathlonHurdleDashTrack *track) {
    u8 layout = LCRandom() % 3;
    ReadWholeNarcMemberByIdPair(track->layoutToSend, NARC_a_2_2_6, (u8)layout);
}

// The flag of the layout at position, -1 if there is no hurdle there.

int PokeathlonHurdleDashTrack_GetHurdleAt(PokeathlonHurdleDashTrack *track, int lane, int position) {
    position /= 8;
    if (!track->layout[lane][position]) {
        position = -1;
    }
    return position;
}

PokeathlonHurdleDashStandings *PokeathlonHurdleDashStandings_New(enum HeapID heapId) {
    PokeathlonHurdleDashStandings *standings = Heap_Alloc(heapId, sizeof(PokeathlonHurdleDashStandings));
    MIi_CpuClearFast(0, (u32 *)standings, sizeof(PokeathlonHurdleDashStandings));
    standings->heapId = heapId;
    return standings;
}

void PokeathlonHurdleDashStandings_Free(PokeathlonHurdleDashStandings *standings) {
    int i;
    for (i = 0; i < 12; i++) {
        Sprite_Delete(standings->mons[i].icon);
        Sprite_Delete(standings->mons[i].frame);
        Sprite_Delete(standings->mons[i].rank);
    }
    SpriteTransfer_DeleteCharTransferTask(standings->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(standings->resources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(standings->resMans[i]);
    }
    SpriteList_Delete(standings->spriteList);
    Heap_Free(standings);
}

void PokeathlonHurdleDashStandings_Init(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons) {
    PokeathlonHurdleDashStandings_LoadResources(standings);
    PokeathlonHurdleDashStandings_InitResourcesHeader(standings);
    PokeathlonHurdleDashStandings_CreateSprites(standings, mons);
    G2dRenderer_SetSubSurfaceCoords(&standings->renderer, 0, 0);
    G2dRenderer_SetMainSurfaceCoords(&standings->renderer, 0, 192 * FX32_ONE);
}

void PokeathlonHurdleDashStandings_SetMonX(PokeathlonHurdleDashStandings *standings, int index, int x) {
    VecFx32 position;
    position.z = 0;
    position.x = x << FX32_SHIFT;
    position.y = 14 << FX32_SHIFT;
    Sprite_SetMatrix(standings->mons[index].icon, &position);
    Sprite_SetMatrix(standings->mons[index].frame, &position);
    Sprite_SetMatrix(standings->mons[index].rank, &position);
}

void PokeathlonHurdleDashStandings_SetRank(PokeathlonHurdleDashStandings *standings, u32 index, u32 rank) {
    GF_ASSERT(rank != 0 && rank <= 8);
    Sprite_SetAnimCtrlSeq(standings->mons[index].rank, rank + 15);
}

void PokeathlonHurdleDashStandings_SetDrawFlag(PokeathlonHurdleDashStandings *standings, BOOL draw) {
    int i;
    for (i = 0; i < 12; i++) {
        Sprite_SetDrawFlag(standings->mons[i].icon, draw);
        Sprite_SetDrawFlag(standings->mons[i].frame, draw);
    }
}

void PokeathlonHurdleDashStandings_SetMonDrawFlag(PokeathlonHurdleDashStandings *standings, int index, BOOL draw) {
    Sprite_SetDrawFlag(standings->mons[index].icon, draw);
    Sprite_SetDrawFlag(standings->mons[index].frame, draw);
    Sprite_SetDrawFlag(standings->mons[index].rank, draw);
}

void PokeathlonHurdleDashStandings_Draw(PokeathlonHurdleDashStandings *standings) {
    SpriteList_RenderAndAnimateSprites(standings->spriteList);
}

static void PokeathlonHurdleDashStandings_LoadResources(PokeathlonHurdleDashStandings *standings) {
    int i;
    standings->spriteList = G2dRenderer_Init(36, &standings->renderer, standings->heapId);
    for (i = 0; i < 6; i++) {
        standings->resMans[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, standings->heapId);
    }
    standings->resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_6, 24, FALSE, 0x80, NNS_G2D_VRAM_TYPE_2DSUB, standings->heapId);
    standings->resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_1_5_6, 21, FALSE, 0x80, NNS_G2D_VRAM_TYPE_2DSUB, 1, standings->heapId);
    standings->resources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_6, 23, FALSE, 0x80, GF_GFX_RES_TYPE_CELL, standings->heapId);
    standings->resources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_6, 22, FALSE, 0x80, GF_GFX_RES_TYPE_ANIM, standings->heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(standings->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreateExtPlttTransferTask(standings->resources[GF_GFX_RES_TYPE_PLTT]);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_3_OFFSET, 0, standings->heapId);
}

static void PokeathlonHurdleDashStandings_InitResourcesHeader(PokeathlonHurdleDashStandings *standings) {
    CreateSpriteResourcesHeader(&standings->header, 0x80, 0x80, 0x80, 0x80, -1, -1, FALSE, 0, standings->resMans[GF_GFX_RES_TYPE_CHAR], standings->resMans[GF_GFX_RES_TYPE_PLTT], standings->resMans[GF_GFX_RES_TYPE_CELL], standings->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

// The icon (its own Pokemon icon tiles, 0x200 bytes each), team frame and rank of each Pokemon; Pokemon
// that don't exist are hidden.

static void PokeathlonHurdleDashStandings_CreateSprites(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons) {
    SpriteTemplate template;
    NNSG2dCharacterData *charData;
    u32 loadSize;
    u32 flushSize;
    NARC *narc;
    void *buffer;
    u8 form;
    u32 offset;
    int x;
    int i;
    u16 species;
    u8 palette;
    narc = NARC_New(NARC_poketool_icongra_poke_icon, standings->heapId);
    buffer = Heap_AllocAtEnd(standings->heapId, 0x1000);
    PokeathlonHurdleDashStandings_InitSpriteTemplate(&template, standings, &standings->header, NNS_G2D_VRAM_TYPE_2DSUB);
    offset = 0x200;
    x = 16;
    flushSize = offset;
    loadSize = offset;
    for (i = 0; i < 12; i++, x += 32) {
        standings->mons[i].icon = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(standings->mons[i].icon, TRUE);
        Sprite_SetAnimCtrlSeq(standings->mons[i].icon, i);
        standings->mons[i].frame = Sprite_CreateAffine(&template);
        Sprite_SetAnimCtrlSeq(standings->mons[i].frame, i / 3 + 12);
        standings->mons[i].rank = Sprite_CreateAffine(&template);
        if (i < 8) {
            Sprite_SetAnimCtrlSeq(standings->mons[i].rank, i + 16);
        }
        species = mons[i].species;
        form = mons[i].form;
        if (species == SPECIES_NONE) {
            Sprite_SetDrawFlag(standings->mons[i].icon, FALSE);
            Sprite_SetDrawFlag(standings->mons[i].frame, FALSE);
            Sprite_SetDrawFlag(standings->mons[i].rank, FALSE);
        }
        Sprite_SetDrawPriority(standings->mons[i].icon, 2);
        Sprite_SetDrawPriority(standings->mons[i].frame, 3);
        Sprite_SetDrawPriority(standings->mons[i].rank, 1);
        PokeathlonHurdleDashStandings_SetMonX(standings, i, x);
        Sprite_SetDrawFlag(standings->mons[i].rank, TRUE);
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(species, FALSE, form), buffer);
        NNS_G2dGetUnpackedBGCharacterData(buffer, &charData);
        palette = GetMonIconPaletteEx(species, form, FALSE) + 3;
        DC_FlushRange(charData->pRawData, flushSize);
        GXS_LoadOBJ(charData->pRawData, offset, loadSize);
        Sprite_SetPaletteOverride(standings->mons[i].icon, palette);
        offset += 0x200;
    }
    Heap_Free(buffer);
    NARC_Delete(narc);
}

static void PokeathlonHurdleDashStandings_InitSpriteTemplate(SpriteTemplate *template, PokeathlonHurdleDashStandings *standings, const SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen) {
    u32 zero = 0;
    template->spriteList = standings->spriteList;
    template->header = &standings->header;
    template->position.x = 240 * FX32_ONE;
    template->position.y = 14 * FX32_ONE;
    template->position.z = zero;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = zero;
    template->drawPriority = 2;
    template->whichScreen = whichScreen;
    template->heapID = standings->heapId;
}

PokeathlonHurdleDashDustLane *PokeathlonHurdleDashDust_New(enum HeapID heapId) {
    PokeathlonHurdleDashDustLane *lanes = Heap_Alloc(heapId, 3 * sizeof(PokeathlonHurdleDashDustLane));
    MI_CpuFill8(lanes, 0, 3 * sizeof(PokeathlonHurdleDashDustLane));
    return lanes;
}

void PokeathlonHurdleDashDust_CreateSprites(PokeathlonHurdleDashDustLane *lanes, PokeathlonCourseSpritePool *spritePool, u16 priority) {
    u8 i;
    u8 j;
    for (i = 0; i < 3; i++) {
        PokeathlonHurdleDashDustLane *lane = &lanes[i];
        for (j = 0; j < 5; j++) {
            PokeathlonHurdleDashDust *dust = &lane->dust[j];
            dust->sprite = PokeathlonCourseSpritePool_CreateAndTake(spritePool, priority, NNS_G2D_VRAM_TYPE_2DMAIN, 106, 10);
            PokeathlonCourseSpritePoolSprite_Show(dust->sprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_SetDrawPriority(dust->sprite, 5);
        }
    }
}

void PokeathlonHurdleDashDust_Free(PokeathlonHurdleDashDustLane *lanes) {
    Heap_Free(lanes);
}

void PokeathlonHurdleDashDust_Spawn(PokeathlonHurdleDashDustLane *lanes, u32 lane, const VecFx32 *position) {
    PokeathlonHurdleDashDustLane *selected;
    PokeathlonHurdleDashDust *dust;
    if (lane >= 3) {
        GF_AssertFail();
    }
    selected = &lanes[lane];
    dust = &selected->dust[selected->next];
    if (dust->active == FALSE) {
        PokeathlonCourseSpritePoolSprite_Show(dust->sprite, TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_SetAnim(dust->sprite, 1);
        PokeathlonCourseSpritePoolSprite_SetPosition(dust->sprite, position);
        dust->active = TRUE;
        selected->next = (selected->next + 1) % 5;
    } else {
        GF_AssertFail();
    }
}

void PokeathlonHurdleDashDust_Scroll(PokeathlonHurdleDashDustLane *lanes, u32 lane, u32 distance) {
    PokeathlonHurdleDashDustLane *selected;
    u8 i;
    VecFx32 position;
    if (lane >= 3) {
        GF_AssertFail();
    }
    selected = &lanes[lane];
    for (i = 0; i < 5; i++) {
        PokeathlonHurdleDashDust *dust = &selected->dust[i];
        if (dust->active != FALSE) {
            position = *PokeathlonCourseSpritePoolSprite_GetPosition(dust->sprite);
            position.y += distance << FX32_SHIFT;
            PokeathlonCourseSpritePoolSprite_SetPosition(dust->sprite, &position);
            if (PokeathlonCourseSpritePoolSprite_IsAnimated(dust->sprite) == FALSE) {
                PokeathlonHurdleDashDust_Remove(dust);
            }
        }
    }
}

static void PokeathlonHurdleDashDust_Remove(PokeathlonHurdleDashDust *dust) {
    dust->active = FALSE;
    PokeathlonCourseSpritePoolSprite_Show(dust->sprite, TRUE, FALSE);
}
