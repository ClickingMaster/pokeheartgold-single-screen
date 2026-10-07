#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {s16 x,y;u32 z,priority,pad,kind,id,a,b,c,tail[4];} To90W30693Setup;
extern void *ov98_0221E6E0(void *,To90W30693Setup *);
extern void ManagedSprite_SetAnimateFlag(void *,int);
typedef struct {u32 id:9,rest:23;u32 a,b;} To90W30693Data;
extern u16 ov99_021E94CC(void *,s8,u8,int);
extern void ov99_021E9464(void *,To90W30693Data *,s8,int,int,int),ov98_0221E970(void *,void *,To90W30693Data *,int,int),ManagedSprite_SetDrawFlag(void *,int),ManagedSprite_SetAnimSpeed(void *,fx32);
void ov99_021E8F50(void *);
void ov99_021E8EBC(void *);
void ov99_021E8EBC(void *s){s16 rowY;To90W30693Setup t={0};u32 row=0,col,base=0;int xoff=0,y=44;for(;row<5;base+=3,xoff+=8,y+=32,row++){int x;col=0;x=60;rowY=y;for(;col<3;col++,x+=24){u32 index=col+base;t.id=index+2;t.a=2;t.b=2;t.c=2;t.kind=2;t.x=x+xoff;t.y=rowY;t.priority=3-col;*(void **)((u8 *)s+(index+22)*4+24)=ov98_0221E6E0(*(void **)((u8 *)s+20),&t);ManagedSprite_SetAnimateFlag(*(void **)((u8 *)s+(index+22)*4+24),1);}}}
