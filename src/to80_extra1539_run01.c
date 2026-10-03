#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void ov59_02238FF4(void*,int);int ov59_02239F18(void*);
int ov59_022380EC(void*p);
int ov59_022380EC(void*p){switch(H(p,60)){case 0:ov59_02238FF4(p,0);H(p,60)++;break;case 1:{int r=ov59_02239F18(p);if(r!=4)H(p,60)=0;return r;}}return 4;}
