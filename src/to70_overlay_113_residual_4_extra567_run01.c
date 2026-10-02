#pragma force_active on
#include "global.h"
typedef struct {u32 heap;u8 pad[12];u16 state;u8 pad2[8];u8 choice;} Work;
extern void BeginNormalPaletteFade(int,int,int,int,int,int,u32);
extern int IsPaletteFadeFinished(void);
extern void ov113_021E5DA4(Work*,u8);

int ov113_021E5C68(Work*w);

int ov113_021E5C68(Work*w){switch(w->state){case 0:BeginNormalPaletteFade(0,4,2,0,6,1,w->heap);w->state++;break;case 1:if(IsPaletteFadeFinished()){ov113_021E5DA4(w,w->choice);w->state++;}break;case 2:BeginNormalPaletteFade(0,3,5,0,6,1,w->heap);w->state++;break;case 3:if(IsPaletteFadeFinished()){w->state=0;return 2;}break;}return 3;}
