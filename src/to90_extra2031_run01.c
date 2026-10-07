#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16);void FillWindowPixelBuffer(void*,u8);
extern const struct{u32 x,y,width,height,bg;} ov70_022454EC[];
void ov70_0223ACF4(u8*w);
void ov70_0223ACF4(u8*w){
 int i,a,b;const typeof(ov70_022454EC[0])*entry;u8*win;
 AddWindowParameterized(P(w,4),w+0xf18,4,2,19,27,4,1,40);FillWindowPixelBuffer(w+0xf18,0);
 AddWindowParameterized(P(w,4),w+0xf58,4,1,21,13,2,1,148);FillWindowPixelBuffer(w+0xf58,0);
 AddWindowParameterized(P(w,4),w+0xf68,4,17,21,13,2,1,174);FillWindowPixelBuffer(w+0xf68,0);
 a=1;i=0;entry=ov70_022454EC;b=200;win=w+0x1058;for(;i<16;i++,entry++,win+=16){if(entry->bg==0){AddWindowParameterized(P(w,4),win,(u8)entry->bg,(u8)entry->x,(u8)entry->y,(u8)entry->width,(u8)entry->height,13,(u16)a);a+=entry->width*entry->height;}else{AddWindowParameterized(P(w,4),win,(u8)entry->bg,(u8)entry->x,(u8)entry->y,(u8)entry->width,(u8)entry->height,1,(u16)b);b+=entry->width*entry->height;}FillWindowPixelBuffer(win,0);}
}
