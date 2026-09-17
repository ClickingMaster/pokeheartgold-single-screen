#ifndef TO50_OVERLAY117_PRIVATE_H
#define TO50_OVERLAY117_PRIVATE_H
#include "global.h"
typedef struct Sprite Sprite;
typedef struct SpriteList SpriteList;
typedef struct {
    s32 value, start, delta, frame, duration;
} Interp;
typedef struct {
    Interp interp;
    u32 pad;
} InterpPad;
typedef struct {
    s32 x, y, z;
} Triple;
typedef struct {
    u8 spriteResources[0x13c];
    void *resource13c;
    u8 pad140[0x30];
    Sprite *sprites170[6];
    InterpPad interp188[6];
    InterpPad interp218[6];
    InterpPad interp2a8[6];
    Interp interp338[6];
    u32 active3b0[6];
    s32 index3c8;
    s32 timer3cc;
} Ov117Work;
typedef struct {
    u32 state, done, unused08;
    Ov117Work *work;
    void *fieldSystem;
    u32 *completionFlag;
    u8 pad18[8];
    void *spriteManager;
} Ov117Task;
void *Heap_Alloc(u32, u32);
void *memset(void *, int, u32);
void ov01_021F05C4(void *, s32, s32);
void ov01_021F05F4(void *);
void ov01_021F0614(void *, void *, void *, u32, s32, u32, u32, u32, u32);
void ov01_021F06EC(void *, void *);
Sprite *ov01_021F0718(void *, void *, s32, s32, s32, s32);
void ov01_021F074C(Triple *, s32, s32, s32);
void ov01_021EFCF8(s32, s32, s32, u32 *, s32);
void ov01_021EFEC8(Interp *, s32, s32, s32, s32);
void ov01_021EFE34(Interp *, s32, s32, s32);
BOOL ov01_021EFF28(Interp *);
BOOL ov01_021EFE44(Interp *);
void ov01_021EFCDC(Ov117Task *, void *);
void GfGfx_EngineATogglePlanes(u32, BOOL);
void Sprite_SetDrawFlag(Sprite *, BOOL);
void Sprite_SetMatrix(Sprite *, Triple *);
void Sprite_SetScaleAndAffineType(Sprite *, Triple *, u8);
void Sprite_SetAffineScale(Sprite *, Triple *);
void Sprite_SetAffineZRotation(Sprite *, u32);
void Sprite_Delete(Sprite *);
void SpriteList_RenderAndAnimateSprites(SpriteList *);
void HBlankSystem_Stop(void *);
void HBlankSystem_Start(void *);
void BeginNormalPaletteFade(s32, s32, s32, u16, s32, s32, s32);
BOOL IsPaletteFadeFinished(void);
void sub_0200FBF4(s32, u16);
extern const s32 ov117_0225FAF4[], ov117_0225FAF8[], ov117_0225FAFC[];
extern const s32 ov117_0225FB00[], ov117_0225FB04[], ov117_0225FB08[];
extern const s32 ov117_0225FB0C[], ov117_0225FB10[];
void ov117_0225F020(void *, Ov117Task *);
#endif
