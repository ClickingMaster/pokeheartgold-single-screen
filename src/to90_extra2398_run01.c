#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

void ov98_0221EEEC(void*,int,int),ov98_0221EBD8(void*,int,int,int),ov98_0221EC08(void*,int,int,int,int),ov98_0221ECD0(void*,int,int,int,int,int),ov98_0221EBEC(void*,int,int,int,int,int),ov96_021EE6A0(void*),ov98_0221EDA4(void*,int,int,int),ov98_0221ED3C(void*,int,int);void*ov98_0221EEFC(void*);int ov96_021EF5F4(void*,int);void BufferIntegerAsString(void*,int,int,int,int,int);void ov96_021EF610(void*);
void ov96_021EF610(void*s){int type=B(s,0x20);u8 rank=B(s,0x22)+1;if(!W(s,0x28))ov98_0221EEEC(P(s,0x30),5,5);ov98_0221EBD8(P(s,0x30),0,197,0);ov98_0221EBD8(P(s,0x30),2,208,0);ov98_0221EC08(P(s,0x30),1,type+198,0,0);ov98_0221EC08(P(s,0x30),3,type*3+209,0,0);ov98_0221ECD0(P(s,0x30),5,239,rank,1,0);if(!W(s,0x24))ov98_0221EBEC(P(s,0x30),4,240,0,0,4);ov96_021EE6A0(P(s,0x34));if(W(s,0x28)){int value=ov96_021EF5F4(s,B(s,0x20));if(value==65535)value=0;if(B(s,0x20)==0){void*f=ov98_0221EEFC(P(s,0x30));BufferIntegerAsString(f,0,value/30,3,0,1);BufferIntegerAsString(f,1,(value%30)*10/30,1,0,1);}else if(B(s,0x20)==6){ov98_0221EDA4(P(s,0x30),(u32)value>>10,2,0);ov98_0221EDA4(P(s,0x30),(value%1024)*10/1024,1,1);}else ov98_0221EDA4(P(s,0x30),value,3,0);ov98_0221EBD8(P(s,0x30),6,176,0);ov98_0221ED3C(P(s,0x30),7,type+177);}}
