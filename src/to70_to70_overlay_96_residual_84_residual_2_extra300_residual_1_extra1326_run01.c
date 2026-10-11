#pragma force_active on
#include "global.h"
typedef struct {u16 species,form;u8 pad[3];u8 gender;u32 unused,pid;} Mon;
extern void PokeathlonCourseSpritePool_CreateSprite(void*,int,int,int,int),PokeathlonCourseSpritePoolSprite_SetPosition(void*,void*),PokeathlonCourseSpritePoolSprite_SetAnim(void*,int),PokeathlonCourseSpritePoolSprite_Show(void*,int,int),PokeathlonCourseSpritePoolSprite_SetDrawPriority(void*,int),ov96_02203DCC(void*,Mon*),ov96_02203F50(void*,void*),ov96_02203F0C(void*);
extern void *PokeathlonCourseSpritePool_Take(void*,int,int);
extern u8 GetMonPicHeightBySpeciesGenderForm(u16,u8,u8,u8,u32);
void ov96_02203E30(void*,u8*,Mon*);
void ov96_02203E30(void *ctx,u8 *w,Mon *mon){struct {int x,y,z;} pos;int i;volatile int x;u8 *cur;Mon *m;
for(i=0;i<3;i++)PokeathlonCourseSpritePool_CreateSprite(ctx,3,2,101,2);
m=mon;cur=w;i=0;x=16;for(;i<3;i++){*(void**)(cur+84)=PokeathlonCourseSpritePool_Take(ctx,101,2);pos.z=0;pos.x=x<<12;pos.y=(GetMonPicHeightBySpeciesGenderForm(m->species,m->gender,0,m->form,m->pid)+376)<<12;PokeathlonCourseSpritePoolSprite_SetPosition(*(void**)(cur+84),&pos);PokeathlonCourseSpritePoolSprite_SetAnim(*(void**)(cur+84),i+1);PokeathlonCourseSpritePoolSprite_Show(*(void**)(cur+84),1,1);PokeathlonCourseSpritePoolSprite_SetDrawPriority(*(void**)(cur+84),19-i);*(void**)(cur+96)=PokeathlonCourseSpritePool_Take(ctx,101,5);PokeathlonCourseSpritePoolSprite_SetAnim(*(void**)(cur+96),0);pos.y-=65536;PokeathlonCourseSpritePoolSprite_SetPosition(*(void**)(cur+96),&pos);PokeathlonCourseSpritePoolSprite_SetDrawPriority(*(void**)(cur+96),4);{int next=x+50;m++;cur+=4;x=next;}}
ov96_02203DCC(w,mon);ov96_02203F50(ctx,w);ov96_02203F0C(w);}
