#include "overlay71_polygon_attr_r9_private.h"
void ov71_02247708(Ov71CamR9 *cam, u32 value) {
    cam->polygonId = value;
    if (value != 0x1F) {
        NNSi_G3dModifyPolygonAttrMask(cam->model, FALSE, 0x1F << 16);
    } else {
        NNSi_G3dModifyPolygonAttrMask(cam->model, TRUE, 0x1F << 16);
    }
}
