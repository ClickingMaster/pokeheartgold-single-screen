#include "global.h"

#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PAT(p, o) (*(void **)((u8 *)(p) + (o)))

extern void *OverlayManager_GetData(void *);
extern void ov74_02229F60(void *, u32);
extern void ov74_02229E28(void *, u32);
extern void ov74_02229DF8(void);
extern void ov74_0222A5CC(void *);
extern void ov74_0222AA18(void *, void *, u32);
extern void ov74_0222A94C(void *, u32, u32);
extern u32 ov74_022365FC(void);
extern void ov74_0222CE10(void *);
extern void ov74_02235390(u32);
extern void ov74_0223539C(u32, u32, u32, u32);
extern void sub_0203A914(void);
extern void Sprite_Delete(void *);
extern void *ov74_0222B0C8(void *, u32);
extern void Sprite_TryChangeAnimSeq(void *, u32);
extern void Sprite_SetAnimationFrame(void *, u32);
extern void Sprite_SetDrawFlag(void *, u32);
extern void Sprite_SetDrawPriority(void *, u32);
extern void Sprite_SetAffineOverwriteMode(void *, u32);
extern s32 *Sprite_GetMatrixPtr(void *);
extern void Sprite_UpdateAnim(void *, s32);
extern void SysTask_Destroy(void *);

extern void *ov74_0223D0A8;

void ov74_0222A2A4(void *manager);
void ov74_0222A494(void *manager);
u32 ov74_0222A4CC(void *manager);
u32 ov74_0222A504(void *manager);
u32 ov74_0222A538(void);
u32 ov74_0222A560(void *manager);
u32 ov74_0222A5AC(void *manager);
u32 ov74_0222A650(void *manager);
u32 ov74_0222A688(void *manager);
u32 ov74_0222A6C0(void *manager);
void ov74_0222B144(void *p);
void ov74_0222B19C(void *p);
void ov74_0222B1F4(void *p);
void ov74_0222B20C(void *p);
void ov74_0222B224(void *p);
void ov74_0222B288(void *p);
void ov74_0222B2C4(void *p);
void ov74_0222B30C(void *p);
void ov74_0222B344(void *p);
void ov74_0222B374(void *p);

u32 ov74_0222A560(void *manager) {
    void *data = OverlayManager_GetData(manager);
    ov74_0222AA18(manager, (u8 *)data + 0x18, 3);
    ov74_02229E28(data, 1);
    ov74_02229F60((u8 *)data + 0x48, 0);
    ov74_02229E28(data, 1);
    U32AT(data, 0x15e0) = 0x1000;
    U32AT(data, 0x267c) = 0;
    return 0x1b;
}
u32 ov74_0222A5AC(void *manager) {
    void *data = OverlayManager_GetData(manager);
    ov74_02229F60((u8 *)data + 0x38, 0);
    ov74_0222A94C(manager, 0xc4, 0);
    return 3;
}
