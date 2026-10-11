#ifndef TO52_R5_W14_PRIVATE_H
#define TO52_R5_W14_PRIVATE_H
#include "global.h"

typedef struct Ov70Root9 {
    u8 pad00[0x24];
    void *options;
} Ov70Root9;

typedef struct Ov70Work9 {
    Ov70Root9 *root;
    void *bgConfig;
    u8 pad08[0x34 - 8];
    u16 initialized;
    u8 pad36[0x11C - 0x36];
    u16 spriteKind;
    u8 pad11E[0xD20 - 0x11E];
    void *resourceManagers[4];
    u8 padD30[0xDCC - 0xD30];
    void *spriteDCC;
} Ov70Work9;

typedef struct Ov70SpriteResourcesHeader9 {
    void *values[8];
    u8 priority;
    u8 padding[3];
} Ov70SpriteResourcesHeader9;

typedef struct Ov70SpriteTemplate9 {
    void *spriteList;
    Ov70SpriteResourcesHeader9 *header;
    s32 x;
    s32 y;
    s32 z;
    s32 scaleX;
    s32 scaleY;
    s32 scaleZ;
    u16 rotation;
    u16 pad22;
    u32 drawPriority;
    u32 whichScreen;
    u32 heapId;
} Ov70SpriteTemplate9;

extern const u16 ov70_02245240[];
extern const u16 ov70_02245242[];

void GfGfx_EngineATogglePlanes(int plane, int enable);
void GfGfx_EngineBTogglePlanes(int plane, int enable);
void Sprite_SetDrawFlag(void *sprite, int enable);
void Sprite_SetAnimActiveFlag(void *sprite, int enable);
void Sprite_SetAnimCtrlSeq(void *sprite, int seq);
void *NARC_New(int narcId, int heapId);
void NARC_Delete(void *narc);
void GfGfxLoader_GXLoadPalFromOpenNarc(void *, int, int, int, int, int);
void LoadFontPal1(int, int, int);
int Options_GetFrame(void *options);
void LoadUserFrameGfx2(void *, int, int, int, int, int);
void LoadUserFrameGfx1(void *, int, int, int, int, int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void CreateSpriteResourcesHeader(Ov70SpriteResourcesHeader9 *, int, int, int, int, int, int, int, int, void *, void *, void *, void *, void *, void *);
void Gts_InitSpriteTemplate(Ov70SpriteTemplate9 *, Ov70Work9 *, Ov70SpriteResourcesHeader9 *, int);
void *Sprite_CreateAffine(Ov70SpriteTemplate9 *);
void ov70_02239C6C(Ov70Work9 *);
void ov70_02239CF8(Ov70Work9 *);

void ov70_02239304(Ov70Work9 *);
void ov70_02239330(Ov70Work9 *);
void ov70_02239414(Ov70Work9 *);
#endif
