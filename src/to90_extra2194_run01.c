#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void*root;s32 offset;}Work;typedef struct{s16 y,x;}Pos;void ov18_021F12C8(void*,int,s16*,s16*,int),ov18_021F1294(void*,int,s16,s16,int);int ov18_021F50C0(Work*);
int ov18_021F50C0(Work*w){Pos a,b;ov18_021F12C8(w->root,2,&a.x,&a.y,1);ov18_021F12C8(w->root,4,&b.x,&b.y,1);w->offset+=0x400;a.y+=w->offset>>12;b.y+=w->offset>>12;if(a.y>=88){ov18_021F1294(w->root,2,a.x,88,1);ov18_021F1294(w->root,4,b.x,88,1);return 0;}ov18_021F1294(w->root,2,a.x,a.y,1);ov18_021F1294(w->root,4,b.x,b.y,1);return 1;}
