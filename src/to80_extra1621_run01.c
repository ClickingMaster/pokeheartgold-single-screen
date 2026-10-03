#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{u8 x[2];} Pair;extern char ov00_0221A650[],ov00_0221A5C8[],ov00_0221A574[];extern int ov00_02215490(void*,void*,int),ov00_02215938(void*,void*,int),ov00_022158AC(void*,void*,int),ov00_02215728(void*,void*,int),ov00_0221556C(void*,void*,int),ov00_021FF970(int,void*,int,int);extern void __msl_assertion_failed(char*,char*,char*,int),ov00_02214130(void*);int ov00_02215A30(u8*);
int ov00_02215A30(u8*p){u16 size;int result=0;while(I(p,128)>=3){memcpy(&size,V(p,124),2);size=((size>>8)&255)|((size<<8)&0xff00);if(size>4096){result=4;break;}if(I(p,128)<size)return 0;switch(*(s8*)((u8*)V(p,124)+2)){case 1:result=ov00_02215490(p,(u8*)V(p,124)+3,size-3);break;case 2:result=ov00_02215938(p,(u8*)V(p,124)+3,size-3);break;case 3:if(ov00_021FF970(I(p,0x4b0),V(p,124),size,0)<=0)return 3;break;case 4:result=ov00_022158AC(p,(u8*)V(p,124)+3,size-3);break;case 5:result=ov00_02215728(p,(u8*)V(p,124)+3,size-3);break;case 6:result=ov00_0221556C(p,(u8*)V(p,124)+3,size-3);break;}I(p,128)-=size;if(I(p,128)<0)__msl_assertion_failed(ov00_0221A650,ov00_0221A5C8,ov00_0221A574,1460);if(I(p,128)&&V(p,124))memmove(V(p,124),(u8*)V(p,124)+size,I(p,128));if(result)break;}if(result)ov00_02214130(p);return result;}
