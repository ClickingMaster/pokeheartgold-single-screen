#ifndef POKEHEARTGOLD_FIELD_AREA_DATA_H
#define POKEHEARTGOLD_FIELD_AREA_DATA_H

#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"

#include "filesystem.h"

typedef struct BuildModelShapes BuildModelShapes;
typedef struct AreaDataManager AreaDataManager;

void AreaDataManager_TrimTextureFile(void *file, NNSG3dResTex *tex);
AreaDataManager *AreaDataManager_Alloc(u8 areaDataBank);
NARC *AreaDataManager_GetMapPropAnimListNarc(AreaDataManager *areaDataManager);
NNSG3dResFileHeader **AreaDataManager_GetMapPropModelFile(AreaDataManager *areaDataManager);
void AreaDataManager_Free(AreaDataManager **areaDataManager);
NNSG3dResTex *AreaDataManager_GetMapTexture(AreaDataManager *areaDataManager);
NNSG3dResTex *AreaDataManager_GetMapPropTexture(AreaDataManager *areaDataManager);
BuildModelShapes *AreaDataManager_GetBuildModelShapes(AreaDataManager *areaDataManager);
u32 AreaDataManager_GetAreaLightArchiveID(AreaDataManager *areaDataManager);
void AreaDataManager_Load(AreaDataManager *areaDataManager, Field3dRenderObjManager *renderObjManager, MapPropAnimationManager *mapPropAnimationManager, FieldSystemUnkSubCC *unkCC, FieldSystemUnkSub104 *unk104);

#endif // POKEHEARTGOLD_FIELD_AREA_DATA_H
