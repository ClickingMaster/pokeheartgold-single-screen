#include "global.h"
#include "overlay89_r11_residual_04_private.h"

void ov89_0225B784(void *unused, void *work)
{
    int i;
    u8 *base = (u8 *)work + 0x94;
    u8 *object;
    u8 alpha = U8_AT(base, 0x16C);

    if (alpha != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, (GXPolygonMode)0, (GXCull)0, alpha, 8, 0);
        NNSi_G3dModifyPolygonAttrMask((NNSG3dResMdl *)PTR_AT(work, 0x14), 0, 0x3F000000);
        NNSi_G3dModifyPolygonAttrMask((NNSG3dResMdl *)PTR_AT(work, 0x14), 0, 0x001F0000);
    }
    for (i = 0, object = base; i < 3; i++, object += 0x78) {
        sub_020181EC(object);
    }
    alpha = U8_AT(base, 0x16C);
    if (alpha != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, (GXPolygonMode)0, (GXCull)0, alpha, 0x1F, 0);
        NNSi_G3dModifyPolygonAttrMask((NNSG3dResMdl *)PTR_AT(work, 0x14), 0, 0x3F000000);
        NNSi_G3dModifyPolygonAttrMask((NNSG3dResMdl *)PTR_AT(work, 0x14), 0, 0x001F0000);
    }
    sub_020181EC((u8 *)work + 0x1C);
}
