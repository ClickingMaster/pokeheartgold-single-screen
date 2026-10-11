#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"
void *PokeathlonUtil_GetSpriteSystem(void*);void *PokeathlonUtil_GetSpriteManager(void*);void SpriteSystem_LoadCharResObjFromOpenNarc(void*,void*,void*,int,int,int,int);void SpriteSystem_LoadPlttResObjFromOpenNarc(void*,void*,void*,int,int,int,int,int);void SpriteSystem_LoadCellResObjFromOpenNarc(void*,void*,void*,int,int,int);void SpriteSystem_LoadAnimResObjFromOpenNarc(void*,void*,void*,int,int,int);void PokeathlonUtil_LoadMonIconResources(void*,void*,int,int,int);void ov99_021E7F64(void*);void *PokeathlonUtil_NewSprite(void*,const void*);void ManagedSprite_SetAnimateFlag(void*,BOOL);
extern u8 ov99_021E9F90[];extern u8 ov99_021EA180[];
void ov99_021E7F90(u8*);void ov99_021E80D0(u8*);
#endif
