#ifndef TO53_SOL_R1_W19_PRIVATE_H
#define TO53_SOL_R1_W19_PRIVATE_H

#include "global.h"
#include <string.h>

typedef struct W19Queue {
    u32 pad00;
    u16 point[8][2];
    u16 read;
    u16 write;
} W19Queue;

typedef struct W19Slots {
    u32 value[4];
    u32 mask;
    u32 selected;
} W19Slots;

void ov91_0225E45C(void *, void *);
void ov91_0225E8BC(void *, void *);
void ov91_0225E9F4(void *, void *);
void ov91_0225E67C(u8 *, u8 *);
void ov91_0225E400(void *);
void ov91_0225E3F4(void *, u8 *, u16, u32);
void ov91_0225E62C(u8 *);
void ov91_0225E648(u8 *);
void ov91_0225E6B0(u8 *);
BOOL ov91_0225E6D0(u8 *);
BOOL ov91_0225E990(u8 *);
BOOL ov91_0225E9AC(u8 *);
BOOL ov91_0225E9C0(u8 *);
void ov91_0225E9E4(u8 *, u8 *);
void ov91_0225EDB0(W19Queue *);
BOOL ov91_0225EE18(W19Queue *, u16 *);
BOOL ov91_0225EE48(W19Queue *, u16 *);
BOOL ov91_0225EE64(W19Queue *, u16 *);
u32 ov91_0225EE88(W19Queue *);
void ov91_0225F23C(u8 *);
void ov91_0225F388(W19Slots *);
void ov91_0225F3A0(W19Slots *, u32, u32);
void ov91_0225F3D4(W19Slots *);
u32 ov91_0225F3E4(W19Slots *);
BOOL ov91_0225F3E8(W19Slots *, u32);
void ov91_0225F4F8(u8 *);
void ov91_0225FA60(u8 *);
void ov91_0225FB48(u8 *);
void ov91_0225FCD8(u8 *, u32);
void ov91_0225FD0C(u8 *);
void ov91_0225FDC8(u8 *);
void ov91_0225FDD8(void);
void ov91_0225FEFC(void);

void Sprite_SetDrawFlag(void *, BOOL);
void FreeBgTilemapBuffer(void *, int);
void Heap_Free(void *);
void sub_020135AC(void *);
void SpriteList_Delete(void *);
void Destroy2DGfxResObjMan(void *);
void ObjCharTransfer_Destroy(void);
void ObjPlttTransfer_Destroy(void);
void OamManager_Free(void);
void *MessageFormat_New(u32);
void *NewMsgDataFromNarc(int, int, int, u32);
void *String_New(u32, u32);
void String_Delete(void *);
void DestroyMsgData(void *);
void MessageFormat_Delete(void *);
void Camera_Delete(void *);
void Camera_PushLookAtToNNSGlb(void);

#endif
