#pragma force_active on
#include "global.h"
#pragma force_active on

#define W(p,o) (*(s32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define F(p,o) (*(float*)((u8*)(p)+(o)))
extern void *PokeathlonCourse_GetHeapAllocPtr4(void*);extern int ov96_022143DC(void*);extern int Point2D_IsLeftOfLine(const void*,const void*,const void*);extern const s32 ov96_0221D474[16];extern void ov96_021E8228(void*,u8,u8,int,int);
void ov96_022130EC(void*);
void ov96_022130EC(void*course){u8*base;int outer;u8*s=PokeathlonCourse_GetHeapAllocPtr4(course);outer=0;u8*p=s+0x62c;base=s+0x5c;for(;outer<2;outer++,p+=76){if(B(p,0x38)&&(B(p,0x39)==0||B(p,0x39)==2)){s32 coords[2];int group,index,row,col,delta;u8*r;coords[0]=(s32)W(p,8)/4096;coords[1]=(s32)W(p,12)/4096;group=ov96_022143DC(p);if(!(u8)Point2D_IsLeftOfLine((const u8 *)ov96_0221D474+group*16,(const u8 *)ov96_0221D474+group*16+8,coords))continue;B(p,0x39)=1;B(p,0x3a)=10;index=B(p,0x3b);if(index==12)continue;row=index/3;col=index%3;r=base+row*372+col*124;if(group==row){if(H(r,0x5e))H(r,0x5e)--;{s32*score=(s32*)(s+(B(p,0x3b)/3)*4);if(W(score,0x6f4))W(score,0x6f4)--;}index=B(p,0x3b);ov96_021E8228(course,(u8)(index/3),(u8)(index%3),8,1);index=B(p,0x3b);ov96_021E8228(course,(u8)(index/3),(u8)(index%3),1,1);}else{delta=0;if(B(p,0x38)==1)delta=1;else if(B(p,0x38)==2)delta=2;else GF_AssertFail();if(H(r,0x5e)+delta<=99)H(r,0x5e)+=delta;{s32*score=(s32*)(s+(B(p,0x3b)/3)*4);if(delta+W(score,0x6f4)<=99)W(score,0x6f4)+=delta;}if(W(s+group*4,0x6f4))W(s+group*4,0x6f4)--;index=B(p,0x3b);ov96_021E8228(course,(u8)(index/3),(u8)(index%3),3,1);}}}}
