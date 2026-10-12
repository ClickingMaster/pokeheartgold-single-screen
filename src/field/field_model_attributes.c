#include "field/model_attributes.h"

#include "global.h"

#include <nnsys/g3d_util_internal.h>

#include "heap.h"

ModelAttributes *ModelAttributes_Init(void) {
    ModelAttributes *modelAttributes = Heap_Alloc(HEAP_ID_FIELD1, sizeof(ModelAttributes));
    MI_CpuClear32(modelAttributes, sizeof(ModelAttributes));
    return modelAttributes;
}

void ModelAttributes_Free(ModelAttributes **modelAttributes) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, *modelAttributes);
    *modelAttributes = NULL;
}

void ModelAttributes_ApplyGlobal(ModelAttributes *modelAttributes, int attributes) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (attributes & MODEL_ATTR_LIGHT_VECTOR(i)) {
            NNS_G3dGlbLightVector((GXLightId)i, modelAttributes->lightVectors[i].x, modelAttributes->lightVectors[i].y, modelAttributes->lightVectors[i].z);
        }
        if (attributes & MODEL_ATTR_LIGHT_COLOR(i)) {
            NNS_G3dGlbLightColor((GXLightId)i, modelAttributes->lightColors[i]);
        }
    }
    if (attributes & (MODEL_ATTR_DIFFUSE | MODEL_ATTR_AMBIENT)) {
        NNS_G3dGlbMaterialColorDiffAmb(modelAttributes->diffuse, modelAttributes->ambient, modelAttributes->setDiffuseColorAsVertexColor);
    }
    if (attributes & (MODEL_ATTR_SPECULAR | MODEL_ATTR_EMISSION)) {
        NNS_G3dGlbMaterialColorSpecEmi(modelAttributes->specular, modelAttributes->emission, modelAttributes->enableSpecularReflectShininessTable);
    }
    if (attributes >= MODEL_ATTR_LIGHT_MASK) {
        NNS_G3dGlbPolygonAttr(modelAttributes->lightMask, modelAttributes->polygonMode, modelAttributes->cullMode, modelAttributes->id, modelAttributes->alpha, modelAttributes->miscFlags);
    }
}

void ModelAttributes_ApplyToModel(ModelAttributes *modelAttributes, NNSG3dResMdl *model, u32 attributes) {
    if (attributes & MODEL_ATTR_DIFFUSE) {
        NNS_G3dMdlSetMdlDiffAll(model, modelAttributes->diffuse);
    }
    if (attributes & MODEL_ATTR_AMBIENT) {
        NNS_G3dMdlSetMdlAmbAll(model, modelAttributes->ambient);
    }
    if (attributes & MODEL_ATTR_SPECULAR) {
        NNS_G3dMdlSetMdlSpecAll(model, modelAttributes->specular);
    }
    if (attributes & MODEL_ATTR_EMISSION) {
        NNS_G3dMdlSetMdlEmiAll(model, modelAttributes->emission);
    }
    if (attributes & MODEL_ATTR_LIGHT_MASK) {
        NNS_G3dMdlSetMdlLightEnableFlagAll(model, modelAttributes->lightMask);
    }
    if (attributes & MODEL_ATTR_POLYGON_MODE) {
        NNS_G3dMdlSetMdlPolygonModeAll(model, modelAttributes->polygonMode);
    }
    if (attributes & MODEL_ATTR_CULL_MODE) {
        NNS_G3dMdlSetMdlCullModeAll(model, modelAttributes->cullMode);
    }
    if (attributes & MODEL_ATTR_POLYGON_ID) {
        NNS_G3dMdlSetMdlPolygonIDAll(model, modelAttributes->id);
    }
    if (attributes & MODEL_ATTR_ALPHA) {
        NNS_G3dMdlSetMdlAlphaAll(model, modelAttributes->alpha);
    }
    if (attributes & MODEL_ATTR_FOG) {
        NNS_G3dMdlSetMdlFogEnableFlagAll(model, modelAttributes->miscFlags & 0x8000);
    }
    if (attributes & MODEL_ATTR_DEPTH_TEST) {
        NNS_G3dMdlSetMdlDepthTestCondAll(model, modelAttributes->miscFlags & 0x4000);
    }
    if (attributes & MODEL_ATTR_1DOT) {
        NNS_G3dMdlSetMdl1DotAll(model, modelAttributes->miscFlags & 0x2000);
    }
    if (attributes & MODEL_ATTR_FAR_CLIP) {
        NNS_G3dMdlSetMdlFarClipAll(model, modelAttributes->miscFlags & 0x1000);
    }
    if (attributes & MODEL_ATTR_XL_DEPTH_UPDATE) {
        NNS_G3dMdlSetMdlXLDepthUpdateAll(model, modelAttributes->miscFlags & 0x800);
    }
}

