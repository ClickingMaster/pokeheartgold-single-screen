#include "global.h"

#include "constants/balls.h"
#include "constants/moves.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "move.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_summary_internal.h"
#include "ribbon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "type_icon_resources.h"
#include "pokemon_summary.h"
#include "vram_transfer_manager.h"

// Pokemon summary screen sprites (ARM9 static 0x0208B1AC-0x0208C3E4): the sprite system of the screen and the
// sprites of each page (type, move and status icons, the shown Pokemon's and the party's icons, markings,
// sheen sparkles, condition, ribbon, Pokeathlon star and shiny leaf sprites).
// Tables: asm/pokemon_summary_sprites_data.s. The force_active regions keep those functions in the link as before.

extern const ResdatIdList gPokemonSummarySpriteResdatIds;
extern const OamCharTransferParam gPokemonSummaryOamTransferParam;
extern const OamManagerParam gPokemonSummaryOamManagerParam;
extern const UnmanagedSpriteTemplate gPokemonSummarySpriteTemplates[POKEMON_SUMMARY_SPRITE_COUNT];
extern const u8 gPokemonSummaryBallPalettes[];
extern const u8 gPokemonSummaryPartySlotPositions[][2];

// Declared with the types their call sites were matched against.
NarcId MoveCategoryIcon_GetNarc(u32 category); // the definition takes no argument; the call passes one

// Called with an int where the prototype has a u8.
#define Sprite_SetFlipMode_Int(sprite, flip) ((void (*)(Sprite *, int))Sprite_SetFlipMode)(sprite, flip)
#define GetRibbonAttr_Int(ribbon, attr)      ((u32(*)(int, RibbonAttr))GetRibbonAttr)(ribbon, attr)

// PokemonSummary_SetPokeathlonStatStars passes the star color as an int.
#define PokemonSummary_GetPokeathlonStarStates_Int(states, maxStars, baseStars, color, modifier) ((int (*)(u8 *, int, int, int, int))PokemonSummary_GetPokeathlonStarStates)(states, maxStars, baseStars, color, modifier)

// The Pokeathlon star counts as CalcBoxmonPokeathlonStars writes them (struct PokeathlonPerformanceStars).
typedef struct PokeathlonStarCounts {
    u16 stat0 : 3;
    u16 stat1 : 3;
    u16 stat2 : 3;
    u16 stat3 : 3;
    u16 stat4 : 3;
    u16 dummy : 1;
    u8 color[PERFORMANCE_MAX];
    u8 dummy7;
} PokeathlonStarCounts;

#pragma force_active on

void PokemonSummary_InitSpriteSystem(PokemonSummary *summary) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamTransferParam;
    ResdatIdList resdatIds;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GF_CreateVramTransferManager(32, HEAP_ID_19);
    summary->spriteSystem = SpriteSystem_Alloc(HEAP_ID_19);
    summary->spriteManager = SpriteManager_New(summary->spriteSystem);
    oamManagerParam = gPokemonSummaryOamManagerParam;
    oamTransferParam = gPokemonSummaryOamTransferParam;
    SpriteSystem_Init(summary->spriteSystem, &oamManagerParam, &oamTransferParam, 32);
    SpriteSystem_InitSprites(summary->spriteSystem, summary->spriteManager, POKEMON_SUMMARY_SPRITE_COUNT);
    resdatIds = gPokemonSummarySpriteResdatIds;
    SpriteSystem_LoadResourceDataFromFileIdList(summary->spriteSystem, summary->spriteManager, &resdatIds);
}

#pragma force_active off

void PokemonSummary_FreeSpriteSystem(PokemonSummary *summary) {
    SpriteSystem_DestroySpriteManager(summary->spriteSystem, summary->spriteManager);
    SpriteSystem_Free(summary->spriteSystem);
}

void PokemonSummary_UpdateSpriteAnims(PokemonSummary *summary) {
    Sprite_UpdateAnim(summary->sprites[21], FX32_ONE);
    Sprite_UpdateAnim(summary->sprites[22], FX32_ONE);
    Sprite_UpdateAnim(summary->sprites[64], FX32_ONE);
    Sprite_UpdateAnim(summary->sprites[65], FX32_ONE);
}

#pragma force_active on

