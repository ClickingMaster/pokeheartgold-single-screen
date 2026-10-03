#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern u8*ov00_021F955C(void);extern char ov00_02217A64[],ov00_02217B50[];extern u8 ov00_02217B54[];
extern int ov00_021F45E0(u32,u8,u32,char*),ov00_0220F724(u32,int,int),ov00_0221386C(u32,int,u32,void*,int);
int ov00_021F4304(u8,u32,u32,u16,u32*,int);int ov00_021F44C8(u8,u32,u32,u32*,u32);
#define WORD(p,o) (*(u32*)((p)+(o)))
#define HALF(p,o) (*(u16*)((p)+(o)))
int ov00_021F4304(u8 type,u32 arg,u32 arg2,u16 arg3,u32*list,int count){char temp[16];char text[512];int length=0,result,i,piece;if(ov00_021F955C()[0x15]==0||((ov00_021F955C()[0x15]==3||ov00_021F955C()[0x1a6]!=0)&&type==6)){result=ov00_021F44C8(type,arg2,arg3,list,count);}else{if(list&&count){length=OS_SNPrintf(text,512,ov00_02217A64,list[0]);for(i=1;i<count;i++){piece=OS_SNPrintf(temp,16,ov00_02217B50,list[i]);if((u32)(length+piece+1)>512)break;MI_CpuCopy8(temp,text+length,piece);length+=piece;}}text[length]=0;result=ov00_021F45E0(WORD(ov00_021F955C(),0),type,arg,text);}if(type==2||type==6||type==8||type==9){ov00_021F955C()[0x3cc]=type;HALF(ov00_021F955C(),0x3ce)=arg3;WORD(ov00_021F955C(),0x3d0)=arg2;WORD(ov00_021F955C(),0x454)=arg;WORD(ov00_021F955C(),0x458)=count;{u8*p=ov00_021F955C();*(OSTick*)(p+0x45c)=OS_GetTick();}if(list&&count)MIi_CpuCopy32(list,(u32*)(ov00_021F955C()+0x3d4),count*4);}return result;}
