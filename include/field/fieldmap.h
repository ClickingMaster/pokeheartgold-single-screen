#ifndef POKEHEARTGOLD_FIELD_FIELDMAP_H
#define POKEHEARTGOLD_FIELD_FIELDMAP_H

#include "bg_window.h"
#include "field_system.h"

void Thunk_BgConfig_Init(BgConfig *bgConfig);
void FieldMap_ClearBgLayer3(BgConfig *bgConfig);
void FieldSystem_SetFieldMapRenderEnabled(FieldSystem *fieldSystem, BOOL setFlag);
void FieldSystem_SetFieldMapTextureFreeEnabled(FieldSystem *fieldSystem, BOOL setFlag);
void FieldMap_FadeScreen(const u8 fadeType);

#endif // POKEHEARTGOLD_FIELD_FIELDMAP_H