void PokemonSummary_CreateSprites(PokemonSummary *summary) {
    for (u16 i = 0; i < POKEMON_SUMMARY_SPRITE_COUNT; i++) {
        summary->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(summary->spriteSystem, summary->spriteManager, &gPokemonSummarySpriteTemplates[i]);
    }
    PokemonSummary_LoadTypeAndIconSprites(summary);
    Sprite_SetDrawFlag(summary->sprites[10], FALSE);
    Sprite_SetDrawFlag(summary->sprites[41], FALSE);
    Sprite_SetDrawFlag(summary->sprites[42], FALSE);
    Sprite_SetDrawFlag(summary->sprites[43], FALSE);
    Sprite_SetDrawFlag(summary->sprites[44], FALSE);
    Sprite_SetDrawFlag(summary->sprites[45], FALSE);
    Sprite_SetDrawFlag(summary->sprites[46], FALSE);
    Sprite_SetDrawFlag(summary->sprites[47], FALSE);
    Sprite_SetDrawFlag(summary->sprites[48], FALSE);
    Sprite_SetDrawFlag(summary->sprites[49], FALSE);
    Sprite_SetDrawFlag(summary->sprites[50], FALSE);
    Sprite_SetDrawFlag(summary->sprites[51], FALSE);
    Sprite_SetDrawFlag(summary->sprites[67], FALSE);
    Sprite_SetDrawFlag(summary->sprites[68], FALSE);
    Sprite_SetDrawFlag(summary->sprites[69], FALSE);
    Sprite_SetDrawFlag(summary->sprites[70], FALSE);
    Sprite_SetDrawFlag(summary->sprites[71], FALSE);
    PokemonSummary_HidePokeathlonStars(summary);
    PokemonSummary_InitShinyLeafSprites(summary);
    Sprite_SetDrawFlag(summary->sprites[72], FALSE);
}

// Sprite 9 marks the shown Pokemon's party slot (not for a box Pokemon).
void PokemonSummary_UpdatePartySlotSprite(PokemonSummary *summary) {
    if (summary->args->monDataType != 2) {
        Sprite_SetPositionXY(summary->sprites[9], gPokemonSummaryPartySlotPositions[summary->args->partySlot][0], gPokemonSummaryPartySlotPositions[summary->args->partySlot][1]);
        Sprite_SetAnimCtrlSeq(summary->sprites[9], 2);
    } else {
        Sprite_SetDrawFlag(summary->sprites[9], FALSE);
    }
}

#pragma force_active off

void sub_0208B448(PokemonSummary *summary) {
    Sprite **sprites = summary->sprites;
    u8 i;
    for (i = 0; i < 8; i++) {
        Sprite_SetDrawFlag(sprites[i], FALSE);
    }
    Sprite_SetDrawFlag(summary->sprites[21], FALSE);
    Sprite_SetDrawFlag(summary->sprites[22], FALSE);
}

void PokemonSummary_LoadBallSprite(PokemonSummary *summary) {
    int pokeball = summary->mon.pokeball;
    int charFile = pokeball == BALL_NONE ? 25 : pokeball + 24;
    SpriteSystem_ReplaceCharResObj(summary->spriteSystem, summary->spriteManager, NARC_a_1_6_2, charFile, FALSE, 0);
    SpriteSystem_ReplacePlttResObj(summary->spriteSystem, summary->spriteManager, NARC_a_1_6_2, gPokemonSummaryBallPalettes[summary->mon.pokeball] + 49, FALSE, 6);
}

// Sprite 20 is the status condition icon, sprite 72 the "no status" one; 7 = fainted (neither).
void PokemonSummary_UpdateStatusSprite(PokemonSummary *summary) {
    if (summary->mon.status == 7) {
        Sprite_SetDrawFlag(summary->sprites[20], FALSE);
        Sprite_SetDrawFlag(summary->sprites[72], FALSE);
        return;
    }
    if (summary->mon.status != 0) {
        Sprite_SetDrawFlag(summary->sprites[72], FALSE);
        Sprite_SetDrawFlag(summary->sprites[20], TRUE);
        Sprite_SetAnimCtrlSeq(summary->sprites[20], summary->mon.status);
        return;
    }
    Sprite_SetDrawFlag(summary->sprites[72], TRUE);
    Sprite_SetDrawFlag(summary->sprites[20], FALSE);
}

