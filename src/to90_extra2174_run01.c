#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern void* ov01_021F5A28(int,int,void*);extern u32 ov01_021F6328(void*,void*);
void ov01_021F47A0(int,int,int,int,void*,int*);
#pragma force_active on
void ov01_021F47A0(int x,int y,int width,int height,void*w,int*out){int tx=x/32,ty=y/32;u32 quadrant=ov01_021F6328(ov01_021F5A28(x,y,w),w);int index=tx+ty*width;
 switch(quadrant){case 0:out[3]=index;out[0]=index-width-1;out[1]=index-width;out[2]=index-1;if(tx-1<0){out[0]=-1;out[2]=-1;}if(ty-1<0)out[1]=-1;break;
 case 1:out[2]=index;out[0]=index-width;out[1]=index-width+1;out[3]=index+1;if(tx+1>=width){out[1]=-1;out[3]=-1;}if(ty-1<0)out[0]=-1;break;
 case 2:out[1]=index;out[0]=index-1;out[2]=index+width-1;out[3]=index+width;if(tx-1<0){out[0]=-1;out[2]=-1;}if(ty+1>height)out[3]=-1;break;
 case 3:out[0]=index;out[1]=index+1;out[2]=index+width;out[3]=index+width+1;if(tx+1>=width){out[1]=-1;out[3]=-1;}if(ty+1>height)out[0]=-1;break;
 default:GF_AssertFail();break;}
}
