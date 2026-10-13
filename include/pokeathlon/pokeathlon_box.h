#ifndef POKEHEARTGOLD_POKEATHLON_BOX_H
#define POKEHEARTGOLD_POKEATHLON_BOX_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"

typedef PartyExtraSub PartyAprijuiceModifier;
typedef struct PokeathlonTodayPerformance PokeathlonTodayPerformance;
typedef struct PokeathlonPerformanceStars PokeathlonPerformanceStars;

#define PokeathlonBox_Init                       ov97_0221E5C0
#define PokeathlonBox_Main                       ov97_0221E5D4
#define PokeathlonBox_Exit                       ov97_0221E69C
#define PokeathlonBox_SetGraphicsBanks           ov97_0221E7F4
#define PokeathlonBox_SetGraphicsModes           ov97_0221E814
#define PokeathlonBox_SetObjCharTransferTemplate ov97_0221E834
#define PokeathlonBox_GetBoxMon                  ov97_0221E898
#define PokeathlonBox_GetLightBoxMon             ov97_0221E91C
#define PokeathlonBox_GetBoxName                 ov97_0221E97C
#define PokeathlonBox_CopySelectedMons           ov97_0221E98C
#define PokeathlonBox_GetPartyMonStats           ov97_0221EA88
#define PokeathlonBox_GetBoxMonStats             ov97_0221EB38
#define PokeathlonBox_GetLightBoxMonStats        ov97_0221EBD8
#define PokeathlonBox_InitGraphics               ov97_0221EE84
#define PokeathlonBox_SetupGraphics              ov97_0221EEA4
#define PokeathlonBox_RenderAndAnimateSprites    ov97_0221F010
#define PokeathlonBox_ReleaseGraphics            ov97_0221F020
#define PokeathlonBox_InitBgFromTemplates        ov97_0221F1D8

typedef struct PokeathlonBox_MonStats {
    s8 power;
    s8 stamina;
    s8 jump;
    s8 skill;
    s8 speed;
    s8 padding;
    u16 stars;
} PokeathlonBox_MonStats;

typedef struct PokeathlonBoxArgs_SelectedMon {
    u32 unk0;                     // 0x00
    u32 unk4;                     // 0x04
    u16 species;                  // 0x08
    u16 form;                     // 0x0A
    u32 personality;              // 0x0C
    PokeathlonBox_MonStats stats; // 0x10
    u8 isShiny;                   // 0x18
    u8 gender;                    // 0x19
    u16 nickname[7];              // 0x1A
} PokeathlonBoxArgs_SelectedMon;  // size: 0x28

typedef struct PokeathlonBoxArgs {
    PokeathlonCourseArgs *courseArgs;              // 0x00
    PokeathlonBoxArgs_SelectedMon selectedMons[3]; // 0x04
    u8 filler7C[8];                                // 0x7C
    u32 courseId;                                  // 0x84
} PokeathlonBoxArgs;

typedef struct PokeathlonBox_BoxMon {
    u32 species;
    u32 personality;
    u16 isEgg;
    u16 form;
    u16 unkC;
    u16 unkE;
    u16 isShiny;
    u16 gender;
    u8 filler14[4];
    u16 nickname[11];
} PokeathlonBox_BoxMon;

typedef struct PokeathlonBox_SelectedMon {
    u32 boxno;
    u32 slot;
} PokeathlonBox_SelectedMon;

// How a performance stat changes for the selected course, and its star display.
typedef struct PokeathlonBox_StatChange {
    s8 direction; // > 0 up, < 0 down, 0 unchanged
    u8 stars[5];  // per star: 0 hidden, 1-4 animation
    u8 unk6[2];
} PokeathlonBox_StatChange; // size: 0x8

typedef struct PokeathlonBox_MonInfo {
    u32 personality;  // 0x00
    u16 species;      // 0x04
    u8 isShiny;       // 0x06
    u8 form;          // 0x07
    u8 gender;        // 0x08
    u8 unk9;          // 0x09
    u16 nickname[11]; // 0x0A
    u8 medals[5];
    u8 unk25;
    u16 unk26;
    PokeathlonBox_StatChange statChanges[5]; // 0x28, power/skill/speed/jump/stamina
} PokeathlonBox_MonInfo;

typedef struct PokeathlonBox_SpriteResources {
    SpriteResource *charResObj;
    SpriteResource *plttResObj;
    SpriteResource *cellResObj;
    SpriteResource *animResObj;
    u8 padding[0x8];
} PokeathlonBox_SpriteResources; // size: 0x18

typedef struct PokeathlonBox_StatRow {
    Sprite *sprite;
    Sprite *sprites[5];
} PokeathlonBox_StatRow; // size: 0x18

typedef struct PokeathlonBox_SubGraphics {
    enum HeapID heapID;                        // 0x000 / 0x078
    SpriteList *spriteList;                    // 0x004 / 0x07C
    G2dRenderer renderer;                      // 0x008 / 0x080
    GF_2DGfxResMan *mgrs[6];                   // 0x130 / 0x1A8
    PokeathlonBox_SpriteResources resources[3]; // 0x148 / 0x1C0
    u8 filler190[0x24];                        // 0x190 / 0x208
    Sprite *sprites[5];                        // 0x1B4 / 0x22C
    Sprite *unk1C8[5];                         // 0x1C8 / 0x23C
    PokeathlonBox_StatRow statRows[5]; // 0x1DC / 0x250
    Sprite *monPicSprite;                            // 0x254 / 0x2C8
    void *monPicChars;                         // 0x258 / 0x2CC
    u8 monPicPalette[0x20];                    // 0x25C / 0x2D0
    u32 monPicCharOffset;                      // 0x27C / 0x2F0
    u32 monPicPlttOffset;                      // 0x280 / 0x2F4
} PokeathlonBox_SubGraphics;

