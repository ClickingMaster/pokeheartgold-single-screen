#ifndef POKEHEARTGOLD_FIELD_AREA_LIGHT_H
#define POKEHEARTGOLD_FIELD_AREA_LIGHT_H

#include "field/model_attributes.h"

typedef struct AreaLightManager AreaLightManager;
typedef struct AreaLightEntry AreaLightEntry;

AreaLightManager *AreaLightManager_New(ModelAttributes *areaModelAttributes, const u32 archiveID);
void AreaLightManager_Free(AreaLightManager **areaLightManager);
void AreaLightManager_UpdateActiveTemplate(AreaLightManager *areaLightManager);
void AreaLight_ApplyEntry(AreaLightEntry *entry, ModelAttributes *modelAttributes);
void AreaLightManager_ApplyActiveEntry(AreaLightManager *areaLightManager);
void ov01_021EA3B0(NNSG3dResMdl *model);
int AreaLight_LoadEntries(const char *path, AreaLightEntry **entriesOut);
void AreaLight_FreeEntries(AreaLightEntry **entries);
const s8 *AreaLight_ParseLight(const s8 *src, u16 *colorOut, s16 *dirOut);
const s8 *AreaLight_ParseColor(const s8 *src, u16 *dest);
void LoadAreaOrDungeonLightTxt(u32 archiveID, ModelAttributes *modelAttributes);

#endif // POKEHEARTGOLD_FIELD_AREA_LIGHT_H
