#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct {u32 values[6];} Config;extern const Config ov05_0221EAD4;typedef struct {u32 words[7];} Row;extern const Row ov05_0221EDA4[];
void ov05_0221CD24(void*,void*,int);void ov05_0221D318(void*);void ov05_0221D890(void*);void*ov05_0221CDC4(void*,const void*);void ov05_0221D3AC(void*,int,int,int);void ov05_0221D4D0(void*);void ov05_0221D904(void*);void ov05_0221D5DC(void*);void ov05_0221D664(void*,int,int,int);
void ov05_0221D7AC(u8*w);
void ov05_0221D7AC(u8*w){Config config=ov05_0221EAD4;u32 i;
 ov05_0221CD24(w,&config,23);ov05_0221D318(w);ov05_0221D890(w);
 for(i=0;i<14;i++)P(w+i*4,0x198)=ov05_0221CDC4(w,&ov05_0221EDA4[i]);
 ov05_0221D3AC(w,0,-256,0);ov05_0221D3AC(w,1,-256,0);ov05_0221D3AC(w,2,256,0);ov05_0221D3AC(w,3,256,0);ov05_0221D4D0(w);ov05_0221D904(w);ov05_0221D5DC(w);ov05_0221D664(w,0,-256,0);ov05_0221D664(w,1,-256,0);ov05_0221D664(w,2,256,0);ov05_0221D664(w,3,256,0);U(w,0x210)=14;
}