void PokemonSummary_LoadTypeAndIconSprites(PokemonSummary *summary) {
    u16 i;
    PokemonSummary_SetMonTypeSprites(summary);
    for (i = 0; i < MAX_MON_MOVES; i++) {
        u16 move = summary->mon.moves[i];
        if (move != MOVE_NONE) {
            PokemonSummary_SetTypeSprite(summary, (u8)(i + 13), (u8)(i + 5), (u8)GetMoveAttr(move, MOVEATTR_TYPE));
        }
    }
    PokemonSummary_LoadShownMonIcon(summary);
}

#pragma force_active on

// Type sprites: 11 and 12 the Pokemon's types, 13..16 its moves', 17 the move to learn; 18 the move category.
void PokemonSummary_UpdateTypeSprites(PokemonSummary *summary) {
    for (u16 i = 11; i <= 19; i++) {
        thunk_Sprite_SetDrawFlag(summary->sprites[i], FALSE);
    }
    if (summary->mon.isEgg) {
        return;
    }
    switch (summary->page) {
    case 0:
        thunk_Sprite_SetDrawFlag(summary->sprites[11], TRUE);
        if (summary->mon.type1 == summary->mon.type2) {
            Sprite_SetPositionXY(summary->sprites[11], 108, 48);
        } else {
            Sprite_SetPositionXY(summary->sprites[11], 91, 48);
            thunk_Sprite_SetDrawFlag(summary->sprites[12], TRUE);
            Sprite_SetPositionXY(summary->sprites[12], 125, 48);
        }
        break;
    case 1:
        for (u16 i = 0; i < MAX_MON_MOVES; i++) {
            if (summary->mon.moves[i]) {
                u32 type = GetMoveAttr(summary->mon.moves[i], MOVEATTR_TYPE);
                PokemonSummary_SetTypeSprite(summary, (u8)(i + 13), (u8)(i + 5), (u8)type);
                thunk_Sprite_SetDrawFlag(summary->sprites[i + 13], TRUE);
                Sprite_SetPositionXY(summary->sprites[i + 13], 24, (s16)(32 * i + 16));
            }
        }
        if (summary->args->moveToLearn) {
            u32 type = GetMoveAttr(summary->args->moveToLearn, MOVEATTR_TYPE);
            PokemonSummary_SetTypeSprite(summary, 17, 9, (u8)type);
            thunk_Sprite_SetDrawFlag(summary->sprites[17], TRUE);
            Sprite_SetPositionXY(summary->sprites[17], 24, 160);
        } else {
            thunk_Sprite_SetDrawFlag(summary->sprites[17], FALSE);
        }
        Sprite_SetPositionXY(summary->sprites[11], 200, 24);
        Sprite_SetPositionXY(summary->sprites[12], 234, 24);
        Sprite_SetPositionXY(summary->sprites[18], 232, 40);
        break;
    }
}

#pragma force_active off

// Sprites 79..103: 5 stars for each of the 5 Pokeathlon stats; 104..108: each stat's Aprijuice modifier.
void PokemonSummary_HidePokeathlonStars(PokemonSummary *summary) {
    int i;
    for (i = 0; i < 25; i++) {
        Sprite_SetDrawFlag(summary->sprites[79 + i], FALSE);
    }
    for (i = 0; i < 5; i++) {
        Sprite_SetDrawFlag(summary->sprites[104 + i], FALSE);
    }
}

// Sprites 109..114: the 5 shiny leaves and the crown.
void PokemonSummary_InitShinyLeafSprites(PokemonSummary *summary) {
    int i;
    for (i = 0; i < 6; i++) {
        Sprite_SetDrawFlag(summary->sprites[109 + i], FALSE);
        Sprite_SetAnimActiveFlag(summary->sprites[109 + i], TRUE);
        Sprite_SetDrawPriority(summary->sprites[109 + i], 6 - i);
    }
}

