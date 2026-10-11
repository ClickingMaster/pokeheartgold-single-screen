#pragma force_active on
#include "global.h"
#pragma force_active on
void SpriteManager_UnloadCharObjById(void *,int),SpriteManager_UnloadPlttObjById(void *,int),SpriteManager_UnloadCellObjById(void *,int),SpriteManager_UnloadAnimObjById(void *,int),Sprite_DeleteAndFreeResources(void *);
void ov40_0222E4D4(void **,u8 *);
typedef struct Tpl{s16 x,y,z;u16 animation;int draw,pal;NNS_G2D_VRAM_TYPE vram;int res[6],bg,transfer;}Tpl;
int sub_02074490(void),sub_0207449C(void),sub_020744A8(void),FrontierCard_GetSpecies(void *),sub_020315F0(void *),FrontierCard_IsEgg(void *),GetMonIconNaixEx(int,int,int),GetMonIconPaletteEx(int,int,int);
int SpriteSystem_LoadPaletteBuffer(void *,int,void *,void *,int,int,int,int,int,int);
void SpriteSystem_LoadCellResObj(void *,void *,int,int,int,int),SpriteSystem_LoadAnimResObj(void *,void *,int,int,int,int),SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(void *,void *,int,int,int,int,int);
void *SpriteSystem_NewSprite(void *,void *,const Tpl *);void ManagedSprite_SetPaletteOverrideOffset(void *,int),ManagedSprite_SetAnim(void *,int);void ov40_0222E510(void **,u8 *);
void ov40_0222E4D4(void **w,u8 *o){SpriteManager_UnloadCharObjById(*(void **)(o+28),100000);SpriteManager_UnloadPlttObjById(*(void **)(o+28),100000);SpriteManager_UnloadCellObjById(*(void **)(o+28),100000);SpriteManager_UnloadAnimObjById(*(void **)(o+28),100000);if(w[1]){Sprite_DeleteAndFreeResources(w[1]);w[1]=0;}}
void ov40_0222E510(void **w,u8 *o){void *system=*(void **)(o+24),*manager=*(void **)(o+28),*palette=*(void **)(o+40);int slot,species,gender,form;Tpl t;
 slot=SpriteSystem_LoadPaletteBuffer(palette,2,system,manager,20,sub_02074490(),0,3,1,100000);
 SpriteSystem_LoadCellResObj(system,manager,20,sub_0207449C(),0,100000);SpriteSystem_LoadAnimResObj(system,manager,20,sub_020744A8(),0,100000);
 species=FrontierCard_GetSpecies(w[0]);gender=sub_020315F0(w[0]);form=FrontierCard_IsEgg(w[0]);if(species){
 SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(system,manager,20,GetMonIconNaixEx(species,form,gender),0,1,100000);
 t.x=76;t.y=60;t.z=0;t.animation=0;t.vram=NNS_G2D_VRAM_TYPE_2DMAIN;t.res[0]=100000;t.res[1]=100000;t.res[2]=100000;t.res[3]=100000;t.res[4]=-1;t.res[5]=-1;t.draw=0;t.pal=0;t.bg=0;t.transfer=0;
 w[1]=SpriteSystem_NewSprite(*(void **)(o+24),*(void **)(o+28),&t);ManagedSprite_SetPaletteOverrideOffset(w[1],slot+GetMonIconPaletteEx(species,gender,form));ManagedSprite_SetAnim(w[1],1);
 }}
