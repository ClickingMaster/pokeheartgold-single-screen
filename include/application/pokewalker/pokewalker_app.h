#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_APP_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_APP_H

#include "application/pokewalker/pokewalker_ir.h"

#include "bag.h"
#include "bg_window.h"
#include "font_types_def.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokewalker.h"
#include "render_window.h"
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

// Particle of the effect shown while data is transferred (sprites 17-116 on the main screen, 117-216 on the sub screen).
typedef struct PokewalkerAppParticle {
    u8 active;
    u8 unk_01;
    s8 swayFactor;
    s8 speed; // pixels per frame, upwards
    s16 x;
    s16 y; // < 0: sub screen
    s16 dx;
    s16 dy;
    u16 angle; // degrees
    s16 life;
} PokewalkerAppParticle;

// Position and vertical speed of the object drawn with sprites 13 (main screen) and 14 (sub screen).
typedef struct PokewalkerAppMotion {
    u8 unk_00[2];
    s8 delta; // added to y each frame; 0 = stopped, 101 = arrived at the bottom
    u8 unk_03;
    s16 x;
    s16 y; // < 0: sub screen
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    s16 time;
} PokewalkerAppMotion;

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
    BoxPokemon *strollMon;        // 0x1E430
    Bag *bag;                     // 0x1E434
    PlayerProfile *profile;       // 0x1E438
    Options *options;             // 0x1E43C
    POKEWALKER *pokewalker;       // 0x1E440
    Pokedex *pokedex;             // 0x1E444
    MessageFormat *msgFormat;     // 0x1E448
    MsgData *msgData;             // 0x1E44C
    MsgData *msgDataItemNames;    // 0x1E450
    String *string;               // 0x1E454
    String *eraseRecordsText;     // 0x1E458
    String *goForStrollText;      // 0x1E45C
    String *receiveGiftText;      // 0x1E460
    String *returnFromStrollText; // 0x1E464
    String *sendHomeText;         // 0x1E468
    String *saveAndStrollText;    // 0x1E46C
    String *blankText;            // 0x1E470
    String *exitText;             // 0x1E474
    String *unk_1E478[7];
    String *strollResultsText;  // 0x1E494
    String *giftsReceivedText;  // 0x1E498
    String *walkerRecordsText;  // 0x1E49C
    String *yourPokemonText;    // 0x1E4A0
    String *itemsObtainedText;  // 0x1E4A4
    String *strollResultsText2; // 0x1E4A8
    String *itemsObtainedText2; // 0x1E4AC
    String *exitText2;          // 0x1E4B0
    String *stepsToDateText;    // 0x1E4B4
    String *unk_1E4B8;
    String *wattsToDateText; // 0x1E4BC
    String *unk_1E4C0[19];
    String *totalWattsText; // 0x1E50C
    String *totalStepsText; // 0x1E510
    String *yesText;        // 0x1E514
    String *noText;         // 0x1E518
    u8 unk_1E51C[0xC];
    SpriteSystem *spriteSystem;   // 0x1E528
    SpriteManager *spriteManager; // 0x1E52C
    ManagedSprite *sprites[217];  // 0x1E530: 0 cursor, 9.. Pokemon icons, 17-116 / 117-216 particles (main / sub)
    u8 unk_1E894[0x1CC];
    void *monIconFile;                    // 0x1EA60
    NNSG2dCharacterData *monIconCharData; // 0x1EA64
    Window windows[20];                   // 0x1EA68, main screen
    Window subWindows[10];                // 0x1EBA8, sub screen
    u8 windowCounts[2];                   // 0x1EC48, main / sub screen
    u8 windowBank;                        // 0x1EC4A, 0xFF = none
    u8 subWindowBank;                     // 0x1EC4B
    WaitingIcon *waitingIcon;             // 0x1EC4C
    int menuSelection;                    // 0x1EC50
    u8 unk_1EC54[0x24];
    int unlockedCourse; // 0x1EC78, set by PokewalkerApp_UnlockNextCourse
    u8 unk_1EC7C[4];
    PokewalkerAppParticle particles[100]; // 0x1EC80
    u8 unk_1F2C0[0x20];
    u16 subBgScrollX; // 0x1F2E0, BG 5 (sub screen) x offset
    u16 menuState;    // 0x1F2E2
    u8 unk_1F2E4[0x94];
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
u32 PokewalkerApp_GetBoxMonIconFile(BoxPokemon *boxMon);
u32 PokewalkerApp_GetSpeciesIconFile(u16 species);
int PokewalkerApp_GetPictureFile(int species, int gender, int form);
void PokewalkerApp_UnlockNextCourse(PokewalkerApp *app);
int PokewalkerApp_GetWattsToNextCourse(PokewalkerApp *app, int watts);
void PokewalkerApp_ShowYesNoMenu(PokewalkerApp *app, int selection);
void PokewalkerApp_ShowSaveEraseStepsMenu(PokewalkerApp *app, int selection);
int PokewalkerApp_HandleTwoChoiceMenu(PokewalkerApp *app);
int PokewalkerApp_GetTouchedButton(int index);
void PokewalkerApp_LoadSubBg1Tilemap(PokewalkerApp *app);
void PokewalkerApp_ShowSubBg1(PokewalkerApp *app, int show);
u8 PokewalkerApp_GetPixel2bpp(const u8 *picture, int x, int y);
void PokewalkerApp_SetPixel2bpp(u8 *picture, int x, int y, int color);
void PokewalkerApp_DrawSpindaSpots(u8 *picture, const u8 *mask, u32 personality, int x, int y);
void PokewalkerApp_DrawSpindaSpotsOnPicture(void *picture, const void *mask, u32 personality);
void PokewalkerApp_AddWindowBank(PokewalkerApp *app, int bank);
void PokewalkerApp_RemoveWindowBank(PokewalkerApp *app, int bank);
void ov112_021E9C94(PokewalkerApp *app);
void PokewalkerApp_LoadStrings(PokewalkerApp *app);
void PokewalkerApp_FreeStrings(PokewalkerApp *app);
void PokewalkerApp_PrintExit(PokewalkerApp *app, int window);
void PokewalkerApp_PrintMessage(PokewalkerApp *app, int window, int msgId, u32 color);
void PokewalkerApp_PrintString(PokewalkerApp *app, int window, String *string, FontID fontId, u32 color);
void PokewalkerApp_PrintStringDefaultColor(PokewalkerApp *app, int window, String *string, FontID fontId);
void PokewalkerApp_PrintSubString(PokewalkerApp *app, int window, String *string, FontID fontId, u32 color);
void PokewalkerApp_PrintSubMessage(PokewalkerApp *app, int window, int msgId, u32 color);
u8 PokewalkerApp_StartMessageBox(PokewalkerApp *app, int window, int msgId);
void PokewalkerApp_ShowMessageBox(PokewalkerApp *app, int window, int msgId);
void PokewalkerApp_CreateWaitingIcon(PokewalkerApp *app);
void PokewalkerApp_DeleteWaitingIcon(PokewalkerApp *app);
void PokewalkerApp_InitSprites(PokewalkerApp *app);
void PokewalkerApp_FreeSprites(PokewalkerApp *app);
void PokewalkerApp_InitSpriteSystem(PokewalkerApp *app);
void PokewalkerApp_LoadSpriteResources(PokewalkerApp *app);
void PokewalkerApp_CreateSprites(PokewalkerApp *app);
void PokewalkerApp_ResetCursor(PokewalkerApp *app);
void PokewalkerApp_SetCursorPosition(PokewalkerApp *app, int position);
void PokewalkerApp_HideCursor(PokewalkerApp *app);
void PokewalkerApp_PressCursor(PokewalkerApp *app);
void ov112_021EA5A4(PokewalkerApp *app, int anim);
void ov112_021EA60C(PokewalkerApp *app);
void ov112_021EA64C(PokewalkerApp *app);
void PokewalkerApp_ShowSprite(PokewalkerApp *app, int index);
void PokewalkerApp_HideSprite(PokewalkerApp *app, int index);
void PokewalkerApp_SetSpriteAnim(PokewalkerApp *app, int index, int anim);
void PokewalkerApp_SetSpritePosition(PokewalkerApp *app, int index, int x, int y);
void PokewalkerApp_DrawParticle(PokewalkerApp *app, int index, int frame, int x, int y);
void PokewalkerApp_SpawnRisingParticle(PokewalkerApp *app);
void PokewalkerApp_SpawnFallingParticle(PokewalkerApp *app);
int PokewalkerApp_AgeParticles(PokewalkerApp *app);
void PokewalkerApp_UpdateParticles(PokewalkerApp *app);
void PokewalkerApp_ClearParticles(PokewalkerApp *app);
int PokewalkerApp_FindFreeParticle(PokewalkerAppParticle *particles);
void ov112_021EA9BC(PokewalkerApp *app, const PokewalkerAppMotion *motion);
void ov112_021EAA10(PokewalkerApp *app);
void ov112_021EAA34(PokewalkerApp *app, int x, int y);
void ov112_021EAA98(PokewalkerApp *app, PokewalkerAppMotion *motion, int kind);
BOOL ov112_021EAAE4(PokewalkerApp *app, PokewalkerAppMotion *motion);
void PokewalkerApp_LoadMonIconPalettes(void);
void PokewalkerApp_LoadMonIcon(PokewalkerApp *app, int slot, int species, int form);
void PokewalkerApp_SetMonIconGraphics(NNSG2dCharacterData *charData, int species, int form, int isEgg, int slot, ManagedSprite *sprite);
void ov112_021F3244(SaveData *saveData, void *dst);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_APP_H
