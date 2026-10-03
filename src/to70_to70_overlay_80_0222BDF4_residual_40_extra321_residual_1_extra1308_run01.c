#pragma force_active on
#include "global.h"
typedef struct{u32 a;u8 x,y,right,bottom;s16 count;s8 mode,pad;} Work;
extern void SysTask_Destroy(void *);
static inline void setRect(int left,int top,int right,int bottom){*(volatile u16 *)0x4000040=((left<<8)&0xff00)|right;*(volatile u16 *)0x4000044=((top<<8)&0xff00)|bottom;}
static inline void setInside(int a,int enable){volatile u16 *reg=(volatile u16 *)0x4000048;int value=(reg[0]&~63)|a;if(enable)value|=32;reg[0]=value;}
static inline void setOutside(int a,int enable){volatile u16 *reg=(volatile u16 *)0x400004a;int value=(reg[0]&~63)|a;if(enable)value|=32;reg[0]=value;}
static inline void setWindow(u32 flags){*(volatile u32 *)0x4000000=(*(volatile u32 *)0x4000000&0xffff1fff)|flags;}
void ov80_0222F53C(void *,Work *);
void ov80_0222F53C(void *task,Work *w){if(w->count<=0){if(w->mode==1){setInside(15,1);setOutside(31,1);setRect(w->x,w->y,w->right,w->bottom);setWindow(0x2000);}else setWindow(0);SysTask_Destroy(task);MI_CpuFill8(w,0,12);}else w->count--;}
