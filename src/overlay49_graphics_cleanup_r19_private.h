#ifndef TO50_OV49_R19_R3_PRIVATE_H
#define TO50_OV49_R19_R3_PRIVATE_H
#include "global.h"

#include "gf_3d_vramman.h"
#include "vram_transfer_manager.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern const u32 ov49_02269734[4];
void FreeBgTilemapBuffer(void *, u8);
void Destroy2DGfxResObjMan(void *);
void SpriteList_Delete(void *);
void ObjCharTransfer_Destroy(void);
void ObjPlttTransfer_Destroy(void);
void OamManager_Free(void);
void ov49_0225A7D0(void *);
#endif
