#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
void FillWindowPixelBuffer(void*,u8);
u32 GetMoveAttr(u16,int);
extern u32 ov08_02224FE0[];
void ov08_0221DDCC(void*,int,int,int,int,int);
void ov08_0221E2E8(void*,int,int,int);
void ov08_0221ECE8(void*,int);void ov08_0221EE18(void*,int);void ov08_0221EFA8(void*,int);void ov08_0221F1B0(void*,int);
void ov08_0221E244(void*,int,int,u32,int,int,u32);
void ov08_0221ED30(void*,int,int);void ov08_0221EE60(void*,int,int);void ov08_0221EF48(void*,int,int);void ov08_0221F00C(void*,int,int);void ov08_0221F07C(void*,int,int,int);
void ov08_0221FDA4(void*);
void ov08_0221FDA4(void *p){u8 *row,*entry;u32 index;int attr;
FillWindowPixelBuffer((u8*)P(p,0x2070)+0,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+32,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+48,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+16,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+64,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+96,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+80,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+112,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+144,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+160,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+128,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+176,0);
ov08_0221DDCC(p,0,0,B(P(p,0),0x11),0,0);ov08_0221E2E8(p,2,0,0);ov08_0221ECE8(p,4);ov08_0221EE18(p,5);ov08_0221EFA8(p,9);index=B(P(p,0),0x34);if(index<4){row=(u8*)p+0x34+B(P(p,0),0x11)*80;entry=row+index*8;ov08_0221E244(p,H(entry,0),1,ov08_02224FE0[index],0,0,0xf0e00);ov08_0221ED30(p,6,B(entry,6));ov08_0221EE60(p,7,B(entry,7));ov08_0221EF48(p,8,H(entry,0));ov08_0221F00C(p,10,B(entry,5));ov08_0221F07C(p,3,B(entry,2),B(entry,3));}else{attr=GetMoveAttr(H(P(p,0),0x24),5);ov08_0221E244(p,H(P(p,0),0x24),1,0x49,0,0,0xf0e00);ov08_0221EF48(p,8,H(P(p,0),0x24));ov08_0221ED30(p,6,GetMoveAttr(H(P(p,0),0x24),4));ov08_0221EE60(p,7,GetMoveAttr(H(P(p,0),0x24),2));ov08_0221F00C(p,10,GetMoveAttr(H(P(p,0),0x24),1));ov08_0221F07C(p,3,attr,attr);}ov08_0221F1B0(p,11);}
