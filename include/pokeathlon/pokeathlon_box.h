#ifndef POKEHEARTGOLD_POKEATHLON_BOX_H
#define POKEHEARTGOLD_POKEATHLON_BOX_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "box_mon_selector.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"

typedef PartyExtraSub PartyAprijuiceModifier;
typedef struct PokeathlonTodayPerformance PokeathlonTodayPerformance;
typedef struct PokeathlonPerformanceStars PokeathlonPerformanceStars;

typedef struct PokeathlonBox_MonStats {
    s8 power;
    s8 stamina;
    s8 jump;
    s8 skill;
    s8 speed;
    s8 padding;
    u16 starColors; // 3 bits per stat, in the order above
} PokeathlonBox_MonStats;

// A Pokemon picked in the box, as the course stores its participants' Pokemon.
typedef struct PokeathlonBoxArgs_SelectedMon {
    u16 species;
    u16 form;
    u32 personality;
    PokeathlonBox_MonStats stats;
    u8 isShiny;
    u8 gender;
    u16 nickname[11];
} PokeathlonBoxArgs_SelectedMon;

typedef struct PokeathlonBoxArgs {
    PokeathlonCourseArgs *courseArgs;
    u32 cancelled;
    u32 npcTeam;
    PokeathlonBoxArgs_SelectedMon selectedMons[3];
    u32 courseId;
} PokeathlonBoxArgs;

typedef struct PokeathlonBox_BoxMon {
    u32 species;
    u32 personality;
    u16 isEgg;
    u16 form;
    u16 heldItem;
    u16 unkE;
    u16 isShiny;
    u16 gender;
    u8 padding_14[4];
    u16 nickname[11];
} PokeathlonBox_BoxMon;

// How a performance stat changes for the selected course, and its star display.
typedef struct PokeathlonBox_StatChange {
    s8 direction; // > 0 up, < 0 down, 0 unchanged
    u8 stars[5];  // per star: 0 hidden, 1-4 animation
    u8 padding_6[2];
} PokeathlonBox_StatChange;

typedef struct PokeathlonBox_MonInfo {
    u32 personality;
    u16 species;
    u8 isShiny;
    u8 form;
    u8 gender;
    u8 displayMode;
    u16 nickname[11];
    u8 medals[5];
    u8 unk25;
    u16 unk26;
    PokeathlonBox_StatChange statChanges[5]; // power/skill/speed/jump/stamina
} PokeathlonBox_MonInfo;

typedef struct PokeathlonBox_SpriteResources {
    SpriteResource *charResObj;
    SpriteResource *plttResObj;
    SpriteResource *cellResObj;
    SpriteResource *animResObj;
    u8 padding[0x8];
} PokeathlonBox_SpriteResources;

typedef struct PokeathlonBox_StatRow {
    Sprite *sprite;
    Sprite *sprites[5];
} PokeathlonBox_StatRow;

typedef struct PokeathlonBox_SubGraphics {
    enum HeapID heapID;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *mgrs[6];
    PokeathlonBox_SpriteResources resources[3];
    u8 padding_190[0x24];
    Sprite *sprites[5];
    Sprite *unk1C8[5];
    PokeathlonBox_StatRow statRows[5];
    Sprite *monPicSprite;
    void *monPicChars;
    u8 monPicPalette[0x20];
    u32 monPicCharOffset;
    u32 monPicPlttOffset;
} PokeathlonBox_SubGraphics;

typedef struct PokeathlonBox_Graphics {
    enum HeapID heapID;
    BgConfig *bgConfig;
    Window window1;
    Window window2;
    Window window3;
    Window window4;
    Window window5;
    Window window6;
    u16 courseId;
    u16 isSupreme;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *nickname;
    PokeathlonBox_SubGraphics sub;
} PokeathlonBox_Graphics;

typedef struct PokeathlonBox {
    BgConfig *bgConfig;
    u32 state;
    BoxMonSelector *monSelector;
    PokeathlonBox_Graphics *graphics;
    BoxMonSelectorResult selectedMons[3];
    BOOL cancelled;
    SaveData *saveData;
    u32 courseId;
} PokeathlonBox;

BOOL PokeathlonBox_Init(OverlayManager *manager);
BOOL PokeathlonBox_Main(OverlayManager *manager);
BOOL PokeathlonBox_Exit(OverlayManager *manager, int *state);

PokeathlonBox_Graphics *PokeathlonBox_InitGraphics(enum HeapID heapID);
void PokeathlonBox_SetupGraphics(PokeathlonBox_Graphics *graphics, BgConfig *bgConfig, u8 courseId, u32 isSupreme);
void PokeathlonBox_DrawMonInfo(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon);
void PokeathlonBox_RenderAndAnimateSprites(PokeathlonBox_Graphics *graphics);
void PokeathlonBox_ReleaseGraphics(PokeathlonBox_Graphics *graphics);

#endif // POKEHEARTGOLD_POKEATHLON_BOX_H