void PokemonSummary_SetPokeathlonStatStars(Sprite **sprites, int firstStar, int maxStars, int baseStars, int color, int modifier, int modifierSprite) {
    u8 states[5];
    int i;
    int result = PokemonSummary_GetPokeathlonStarStates_Int(states, maxStars, baseStars, color, modifier);

    for (i = 0; i < 5; i++) {
        if (states[i] == 0) {
            Sprite_SetDrawFlag(sprites[firstStar + i], FALSE);
        } else if (states[i] == 1) {
            Sprite_SetDrawFlag(sprites[firstStar + i], TRUE);
            Sprite_SetAnimCtrlSeq(sprites[firstStar + i], 3);
        } else if (states[i] == 2) {
            Sprite_SetDrawFlag(sprites[firstStar + i], TRUE);
            Sprite_SetAnimCtrlSeq(sprites[firstStar + i], 0);
        } else if (states[i] == 3) {
            Sprite_SetDrawFlag(sprites[firstStar + i], TRUE);
            Sprite_SetAnimCtrlSeq(sprites[firstStar + i], 1);
        } else if (states[i] == 4) {
            Sprite_SetDrawFlag(sprites[firstStar + i], TRUE);
            Sprite_SetAnimCtrlSeq(sprites[firstStar + i], 2);
        }
    }
    if (result == 0) {
        Sprite_SetDrawFlag(sprites[modifierSprite], FALSE);
    } else if (result > 0) {
        Sprite_SetDrawFlag(sprites[modifierSprite], TRUE);
        Sprite_SetAnimCtrlSeq(sprites[modifierSprite], 4);
    } else if (result < 0) {
        Sprite_SetDrawFlag(sprites[modifierSprite], TRUE);
        Sprite_SetAnimCtrlSeq(sprites[modifierSprite], 5);
    }
}

// Each star: 0 hidden, 1 empty (above the base), else 2 + color. Returns the sign of the Aprijuice modifier.
int PokemonSummary_GetPokeathlonStarStates(u8 *states, int maxStars, int baseStars, u8 color, int modifier) {
    int i;
    color += 2;
    for (i = 0; i < 5; i++) {
        if (i > maxStars) {
            states[i] = 0;
        } else if (i > baseStars) {
            states[i] = 1;
        } else {
            states[i] = color;
        }
    }
    if (modifier > 0) {
        return 1;
    }
    if (modifier < 0) {
        return -1;
    }
    return 0;
}

#pragma force_active on

void PokemonSummary_UpdatePokeathlonStars(PokemonSummary *summary) {
    struct {
        PokeathlonStarCounts stars;
        PartyExtraSub aprijuice;
        u8 pad;
        struct PokeathlonTodayPerformance performance;
    } data;

    BoxPokemon *mon;

    if (summary->page == 2 && summary->args->isFlag982Set) {
        mon = PokemonSummary_GetShownMon(summary);
        MI_CpuFill8(&data.aprijuice, 0, sizeof(PartyExtraSub));
        CalcBoxMonPokeathlonPerformance(mon, &data.performance);
        if (summary->args->monDataType == 1) {
            Party_GetMonAprijuiceModifiers(summary->args->party, &data.aprijuice, summary->args->partySlot);
        }
        CalcBoxmonPokeathlonStars((struct PokeathlonPerformanceStars *)&data.stars, mon, (s8 *)&data.aprijuice, HEAP_ID_19);
        PokemonSummary_SetPokeathlonStatStars(summary->sprites, 79, data.performance.stats[4].hi, data.stars.stat4, data.stars.color[4], (s8)data.aprijuice.stats[4], 104);
        PokemonSummary_SetPokeathlonStatStars(summary->sprites, 84, data.performance.stats[0].hi, data.stars.stat0, data.stars.color[0], (s8)data.aprijuice.stats[0], 105);
        PokemonSummary_SetPokeathlonStatStars(summary->sprites, 89, data.performance.stats[2].hi, data.stars.stat2, data.stars.color[2], (s8)data.aprijuice.stats[2], 106);
        PokemonSummary_SetPokeathlonStatStars(summary->sprites, 94, data.performance.stats[1].hi, data.stars.stat1, data.stars.color[1], (s8)data.aprijuice.stats[1], 107);
        PokemonSummary_SetPokeathlonStatStars(summary->sprites, 99, data.performance.stats[3].hi, data.stars.stat3, data.stars.color[3], (s8)data.aprijuice.stats[3], 108);
    } else {
        PokemonSummary_HidePokeathlonStars(summary);
    }
}

