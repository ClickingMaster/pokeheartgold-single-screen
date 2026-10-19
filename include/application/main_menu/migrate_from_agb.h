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
// the GBA cartridge, they are sent to the Pal Park (MigratedPokemon) and erased from the GBA save.

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

// A mon icon waiting for the VBlank upload (gMigrateFromAgbIcons points at the 30 of the box)
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
    int agbVersion;   // VERSION_RUBY ...
    int pendingMsg;   // message of MigrateFromAgb_PrintResultMsg / MigrateFromAgb_PrintResultMsgWithYesNo still to print
    int pendingMsgId; // message of MigrateFromAgb_PrintPendingMsg still to print (-1: none)
    u32 frameCount;
    SaveData *saveData;
    PlayerProfile *profile;
    Options *options;
    int frame;
    BgConfig *bgConfig;
    int nextState; // after the fade
    SpriteList *spriteList;
    G2dRenderer renderer;
    void *resMan[6];
    SpriteResource *resources[4];
    u8 unk_017C[8];
    SpriteResourcesHeader resHeader;
    MigrateFromAgbSlotSprites slots[AGB_BOX_MON_COUNT];
    TouchscreenHitbox hitboxes[34];  // 30 box slots, exit, previous box, next box, end
    MigrateFromAgbButton buttons[3]; // exit, next box, previous box
    MigrateFromAgbPick picks[MIGRATE_FROM_AGB_MON_COUNT];
    int pickCount;
    Sprite *pickIcons[MIGRATE_FROM_AGB_MON_COUNT]; // confirmation screen
    MigrateFromAgbText msgBox;
    Window boxNameWindow;
    Window monInfoWindow;
    Window msgWindow;
    int cartridgeResult; // PmAgbCartridgeGetOffsets
    int loadResult;      // PmAgbCartridge_LoadSave
    u8 agbSave[0xE3D0];  // the cartridge save (sector buffer + data)
    AgbPokemonStorage *storage;
    int box;
    int pickDoneTimer;
    YesNoPrompt *yesNoPrompt;
    MigrateFromAgbSave save;
    WaitingIcon *waitingIcon;
    MigrateFromAgbIcon icons[AGB_BOX_MON_COUNT];
    void (*vblankCallback)(void);
    String *string;
    String *rawString;
} MigrateFromAgbApp;

BOOL MigrateFromAgb_Init(OverlayManager *man, int *state);
BOOL MigrateFromAgb_Main(OverlayManager *man, int *state);
BOOL MigrateFromAgb_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MIGRATE_FROM_AGB_H
