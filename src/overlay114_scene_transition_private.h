#include "global.h"

typedef struct Sprite Sprite;
typedef struct SpriteList SpriteList;

typedef struct {
    s32 value;
    s32 start;
    s32 delta;
    s32 frame;
    s32 duration;
} Interp;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Triple;

typedef struct {
    Interp fade;
    Interp scale;
    u8 pad28[4];
    u8 spriteResources[0x13c];
    void *resource168;
    u8 pad16c[0x30];
    Sprite *sprite19c;
} Ov114WorkA;

typedef struct {
    u8 spriteResources[0x13c];
    void *resource13c;
    u8 pad140[0x30];
    Sprite *sprites170[4];
    Interp horiz;
    u8 pad194[4];
    Interp vert;
    u8 pad1ac[4];
} Ov114WorkB;

typedef struct {
    u32 state;
    u32 done;
    u32 unused08;
    void *work;
    void *fieldSystem;
    u32 *completionFlag;
    u8 pad18[8];
    void *spriteManager;
} Ov114Task;

void *Heap_Alloc(u32, u32);
void *memset(void *, int, u32);
void GfGfx_EngineATogglePlanes(u32, BOOL);
void EncounterEffect_StartFlash(s32, s32, s32, u32 *, s32);
void EncounterEffectInterp_Init(Interp *, s32, s32, s32);
BOOL EncounterEffectInterp_Update(Interp *);
void EncounterEffectQuadInterp_Init(Interp *, s32, s32, s32, s32);
BOOL EncounterEffectQuadInterp_Update(Interp *);
void EncounterEffect_InitSprites(void *, s32, s32);
void EncounterEffect_FreeSprites(void *);
void EncounterEffect_LoadSpriteResources(void *, void *, void *, u32, s32, u32, u32, u32, u32);
void EncounterEffect_FreeSpriteResources(void *, void *);
Sprite *EncounterEffect_CreateSprite(void *, void *, s32, s32, s32, s32);
void EncounterEffect_SetVecFx32(Triple *, s32, s32, s32);
void EncounterEffect_Finish(Ov114Task *, void *);
void Sprite_SetDrawFlag(Sprite *, BOOL);
void Sprite_SetAffineOverwriteMode(Sprite *, int);
void Sprite_SetOamMode(Sprite *, int);
void Sprite_SetAffineScale(Sprite *, Triple *);
void Sprite_SetMatrix(Sprite *, Triple *);
void Sprite_Delete(Sprite *);
void SpriteList_RenderAndAnimateSprites(SpriteList *);
void HBlankSystem_Stop(void *);
void HBlankSystem_Start(void *);
void BeginNormalPaletteFade(s32, s32, s32, u16, s32, s32, s32);
BOOL IsPaletteFadeFinished(void);
void SetMasterBrightnessByColor(s32, u16);
void EncounterEffect_FrontierOrLinkSingle(void *, Ov114Task *);
void EncounterEffect_Double(void *, Ov114Task *);