void PokemonSummary_UpdateShinyLeafSprites(PokemonSummary *summary) {
    int i;
    if (summary->page == 0) {
        if (summary->mon.shinyLeaves[5]) {
            Sprite_SetDrawFlag(summary->sprites[114], TRUE);
            for (i = 0; i < 5; i++) {
                Sprite_SetDrawFlag(summary->sprites[109 + i], FALSE);
            }
        } else {
            for (i = 0; i < 5; i++) {
                if (summary->mon.shinyLeaves[i]) {
                    Sprite_SetDrawFlag(summary->sprites[109 + i], TRUE);
                } else {
                    Sprite_SetDrawFlag(summary->sprites[109 + i], FALSE);
                }
            }
            Sprite_SetDrawFlag(summary->sprites[114], FALSE);
        }
    } else {
        for (i = 0; i < 6; i++) {
            Sprite_SetDrawFlag(summary->sprites[109 + i], FALSE);
        }
    }
}

#pragma force_active off

void PokemonSummary_SetMonTypeSprites(PokemonSummary *summary) {
    PokemonSummary_SetTypeSprite(summary, 11, 3, summary->mon.type1);
    PokemonSummary_SetTypeSprite(summary, 12, 4, summary->mon.type2);
}

void PokemonSummary_SetTypeSprite(PokemonSummary *summary, u8 sprite, u8 resId, u8 type) {
    NarcId narc = TypeIcon_GetNarc();
    int charFile = TypeIcon_GetCharFile(type);
    SpriteSystem_ReplaceCharResObj(summary->spriteSystem, summary->spriteManager, narc, charFile, TRUE, resId);
    thunk_Sprite_SetPaletteOverride(summary->sprites[sprite], TypeIcon_GetPalette(type) + 3);
}

// Finds the move type sprite (13..16) shown in the given move slot row; *slot becomes its index.
void PokemonSummary_FindMoveTypeSprite(PokemonSummary *summary, u8 *slot, s16 *x, s16 *y) {
    s16 targetY = (s16)((*slot << 5) + 16);
    s16 i;
    for (i = 0; i < MAX_MON_MOVES; i++) {
        Sprite_GetPositionXY(summary->sprites[13 + i], x, y);
        if (targetY == *y) {
            *slot = i;
            return;
        }
    }
}

void PokemonSummary_SwapMoveTypeSprites(PokemonSummary *summary, u8 slotA, u8 slotB, int unused) {
    s16 posA[2];
    s16 posB[2];
    (void)unused;
    PokemonSummary_FindMoveTypeSprite(summary, &slotA, &posA[1], &posA[0]);
    PokemonSummary_FindMoveTypeSprite(summary, &slotB, &posB[1], &posB[0]);
    Sprite_SetPositionXY(summary->sprites[slotA + 13], posB[1], posB[0]);
    Sprite_SetPositionXY(summary->sprites[slotB + 13], posA[1], posA[0]);
}

void PokemonSummary_SetMoveCategorySprite(PokemonSummary *summary, u16 move) {
    int category = GetMoveAttr(move, MOVEATTR_CLASS);
    NarcId narc = MoveCategoryIcon_GetNarc(category);
    int charFile = MoveCategoryIcon_GetCharFile(category);
    SpriteSystem_ReplaceCharResObj(summary->spriteSystem, summary->spriteManager, narc, charFile, TRUE, 10);
    thunk_Sprite_SetPaletteOverride(summary->sprites[18], MoveCategoryIcon_GetPalette(category) + 3);
}

#pragma force_active on

// Sprite 9 is also the move cursor on the moves page; slot 4 is the move to learn, 5 the cancel button.
void PokemonSummary_UpdateMoveCursorSprite(PokemonSummary *summary) {
    u32 slot = summary->moveSlot;
    switch (slot) {
    case 4:
        if (summary->args->mode == 2) {
            Sprite_SetPositionXY(summary->sprites[9], 68, (s16)(slot * 32 + 40));
        } else {
            Sprite_SetPositionXY(summary->sprites[9], 68, (s16)(slot * 32 + 24));
        }
        Sprite_SetAnimCtrlSeq(summary->sprites[9], 0);
        break;
    case 5:
        if (summary->args->mode == 2) {
            Sprite_SetPositionXY(summary->sprites[9], 220, 176);
            Sprite_SetAnimCtrlSeq(summary->sprites[9], 3);
        }
        break;
    default:
        Sprite_SetPositionXY(summary->sprites[9], 68, (s16)(slot * 32 + 24));
        Sprite_SetAnimCtrlSeq(summary->sprites[9], 0);
        break;
    }
}

