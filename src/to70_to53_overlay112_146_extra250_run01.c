#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Flag{u8 value:5,rest:3;}Flag;
int sub_02032718(void *);void BufferPokewalkerCourseName(void *,int,u8);
void sub_02032674(void *,u32 *,u32 *);void BufferIntegerAsString(void *,int,int,int,int,int);
void ov112_021E9F5C(void *,int,int,int);void ov112_021E9FA4(void *,int,void *,int,int);
void ov112_021EA670(void *,int);void ov112_021EA6B8(void *,int,int,int);
void ov112_021EAB78(void *,int,int,int);void BeginNormalPaletteFade(int,int,int,int,int,int,int);
int ov112_021EF758(u8 *);
#define P(o) (*(void **)(w+(o)))
#define W(o) (*(u32 *)(w+(o)))
#define H(o) (*(u16 *)(w+(o)))
int ov112_021EF758(u8 *w){u32 first,second,difference=0,total,previous;
 BufferPokewalkerCourseName(P(0x1e448),2,(u8)sub_02032718(P(0x1e440)));
 total=*(volatile u32 *)(w+0x10f0);previous=*(volatile u32 *)(w+0x1d758);if(total>previous)difference=total-previous;
 sub_02032674(P(0x1e440),&first,&second);
 BufferIntegerAsString(P(0x1e448),5,difference,7,0,1);
 BufferIntegerAsString(P(0x1e448),3,H(0x9d7a),7,0,1);
 BufferIntegerAsString(P(0x1e448),6,first,7,0,1);
 BufferIntegerAsString(P(0x1e448),7,second,7,0,1);
 ov112_021E9F5C(w,2,56,0x30400);ov112_021E9F5C(w,3,57,0x30400);ov112_021E9F5C(w,4,58,0x30400);
 ov112_021E9FA4(w,5,P(0x1e4b4),0,0x30400);ov112_021E9F5C(w,6,60,0x30400);
 ov112_021E9FA4(w,7,P(0x1e4bc),0,0x30400);ov112_021E9F5C(w,8,62,0x30400);
 ov112_021EA670(w,9);ov112_021EA6B8(w,9,210,64);ov112_021EAB78(w,0,H(0x9d44),((Flag *)(w+0x9d51))->value);
 BeginNormalPaletteFade(0,1,1,0,6,1,154);return 2;
}
