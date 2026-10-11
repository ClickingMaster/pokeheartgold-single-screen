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

// A Pokemon that can be found on a Pokewalker course.
typedef struct PokewalkerRouteMon {
    u16 species;
    u16 level;
    u16 heldItem;
    u8 form;
    u8 gender;
    u16 moves[MAX_MON_MOVES];
    u16 steps;        // steps needed before it can appear
    u16 chance;       // percent
} PokewalkerRouteMon; // size=0x14

// An item that can be found on a Pokewalker course.
typedef struct PokewalkerRouteItem {
    u16 item;
    u16 steps;
    u16 chance;
} PokewalkerRouteItem;

// Pokewalker course (table sPokewalkerCourses, one per course).
typedef struct PokewalkerCourse {
    u32 wattsRequired;          // watts needed to unlock the course
    u32 routeImageId;           // index into the route image table
    PokewalkerRouteMon mons[6]; // pairs; one of each pair is picked
    PokewalkerRouteItem items[10];
    u8 advantageTypes[3]; // a Pokemon of these types finds the course's Pokemon sooner
    u8 padding_BF;
} PokewalkerCourse; // size=0xC0

// A Pokemon in the Pokewalker's own format (sent with the stroll data).
typedef struct PokewalkerMon {
    u16 species;
    u16 heldItem;
    u16 moves[MAX_MON_MOVES];
    u8 level;
    u8 form : 5;
    u8 gender : 2;
    u8 unk_0D_7 : 1;
    u8 canFlip : 1; // BASE_FLIP of the species
    u8 isShiny : 1;
    u8 unk_0E_2 : 6;
    u8 padding_0F;
} PokewalkerMon; // size=0x10

// Stroll data sent to the Pokewalker with the Pokemon: the Pokemon, its course, the course's Pokemon and items,
// and their pictures and names rendered to the Pokewalker's 2bpp format.
typedef struct PokewalkerStrollData {
    PokewalkerMon mon;
    u16 nickname[POKEMON_NAME_LENGTH + 1];
    u8 friendship;
    u8 routeImageId;
    u16 courseName[21];
    PokewalkerMon routeMons[3];
    u16 routeMonSteps[3];
    u8 routeMonChances[3];
    u8 padding_8B;
    u16 routeItems[10];
    u16 routeItemSteps[10];
    u8 routeItemChances[10];
    u8 routeImage[0xC0];
    u8 courseNameImage[0x140];
    u8 monIcon[0x180];
    u8 monPicture[0x600];
    u8 nicknameImage[0x140];
    u8 routeMonIcons[3 * 0x180];
    u8 routeMonPicture[0x600];
    u8 routeMonNameImages[3 * 0x140];
    u8 routeItemNameImages[10 * 0x180];
} PokewalkerStrollData; // size=0x28BE

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
    u8 uploadImage[0x8C50];            // 0x10F4, built by PokewalkerApp_BuildUploadImage
    u8 unk_9D44[0x2BCC];
    PokewalkerStrollData strollData; // 0xC910
    u32 unk_F1D0;
    u16 unk_F1D4;
    u8 unk_F1D6[2];
    PokewalkerIrClock unk_F1D8;  // copied from the Pokewalker save data
    u8 unk_F200[0x1634];         // 0xF200, filled by ov112_021F3244
    u8 irCompressBuffer[0xCF1C]; // 0x10834
    u8 unk_1D750[0x2C];
    u16 strollMonSpecies; // 0x1D77C
    u8 unk_1D77E[0x16];
    u8 strollMonForm; // 0x1D794
    u8 unk_1D795;
    u8 strollMonGender; // 0x1D796
    u8 unk_1D797;
    int courseId;                   // 0x1D798
    PokepicTemplate monPicTemplate; // 0x1D79C
    u8 monSpriteBuffer[0xC80];      // 0x1D7AC, 80x80 4bpp
    void *unk_1E42C;
    BoxPokemon *strollMon;     // 0x1E430
    Bag *bag;                  // 0x1E434
    PlayerProfile *profile;    // 0x1E438
    Options *options;          // 0x1E43C
    POKEWALKER *pokewalker;    // 0x1E440
    Pokedex *pokedex;          // 0x1E444
    MessageFormat *msgFormat;  // 0x1E448
    MsgData *msgData;          // 0x1E44C
    MsgData *msgDataItemNames; // 0x1E450
    String *string;            // 0x1E454
    u8 unk_1E458[0xD0];
    SpriteSystem *spriteSystem;   // 0x1E528
    SpriteManager *spriteManager; // 0x1E52C
    u8 unk_1E530[0xDB0];
    u16 subBgScrollX; // 0x1F2E0, BG 5 (sub screen) x offset
    u8 unk_1F2E2[0x96];
} PokewalkerApp; // size=0x1F378

extern const PokewalkerCourse sPokewalkerCourses[];

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
void PokewalkerApp_BuildUploadImage(PokewalkerApp *app, u8 *dst);
void PokewalkerApp_PickRouteMons(PokewalkerMon *mons, u16 *steps, u8 *chances, int courseId, u8 *chosen);
void PokewalkerApp_ApplyAdvantageTypes(const u8 *types, BoxPokemon *boxMon, u16 *steps);
void PokewalkerApp_BuildStrollData(PokewalkerStrollData *data, PokewalkerApp *app);
void PokewalkerApp_PrintPlayerName(Window *window, PokewalkerApp *app);
void PokewalkerApp_PrintStrollTexts(Window *windows, PokewalkerApp *app, const PokewalkerCourse *course, const u8 *chosen);
int PokewalkerApp_LoadMonPicture(BoxPokemon *boxMon, void *buffer, PokepicTemplate *template, int screen);
BOOL ov112_021E9338(PokewalkerApp *app);
int PokewalkerApp_GetPixel4bpp(const u8 *pixels, int widthTiles, int x, int y);
int PokewalkerApp_ConvertTo2bpp(const u8 *src, int widthTiles, int unused, u8 *dst, int x, int y, int width, int height);
u32 ov112_021E9464(BoxPokemon *boxMon);
u32 ov112_021E9470(u16 species);
int ov112_021E9480(int species, int gender, int form);
void ov112_021E9A30(void *picture, const void *mask, u32 personality);
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
