#ifndef POKEHEARTGOLD_GLOBAL_H
#define POKEHEARTGOLD_GLOBAL_H

#include <MSL.h>
#include <nitro.h>
#include <nitro/code16.h>
#include <nnsys.h>
#include <stddef.h>
#include <string.h>

#include "constants/global.h"

#include "assert.h"
#include "config.h" // MUST COME BEFORE ANY OTHER GAMEFREAK HEADERS
#include "pm_version.h"

// This branch builds the single-screen game. The define lives here, not on the command
// line, because the partition build tools check each compiler command token by token.
// Remove it to build the original two-screen game.
#define SINGLE_SCREEN 1

#ifdef SINGLE_SCREEN
// Single-screen builds keep the main engine (the only one with 3D) on the bottom screen,
// whatever a scene asks for. Code that sets or tests the display-select bit directly sees
// a mask of 0 (read from memory, so that `|=` of it is not rejected as having no effect).
// The SDK's own sources are built without this header and are not affected.
extern const volatile u16 gSingleScreenDispSelectMask;
#undef REG_GX_POWCNT_DSEL_MASK
#define REG_GX_POWCNT_DSEL_MASK gSingleScreenDispSelectMask
#define GX_SetDispSelect(sel) GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN)
// Puts the sub engine on the visible screen, for scenes that are drawn wholly with it, or
// puts the main engine back.
void SingleScreen_ShowSubEngine(BOOL show);
BOOL SingleScreen_IsSubEngineShown(void);
// For scenes with something worth seeing on the hidden screen: SELECT swaps the screens.
// Returns TRUE when it did.
BOOL SingleScreen_HandleSwapKey(void);
#endif

#define NELEMS(a) (sizeof(a) / sizeof(*(a)))

typedef struct {
    int x;
    int y;
    int z;
} Vec32;

#define ARRAY_ASSIGN_EX(dest, src, as_type, count)            \
    {                                                         \
        struct tmp {                                          \
            as_type _[count];                                 \
        };                                                    \
        *(struct tmp *)&(dest) = *(const struct tmp *)&(src); \
    }

#define ARRAY_ASSIGN(dest, src) ARRAY_ASSIGN_EX(dest, src, typeof(*(dest)), NELEMS(dest))

#endif // POKEHEARTGOLD_GLOBAL_H
