#pragma force_active on
#include "global.h"
#include "sound_playback.h"
typedef struct Packet{u8 x,y;struct{u8 height:6,event:2;} motion;u8 unused;struct{u8 state:4,opponent:2,team:2;} status;struct{u8 low:3,power:2,hidden:2,other:1;} detail;}Packet;
typedef struct Input{Packet packet[4];u32 padding;union{struct{u32 low:7,pressed:1,rest:24;}bits;u32 raw;}flags;}Input;
typedef struct Participant{void*actor;void*effect;void*indicator;void*unused;void*launch;void*collision;int state;u8 buffers[0xc0];int scale;u8 pade0;s8 time;u16 pade2;struct{u32 count:8,pad8:6,team:2,opponent:2,pad18:1,launch:1,air:1,remote:1,pad22:1,event:2,other:2,rest:5;} flags;}Participant;
typedef struct Scene{void*ui;u8 pad4[12];Participant participant[4];u8 pad3b0[9];struct{u8 pressed:1,rest:7;}flags;}Scene;
typedef struct Heap{char pad[0x1c];void*actors;char pad20[0x160];void*remote;}Heap;
typedef struct Rank{u8 y,pad[3];void*actor;}Rank;
extern Heap*PokeathlonCourse_GetHeapAllocPtr4(void*);extern int PokeathlonCourse_GetParticipantCount(void*),ov96_021E5F24(void*);extern void*PokeathlonCourseMonObjects_Get(void*,u8),*ov96_0221935C(void*,u8,int);
extern void ov96_02219770(void*,BOOL,BOOL),ov96_02215884(void*,int,int),ManagedSprite_SetPositionXYWithSubscreenOffset(void*,s16,s16,int),ManagedSprite_ResetSpriteAnimCtrlState(void*),ManagedSprite_SetDrawFlag(void*,BOOL),PokeathlonCourseMonObject_SetAnimState(void*,int),ov96_021EAF60(void*,u8,int,void*),PlaySE_SetVolume(u16,int),ManagedSprite_SetAnimNoRestart(void*,u16),ManagedSprite_SetAnim(void*,u16),PokeathlonCourseMonObject_SetAnim(void*,int),PokeathlonCourseMonObject_SetDrawFlag(void*,int),ov96_02216AA4(Packet*,Participant*,void*,void*,u8),ov96_0221996C(void*,u8,int,int),ov96_022193CC(u16,BOOL),ov96_021EB01C(void*,int,int,int),ov96_021EB10C(void*,float,float),ov96_021EB06C(void*,int,int,int*,int*),ov96_02218510(void*,int),PokeathlonCourseMonObject_SetDrawPriority(void*,int);
extern int ManagedSprite_IsAnimated(void*),ov96_02219940(void*,u8,u8),ov96_02219794(void*,u8,u8),ov96_02216C00(const void*,const void*,void*);
extern void MATH_QSort(void*,u32,u32,int(*)(const void*,const void*,void*),void*);
void ov96_02216C38(Scene*,Input*,void*);
// @fn ov96_02216C38
void ov96_02216C38(Scene*scene,Input*input,void*course){int participant;Packet*packet;Participant*part;Rank ranks[4]={0};Heap*heap;void*actor;BOOL show;float sx;BOOL remote;s16 hx;s16 hy;int base;int t;int team;u8 actorIndex;float sy;heap=PokeathlonCourse_GetHeapAllocPtr4(course);
 if(input->flags.bits.pressed&&!scene->flags.pressed)PlaySE(2260);participant=0;base=0;scene->flags.pressed=input->flags.bits.pressed;packet=input->packet;part=scene->participant;
 for(;participant<4;participant++,packet++,part++,base+=3){BOOL excluded=PokeathlonCourse_GetParticipantCount(course)<=participant;BOOL localZero=ov96_021E5F24(course)==0;remote=FALSE;if(localZero&&excluded)remote=TRUE;
 if(!packet)GF_AssertFail();if(!part)GF_AssertFail();team=packet->status.team;part->flags.team=team;actorIndex=(u8)(packet->status.team+base);actor=PokeathlonCourseMonObjects_Get(heap->actors,actorIndex);part->actor=actor;if(!actor)GF_AssertFail();
 if(participant==ov96_021E5F24(course)){t=packet->status.team;u32 bits=input->flags.raw>>8;int first=(u8)((t+2)%3+base);BOOL a=((bits>>(first*2))&3)==1;int second=(u8)((t+1)%3+base);BOOL b=((bits>>(second*2))&3)==1;ov96_02219770(scene->ui,a,b);}
 if(part->flags.event&&packet->motion.event){int other=packet->status.opponent;show=0;if(other<=participant)other--;
 if(packet->motion.event==1){if(other==ov96_021E5F24(course))ov96_02215884((char*)heap+0x7f8,4,10);show=1;PlaySE(2256);}else if(packet->motion.event==2){if(other==ov96_021E5F24(course))ov96_02215884((char*)heap+0x7f8,10,15);show=1;PlaySE(2258);}
 if(show){int x,y,ox,oy;int dx,dy;s16 px,py;y=packet->y;oy=input->packet[other].y;ox=input->packet[other].x;x=packet->x;dx=x-ox;if(dx<0)dx=-dx;hx=dx/2;dy=y-oy;if(dy<0)dy=-dy;hy=dy/2;if(x>ox)x=ox;px=hx+x;if(y>oy)y=oy;py=hy+y;ManagedSprite_SetPositionXYWithSubscreenOffset(part->collision,px,py,0x1e0000);ManagedSprite_ResetSpriteAnimCtrlState(part->collision);ManagedSprite_SetDrawFlag(part->collision,1);}}
 part->flags.event=packet->motion.event;part->flags.opponent=packet->status.opponent;PokeathlonCourseMonObject_SetAnimState(actor,packet->detail.power+1);
 if(packet->status.state==4&&part->state!=4)ov96_021EAF60(heap->actors,actorIndex,1,part->buffers+32+team*64);else if(part->state==4&&packet->status.state!=4)ov96_021EAF60(heap->actors,actorIndex,1,part->buffers+team*64);
 if(packet->status.state==5&&part->state!=5){if(participant==ov96_021E5F24(course))PlaySE_SetPitch(2257,256);else PlaySE_SetVolume(2261,64);}else if(packet->status.state==12&&part->state!=12){if(*((u8*)ov96_0221935C(course,(u8)participant,team)+24))PlaySE(2259);}
 if(packet->status.state==6&&part->state!=6){ManagedSprite_SetAnimNoRestart(part->effect,(u16)(packet->detail.power+11));ManagedSprite_SetDrawFlag(part->effect,1);}else if((packet->status.state!=6&&part->state==6)||(packet->status.state==7&&part->state==5)){ManagedSprite_SetAnim(part->effect,(u16)(packet->detail.power+15));ManagedSprite_SetDrawFlag(part->effect,1);}
 if(ManagedSprite_IsAnimated(part->effect)==1)ManagedSprite_SetPositionXYWithSubscreenOffset(part->effect,packet->x,packet->y,0x1e0000);
 if(packet->status.state==6)PokeathlonCourseMonObject_SetAnim(actor,12);
 if(!packet->x||packet->x==255||!packet->y||packet->y==255){PokeathlonCourseMonObject_SetDrawFlag(actor,0);ManagedSprite_SetDrawFlag(part->effect,0);ManagedSprite_SetDrawFlag(part->indicator,0);}else{BOOL hidden=packet->detail.hidden!=0;BOOL state3=packet->status.state==3;u8 indicator=((input->flags.raw>>8)>>(actorIndex*2))&3;
 ManagedSprite_SetPositionXYWithSubscreenOffset(part->indicator,packet->x,(s16)(packet->y-packet->motion.height-24),0x1e0000);if(state3||hidden)indicator=0;else if(indicator==2){ManagedSprite_SetAnimNoRestart(part->indicator,1);if(!IsSEPlaying(2192))PlaySE(2192);}else if(indicator==1)ManagedSprite_SetAnimNoRestart(part->indicator,9);ManagedSprite_SetDrawFlag(part->indicator,indicator!=0);}
 ov96_02216AA4(packet,part,scene->ui,course,(u8)participant);
 {u8 isAir=packet->status.state==3;if(isAir==1&&!part->flags.air){part->flags.launch=1;part->time=6;part->scale=4096;PlaySE(2206);ov96_0221996C(scene->ui,(u8)participant,team,1);if(participant==ov96_021E5F24(course)||remote)part->flags.remote=1;}
 else if(!isAir&&part->flags.air==1){ManagedSprite_SetPositionXYWithSubscreenOffset(part->launch,packet->x,packet->y,0x1e0000);ManagedSprite_SetDrawFlag(part->launch,1);ManagedSprite_ResetSpriteAnimCtrlState(part->launch);ov96_022193CC(2230,participant==ov96_021E5F24(course));ov96_0221996C(scene->ui,(u8)participant,team,0);PokeathlonCourseMonObject_SetDrawFlag(actor,1);part->flags.launch=0;}part->flags.air=isAir;}
 ov96_021EB01C(actor,packet->x,packet->y,1);ov96_021EB01C(actor,packet->x,packet->y-packet->motion.height,0);
 if(part->flags.launch){if(part->flags.remote){if(remote){if(ov96_02219940(heap->remote,(u8)participant,(u8)part->flags.team))part->flags.remote=0;}else{if(ov96_02219794(heap->remote,(u8)participant,(u8)part->flags.team))part->flags.remote=0;}}
 if(--part->time<=0){PokeathlonCourseMonObject_SetDrawFlag(actor,0);}else{int x=packet->x,y=packet->y;float scale;part->scale=(s32)((u32)part->scale-410);scale=(float)part->scale/4096.0f;ov96_021EB10C(actor,scale,scale);ov96_021EB06C(actor,x,y,&x,&y);if(y>=160){int ny=6-part->time;ny=packet->y+ny*4;ov96_021EB01C(actor,packet->x,ny,1);}}}
 else{sx=1.2f-(float)((160-packet->y)*12)/4096.0f;sy=sx;if(packet->status.state==5){u32 count=part->flags.count;sx=(float)((double)sx*(0.8-(double)count*0.1));if(count<4)part->flags.count=count+1;}
 if(packet->status.state==8){u8*detail=ov96_0221935C(course,(u8)participant,team);u32 count;if(!part->flags.count){if(!detail[24]){if(participant==ov96_021E5F24(course))PlaySE(1523);}else PlaySE(1546);}count=part->flags.count;sy=(float)((double)sy*(0.8-(double)count*0.1));if(count<5)part->flags.count=count+1;}
 if(packet->status.state!=8&&packet->status.state!=5){if(part->flags.count){ov96_02218510(actor,1);part->flags.count=0;}}else ov96_02218510(actor,0);ov96_021EB10C(actor,sx,sy);}
 part->state=packet->status.state;
 }
 {int i,base2;Packet*p;Rank*r;for(i=0,base2=0,p=input->packet,r=ranks;i<4;i++,p++,base2+=3,r++){void*actor=PokeathlonCourseMonObjects_Get(heap->actors,(u8)(p->status.team+base2));int x=p->x,y=p->y;ov96_021EB06C(actor,x,y,&x,&y);r->y=y;r->actor=actor;}MATH_QSort(ranks,4,sizeof(Rank),ov96_02216C00,0);for(i=0;i<4;i++)PokeathlonCourseMonObject_SetDrawPriority(ranks[i].actor,i+7);}
}
