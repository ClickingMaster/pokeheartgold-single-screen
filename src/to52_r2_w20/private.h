#ifndef TO52_SOL_R2_W20_PRIVATE_H
#define TO52_SOL_R2_W20_PRIVATE_H
#include "global.h"
#include "filesystem.h"
#pragma require_prototypes off
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
typedef struct W20Work {
    u8 pad00[0xbf8];
    void *spriteList;
    u8 renderer[0x128];
    void *managers[4];
    void *resources[12];
} W20Work;
typedef struct W20TransferTemplate { u32 word[4]; } W20TransferTemplate;
extern const W20TransferTemplate ov72_0223B344;
void ObjCharTransfer_Init(const void *);
void ObjPlttTransfer_Init(int, int);
void ObjCharTransfer_ClearBuffers(void);
void ObjPlttTransfer_Reset(void);
void NNS_G2dInitOamManagerModule(void);
void *OamManager_Create(int,int,int,int,int,int,int,int,int);
void *G2dRenderer_Init(int,void *,int);
void G2dRenderer_SetSubSurfaceCoords(void *,int,int);
void *Create2DGfxResObjMan(int,int,int);
void *AddCharResObjFromOpenNarc(void *,void *,int,int,int,int,int);
void *AddPlttResObjFromOpenNarc(void *,void *,int,int,int,int,int,int);
void *AddCellOrAnimResObjFromOpenNarc(void *,void *,int,int,int,int,int);
void SpriteTransfer_CreateCharTransferTask(void *);
void SpriteTransfer_CreateExtPlttTransferTask(void *);
#endif
