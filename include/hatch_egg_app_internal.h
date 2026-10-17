#ifndef POKEHEARTGOLD_HATCH_EGG_APP_INTERNAL_H
#define POKEHEARTGOLD_HATCH_EGG_APP_INTERNAL_H

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "hatch_egg_task.h"
#include "message_format.h"
#include "palette.h"
#include "particle_system.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "sprite_system.h"

// Layout of HatchEggTaskData, which the task passes as the overlay's args.
typedef struct HatchEggAppArgs {
    u32 taskState;
    BOOL wantToNameNewMon; // set by the nickname prompt
    void *namingScreenArgs;
    HatchEggArgs hatch;
} HatchEggAppArgs;

typedef struct HatchEggParticlesParam {
    enum HeapID heapId;
    u32 fileId; // NARC_a_1_1_6 member: 0 = egg, 1 = Manaphy egg
} HatchEggParticlesParam;

typedef struct HatchEggParticles {
    enum HeapID heapId;
    u32 fileId;
    u32 unk_08;
    SPLEmitter *particleSystem;
} HatchEggParticles;

// The "Give a nickname?" yes/no buttons on the touch screen.
typedef struct HatchEggYesNoMenu {
    enum HeapID heapId;           // 0x00
    SpriteSystem *spriteSystem;   // 0x04
    SpriteManager *spriteManager; // 0x08
    BgConfig *bgConfig;           // 0x0C
    ManagedSprite *cursor;        // 0x10
    MessageFormat *msgFormat;     // 0x14
    u8 state;                     // 0x18
    u8 selection;                 // 0x19: 0 = none, 1 = yes, 2 = no
    Window buttons[2];            // 0x1C
} HatchEggYesNoMenu;              // size: 0x3C

typedef struct HatchEggApp {
    HatchEggAppArgs *args;          // 0x00
    BgConfig *bgConfig;             // 0x04
    PaletteData *paletteData;       // 0x08
    u32 printerId;                  // 0x0C
    u32 textSpeed;                  // 0x10
    u32 frame;                      // 0x14
    Window window;                  // 0x18
    u8 filler_28[0x10];             // 0x28
    GF3DVramMan *vramMan;           // 0x38
    PokepicManager *pokepicManager; // 0x3C
    NARC *narc;                     // 0x40
    HatchEggParticles *particles;   // 0x44
    u8 filler_48[8];                // 0x48
    SpriteManager *spriteManager;   // 0x50
    SpriteSystem *spriteSystem;     // 0x54
    PokepicAnimManager *picManager; // 0x58
    u32 sceneType;                  // 0x5C: index into sHatchEggSceneFuncs
    u32 state;                      // 0x60
    s32 timer;                      // 0x64
    u32 subState;                   // 0x68
    u8 animParams[4];               // 0x6C
    Pokepic *pokepic;               // 0x70
    ManagedSprite *eggSprite;       // 0x74
    ManagedSprite *topBar;          // 0x78
    ManagedSprite *bottomBar;       // 0x7C
    int shakeState;                 // 0x80
    int shakeFrame;                 // 0x84
    HatchEggYesNoMenu *yesNoMenu;   // 0x88
} HatchEggApp;                      // size: 0x8C

// HatchEggApp_InitSpriteSystem and HatchEggApp_FreeSpriteSystem are passed &app->bgConfig and address
// the sprite system relative to it.
typedef struct HatchEggAppFromBgConfig {
    u8 filler_00[0x4C];
    SpriteManager *spriteManager; // HatchEggApp 0x50
    SpriteSystem *spriteSystem;   // HatchEggApp 0x54
} HatchEggAppFromBgConfig;

typedef struct HatchEggBgTemplates {
    BgTemplate templates[3];
} HatchEggBgTemplates;

void HatchEggApp_HideLayers(void);
void HatchEggApp_InitBlend(void);
GF3DVramMan *HatchEggApp_Create3DVramMan(void);
void HatchEggApp_InitParticleVram(void);
void HatchEggApp_InitBgs(BgConfig *bgConfig);
void HatchEggApp_VBlankCB(void *data);
void HatchEggApp_Draw3D(void);
void HatchEggApp_FadeIn(void);
void HatchEggApp_FadeOut(void);
void HatchEggApp_LoadWindowGfx(BgConfig *bgConfig, PaletteData *paletteData, u32 frame);
void HatchEggApp_CreateMessageWindow(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int paletteNum);
int HatchEggApp_PrintMessage(Window *window, int msgId, Pokemon *mon, int textSpeed);
void HatchEggApp_RemoveMessageWindow(Window *window);
void HatchEggApp_LoadBackground(BgConfig *bgConfig, PaletteData *paletteData);
void HatchEggApp_DestroyParticleSystem(SPLEmitter *particleSystem);
HatchEggParticles *HatchEggParticles_New(HatchEggParticlesParam *param);
void HatchEggParticles_Emit(HatchEggParticles *particles, int resId);
BOOL HatchEggParticles_IsRunning(HatchEggParticles *particles);
void HatchEggParticles_Free(HatchEggParticles *particles);
void HatchEggApp_LoadSpriteResources(HatchEggApp *app);
void HatchEggApp_CreateSprites(HatchEggApp *app);
int HatchEggApp_UpdateEggShake(HatchEggApp *app, int pattern);
void HatchEggApp_DeleteSprites(HatchEggApp *app);
void HatchEggApp_InitSpriteSystem(HatchEggAppFromBgConfig *app);
void HatchEggApp_FreeSpriteSystem(HatchEggAppFromBgConfig *app);
void HatchEggApp_CreatePokepic(HatchEggApp *app);
void HatchEggApp_StartPokepicAnim(HatchEggApp *app);
void HatchEggApp_DeletePokepic(HatchEggApp *app);
void HatchEggApp_SetPokepicVanished(HatchEggApp *app, int vanished);

#endif // POKEHEARTGOLD_HATCH_EGG_APP_INTERNAL_H
