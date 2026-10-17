#ifndef OV58_PRIVATE_H
#define OV58_PRIVATE_H

#include "global.h"

#include "constants/pokemon.h"

#include "apricorn_box.h"
#include "apricorn_internal.h"
#include "bag.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail.h"
#include "menu_input_state.h"
#include "overlay_58.h"
#include "overlay_manager.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pokemon_mood.h"
#include "save.h"
#include "screen_fade.h"
#include "system.h"

// ApricornBoxApp.mode, the Apricorn Box screen's mode
enum ApricornBoxMode {
    APRICORN_BOX_MODE_VIEW,
    APRICORN_BOX_MODE_BLENDER,
    APRICORN_BOX_MODE_KURT,
    APRICORN_BOX_MODE_USE_JUICE,
};

// ApricornBoxArgs.mode, how the Apricorn Box was opened
enum ApricornBoxLaunchMode {
    APRICORN_BOX_LAUNCH_VIEW,
    APRICORN_BOX_LAUNCH_BLENDER,
    APRICORN_BOX_LAUNCH_KURT,
    APRICORN_BOX_LAUNCH_JUICE_STAND,
};

// No tracked definition found for the Apricorn Box application work block.
// Derived from OverlayManager_CreateAndGetData(man, 0x20, ...) + MI_CpuFill8(.., 0x20).
typedef struct ApricornBoxApp {
    /* 0x00 */ enum HeapID heapId;
    /* 0x04 */ ApricornBoxArgs *args;
    /* 0x08 */ OverlayManager *child;
    /* 0x0C */ u32 unk_0C;
    /* 0x10 */ PartyMenuArgs *partyMenuArgs;
    /* 0x14 */ u32 juiceIndex;
    /* 0x18 */ u8 mode;
    /* 0x19 */ u8 unk_19;
    /* 0x1A */ u8 lastRows[2];
    /* 0x1C */ u8 partySlot;
} ApricornBoxApp;

// No tracked definition: output block of ApricornJuice_RankFlavors.
typedef struct ApricornJuiceEntry {
    u16 stat : 4;
    u16 strength : 4;
    u16 amount : 8;
} ApricornJuiceEntry;

typedef struct ApricornJuiceData {
    /* 0x00 */ ApricornJuiceEntry entries[2];
    /* 0x04 */ u16 thirdEntry;
    /* 0x06 */ u8 statOverride[2];
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u8 penaltyStat;
    /* 0x0B */ u8 ties;
} ApricornJuiceData;

void SetMasterBrightnessByColor(PMLCDTarget screen, u16 color);

// Output of ApricornJuice_RankFlavors: the juice made from the blended apricorns.
typedef struct ApricornBoxJuice {
    ApricornJuiceEntry entries[3]; // 0x00
    u8 flavorOrder[6];             // 0x06
    u8 numEntries;                 // 0x0C
} ApricornBoxJuice;

#endif // OV58_PRIVATE_H