typedef struct PokeathlonBox_Graphics {
    enum HeapID heapID;            // 0x000
    BgConfig *bgConfig;            // 0x004
    Window window1;                // 0x008
    Window window2;                // 0x018
    Window window3;                // 0x028
    Window window4;                // 0x038
    Window window5;                // 0x048
    Window window6;                // 0x058
    u16 courseId;                  // 0x068
    u16 isSupreme;                     // 0x06A
    MsgData *msgData;              // 0x06C
    MessageFormat *msgFormat;      // 0x070
    String *nickname;              // 0x074
    PokeathlonBox_SubGraphics sub; // 0x078
} PokeathlonBox_Graphics;          // size: 0x2FC

typedef struct PokeathlonBox {
    BgConfig *bgConfig; // 0x0
    u32 state;          // 0x4
    u32 monSelector;
    PokeathlonBox_Graphics *graphics;
    PokeathlonBox_SelectedMon selectedMons[3];
    BOOL cancelled;
    SaveData *saveData; // 0x2C
    u32 courseId;
} PokeathlonBox; // size: 0x34

BOOL PokeathlonBox_Init(OverlayManager *manager);
BOOL PokeathlonBox_Main(OverlayManager *manager);
BOOL PokeathlonBox_Exit(OverlayManager *manager);
BOOL PokeathlonBox_RunSetup(OverlayManager *manager);
BOOL PokeathlonBox_Setup(OverlayManager *manager);
void PokeathlonBox_SetGraphicsBanks();
void PokeathlonBox_SetGraphicsModes(BgConfig *bgConfig);
void PokeathlonBox_SetObjCharTransferTemplate();
void PokeathlonBox_CreateOamManager(PokeathlonBox *data);
void PokeathlonBox_VBlankCallback(void *data);
BOOL PokeathlonBox_GetBoxMon(PCStorage *storage, u32 boxno, u32 slotno, PokeathlonBox_BoxMon *ptr);
BOOL PokeathlonBox_GetLightBoxMon(PCStorage *storage, u32 boxno, u32 slotno, PokeathlonBox_BoxMon *ptr);
void PokeathlonBox_GetBoxName(String *dest, PCStorage *storage, u32 boxno);
void PokeathlonBox_CopySelectedMons(PokeathlonBox *data, PokeathlonBoxArgs *args);
void PokeathlonBox_GetPartyMonStats(Party *party, u8 slot, PokeathlonBox_MonStats *stats);
void PokeathlonBox_GetBoxMonStats(BoxPokemon *boxMon, PokeathlonBox_MonStats *stats);
void PokeathlonBox_GetLightBoxMonStats(BoxPokemon *boxMon, PokeathlonBox_MonStats *stats);
void PokeathlonBox_ShowMonInfo(int boxno, u8 slot, PokeathlonBox *data);
void PokeathlonBox_CalcStatChanges(PokeathlonBox_MonStats *stats1, PokeathlonBox_MonStats *stats2, PartyAprijuiceModifier *aprijuiceModifier, PokeathlonBox_MonInfo *mon);
PokeathlonBox_Graphics *PokeathlonBox_InitGraphics(enum HeapID heapID);
void PokeathlonBox_SetupGraphics(PokeathlonBox_Graphics *graphics, BgConfig *bgConfig, u8 courseId, u32 arg3);
void PokeathlonBox_DrawMonInfo(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon);
void PokeathlonBox_RenderAndAnimateSprites(PokeathlonBox_Graphics *graphics);
void PokeathlonBox_ReleaseGraphics(PokeathlonBox_Graphics *graphics);
void PokeathlonBox_DeleteSprites(PokeathlonBox_SubGraphics *subGraphics);
void PokeathlonBox_LoadBgGraphics(BgConfig *bgConfig, enum HeapID heapID);
void PokeathlonBox_InitBgFromTemplates(BgConfig *bgConfig);
void PokeathlonBox_PrintStaticTexts(PokeathlonBox_Graphics *graphics);
void PokeathlonBox_PrintMonTexts(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon);
void PokeathlonBox_ShowMonSprites(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon);

void PokeathlonBox_ShowStatStars(PokeathlonBox_SubGraphics *subGraphics, PokeathlonBox_MonInfo *mon);
void PokeathlonBox_HideMonSprites();
void PokeathlonBox_LoadSpriteResources(PokeathlonBox_SubGraphics *subGraphics);
void PokeathlonBox_CreateSprites(PokeathlonBox_SubGraphics *subGraphics);
Sprite *PokeathlonBox_CreateSprite(PokeathlonBox_SubGraphics *sub, int resourceId, int drawPriority, int priority, int x, int y, int anim, BOOL draw);
void PokeathlonBox_InitSpriteTemplate(SpriteTemplate *template, SpriteList *spriteList, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen, enum HeapID heapID);
void PokeathlonBox_GetMonPicVramOffsets(PokeathlonBox_SubGraphics *sub);
void PokeathlonBox_LoadMonPic(); // (PokeathlonBox_SubGraphics *, PokeathlonBox_MonInfo *), called with the second argument left in r1
void PokeathlonBox_TransferMonPic(PokeathlonBox_SubGraphics *subGraphics);

#endif // POKEHEARTGOLD_POKEATHLON_BOX_H
