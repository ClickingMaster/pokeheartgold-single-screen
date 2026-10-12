#include "global.h"

#include "constants/sndseq.h"

#include "pokeathlon/pokeathlon_disc_catch_play.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "player_data.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "text.h"

extern const s16 ov96_0221D14C[DISC_CATCH_PARTICIPANTS - 1][2]; // where the other players' badges go
extern const WindowTemplate ov96_0221D15C[DISC_CATCH_PARTICIPANTS];
extern const ManagedSpriteTemplate ov96_0221D17C;

#pragma force_active on

PokeathlonDiscCatchScoreboard *PokeathlonDiscCatchScoreboard_New(SpriteSystem *spriteSystem, SpriteManager *spriteManager, BgConfig *bgConfig, MsgData *msgData, MessageFormat *messageFormat, u8 participant, enum HeapID heapId, PokeathlonCourseData *course) {
    PokeathlonDiscCatchScoreboard *scoreboard = Heap_Alloc(heapId, sizeof(PokeathlonDiscCatchScoreboard));
    MI_CpuFill8(scoreboard, 0, sizeof(PokeathlonDiscCatchScoreboard));
    scoreboard->spriteSystem = spriteSystem;
    scoreboard->spriteManager = spriteManager;
    scoreboard->participant = participant;
    scoreboard->heapId = heapId;
    scoreboard->bgConfig = bgConfig;
    scoreboard->msgData = msgData;
    scoreboard->messageFormat = messageFormat;
    scoreboard->course = course;
    PokeathlonDiscCatchScoreboard_InitWindows(scoreboard);
    scoreboard->sprites[DISC_CATCH_SCOREBOARD_CLOCK] = PokeathlonDiscCatchScoreboard_NewSprite(scoreboard->spriteSystem, scoreboard->spriteManager, 210, 76, 8, 0);
    PokeathlonDiscCatchScoreboard_NewSideMarks(scoreboard);
    PokeathlonDiscCatchScoreboard_NewSecondsDigits(scoreboard);
    PokeathlonDiscCatchScoreboard_PrintNames(scoreboard);
    scoreboard->digitsFile = GfGfxLoader_GetScrnData(NARC_a_2_3_5, 14, TRUE, &scoreboard->digits, scoreboard->heapId);
    PokeathlonDiscCatchScoreboard_SetSeconds(scoreboard, 60);
    PokeathlonDiscCatchScoreboard_DrawScore(scoreboard, 0);
    return scoreboard;
}

void PokeathlonDiscCatchScoreboard_Delete(PokeathlonDiscCatchScoreboard *scoreboard) {
    int i;
    GF_ASSERT(scoreboard != NULL);
    Heap_Free(scoreboard->digitsFile);
    for (i = 0; i < DISC_CATCH_PARTICIPANTS; i++) {
        RemoveWindow(&scoreboard->windows[i]);
    }
    for (i = 0; i < DISC_CATCH_SCOREBOARD_SPRITES; i++) {
        GF_ASSERT(scoreboard->sprites[i] != NULL);
        Sprite_DeleteAndFreeResources(scoreboard->sprites[i]);
    }
    Heap_Free(scoreboard);
}

#pragma force_active reset

void PokeathlonDiscCatchScoreboard_Pause(PokeathlonDiscCatchScoreboard *scoreboard) {
    PokeathlonDiscCatchScoreboard_StopAnims(scoreboard);
}

// Takes the own score from a frame packet (7 bits: counts the wraps); returns how much it went up.
u8 PokeathlonDiscCatchScoreboard_SetOwnScore(PokeathlonDiscCatchScoreboard *scoreboard, u32 participant, u16 score) {
    u16 delta;
    if (participant == ov96_021E5F24(scoreboard->course)) {
        if (score != scoreboard->shownScore) {
            if (scoreboard->shownScore > score) {
                scoreboard->wraps++;
            }
            {
                s32 total = score + ((u32)scoreboard->wraps << 7);
                if (total > 999) {
                    total = 999;
                } else if (total < 0) {
                    total = 0;
                }
                PokeathlonDiscCatchScoreboard_DrawScore(scoreboard, (u8)total);
            }
            delta = score - scoreboard->shownScore;
            scoreboard->shownScore = score;
        }
    }
    return delta;
}

