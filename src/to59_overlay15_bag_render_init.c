#include <nnsys/g3d.h>

#include "global.h"

#include "camera.h"
#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "system.h"

typedef struct Triple15 {
    u32 w[3];
} Triple15;
typedef struct Five15 {
    u32 w[5];
} Five15;
extern const Triple15 ov15_02200500;
extern const Triple15 ov15_0220050C;
extern const Five15 ov15_0220053C;
extern const u16 ov15_02201304[];
extern void SetBgPriority(int, int);
extern void ov15_021FDAD0(void *);
extern void ov15_021FDAF4(void *, int, int);
extern void ov15_021FDD70(void *);
void ov15_021FD93C(void *);

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov15_021FD93C(void *work) {
    u32 i;
    GF3dRender_InitSimpleManager(HEAP_ID_6);
    *(vu16 *)0x04000060 = 0x10 | (*(vu16 *)0x04000060 & 0xffffcfff);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0, 0, 0x7fff, 0, FALSE);
    NNS_G3dGlbMaterialColorDiffAmb(0x3def, 0x294a, FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(0x3def, 0x3def, FALSE);
    NNS_G3dGlbPolygonAttr(15, GX_POLYGONMODE_MODULATE, (GXCull)3, 0, 31, 0);
    *(vu16 *)0x04000060 = 0x20 | (*(vu16 *)0x04000060 & 0xffffcfff);
    G3X_SetEdgeColorTable(ov15_02201304);
    SetBgPriority(0, 0);
    GfGfx_EngineATogglePlanes(1, 1);
    PTR(work, 0x818) = Camera_New(HEAP_ID_6);
    *(Triple15 *)((u8 *)work + 0x904) = ov15_02200500;
    *(Five15 *)((u8 *)work + 0x910) = ov15_0220053C;
    Camera_Init_FromTargetDistanceAndAngle((VecFx32 *)((u8 *)work + 0x904), U32(work, 0x910), (CameraAngle *)((u8 *)work + 0x914), U16(work, 0x91e), U8(work, 0x91c), TRUE, PTR(work, 0x818));
    *(Triple15 *)((u8 *)work + 0x934) = ov15_0220050C;
    ov15_021FDAD0((u8 *)work + 0x808);
    ov15_021FDAF4((u8 *)work + 0x808, U8(PTR(work, 0x234), 0x64) + 1, 7);
    Camera_SetPerspectiveClippingPlane(0x7b000, 0x6a4000, PTR(work, 0x818));
    Camera_SetStaticPtr(PTR(work, 0x818));
    for (i = 0; i < 4; i++) {
        NNS_G3dGlbLightVector((GXLightId)i, 0x1000, 0, 0);
        NNS_G3dGlbLightColor((GXLightId)i, 0x7fff);
    }
    ov15_021FDD70(work);
    GfGfx_EngineATogglePlanes(1, 1);
    *(vu16 *)0x04000008 = 2 | (*(vu16 *)0x04000008 & ~3);
}
