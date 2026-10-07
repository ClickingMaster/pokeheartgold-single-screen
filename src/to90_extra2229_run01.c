#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 data[16];} Window;
typedef struct{u8 data[0x20c];} Participant;
typedef struct{int first,second;void*args[28];} Group;
typedef struct{u32 first;void*pointer;Group groups[5];void*task;void*context;} Work;
typedef struct{u8 pad[0x14];void*reference;} Owner;
typedef struct{u8 pad[0x14];Owner*owner;u8 pad2[0xd0-0x18];void*sprites[4];u8 pad3[0x108-0xe0];u32 a,b;u8 pad4[0x944-0x110];Window window;u8 pad5[0xb50-0x954];Participant participants[8];u8 pad6[0x2228-0x1bb0];Work work;u8 pad7[0x2ae8-0x2490];u32 payload;} State;
typedef struct{s16 y,x;} Position;
extern void ManagedSprite_GetPositionXY(void*,s16*,s16*),ManagedSprite_SetPositionXY(void*,int,int),ov92_02261448(void*,void*);
extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov92_0226156C(State*);
void ov92_0226156C(State*s){int i,j;Position a,b;Group*g;s->work.context=s->owner;s->work.pointer=&s->payload;s->work.first=*(u32*)s->work.pointer;for(i=0,g=s->work.groups;i<5;i++,g++){g->first=0;g->second=0;switch(i){case 0:g->args[0]=&s->window;g->args[1]=&s->a;g->args[3]=&s->b;g->args[2]=s->owner->reference;break;case 1:g->args[0]=s->sprites[0];g->args[1]=s->sprites[1];g->args[2]=0;g->args[3]=0;ManagedSprite_GetPositionXY(s->sprites[0],&a.x,&a.y);ManagedSprite_SetPositionXY(s->sprites[0],a.x,224);ManagedSprite_GetPositionXY(s->sprites[1],&a.x,&a.y);ManagedSprite_SetPositionXY(s->sprites[1],a.x,224);break;case 2:for(j=0;j<8;j++)g->args[j]=&s->participants[j];break;case 3:g->args[0]=0;g->args[1]=0;g->args[2]=s->sprites[2];g->args[3]=s->sprites[3];ManagedSprite_GetPositionXY(s->sprites[2],&b.x,&b.y);ManagedSprite_SetPositionXY(s->sprites[2],b.x,224);ManagedSprite_GetPositionXY(s->sprites[3],&b.x,&b.y);ManagedSprite_SetPositionXY(s->sprites[3],b.x,224);break;case 4:g->args[0]=s;break;}}s->work.task=SysTask_CreateOnMainQueue(ov92_02261448,&s->work,4096);}
