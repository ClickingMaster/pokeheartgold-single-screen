#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[8];u8 state,mode;u8 gap[5];u8 ready;}W;extern int ov80_02237D8C(u8),sub_02037B38(int),ov83_02241368(W*,int,int),IsPaletteFadeFinished(void);extern void sub_02037BEC(void),sub_02037AC0(int),ov83_0223E10C(W*),BeginNormalPaletteFade(int,int,int,int,int,int,int);int ov83_0223E008(W*);
int ov83_0223E008(W*w){switch(w->state){case 0:if(ov80_02237D8C(w->mode)==1){sub_02037BEC();sub_02037AC0(215);}w->state++;break;case 1:if(ov80_02237D8C(w->mode)==1){if(sub_02037B38(215)==1){sub_02037BEC();w->state++;}}else w->state++;break;case 2:if(ov80_02237D8C(w->mode)==1){if(ov83_02241368(w,10,0)==1)w->state++;}else{ov83_0223E10C(w);BeginNormalPaletteFade(0,1,1,0,6,3,107);w->state++;}break;case 3:if(ov80_02237D8C(w->mode)==1){if(w->ready>=2){w->ready=0;ov83_0223E10C(w);BeginNormalPaletteFade(0,1,1,0,6,3,107);w->state++;}}else w->state++;break;case 4:if(IsPaletteFadeFinished()==1)return 1;break;}return 0;}
