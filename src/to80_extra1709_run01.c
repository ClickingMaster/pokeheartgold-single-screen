#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct S {int a,b,c,d,toggleTime,toggle,attr;void *pic;u8 state,pad[3];void *owner;} S;
extern void Heap_Free(void *);
extern void ov07_0222BADC(void *,int,int,int,int);
extern void ov07_0221C448(void *,void *);
extern void Pokepic_SetAttr(void *,int,int);
void ov07_0222BAF4(void *,S *);
static inline void SetWin(int left,int top,int right,int bottom){*(vu16 *)0x04000040=((left<<8)&0xff00)|(u8)right;*(vu16 *)0x04000044=((top<<8)&0xff00)|(u8)bottom;}
static inline void ClearWin(int mask){*(vu16 *)0x04000048=(*(vu16 *)0x04000048 & ~0x3f)|mask;*(vu16 *)0x0400004a=(*(vu16 *)0x0400004a & ~0x3f)|mask;}
void ov07_0222BAF4(void *arg,S *s){
 switch(s->state){
 case 0:s->toggleTime+=79;if(s->toggleTime>=80){s->state++;s->toggle^=1;s->toggleTime=0;}else if(s->toggle==0){ov07_0222BADC(s->pic,0,s->toggleTime,(s->a+2)*2,2);}else{ov07_0222BADC(s->pic,80-s->toggleTime,s->toggleTime,(s->a+2)*2,2);}break;
 case 1:s->b--;s->d++;SetWin(s->c,s->b,s->c+80,s->d);s->state++;break;
 case 2:s->b--;s->d++;SetWin(s->c,s->b,s->c+80,s->d);s->a++;if(s->a>38)s->state++;else s->state=0;break;
 case 3:Pokepic_SetAttr(s->pic,14,0);s->state++;break;
 default:*(vu32 *)0x04000000 &= 0xffff1fff;ClearWin(0);*(vu16 *)0x04000040=0;*(vu16 *)0x04000044=0;ov07_0221C448(s->owner,arg);Pokepic_SetAttr(s->pic,23,s->attr);Heap_Free(s);break;
 }
}
