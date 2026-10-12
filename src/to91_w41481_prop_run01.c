#pragma force_active on
#include "global.h"
#include "overlay_01.h"
#include "field_system.h"
#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "heap.h"
#pragma force_active on
void sub_02054A60(int,int,int,int,int,int,fx32*);
int *sub_02054D10(FieldSystem*,u32,int,fx32*,u32);
BOOL sub_02054AE4(FieldSystem*,int,fx32*,MapProp**);
NNSG3dResTex *AreaDataManager_GetMapPropTexture(AreaDataManager*);
void ov01_021E9AE8(FieldSystem *field,int x,int z,u8 token) {
 fx32 rectangle[4];
 struct {MapProp *prop;u8 metadata[24];} selected;
 NARC *archive;
 int found;
 int id;
 int *models;
 int i;
 int *cursor;
 sub_02054A60(x,z,-1,0,3,1,rectangle);
 archive=AreaDataManager_GetMapPropAnimListNarc(field->areaDataManager);
 models=sub_02054D10(field,HEAP_ID_FIELD1,4,rectangle,0);
 found=0;
 for(i=0,cursor=models;i<4;i++,cursor++) {
  if(*cursor) {
   NARC_ReadWholeMember(archive,*cursor,selected.metadata);
   if(selected.metadata[4]) {
    sub_02054AE4(field,models[i],rectangle,&selected.prop);
    found=1;
    id=models[i];
    break;
   }
  }
 }
 Heap_Free(models);
 if(found) {
  int count=MapPropAnimationManager_GetPropAnimationCount(field->mapPropAnimationManager,id);
  if(count) {
   NNSG3dResTex *texture=AreaDataManager_GetMapPropTexture(field->areaDataManager);
   NNSG3dRenderObj *render=MapProp_GetRenderSurface(selected.prop);
   NNSG3dResMdl *model=MapProp_GetResModel(selected.prop);
   MapPropOneShotAnimationManager_LoadPropAnimations(field->mapPropAnimationManager,field->mapPropOneShotAnimationManager,token,id,render,model,texture,count,1,FALSE);
  }
 } else GF_AssertFail();
}
