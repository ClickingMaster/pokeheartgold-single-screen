#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x94];void*obj[2];u8 q[0x1a0-0x9c];void*list;u8 r[0x2cc-0x1a4];void*man[4];u8 s[0x2fc-0x2dc];u8 headA[36],headB[36];void*main[2];u8 t[0x37c-0x34c];void*other[12];void*text[12];void*text2[12];void*bottom[6];void*single;} Work;
typedef struct {void*list;void*header;int x,y,z,sx,sy,sz;u16 rot;u16 pad;int priority,vram,heap;} Template;
typedef struct {int x,y;} Pos;
typedef struct {int x,y,seq;} Bottom;
extern const Pos ov65_0221FF4C[];extern const Bottom ov65_0221FEA4[];
extern void CreateSpriteResourcesHeader(void*,int,int,int,int,int,int,int,int,void*,void*,void*,void*,void*,void*);
extern void*Sprite_CreateAffine(const Template*);extern void Sprite_SetAnimActiveFlag(void*,int);extern void Sprite_SetDrawPriority(void*,int);extern void Sprite_SetDrawFlag(void*,int);extern void Sprite_SetOamMode(void*,int);extern void Sprite_SetAnimCtrlSeq(void*,int);extern void ov65_0221DD34(void*,void*,int);
void ov65_0221D930(Work*);
void ov65_0221D930(Work*w){Template t;int i;CreateSpriteResourcesHeader(w->headA,0,0,0,0,-1,-1,0,2,w->man[0],w->man[1],w->man[2],w->man[3],0,0);CreateSpriteResourcesHeader(w->headB,1,1,1,1,-1,-1,0,0,w->man[0],w->man[1],w->man[2],w->man[3],0,0);t.list=w->list;t.header=w->headA;t.x=32<<12;t.y=96<<12;t.z=0;t.sx=4096;t.sy=4096;t.sz=4096;t.rot=0;t.priority=0;t.vram=1;t.heap=26;
for(i=0;i<2;i++){t.x=ov65_0221FF4C[i].x<<12;t.y=ov65_0221FF4C[i].y<<12;w->main[i]=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(w->main[i],1);ov65_0221DD34(w->obj[i],w->main[i],i);Sprite_SetDrawPriority(w->main[i],100);Sprite_SetDrawFlag(w->main[i],0);Sprite_SetOamMode(w->main[i],1);}
for(i=0;i<12;i++){t.x=(ov65_0221FF4C[i].x+16)<<12;t.y=(ov65_0221FF4C[i].y-6)<<12;w->other[i]=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(w->other[i],1);Sprite_SetAnimCtrlSeq(w->other[i],i+5);Sprite_SetDrawPriority(w->other[i],5);Sprite_SetDrawFlag(w->other[i],0);Sprite_SetOamMode(w->other[i],1);}
for(i=0;i<12;i++){t.x=(ov65_0221FF4C[i].x+36)<<12;t.y=(ov65_0221FF4C[i].y+16)<<12;t.priority=0;w->text[i]=Sprite_CreateAffine(&t);Sprite_SetDrawPriority(w->text[i],3);Sprite_SetDrawFlag(w->text[i],0);Sprite_SetOamMode(w->text[i],1);}
for(i=0;i<12;i++){t.x=(ov65_0221FF4C[i].x+45)<<12;t.y=(ov65_0221FF4C[i].y+16)<<12;t.priority=0;w->text2[i]=Sprite_CreateAffine(&t);Sprite_SetDrawPriority(w->text2[i],3);Sprite_SetDrawFlag(w->text2[i],0);Sprite_SetOamMode(w->text2[i],1);}
t.x=96<<12;t.y=66<<12;w->single=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(w->single,1);Sprite_SetAnimCtrlSeq(w->single,20);Sprite_SetDrawFlag(w->single,0);
for(i=0;i<6;i++){t.header=w->headB;t.x=ov65_0221FEA4[i].x<<12;t.y=(ov65_0221FEA4[i].y<<12)+0xc0000;t.vram=2;w->bottom[i]=Sprite_CreateAffine(&t);Sprite_SetDrawFlag(w->bottom[i],0);Sprite_SetAnimActiveFlag(w->bottom[i],0);Sprite_SetAnimCtrlSeq(w->bottom[i],ov65_0221FEA4[i].seq);}}
