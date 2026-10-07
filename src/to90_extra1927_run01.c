#pragma force_active on
#include "global.h"
typedef struct {s16 x,y,z;u16 animation;int priority,pal,vram,res[6],bg,transfer;} Template;
extern void* SpriteSystem_NewSprite(void*,void*,const Template*);extern void ManagedSprite_TickFrame(void*),ManagedSprite_SetAnim(void*,int),ManagedSprite_SetPaletteOverrideOffset(void*,int),ManagedSprite_SetAffineOverwriteMode(void*,int);
typedef struct {u8 p[0x18];void*system,*manager;} Work;
extern int GetMonIconNaixEx(int,int,int),GetMonIconPaletteEx(int,int,int);extern void SpriteSystem_LoadCharResObjFromOpenNarcWithHardwareMappingType(void*,void*,void*,int,int,int,int);
void* ov40_0222FEA0(Work*,void*,int,int,int,int);
#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_dead_assignments off
#pragma opt_propagation off
#pragma opt_common_subs off
void* ov40_0222FEA0(Work*w,void*narc,int id,int species,int form,int egg){int e,f;void*system;void*manager;system=w->system;Template t;void*s;manager=w->manager;if(species==0)return 0;f=form;e=egg;SpriteSystem_LoadCharResObjFromOpenNarcWithHardwareMappingType(system,manager,narc,GetMonIconNaixEx(species,e,f),0,1,id+100000);t.x=0;t.y=-48;t.z=0;t.animation=0;t.priority=0;t.pal=0;t.vram=1;t.bg=0;t.transfer=0;t.res[0]=id+100000;t.res[1]=100000;t.res[2]=100000;t.res[3]=100000;t.res[4]=-1;t.res[5]=-1;s=SpriteSystem_NewSprite(system,manager,&t);ManagedSprite_SetPaletteOverrideOffset(s,GetMonIconPaletteEx(species,f,e)+4);ManagedSprite_SetAnim(s,1);ManagedSprite_TickFrame(s);return s;}
