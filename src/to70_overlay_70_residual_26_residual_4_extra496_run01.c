#pragma force_active on
#include "global.h"
typedef struct {u8 a[0x18];int mode,stateA,stateB;u8 b[8];int state;u8 c[0x13c-0x30];u8 inner[0x11c8-0x13c];void *prompt;u8 d[0x11fc-0x11cc];int flag;} Work;
extern int ov70_02238C8C(Work*);extern void YesNoPrompt_Destroy(void*);extern void ov70_02238E50(Work*,int,int);extern void ov70_0223F7A4(void*,Work*);extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);

int ov70_0223F108(Work*w);
int ov70_0223F164(Work*w);

int ov70_0223F108(Work*w){int r=ov70_02238C8C(w);if(r==1){YesNoPrompt_Destroy(w->prompt);ov70_02238E50(w,7,7);w->state=2;w->flag=1;ov70_0223F7A4(w->inner,w);}else if(r==2){YesNoPrompt_Destroy(w->prompt);ov70_02238E50(w,5,5);w->state=2;}return 3;}
int ov70_0223F164(Work*w){if(w->mode==0){BeginNormalPaletteFade(0,0,0,0,6,1,61);w->flag=1;}else BeginNormalPaletteFade(3,0,0,0,6,1,61);w->state=0;return 4;}
