#pragma force_active on
#include "global.h"
#pragma force_active on

extern void BgSetPosTextAndCommit(void*,u32,u32,int);extern int Bg_GetXpos(void*,u32);
extern VecFx32*ov81_02242F30(void*);extern int ov81_02242F8C(void*),ov81_02242F90(void*);extern void ov81_02242F10(void*,int,int),ov81_02242EC4(VecFx32*,void*,int,int),ov81_022405F0(void*);
typedef struct{u8 pad[12];int x;u16 gap;u8 count;u8 pad13[0x4c-0x13];void*bg;u8 pad50[0x360-0x50];void*sprites[6];} W31794;
BOOL ov81_022404B4(void*);
BOOL ov81_022404B4(void *parent){W31794 *w=parent;int reset=0,i;VecFx32 v;BgSetPosTextAndCommit(w->bg,6,1,8);w->x=Bg_GetXpos(w->bg,6);for(i=0;i<w->count;i++){if(ov81_02242F30(w->sprites[i])->x/4096-8<=ov81_02242F8C(w->sprites[i])){int x=ov81_02242F8C(w->sprites[i]);int y=ov81_02242F90(w->sprites[i]);ov81_02242F10(w->sprites[i],x,y);reset=1;}else ov81_02242EC4(&v,w->sprites[i],-8,0);}ov81_022405F0(w);if(reset==1)return TRUE;return FALSE;}
