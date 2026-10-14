#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MIGRATE_FROM_AGB_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MIGRATE_FROM_AGB_H

#include "global.h"

#include "application/main_menu/agb_cartridge.h"

#include "bg_window.h"
#include "message_format.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "touchscreen.h"
#include "sprite_resource_header.h"
#include "yes_no_prompt.h"

// Main menu > Migrate from Ruby/Sapphire/Emerald/FireRed/LeafGreen (Pal Park): pick six Pokemon from the boxes of
// the GBA cartridge, they are sent to the Pal Park (MigratedPokemon) and erased from the GBA save
// (src/application/main_menu/migrate_from_agb.c).

#define MIGRATE_FROM_AGB_MON_COUNT 6

// Parameters of one text print (MigrateFromAgb_PrintText)
typedef struct MigrateFromAgbText {
    Window *window;
    u32 unk_04;
    int x; // window position and size (tiles)
    int y;
    int width;
    int height;
    int printX; // ignored when centered
    int printY;
    u32 baseTile;
    u32 bgId;
    int fontId;
    u32 color;
    u32 fillColor;
    int msgId; // -1: none
    const u16 *chars;
    String *string;
    MessageFormat *msgFormat; // NULL: a temporary one
    int textSpeed;            // 0: instant
    u32 printerId;
} MigrateFromAgbText;

// A Pokemon picked for migration
typedef struct MigrateFromAgbPick {
    Sprite *sprite; // copy of the box icon
    int slot;       // -1: none
    int box;
} MigrateFromAgbPick;

typedef struct MigrateFromAgbSlotSprites {
    Sprite *mon;
    Sprite *item;
    Sprite *unused;
} MigrateFromAgbSlotSprites;

typedef struct MigrateFromAgbButton {
    Sprite *sprite;
    u8 unk_04[12];
} MigrateFromAgbButton;

// A mon icon waiting for the VBlank upload (sMigrateFromAgbIcons points at the 30 of the box)
typedef struct MigrateFromAgbIcon {
    u32 vramOffset;
    int palette;
    Sprite *sprite; // NULL: nothing to load
    u32 charData[0x80];
} MigrateFromAgbIcon;

typedef struct MigrateFromAgbSave {
    u32 state; // MigrateFromAgb_RunTransfer (3 and 4: the DS save is being written)
    u32 delay;
} MigrateFromAgbSave;

typedef struct MigrateFromAgbApp {
    int agbVersion;                                       // 0x00000, VERSION_RUBY ...
    int pendingMsg;                                       // 0x00004, message of MigrateFromAgb_PrintResultMsg / MigrateFromAgb_PrintResultMsgWithYesNo still to print
    int pendingMsgId;                                     // 0x00008, message of MigrateFromAgb_PrintPendingMsg still to print (-1: none)
    u32 frameCount;                                       // 0x0000C
    SaveData *saveData;                                   // 0x00010
    PlayerProfile *profile;                               // 0x00014
    Options *options;                                     // 0x00018
    int frame;                                            // 0x0001C
    BgConfig *bgConfig;                                   // 0x00020
    int nextState;                                        // 0x00024, after the fade
    SpriteList *spriteList;                               // 0x00028
    G2dRenderer renderer;                                 // 0x0002C
    void *resMan[6];                                      // 0x00154
    SpriteResource *resources[4];                         // 0x0016C
    u8 unk_017C[8];                                       // 0x0017C
    SpriteResourcesHeader resHeader;                      // 0x00184
    MigrateFromAgbSlotSprites slots[AGB_BOX_MON_COUNT];   // 0x001A8
    TouchscreenHitbox hitboxes[34];                       // 0x00310, 30 box slots, exit, previous box, next box, end
    MigrateFromAgbButton buttons[3];                      // 0x00398, exit, next box, previous box
    MigrateFromAgbPick picks[MIGRATE_FROM_AGB_MON_COUNT]; // 0x003C8
    int pickCount;                                        // 0x00410
    Sprite *pickIcons[MIGRATE_FROM_AGB_MON_COUNT];        // 0x00414, confirmation screen
    MigrateFromAgbText msgBox;                            // 0x0042C
    Window boxNameWindow;                                 // 0x00478
    Window monInfoWindow;                                 // 0x00488
    Window msgWindow;                                     // 0x00498
    int cartridgeResult;                                  // 0x004A8, PmAgbCartridgeGetOffsets
    int loadResult;                                       // 0x004AC, PmAgbCartridge_LoadSave
    u8 agbSave[0xE3D0];                                   // 0x004B0, the cartridge save (sector buffer + data)
    AgbPokemonStorage *storage;                           // 0x0E880
    int box;                                              // 0x0E884
    int pickDoneTimer;                                    // 0x0E888
    YesNoPrompt *yesNoPrompt;                             // 0x0E88C
    MigrateFromAgbSave save;                              // 0x0E890
    WaitingIcon *waitingIcon;                             // 0x0E898
    MigrateFromAgbIcon icons[AGB_BOX_MON_COUNT];          // 0x0E89C
    void (*vblankCallback)(void);                         // 0x12604
    String *string;                                       // 0x12608
    String *rawString;                                    // 0x1260C
} MigrateFromAgbApp;                                      // size: 0x12610

BOOL MigrateFromAgb_Init(OverlayManager *man, int *state);
BOOL MigrateFromAgb_Main(OverlayManager *man, int *state);
BOOL MigrateFromAgb_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MIGRATE_FROM_AGB_H
