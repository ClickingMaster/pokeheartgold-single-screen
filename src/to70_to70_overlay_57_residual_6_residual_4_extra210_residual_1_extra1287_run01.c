#pragma force_active on
#include "global.h"
typedef struct{int count;void*mons[16];} Input;
typedef struct{void*sprite;} Managed;
typedef struct{Input*input;u8 p[0xd8];void*system,*manager;u8 p2[0x240];Managed*sprites[16];} Work;
typedef struct{s16 x,y,z;u16 anim;int priority,pal,vram,ids[6],bgPriority,transfer;} Template;
int Pokemon_GetIconNaix(void*);
void SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(void*,void*,int,int,int,int,int);
Managed*SpriteSystem_NewSprite(void*,void*,Template*);
u32 GetMonData(void*,int,void*);
int GetMonIconPaletteEx(u32,u32,u32);
void Sprite_SetPalOffsetRespectVramOffset(void*,int);
void ManagedSprite_SetAnim(Managed*,int);
void ov57_02238E48(Work*);
void ov57_02238E48(Work*w){int i;for(i=0;i<w->input->count;i++){void*mon=w->input->mons[i];Template t;u32 species,form,egg;
SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(w->system,w->manager,20,Pokemon_GetIconNaix(mon),0,1,15000+i);
t.x=0;t.y=0;t.z=0;t.anim=0;t.priority=10;t.pal=0;t.vram=1;t.bgPriority=2;t.transfer=0;t.ids[0]=15000+i;t.ids[1]=16000;t.ids[2]=17000;t.ids[3]=18000;t.ids[4]=-1;t.ids[5]=-1;w->sprites[i]=SpriteSystem_NewSprite(w->system,w->manager,&t);
species=GetMonData(mon,5,0);form=GetMonData(mon,76,0);egg=GetMonData(mon,112,0);Sprite_SetPalOffsetRespectVramOffset(w->sprites[i]->sprite,GetMonIconPaletteEx(species,egg,form));ManagedSprite_SetAnim(w->sprites[i],1);}}
