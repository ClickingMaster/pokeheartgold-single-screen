#pragma force_active on
#include "global.h"
typedef struct {u32 words[9];} Template;
typedef struct {s16 x,y;} Coord;
extern void ov102_021EAD5C(void *,Coord *),EasyChatAppView_InitSpriteHeader(void *,Template *,int,int),ov102_021EB088(void *,int),Sprite_SetAnimCtrlSeq(void *,int),Sprite_SetDrawFlag(void *,int);
extern void *EasyChatAppView_CreateSprite(void *,Template *,int,int,int,int);extern int EasyChatApp_GetType(void *);
void ov102_021EA920(u8 *);
void ov102_021EA920(u8 *w) {
 Template a,b;Coord pos;
 if(*(u32*)(w+0x8c))ov102_021EAD5C(w+0x84,&pos);else {pos.x=128;pos.y=24;}
 EasyChatAppView_InitSpriteHeader(*(void**)w,&a,0,0);EasyChatAppView_InitSpriteHeader(*(void**)w,&b,1,0);
 *(void**)(w+0x60)=EasyChatAppView_CreateSprite(*(void**)w,&a,pos.x,pos.y,0,1);
 *(void**)(w+0x64)=EasyChatAppView_CreateSprite(*(void**)w,&b,pos.x,pos.y-*(s16*)(w+0x1e2),0,2);Sprite_SetAnimCtrlSeq(*(void**)(w+0x64),1);ov102_021EB088(w,1);
 *(void**)(w+0x68)=EasyChatAppView_CreateSprite(*(void**)w,&a,8,64,1,1);Sprite_SetAnimCtrlSeq(*(void**)(w+0x68),19);
 *(void**)(w+0x6c)=EasyChatAppView_CreateSprite(*(void**)w,&a,248,64,1,1);Sprite_SetAnimCtrlSeq(*(void**)(w+0x6c),21);
 *(void**)(w+0x74)=EasyChatAppView_CreateSprite(*(void**)w,&a,28,64,1,1);Sprite_SetAnimCtrlSeq(*(void**)(w+0x74),18);
 *(void**)(w+0x70)=EasyChatAppView_CreateSprite(*(void**)w,&a,128,64,1,1);Sprite_SetAnimCtrlSeq(*(void**)(w+0x70),23);
 if(EasyChatApp_GetType(*(void**)(w+4))!=2){Sprite_SetDrawFlag(*(void**)(w+0x6c),0);Sprite_SetDrawFlag(*(void**)(w+0x68),0);Sprite_SetDrawFlag(*(void**)(w+0x70),0);Sprite_SetDrawFlag(*(void**)(w+0x74),0);}
}