void ModelAttributes_SetLightVector(ModelAttributes *modelAttributes, int light, s16 x, s16 y, s16 z) {
    modelAttributes->lightVectors[light].x = x;
    modelAttributes->lightVectors[light].y = y;
    modelAttributes->lightVectors[light].z = z;
    NNS_G3dGlbLightVector((GXLightId)light, modelAttributes->lightVectors[light].x, modelAttributes->lightVectors[light].y, modelAttributes->lightVectors[light].z);
}

void ModelAttributes_SetLightColor(ModelAttributes *modelAttributes, int light, GXRgb color) {
    modelAttributes->lightColors[light] = color;
    NNS_G3dGlbLightColor((GXLightId)light, modelAttributes->lightColors[light]);
}

void ModelAttributes_SetDiffuse(ModelAttributes *modelAttributes, GXRgb diffuse, BOOL setAsVertexColor, BOOL applyGlobal) {
    modelAttributes->diffuse = diffuse;
    modelAttributes->setDiffuseColorAsVertexColor = setAsVertexColor;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_DIFFUSE);
    }
}

void ModelAttributes_SetAmbient(ModelAttributes *modelAttributes, GXRgb ambient, BOOL applyGlobal) {
    modelAttributes->ambient = ambient;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_AMBIENT);
    }
}

void ModelAttributes_SetSpecular(ModelAttributes *modelAttributes, GXRgb specular, BOOL enableShininessTable, BOOL applyGlobal) {
    modelAttributes->specular = specular;
    modelAttributes->enableSpecularReflectShininessTable = enableShininessTable;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_SPECULAR);
    }
}

void ModelAttributes_SetEmission(ModelAttributes *modelAttributes, GXRgb emission, BOOL applyGlobal) {
    modelAttributes->emission = emission;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_EMISSION);
    }
}

void ModelAttributes_SetPolygonMode(ModelAttributes *modelAttributes, GXPolygonMode polygonMode, BOOL applyGlobal) {
    modelAttributes->polygonMode = polygonMode;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_POLYGON_MODE);
    }
}

void ModelAttributes_SetCullMode(ModelAttributes *modelAttributes, GXCull cullMode, BOOL applyGlobal) {
    modelAttributes->cullMode = cullMode;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_CULL_MODE);
    }
}

void ModelAttributes_SetAlpha(ModelAttributes *modelAttributes, int alpha, BOOL applyGlobal) {
    modelAttributes->alpha = alpha;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_ALPHA);
    }
}

// Sets or clears miscFlagsMask in the polygon attributes.
void ModelAttributes_SetMiscAttrEnabled(ModelAttributes *modelAttributes, int miscFlagsMask, BOOL enable, BOOL applyGlobal) {
    if (enable == TRUE) {
        if (modelAttributes->miscFlags & miscFlagsMask) {
            return;
        }
    } else {
        if (!(modelAttributes->miscFlags & miscFlagsMask)) {
            return;
        }
    }
    modelAttributes->miscFlags ^= miscFlagsMask;
    if (applyGlobal == TRUE) {
        ModelAttributes_ApplyGlobal(modelAttributes, MODEL_ATTR_LIGHT_MASK);
    }
}
