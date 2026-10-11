#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16);void FillWindowPixelBuffer(void*,u8);
typedef struct {u16 x,y;} Row;extern const Row ov70_02245640[],ov70_0224562C[],ov70_02245634[];void Gts_CreatePanelWindow(void*,int);
void ov70_0223BAE0(u8*w);
void ov70_0223BAE0(u8*w){int i,tile;const Row*row;u8*win;
 AddWindowParameterized(P(w,4),w+0xf18,0,2,21,27,2,13,96);FillWindowPixelBuffer(w+0xf18,0);
 row=ov70_02245640;i=0;tile=1;win=w+0x1058;for(;i<6;i++,tile+=22,row++,win+=16){AddWindowParameterized(P(w,4),win,3,(u8)row->x,(u8)row->y,11,2,13,(u16)tile);FillWindowPixelBuffer(win,0);}
 row=ov70_0224562C;i=0;tile=133;win=w+0x1178;for(;i<2;i++,tile+=56,row++,win+=16){AddWindowParameterized(P(w,4),win,3,(u8)row->x,(u8)row->y,28,2,13,(u16)tile);FillWindowPixelBuffer(win,0);}
 i=0;win=w+0x1058;tile=285;row=ov70_02245634;for(;i<3;i++,tile+=20,row++){int offset=(i+6)*16;AddWindowParameterized(P(w,4),win+offset,3,(u8)row->x,(u8)row->y,10,2,13,(u16)tile);FillWindowPixelBuffer(win+offset,0);}
 Gts_CreatePanelWindow(w,4);
}
