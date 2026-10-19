#ifndef POKEHEARTGOLD_HATCH_EGG_APP_INTERNAL_H
#define POKEHEARTGOLD_HATCH_EGG_APP_INTERNAL_H

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "hatch_egg_task_data.h"
#include "message_format.h"
#include "palette.h"
#include "particle_system.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "sprite_system.h"

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

// The display state of the app; HatchEggApp_InitSpriteSystem and HatchEggApp_FreeSpriteSystem take it on its own.
typedef struct HatchEggAppGraphics {
    BgConfig *bgConfig;
    PaletteData *paletteData;
    u32 printerId;
    u32 textSpeed;
    u32 frame;
    Window window;
    u8 filler_28[0x10];
    GF3DVramMan *vramMan;
    PokepicManager *pokepicManager;
    NARC *narc;
    HatchEggParticles *particles;
    u8 filler_48[8];
    SpriteManager *spriteManager;
    SpriteSystem *spriteSystem;
} HatchEggAppGraphics;

typedef struct HatchEggApp {
    HatchEggTaskData *args;
    HatchEggAppGraphics gfx;
    PokepicAnimManager *picManager;
    u32 sceneType; // index into sHatchEggSceneFuncs
    u32 state;
    s32 timer;
    u32 subState;
    u8 animParams[4];
    Pokepic *pokepic;
    ManagedSprite *eggSprite;
    ManagedSprite *topBar;
    ManagedSprite *bottomBar;
    int shakeState;
    int shakeFrame;
    HatchEggYesNoMenu *yesNoMenu;
} HatchEggApp;

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
void HatchEggApp_InitSpriteSystem(HatchEggAppGraphics *gfx);
void HatchEggApp_FreeSpriteSystem(HatchEggAppGraphics *gfx);
void HatchEggApp_CreatePokepic(HatchEggApp *app);
void HatchEggApp_StartPokepicAnim(HatchEggApp *app);
void HatchEggApp_DeletePokepic(HatchEggApp *app);
void HatchEggApp_SetPokepicVanished(HatchEggApp *app, int vanished);

#endif // POKEHEARTGOLD_HATCH_EGG_APP_INTERNAL_H
