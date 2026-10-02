#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u32 heap;u8 p4[0x29fc-4];void*bg;u8 p2A00[0x2bb4-0x2a00];void*arg;u8 p2bb8[8];u32 flag;u8 p2BC4[0x2d7c-0x2bc4];void*volatile sprites[5];};void ov74_0222FCC4(void*);void*Heap_AllocAtEnd(int,u32);void*memset(void*,int,u32);void LoadFontPal0(int,int,int);void ov74_0222D824(struct S*,int);void Heap_Free(void*);void ov74_0222EC08(void*,void*,int);
void Sprite_Delete(void*);void ov74_022359BC(void);void ov74_0222EC60(struct S*);
static inline void Delete(void*p){if(p)Sprite_Delete(p);}
void ov74_0222EC60(struct S*s){Delete(s->sprites[0]);Delete(s->sprites[1]);s->sprites[1]=0;s->sprites[0]=s->sprites[1];Delete(s->sprites[2]);Delete(s->sprites[3]);Delete(s->sprites[4]);s->sprites[4]=0;s->sprites[3]=s->sprites[4];s->sprites[2]=s->sprites[3];ov74_022359BC();}