#pragma force_active off

// Sprite 10 stays on the first move picked for a swap.
void PokemonSummary_ShowSwapCursorSprite(PokemonSummary *summary) {
    s16 pos[2];
    Sprite_GetPositionXY(summary->sprites[9], &pos[1], &pos[0]);
    Sprite_SetPositionXY(summary->sprites[10], pos[1], pos[0]);
    Sprite_SetDrawFlag(summary->sprites[10], TRUE);
}

void PokemonSummary_ResetSwapCursorSprite(PokemonSummary *summary) {
    Sprite_SetPositionXY(summary->sprites[10], 68, 168);
    Sprite_SetAnimCtrlSeq(summary->sprites[10], 0);
}

#pragma force_active on

// Sprites 29..40: the sheen sparkles, one per 1/12 of the sheen.
void PokemonSummary_InitSheenSparkles(PokemonSummary *summary) {
    u32 i;
    summary->sparkleState = 0;
    summary->unk7C3 = 0;
    summary->unk7C2 = 0;
    if (summary->mon.contestStats[5] == 0) {
        summary->numSparkles = 0;
    } else if (summary->mon.contestStats[5] == 255) {
        summary->numSparkles = 12;
    } else {
        summary->numSparkles = (12 * summary->mon.contestStats[5]) >> 8;
    }
    for (i = 29; i <= 40; i++) {
        Sprite_SetDrawFlag(summary->sprites[i], FALSE);
        Sprite_SetAnimationFrame(summary->sprites[i], 0);
    }
}

// Sprites 23..28: the markings; 52: the shiny star; 53: the cured Pokerus mark.
void PokemonSummary_UpdateMarkingSprites(PokemonSummary *summary) {
    u32 i;
    for (i = 0; i < 6; i++) {
        if (summary->mon.markings & (1 << i)) {
            Sprite_SetAnimCtrlSeq(summary->sprites[23 + i], 1);
        } else {
            Sprite_SetAnimCtrlSeq(summary->sprites[23 + i], 0);
        }
    }
    if (summary->mon.shiny == TRUE && summary->mon.isEgg == FALSE) {
        thunk_Sprite_SetDrawFlag(summary->sprites[52], TRUE);
    } else {
        thunk_Sprite_SetDrawFlag(summary->sprites[52], FALSE);
    }
    if (summary->mon.pokerus == 2) {
        thunk_Sprite_SetDrawFlag(summary->sprites[53], TRUE);
    } else {
        thunk_Sprite_SetDrawFlag(summary->sprites[53], FALSE);
    }
}

#pragma force_active off

void PokemonSummary_ShowConditionChangeSprites(PokemonSummary *summary) {
    u8 i;
    for (i = 0; i < 5; i++) {
        if ((summary->unk7BE & (1 << i)) != 0) {
            Sprite_SetDrawFlag(summary->sprites[41 + i], TRUE);
        }
    }
}

// Loads a Pokemon's icon graphics into the sprite's VRAM.
void PokemonSummary_LoadMonIcon(PokemonSummary *summary, void *mon, int sprite) {
    NNSG2dCharacterData *charData;
    void *file;
    u32 location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(summary->sprites[sprite]), NNS_G2D_VRAM_TYPE_2DSUB);
    int fileId;
    if (summary->args->monDataType == 2) {
        fileId = Boxmon_GetIconNaix(mon);
    } else {
        fileId = Pokemon_GetIconNaix(mon);
    }
    file = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, fileId, FALSE, &charData, HEAP_ID_19);
    DC_FlushRange(charData->pRawData, 0x200);
    GXS_LoadOBJ(charData->pRawData, location, 0x200);
    Heap_Free(file);
}

// Sprite 19: the shown Pokemon's icon.
void PokemonSummary_LoadShownMonIcon(PokemonSummary *summary) {
    void *mon = PokemonSummary_GetShownMon(summary);
    int palette;
    PokemonSummary_LoadMonIcon(summary, mon, 19);
    palette = GetMonIconPaletteEx(summary->mon.species, summary->mon.form, summary->mon.isEgg);
    thunk_Sprite_SetPaletteOverride(summary->sprites[19], palette + 12);
    Sprite_SetFlipMode_Int(summary->sprites[19], 1 ^ GetMonBaseStat_HandleAlternateForm(summary->mon.species, summary->mon.form, BASE_FLIP));
}

