#pragma force_active on
#include "global.h"
#pragma force_active on
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define WORD(p,o) (*(u32 *)((u8 *)(p)+(o)))
typedef struct Template {s16 x,y,z,anim;int priority,palette,vram;int resources[6];int extra[2];} Template;
int sub_02074490(void *);int sub_0207449C(void);int sub_020744A8(void);
void SpriteSystem_LoadPaletteBuffer(void *,int,void *,void *,int,int,int,int,int,int);
void SpriteSystem_LoadCellResObj(void *,void *,int,int,int,int);void SpriteSystem_LoadAnimResObj(void *,void *,int,int,int,int);
int ov40_022371D4(u32,u32);u32 GetMonIconNaixEx(u32,int,u32);u32 GetMonIconPaletteEx(u32,u32,int);
void SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(void *,void *,int,int,int,int,int);
void *SpriteSystem_NewSprite(void *,void *,Template *);void ManagedSprite_SetPaletteOverrideOffset(void *,int);void ManagedSprite_SetAnim(void *,int);void ManagedSprite_SetDrawPriority(void *,int);
typedef struct Data {u8 pad0[0x2c];u16 species[30];u8 pad68[0x158-0x68];u32 mask;u8 forms[30];u8 pad17a[0x2b4-0x17a];void *sprites[30];} Data;
void ov40_02237284(void *);
void ov40_02237284(void *ctx) {
 Data *data;void *palette,*system,*manager;int i;Template t;
 data=PTR(ctx,0x860);palette=PTR(ctx,0x28);system=PTR(ctx,0x18);manager=PTR(ctx,0x1c);
 SpriteSystem_LoadPaletteBuffer(palette,2,system,manager,20,sub_02074490(ctx),0,3,1,100000);
 SpriteSystem_LoadCellResObj(system,manager,20,sub_0207449C(),0,100000);
 SpriteSystem_LoadAnimResObj(system,manager,20,sub_020744A8(),0,100000);
 for(i=0;i<30;i++) {
  u16 species=data->species[i];u32 form=data->forms[i];int egg=ov40_022371D4(data->mask,1<<i);
  if(species) {
   SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(system,manager,20,GetMonIconNaixEx(species,egg,form),0,1,100000+i);
   t.x=110+(i%6)*24;t.y=48+(i/6)*22;t.z=0;t.anim=0;t.priority=0;t.palette=0;t.vram=1;t.extra[0]=0;t.extra[1]=0;
   t.resources[0]=100000+i;t.resources[1]=100000;t.resources[2]=100000;t.resources[3]=100000;t.resources[4]=-1;t.resources[5]=-1;
   data->sprites[i]=SpriteSystem_NewSprite(system,manager,&t);
   ManagedSprite_SetPaletteOverrideOffset(data->sprites[i],GetMonIconPaletteEx(species,form,egg)+4);
   ManagedSprite_SetAnim(data->sprites[i],1);ManagedSprite_SetDrawPriority(data->sprites[i],30-i);
  }
 }
}
