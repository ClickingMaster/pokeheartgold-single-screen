#ifndef POKEHEARTGOLD_FIELD_MODEL_ATTRIBUTES_H
#define POKEHEARTGOLD_FIELD_MODEL_ATTRIBUTES_H

// GX_LIGHTS_COUNT == 4

#include "global.h"

// Which attributes ModelAttributes_ApplyGlobal / ModelAttributes_ApplyToModel set.
#define MODEL_ATTR_LIGHT_VECTOR(light) (1 << (light))
#define MODEL_ATTR_LIGHT_COLOR(light)  (1 << ((light) + 4))
#define MODEL_ATTR_DIFFUSE             (1 << 8)
#define MODEL_ATTR_AMBIENT             (1 << 9)
#define MODEL_ATTR_SPECULAR            (1 << 10)
#define MODEL_ATTR_EMISSION            (1 << 11)
#define MODEL_ATTR_LIGHT_MASK          (1 << 12)
#define MODEL_ATTR_POLYGON_MODE        (1 << 13)
#define MODEL_ATTR_CULL_MODE           (1 << 14)
#define MODEL_ATTR_POLYGON_ID          (1 << 15)
#define MODEL_ATTR_ALPHA               (1 << 16)
#define MODEL_ATTR_FOG                 (1 << 17)
#define MODEL_ATTR_DEPTH_TEST          (1 << 18)
#define MODEL_ATTR_1DOT                (1 << 19)
#define MODEL_ATTR_FAR_CLIP            (1 << 20)
#define MODEL_ATTR_XL_DEPTH_UPDATE     (1 << 21)

typedef struct ModelAttributes {
    VecFx16 lightVectors[4];
    GXRgb lightColors[4];
    GXRgb diffuse;
    GXRgb ambient;
    GXRgb specular;
    GXRgb emission;
    BOOL setDiffuseColorAsVertexColor;
    BOOL enableSpecularReflectShininessTable;
    int lightMask;
    GXPolygonMode polygonMode;
    GXCull cullMode;
    int id;
    int alpha;
    int miscFlags;
} ModelAttributes;

ModelAttributes *ModelAttributes_Init();
void ModelAttributes_Free(ModelAttributes **modelAttributes);
void ModelAttributes_ApplyGlobal(ModelAttributes *modelAttributes, int attributes);
void ModelAttributes_ApplyToModel(ModelAttributes *modelAttributes, NNSG3dResMdl *model, u32 attributes);
void ModelAttributes_SetLightVector(ModelAttributes *modelAttributes, int light, s16 x, s16 y, s16 z);
void ModelAttributes_SetLightColor(ModelAttributes *modelAttributes, int light, GXRgb color);
void ModelAttributes_SetDiffuse(ModelAttributes *modelAttributes, GXRgb diffuse, BOOL setAsVertexColor, BOOL applyGlobal);
void ModelAttributes_SetAmbient(ModelAttributes *modelAttributes, GXRgb ambient, BOOL applyGlobal);
void ModelAttributes_SetSpecular(ModelAttributes *modelAttributes, GXRgb specular, BOOL enableShininessTable, BOOL applyGlobal);
void ModelAttributes_SetEmission(ModelAttributes *modelAttributes, GXRgb emission, BOOL applyGlobal);
void ModelAttributes_SetPolygonMode(ModelAttributes *modelAttributes, GXPolygonMode polygonMode, BOOL applyGlobal);
void ModelAttributes_SetCullMode(ModelAttributes *modelAttributes, GXCull cullMode, BOOL applyGlobal);
void ModelAttributes_SetAlpha(ModelAttributes *modelAttributes, int alpha, BOOL applyGlobal);
void ModelAttributes_SetMiscAttrEnabled(ModelAttributes *modelAttributes, int miscFlagsMask, BOOL enable, BOOL applyGlobal);

#endif // POKEHEARTGOLD_FIELD_MODEL_ATTRIBUTES_H