// Sprites 73..78: the party's icons (only the shown Pokemon's when not viewing a party).
void PokemonSummary_LoadPartyIcons(PokemonSummary *summary) {
    void *mon = PokemonSummary_GetShownMon(summary);
    int count;
    int i;

    switch (summary->args->monDataType) {
    case 0:
        count = 1;
        break;
    case 2:
        count = 0;
        break;
    case 1:
        count = summary->args->partyCount;
        break;
    }
    for (i = 0; i < count; i++) {
        switch (summary->args->monDataType) {
        case 0:
            mon = PokemonSummary_GetShownMon(summary);
            thunk_Sprite_SetPaletteOverride(summary->sprites[73 + i], Pokemon_GetIconPalette(mon) + 12);
            break;
        case 2:
            thunk_Sprite_SetPaletteOverride(summary->sprites[73 + i], Boxmon_GetIconPalette(mon) + 12);
            break;
        case 1:
            mon = Party_GetMonByIndex(summary->args->party, i);
            thunk_Sprite_SetPaletteOverride(summary->sprites[73 + i], Pokemon_GetIconPalette(mon) + 12);
            break;
        }
        PokemonSummary_LoadMonIcon(summary, mon, i + 73);
    }
    for (; i < PARTY_SIZE; i++) {
        thunk_Sprite_SetDrawFlag(summary->sprites[73 + i], FALSE);
    }
}

void PokemonSummary_SetBoxMonIconPos(PokemonSummary *summary) {
    PokemonSummaryArgs *args = summary->args;
    if (args->monDataType == 2) {
        Sprite_SetPositionXY(summary->sprites[73], 215, 80);
        Sprite_SetPositionXY(summary->sprites[74], 215, 80);
    }
}

// A box Pokemon's icon alternates between sprites 73 and 74.
void PokemonSummary_AlternateBoxMonIcon(PokemonSummary *summary) {
    void *mon;
    if (summary->args->monDataType != 2) {
        return;
    }
    mon = PokemonSummary_GetShownMon(summary);
    if (summary->boxIconSprite == 0) {
        thunk_Sprite_SetDrawFlag(summary->sprites[73], TRUE);
        thunk_Sprite_SetDrawFlag(summary->sprites[74], FALSE);
        PokemonSummary_LoadMonIcon(summary, mon, 73);
        thunk_Sprite_SetPaletteOverride(summary->sprites[73], Boxmon_GetIconPalette(mon) + 12);
    } else {
        thunk_Sprite_SetDrawFlag(summary->sprites[73], FALSE);
        thunk_Sprite_SetDrawFlag(summary->sprites[74], TRUE);
        PokemonSummary_LoadMonIcon(summary, mon, 74);
        thunk_Sprite_SetPaletteOverride(summary->sprites[74], Boxmon_GetIconPalette(mon) + 12);
    }
    summary->boxIconSprite ^= 1;
}

void PokemonSummary_ShowMonIconSprite(PokemonSummary *summary) {
    if (summary->page == 1) {
        Sprite_SetPositionXY(summary->sprites[19], 160, 16);
    } else {
        Sprite_SetPositionXY(summary->sprites[19], 32, 68);
    }
    thunk_Sprite_SetDrawFlag(summary->sprites[19], TRUE);
}

// A condition chart corner: from the chart's center towards its tip, by the contest stat.
s16 PokemonSummary_GetConditionPoint(int stat, s16 tip, s16 center) {
    u32 delta;
    if (center > tip) {
        delta = ((u32)((center - tip) * (stat + 44)) << 16) / 300;
        delta >>= 16;
        return center - (s16)delta;
    }
    delta = ((u32)((tip - center) * (stat + 44)) << 16) / 300;
    delta >>= 16;
    return center + (s16)delta;
}