// A team scored: its badge flashes; for the own team the score is redrawn with a sound by the points.
#pragma force_active on

BOOL PokeathlonDiscCatchScoreboard_OnScore(PokeathlonDiscCatchScoreboard *scoreboard, u32 participant, u16 score) {
    int delta;
    GF_ASSERT(scoreboard != NULL);
    GF_ASSERT(participant < DISC_CATCH_PARTICIPANTS);
    u32 badge = participant + DISC_CATCH_SCOREBOARD_BADGES;
    ManagedSprite_SetAnim(scoreboard->sprites[badge], participant + 4);
    if (participant == ov96_021E5F24(scoreboard->course) && score != scoreboard->shownScore) {
        delta = PokeathlonDiscCatchScoreboard_SetOwnScore(scoreboard, participant, score);
        if (delta == 1) {
        } else if (delta == 2) {
            PlaySE_SetPitch(SEQ_SE_GS_POINT1, 128);
        } else if (delta == 3) {
            PlaySE_SetPitch(SEQ_SE_GS_POINT1, 256);
        } else {
            PlaySE_SetPitch(SEQ_SE_GS_POINT1, 320);
        }
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// The seconds left; the digits turn yellow at 20 and red at 3.
#pragma force_active on

void PokeathlonDiscCatchScoreboard_SetSeconds(PokeathlonDiscCatchScoreboard *scoreboard, int seconds) {
    u8 shown = seconds % 100;
    int tens = (u8)(shown / 10);
    int ones = (u8)(shown % 10);
    switch (scoreboard->secondsColor) {
    case 0:
        if (shown <= 20) {
            int i;
            for (i = 0; i < 2; i++) {
                ManagedSprite_SetPaletteOverride(scoreboard->sprites[DISC_CATCH_SCOREBOARD_SECONDS + i], 2);
            }
            scoreboard->secondsColor++;
        }
        break;
    case 1:
        if (shown <= 3) {
            int i;
            for (i = 0; i < 2; i++) {
                ManagedSprite_SetPaletteOverride(scoreboard->sprites[DISC_CATCH_SCOREBOARD_SECONDS + i], 3);
            }
            scoreboard->secondsColor++;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    case 2:
        break;
    }
    tens += 9;
    ManagedSprite_SetAnim(scoreboard->sprites[DISC_CATCH_SCOREBOARD_SECONDS], tens);
    ones += 9;
    ManagedSprite_SetAnim(scoreboard->sprites[DISC_CATCH_SCOREBOARD_SECONDS + 1], ones);
}

#pragma force_active reset

u16 PokeathlonDiscCatchScoreboard_GetOwnScore(PokeathlonDiscCatchScoreboard *scoreboard) {
    return scoreboard->shownScore + scoreboard->wraps * 128;
}

#pragma force_active on

ManagedSprite *PokeathlonDiscCatchScoreboard_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u16 animation, u16 priority) {
    ManagedSpriteTemplate template = ov96_0221D17C;
    ManagedSprite *sprite;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.x = x;
    template.y = y;
    template.animation = animation;
    template.drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, 480 * FX32_ONE);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

#pragma force_active reset

void PokeathlonDiscCatchScoreboard_InitWindows(PokeathlonDiscCatchScoreboard *scoreboard) {
    u8 i;
    GF_ASSERT(scoreboard != NULL);
    GF_ASSERT(scoreboard->bgConfig != NULL);
    for (i = 0; i < DISC_CATCH_PARTICIPANTS; i++) {
        AddWindow(scoreboard->bgConfig, &scoreboard->windows[i], &ov96_0221D15C[i]);
    }
    BG_FillCharDataRange(scoreboard->bgConfig, GF_BG_LYR_SUB_3, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, scoreboard->heapId);
}

// The own score in three big digits (5 x 2 digits of 4 x 8 tiles in the screen file).
void PokeathlonDiscCatchScoreboard_DrawScore(PokeathlonDiscCatchScoreboard *scoreboard, s32 score) {
    u8 srcX[3];
    u8 srcY[3];
    u8 digit;
    u8 i;
    digit = score / 100;
    srcX[0] = (digit % 5) * 4;
    srcY[0] = (digit / 5) * 8;
    digit = (score - digit * 100) / 10;
    srcX[1] = (digit % 5) * 4;
    srcY[1] = (digit / 5) * 8;
    digit = score % 10;
    srcX[2] = (digit % 5) * 4;
    srcY[2] = (digit / 5) * 8;
    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(scoreboard->bgConfig, GF_BG_LYR_SUB_2, i * 4, 0, 4, 8, scoreboard->digits->rawData, srcX[i], srcY[i], 20, 16);
    }
    ScheduleBgTilemapBufferTransfer(scoreboard->bgConfig, GF_BG_LYR_SUB_2);
}

#pragma force_active on

void PokeathlonDiscCatchScoreboard_NewSecondsDigits(PokeathlonDiscCatchScoreboard *scoreboard) {
    int i, x;
    for (i = 0, x = 230; i < 2; x += 16, i++) {
        scoreboard->sprites[DISC_CATCH_SCOREBOARD_SECONDS + i] = PokeathlonDiscCatchScoreboard_NewSprite(scoreboard->spriteSystem, scoreboard->spriteManager, x, 76, i + 9, 0);
        ManagedSprite_SetPaletteOverride(scoreboard->sprites[DISC_CATCH_SCOREBOARD_SECONDS + i], 1);
    }
}

#pragma force_active reset

// The players' names (the own one in the big window) and their badges.
void PokeathlonDiscCatchScoreboard_PrintNames(PokeathlonDiscCatchScoreboard *scoreboard) {
    u8 other = 0;
    s16 x;
    s16 y;
    int i;
    u8 window;
    String *name;
    GF_ASSERT(scoreboard != NULL);
    for (i = 0; i < DISC_CATCH_PARTICIPANTS; i++) {
        if (i == scoreboard->participant) {
            x = 8;
            window = 0;
            y = 72;
        } else {
            window = other + 1;
            x = ov96_0221D14C[other][0];
            y = ov96_0221D14C[other][1];
            other = window;
        }
        name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(scoreboard->course, i), scoreboard->heapId);
        FillWindowPixelBuffer(&scoreboard->windows[window], 0);
        AddTextPrinterParameterizedWithColor(&scoreboard->windows[window], 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
        CopyWindowToVram(&scoreboard->windows[window]);
        String_Delete(name);
        scoreboard->sprites[DISC_CATCH_SCOREBOARD_BADGES + i] = PokeathlonDiscCatchScoreboard_NewSprite(scoreboard->spriteSystem, scoreboard->spriteManager, x, y, i, 65);
    }
}

static inline int GetSideSign(int column) {
    if (column == 0) {
        return 1;
    }
    return -1;
}

#pragma force_active on

void PokeathlonDiscCatchScoreboard_NewSideMarks(PokeathlonDiscCatchScoreboard *scoreboard) {
    int row, column;
    int y, first;
    s16 x;
    int slot;
    for (row = 0, first = DISC_CATCH_SCOREBOARD_MARKS, y = 8; row < 12; first += 2, y += 16, row++) {
        for (column = 0; column < 2; column++) {
            slot = column + first;
            x = 128;
            x += GetSideSign(column) * ((row + 4) * 8);
            scoreboard->sprites[slot] = PokeathlonDiscCatchScoreboard_NewSprite(scoreboard->spriteSystem, scoreboard->spriteManager, x, y, 20, 64);
            ManagedSprite_SetPriority(scoreboard->sprites[slot], 3);
        }
    }
}

#pragma force_active reset

void PokeathlonDiscCatchScoreboard_StopAnims(PokeathlonDiscCatchScoreboard *scoreboard) {
    int i;
    for (i = 0; i < DISC_CATCH_SCOREBOARD_SPRITES; i++) {
        ManagedSprite_SetAnimateFlag(scoreboard->sprites[i], FALSE);
    }
}
