#ifndef POKEHEARTGOLD_OVERLAY87_RESOURCE_UI_R13_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY87_RESOURCE_UI_R13_INTERNAL_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct Ov87ResourceWork {
    u8 pad_000[0x12C];
    void *managers[4];
    void *objects[5][4];
} Ov87ResourceWork;
extern const u8 ov87_021E8394[];
void ov87_021E7EF0(void);
void GF_CreateVramTransferManager(int, int);
void NNS_G2dInitOamManagerModule(void);
void OamManager_Create(int, int, int, int, int, int, int, int, int);
void *G2dRenderer_Init(int, void *, int);
void G2dRenderer_SetSubSurfaceCoords(void *, int, int);
void *Create2DGfxResObjMan(int, int, int);
void ov87_021E7BC4(u8 *, int);
void ov87_021E7C50(u8 *, int, int, int);
void ov87_021E7CE0(u8 *, int);
void ov87_021E7D6C(u8 *, int);
void SpriteTransfer_CreateCharTransferTask(void *);
void SpriteTransfer_CreateExtPlttTransferTask(void *);
void GfGfx_EngineBTogglePlanes(int, int);
void GfGfx_EngineATogglePlanes(int, int);
void *AddCharResObjFromNarc(void *, int, int, int, int, int, int);
void *AddPlttResObjFromNarc(void *, int, int, int, int, int, int, int);
void *AddCellOrAnimResObjFromNarc(void *, int, int, int, int, int, int);
void BG_LoadCharTilesData(void *, int, void *, void *, int);
void BgCommitTilemapBufferToVram(void *, int);
void CreateSpriteResourcesHeader(void *, int, int, int, int, int, int, int, int, void *, void *, void *, void *, void *, void *);
typedef struct Ov87SpriteHeader {
    u8 raw[0x24];
} Ov87SpriteHeader;
typedef struct Ov87SpriteTemplate {
    void *spriteList;
    Ov87SpriteHeader *header;
    s32 position[3];
    s32 scale[3];
    u16 rotation;
    u16 pad;
    u32 drawPriority;
    int whichScreen;
    int heapId;
} Ov87SpriteTemplate;
void *Sprite_CreateAffine(Ov87SpriteTemplate *);
void Sprite_SetAnimActiveFlag(void *, int);
void Sprite_SetAnimSpeed(void *, int);
void Sprite_SetAnimCtrlSeq(void *, int);
void ov87_021E7A44(u8 *, int, int);
void ov87_021E7AB0(u8 *, u32, u32);
void ov87_021E7AF8(u8 *);
void *ov87_021E7DF8(u8 *, int, int, int, int, u8);
#endif
