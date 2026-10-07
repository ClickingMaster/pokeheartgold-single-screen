#include "overlay96_results_transition_r52_private.h"
#pragma force_active on
extern s16 ov96_0221BA80[8][2];
// @fn ov96_021EF95C
void ov96_021EF95C(Ov96R52Work*w,int n){int i=0;s16(*p)[2]=ov96_0221BA80+n*4;again:ManagedSprite_SetPositionXYWithSubscreenOffset(w->sprites[i],(*p)[0],(*p)[1],2<<20);p++;if(++i<4)goto again;}
