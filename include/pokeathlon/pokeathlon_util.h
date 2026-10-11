#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_UTIL_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_UTIL_H

#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"

// Sprite and text helpers shared by the Pokeathlon course, event and record screens (OVY_98).

typedef struct PokeathlonSpriteContext {
    enum HeapID heapId;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *paletteData;
    NARC *narc;       // Pokemon icon NARC, open for the context's lifetime
    s8 paletteOffset; // first palette slot of the Pokemon icon palettes, -1 until loaded
} PokeathlonSpriteContext;

typedef struct PokeathlonTextContext {
    enum HeapID heapId;
    Window *windows;
    MsgData *msgData;      // the screen's own message bank
    MsgData *msgDataFixed; // msg 237: species names
    MessageFormat *msgFormat;
    String *string;
    u32 windowCount;
} PokeathlonTextContext;

typedef struct PokeathlonMonInfo {
    u32 species : 9;
    u32 form : 5;
    u32 gender : 2;
    u32 shiny : 1;
    u32 personality;
} PokeathlonMonInfo;

extern const s32 ov98_0221F220[10];
extern const u16 ov98_0221F1F8[][2];
extern const u32 ov98_0221F1E4[5];
extern const GraphicsBanks ov98_0221F248;
extern const TouchscreenHitbox ov98_0221F1E0;
extern const OamManagerParam ov98_0221F1C0;
extern const OamCharTransferParam ov98_0221F1AC;

void *PokeathlonUtil_GetSpriteSystem(PokeathlonSpriteContext *ctx);
void *PokeathlonUtil_GetSpriteManager(PokeathlonSpriteContext *ctx);
PokeathlonSpriteContext *PokeathlonUtil_CreateSpriteContext(enum HeapID heapId, SpriteResourceCountsListUnion *counts, int maxSprites);
void PokeathlonUtil_FreeSpriteContext(PokeathlonSpriteContext *ctx, ManagedSprite **sprites, u32 count);
void PokeathlonUtil_DrawSprites(PokeathlonSpriteContext *ctx);
ManagedSprite *PokeathlonUtil_NewSprite(PokeathlonSpriteContext *ctx, const ManagedSpriteTemplate *template);
void PokeathlonUtil_CreateSpriteRow(PokeathlonSpriteContext *ctx, ManagedSprite **sprites, const int *resources, u32 activeCount, u32 startIndex, s16 x, s16 y, BOOL subScreen);
void ov98_0221E784(SpriteSystem *system, SpriteManager *manager, const int *resourceIds, int vram);
void PokeathlonUtil_LoadMonPicSprite(ManagedSprite *sprite, const PokeathlonMonInfo *mon, BOOL isMain, BOOL grayscale, enum HeapID heapId);
void PokeathlonUtil_LoadMonIconResources(PokeathlonSpriteContext *ctx, const int *resIds, int count, int screen, BOOL flag);
void PokeathlonUtil_SetMonIconSprite(PokeathlonSpriteContext *ctx, ManagedSprite *sprite, const PokeathlonMonInfo *mon, BOOL isMain, BOOL animate);
void PokeathlonUtil_LoadSpriteChars(ManagedSprite *sprite, void *data, u32 size);
void PokeathlonUtil_LoadSpritePalette(ManagedSprite *sprite, NarcId narcId, s32 memberNo, BOOL isMain, BOOL grayscale, enum HeapID heapId);
void *PokeathlonUtil_LoadCharDataFromNarc(PokeathlonSpriteContext *ctx, int member, NNSG2dCharacterData **out);

PokeathlonTextContext *PokeathlonUtil_CreateTextContext(enum HeapID heapId, BgConfig *bgConfig, int windowCount, const WindowTemplate *templates, s32 msgBank);
void PokeathlonUtil_FreeTextContext(PokeathlonTextContext *ctx, int count);
void ov98_0221EBD8(PokeathlonTextContext *ctx, u32 window, u32 message, BOOL center);
void ov98_0221EBEC(PokeathlonTextContext *ctx, u32 window, u32 message, BOOL center, u8 y, u32 font);
void ov98_0221EC08(PokeathlonTextContext *ctx, u32 window, u32 message, u8 x, u8 y);
void PokeathlonUtil_PrintMessageCentered(PokeathlonTextContext *ctx, u32 window, u32 message, u8 fill);
void PokeathlonUtil_PrintMessageEx(PokeathlonTextContext *ctx, u32 window, u32 message, BOOL center, u8 x, u8 y, u32 font, u8 fill);
void PokeathlonUtil_PrintMessageWithNumber(PokeathlonTextContext *ctx, u32 window, u32 message, s32 number, u32 digits, u32 field);
void PokeathlonUtil_PrintMessage(PokeathlonTextContext *ctx, u32 window, u32 message);
void PokeathlonUtil_PrintMessageAt(PokeathlonTextContext *ctx, u32 window, u32 message, u32 x, u8 y);
void PokeathlonUtil_BufferNumber(PokeathlonTextContext *ctx, s32 number, u32 digits, u32 field);
void PokeathlonUtil_PrintMessageWithPlayerName(PokeathlonTextContext *ctx, u32 window, u32 message, u32 field, PlayerProfile *profile);
void PokeathlonUtil_PrintSpeciesName(PokeathlonTextContext *ctx, u32 window, u32 species);
void PokeathlonUtil_ClearAndCopyWindow(PokeathlonTextContext *ctx, u32 window);
void PokeathlonUtil_PrintString(PokeathlonTextContext *ctx, u32 window, String *string, u32 x, u8 y);
void PokeathlonUtil_ClearWindowTilemap(PokeathlonTextContext *ctx, u32 window);
void PokeathlonUtil_SetWindowY(PokeathlonTextContext *ctx, u32 window, u8 y);
void *PokeathlonUtil_GetMessageFormat(PokeathlonTextContext *ctx);
void PokeathlonUtil_ClearWindow(PokeathlonTextContext *ctx, u32 window);

BOOL PokeathlonUtil_CheckExitInput(void);
u32 ov98_0221EF64(u32 value);
u8 ov98_0221EF80(s32 value);
u16 ov98_0221EFA4(u32 row, u32 column);
BOOL ov98_0221EFB4(u32 direction, u32 column, u16 value);
BOOL ov98_0221EFE8(u32 direction, u32 column, u16 value);
u16 ov98_0221F01C(const void *records, u32 index);
u32 PokeathlonUtil_CountSpeciesWithAllMedals(const u8 *medals);
u8 ov98_0221F058(const u16 (*records)[22]);
void PokeathlonUtil_ResetDisplay(void);
void PokeathlonUtil_DisableDisplay(void);
u32 PokeathlonUtil_GetDecimalDigit(u32 value, u32 digit);
u8 PokeathlonUtil_CountDecimalDigits(s32 value);
void PokeathlonUtil_SetGfxBanks(void);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_UTIL_H