// Sprites 47..51: the corners of the condition chart (cool, beauty, cute, smart, tough).
void PokemonSummary_ShowConditionSprites(PokemonSummary *summary) {
    s16 x;
    x = PokemonSummary_GetConditionPoint(summary->mon.contestStats[0], 88, 88);
    Sprite_SetPositionXY(summary->sprites[47], x, PokemonSummary_GetConditionPoint(summary->mon.contestStats[0], 49, 73));
    x = PokemonSummary_GetConditionPoint(summary->mon.contestStats[1], 110, 88);
    Sprite_SetPositionXY(summary->sprites[48], x, PokemonSummary_GetConditionPoint(summary->mon.contestStats[1], 65, 73));
    x = PokemonSummary_GetConditionPoint(summary->mon.contestStats[2], 103, 88);
    Sprite_SetPositionXY(summary->sprites[49], x, PokemonSummary_GetConditionPoint(summary->mon.contestStats[2], 92, 73));
    x = PokemonSummary_GetConditionPoint(summary->mon.contestStats[3], 72, 87);
    Sprite_SetPositionXY(summary->sprites[50], x, PokemonSummary_GetConditionPoint(summary->mon.contestStats[3], 92, 73));
    x = PokemonSummary_GetConditionPoint(summary->mon.contestStats[4], 65, 87);
    Sprite_SetPositionXY(summary->sprites[51], x, PokemonSummary_GetConditionPoint(summary->mon.contestStats[4], 65, 73));
    Sprite_SetDrawFlag(summary->sprites[47], TRUE);
    Sprite_SetDrawFlag(summary->sprites[48], TRUE);
    Sprite_SetDrawFlag(summary->sprites[49], TRUE);
    Sprite_SetDrawFlag(summary->sprites[50], TRUE);
    Sprite_SetDrawFlag(summary->sprites[51], TRUE);
}

void PokemonSummary_HideConditionSprites(PokemonSummary *summary) {
    Sprite_SetDrawFlag(summary->sprites[47], FALSE);
    Sprite_SetDrawFlag(summary->sprites[48], FALSE);
    Sprite_SetDrawFlag(summary->sprites[49], FALSE);
    Sprite_SetDrawFlag(summary->sprites[50], FALSE);
    Sprite_SetDrawFlag(summary->sprites[51], FALSE);
}

// Sprites 54..62: the 9 ribbons of the current ribbon row page; 63 the ribbon cursor.
void PokemonSummary_SetRibbonSprite(PokemonSummary *summary, int ribbon, int index) {
    int attr = GetRibbonAttr_Int(ribbon, RIBBONDAT_NCGR);
    SpriteSystem_ReplaceCharResObj(summary->spriteSystem, summary->spriteManager, NARC_a_0_3_9, attr, FALSE, index + 25);
    attr = GetRibbonAttr_Int(ribbon, RIBBONDAT_NCLR);
    thunk_Sprite_SetPaletteOverride(summary->sprites[index + 54], attr + 7);
}

#pragma force_active on

void PokemonSummary_ShowRibbonSprites(PokemonSummary *summary) {
    u16 i;
    for (i = 54; i <= 66; i++) {
        Sprite_SetDrawFlag(summary->sprites[i], FALSE);
    }
    if (summary->page == 2) {
        for (i = 0; i < 9; i++) {
            if (i < summary->ribbonCount) {
                Sprite_SetDrawFlag(summary->sprites[54 + i], TRUE);
                PokemonSummary_SetRibbonSprite(summary, PokemonSummary_GetRibbonAt(summary, (u8)i), (u8)i);
            }
        }
        Sprite_SetAnimCtrlSeq(summary->sprites[66], 0);
    }
}

void PokemonSummary_UpdateRibbonSprites(PokemonSummary *summary) {
    u16 i;
    for (i = 0; i < 9; i++) {
        if (summary->ribbonRow * 9 + i < summary->ribbonCount) {
            Sprite_SetDrawFlag(summary->sprites[54 + i], TRUE);
            PokemonSummary_SetRibbonSprite(summary, PokemonSummary_GetRibbonAt(summary, (u8)i), (u8)i);
        } else {
            Sprite_SetDrawFlag(summary->sprites[54 + i], FALSE);
        }
    }
}

#pragma force_active off

void PokemonSummary_UpdateRibbonCursorSprite(PokemonSummary *summary) {
    int cursor = summary->ribbonCursor;
    int column = cursor % 3;
    int row = cursor / 3;
    Sprite_SetPositionXY(summary->sprites[63], (s16)(column * 32 + 32), (s16)(row * 40 + 24));
}

void sub_0208C3C0(PokemonSummary *summary) {
    if (Sprite_GetDrawFlag(summary->sprites[66]) == TRUE) {
        Sprite_UpdateAnim(summary->sprites[66], FX32_ONE);
    }
}
