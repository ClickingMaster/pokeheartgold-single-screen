#pragma force_active on
#include "global.h"
#pragma force_active on
#include "bg_window.h"
typedef struct{u8 pad[0xc];int x;u16 pad10;u8 count;u8 pad13[0x39];BgConfig *bg;u8 pad50[0x310];void *sprites[1];} Work;
const VecFx32 *ov81_02242F30(void*);
void ov81_02242EB8(void*,BOOL);
void ov81_02242EC4(VecFx32*,void*,fx32,fx32);
void ov81_022405F0(Work*);
BOOL ov81_02240564(Work*);
BOOL ov81_02240564(Work *w){int hidden=0,i;VecFx32 v;BgSetPosTextAndCommit(w->bg,6,BG_POS_OP_ADD_X,8);w->x=Bg_GetXpos(w->bg,(GFBgLayer)6);for(i=0;i<w->count;i++){int x=ov81_02242F30(w->sprites[i])->x/FX32_ONE-8;if(x<-24){ov81_02242EB8(w->sprites[i],FALSE);hidden++;}else ov81_02242EC4(&v,w->sprites[i],-8,0);}ov81_022405F0(w);if(hidden==w->count)return TRUE;return FALSE;}
