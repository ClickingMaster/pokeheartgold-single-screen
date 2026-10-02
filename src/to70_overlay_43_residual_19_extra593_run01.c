#pragma force_active on
#include "global.h"
typedef struct {u8 p[0xf8];void *sprite;} W;typedef struct {u16 x,y,z;u8 p[2];u8 win[16];} Window;typedef struct {void *bg;} BG;
extern void Sprite_SetDrawFlag(void*,int);extern void Sprite_SetAnimActiveFlag(void*,int);extern void ov43_0222AD74(void*,int,int);extern void InitWindow(void*);extern void AddWindowParameterized(void*,void*,int,int,int,int,int,int,int);
void ov43_0222C750(W*,void*);void ov43_0222C788(Window*,BG*,s16*,int);
void ov43_0222C750(W*w,void*a) {*(volatile u32*)0x4000000&=0xffff1fff;Sprite_SetDrawFlag(w->sprite,0);Sprite_SetAnimActiveFlag(w->sprite,0);ov43_0222AD74(a,1,255);}
void ov43_0222C788(Window*w,BG*b,s16*p,int tile) {int y,x;w->x=((u16*)p)[0];w->y=((u16*)p)[1];w->z=((u16*)p)[2];InitWindow(w->win);y=p[1];if(y<0)y=-y;x=p[0];if(x<0)x=-x;AddWindowParameterized(b->bg,w->win,3,(u8)(x+4),(u8)(y+4),26,17,11,(u16)tile);}
