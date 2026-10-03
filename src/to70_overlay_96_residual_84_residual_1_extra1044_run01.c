#pragma force_active on
#include "global.h"
typedef struct{s16 x,y;} Pair;
typedef struct{u32 a;void *bg;u8 p[0x4c];void *sprites[3];} Work;
extern VecFx32 *ov96_021EB594(void *);
extern void ov96_021E634C(void *,int,int,int,int,int,Pair *);
void ov96_02203D74(Work *,int,int);
void ov96_02203D74(Work *w,int x,int y){Pair pairs[3];int i;for(i=0;i<3;i++){VecFx32 *pos=ov96_021EB594(w->sprites[i]);pairs[i].x=pos->x/4096;pairs[i].y=pos->y/4096;ov96_021E634C(w->bg,0,y,x,2,3,pairs);}}
