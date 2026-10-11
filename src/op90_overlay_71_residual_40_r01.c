#include "global.h"
#include "gf_3d_vramman.h"
typedef struct { u16 x, y, z; u8 pad[10]; } Ov71Rot;
Ov71Rot ov71_0224C0D0;
extern const u16 ov71_0224BF0C[];
void GfGfx_EngineATogglePlanes(int, int);
void *TradeSequence_CreateModels(int, int, int, int);
void *TradeSequence_LoadModel(void *, int, int, int, int, int, int, int);
void TradeSequence_SetCameraAngle(void *, void *);
void TradeSequence_SetCameraPerspectiveType(void *, int);
void TradeSequence_SetModelScale(void *, int);
void TradeSequence_GetModelRotation(void *, void *);
void ov71_0224ADE4(u8 *w);
#define R16(a) (*(volatile u16 *)(a))
#define R32(a) (*(volatile u32 *)(a))
#define P(o) (*(void **)(w + (o)))
// @fn ov71_0224ADE4
void ov71_0224ADE4(u8 *w) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    R16(0x04000060) &= ~0x3002;
    R16(0x04000060) = (R16(0x04000060) & ~0x3000) | 16;
    R16(0x04000060) &= 0xcffb;
    R16(0x04000060) = (R16(0x04000060) & ~0x3000) | 8;
    R16(0x04000060) = (R16(0x04000060) & ~0x3000) | 32;
    G3X_SetEdgeColorTable(ov71_0224BF0C);
    G3X_SetClearColor(0x43ff, 0, 0x7fff, 63, FALSE);
    R32(0x04000540) = 2;
    R32(0x04000580) = 0xbfff0000;
    GF_3DVramMan_InitFrameTexVramManager(1, 1);
    GF_3DVramMan_InitFramePlttVramManager(16384, 1);
    GfGfx_EngineATogglePlanes(1, 1);
    R16(0x04000008) = (R16(0x04000008) & ~3) | 0;
    P(0x20) = TradeSequence_CreateModels(1, 0, 0, 0);
    P(0x24) = TradeSequence_LoadModel(P(0x20), 0, 89, 27, 0, 0x8c000, 0, 1);
    ov71_0224C0D0.x = 0;
    ov71_0224C0D0.y = 0;
    ov71_0224C0D0.z = 0;
    TradeSequence_SetCameraAngle(P(0x20), &ov71_0224C0D0);
    TradeSequence_SetCameraPerspectiveType(P(0x20), 1);
    TradeSequence_SetModelScale(P(0x24), 0x2000);
    TradeSequence_GetModelRotation(P(0x24), w + 0x28);
}
