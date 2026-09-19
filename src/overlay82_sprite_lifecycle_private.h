#ifndef TO52_R2_W17_PRIVATE_H
#define TO52_R2_W17_PRIVATE_H
#include "global.h"
#include "obj_char_transfer.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

typedef struct Ov82SpriteWorkW17 {
    SpriteList *spriteList;
    u8 rendererAndPadding[0x128];
    GF_2DGfxResMan *resourceManagers[4];
    SpriteResource *resourceObjects[2][4];
} Ov82SpriteWorkW17;

void NNS_G2dInitOamManagerModule(void);
void OamManager_Create(int, int, int, int, int, int, int, int, enum HeapID);
void OamManager_Free(void);
void ObjPlttTransfer_Destroy(void);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void ov82_0223FC14(void);
extern const u8 ov82_0223FEEC[4];

void ov82_0223F95C(Ov82SpriteWorkW17 *, Pokemon *);
Sprite *ov82_0223FB08(Ov82SpriteWorkW17 *, int, int, int, u8);
void ov82_0223FBBC(Ov82SpriteWorkW17 *);

#endif
