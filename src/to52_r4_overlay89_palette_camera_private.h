#ifndef W17_R4_PRIVATE_H
#define W17_R4_PRIVATE_H
#include "global.h"
#include "bg_window.h"
#include "camera.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "geometry_math.h"
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
extern const u16 _0225C9B0[4];
extern const CameraAngle ov89_0225C9B8;
extern const VecFx32 ov89_0225C9C0;
extern u32 ov89_0225C88C(u32, int, u32);
extern void ov89_0225A1D8(void *, int);
extern void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
BOOL ov89_02259A3C(int, int, ManagedSprite *, NARC *, NARC *, int, void *);
void ov89_02259B00(u8 *);

#endif
