#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern void ov00_022148A4(void*);
extern void *ov00_021FE5B4(int,int,int);
extern int ov00_02213FD4(u8*,int);
extern u32 ov00_02213EE8(void*,u8*);
extern void ov00_021FE754(void*,void*);
int ov00_02215490(u8*,u8*,int);
int ov00_02215490(u8*c,u8*p,int len){int i;int count=*p++;u32 pair[2];len--;if(W(c,8))ov00_022148A4(c);W(c,8)=(u32)ov00_021FE5B4(8,count,0);if(!W(c,8))return 5;for(i=0;i<count;i++){int n;if(len<2)return 4;n=ov00_02213FD4(p+1,len-1);if(n==-1)return 4;pair[1]=p[0];pair[0]=ov00_02213EE8(c,p+1);ov00_021FE754((void*)W(c,8),pair);p+=n+1;len-=n+1;}return 0;}
