#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
extern u8 *_0221A680;extern u8 _0221640C[],_02216400[];
void ov00_021E65D4(void),ov00_021E6638(void),ov00_021E61A8(void);void ov00_021EC294(void(*)(void),void(*)(void)),ov00_021EC3F0(void*,int,int,int),ov00_021EC454(int),ov00_021EC4A4(void),ov00_021EC60C(void),ov00_021EDB1C(void);
int ov00_021EC5B4(void),ov00_021EC724(void),ov00_021E6690(void);
void ov00_021ED730(void*,u32,int,void*,void*,int,int,void*,int);void ov00_021EDD6C(u16*,int,void(*)(void),void*);
int ov00_021E5B6C(void);
int ov00_021E5B6C(void){OSOwnerInfo owner;int err;switch(W(_0221A680,0x1070)){case 0:ov00_021EC294(ov00_021E65D4,ov00_021E6638);ov00_021EC3F0(_0221A680+0xf14,2,1,20);ov00_021EC454(2);ov00_021EC4A4();W(_0221A680,0x1070)=1;W(_0221A680,0x1090)=0;case 1:if(ov00_021EC5B4()){if(ov00_021EC724()==4)W(_0221A680,0x1070)=2;else W(_0221A680,0x1070)=12;}else{ov00_021EC60C();break;}case 2:err=ov00_021E6690();if(err)return err;ov00_021ED730(_0221A680+0x104,W(_0221A680,0xf10),0x29e7,_02216400,_0221640C,0,0,P(_0221A680,0x100),32);OS_GetOwnerInfo(&owner);ov00_021EDD6C(owner.nickName,0,ov00_021E61A8,NULL);W(_0221A680,0x1070)=3;case 3:ov00_021EDB1C();break;case 4:ov00_021EDB1C();return 27;}return ov00_021E6690();}
