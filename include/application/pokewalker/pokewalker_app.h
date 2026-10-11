#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_APP_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_APP_H

#include "application/pokewalker/pokewalker_ir.h"

#include "bag.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokewalker.h"
#include "save.h"
#include "sprite_system.h"

// Work of the Pokewalker connection app (main menu option).
typedef struct PokewalkerApp {
    int mode;     // index into the mode table (enter/update/exit functions)
    int nextMode; // 11 = leave the app
    u8 unk_08[0x10];
    BgConfig *bgConfig; // 0x18
    u8 unk_1C[4];
    SaveData *saveData;                // 0x20
    u8 irThreadStack[0x1000];          // 0x24
    PokewalkerIrIdentity identity;     // 0x1024
    PokewalkerIrIdentity peerIdentity; // 0x108C
    u8 uploadImage[0x8C50];            // 0x10F4, built by ov112_021E7DFC
    u8 unk_9D44[0x2BCC];
    u8 unk_C910[0x28C0]; // 0xC910, built by ov112_021E8CB8
    u32 unk_F1D0;
    u16 unk_F1D4;
    u8 unk_F1D6[2];
    PokewalkerIrClock unk_F1D8;  // copied from the Pokewalker save data
    u8 unk_F200[0x1634];         // 0xF200, filled by ov112_021F3244
    u8 irCompressBuffer[0xDBFC]; // 0x10834
    void *unk_1E430;
    Bag *bag;                 // 0x1E434
    PlayerProfile *profile;   // 0x1E438
    Options *options;         // 0x1E43C
    POKEWALKER *pokewalker;   // 0x1E440
    Pokedex *pokedex;         // 0x1E444
    MessageFormat *msgFormat; // 0x1E448
    MsgData *msgData;         // 0x1E44C
    MsgData *msgDataCourses;  // 0x1E450
    String *string;           // 0x1E454
    u8 unk_1E458[0xD0];
    SpriteSystem *spriteSystem;   // 0x1E528
    SpriteManager *spriteManager; // 0x1E52C
    u8 unk_1E530[0xDB0];
    u16 subBgScrollX; // 0x1F2E0, BG 5 (sub screen) x offset
    u8 unk_1F2E2[0x96];
} PokewalkerApp; // size=0x1F378

BOOL PokewalkerApp_Init(OverlayManager *manager, int *state);
BOOL PokewalkerApp_Main(OverlayManager *manager, int *state);
BOOL PokewalkerApp_Exit(OverlayManager *manager, int *state);
void PokewalkerApp_Setup(PokewalkerApp *app);
void PokewalkerApp_Teardown(PokewalkerApp *app);
void PokewalkerApp_GetSaveData(PokewalkerApp *app);
void PokewalkerApp_VBlankCB(void *arg);
void PokewalkerApp_SetGxBanks(void);
void PokewalkerApp_InitBgs(BgConfig *bgConfig);
void PokewalkerApp_FreeBgs(BgConfig *bgConfig);
void PokewalkerApp_LoadBgGraphics(PokewalkerApp *app);
void PokewalkerApp_InitMessages(PokewalkerApp *app);
void PokewalkerApp_FreeMessages(PokewalkerApp *app);
void PokewalkerApp_LoadTilemap(PokewalkerApp *app, int bgId, int member);
void PokewalkerApp_StartLink(PokewalkerApp *app);
void ov112_021E7D7C(PokewalkerApp *app);
void ov112_021E7DA4(PokewalkerApp *app);
void ov112_021E7DC0(PokewalkerApp *app);
void ov112_021E7DFC(PokewalkerApp *app, void *dst);
void ov112_021E8CB8(void *dst, PokewalkerApp *app);
void ov112_021E9A78(PokewalkerApp *app, int type);
void ov112_021E9C10(PokewalkerApp *app, int which);
void ov112_021E9C94(PokewalkerApp *app);
void ov112_021E9C98(PokewalkerApp *app);
void ov112_021E9E30(PokewalkerApp *app);
void ov112_021EA1AC(PokewalkerApp *app);
void ov112_021EA1F8(PokewalkerApp *app);
void ov112_021EA4F4(PokewalkerApp *app);
void ov112_021F3244(SaveData *saveData, void *dst);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_APP_H
