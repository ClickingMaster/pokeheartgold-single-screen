#pragma force_active on
#include "global.h"
extern void AddWindowParameterized(void*,void*,int,u8,int,int,int,int,u16);
extern int ov72_0223B46C[];
typedef struct{int a;void*bg;u8 p[0x1310];u8 window[16],windows[3][16];}Work;
void ov72_0223ACA8(Work*);
void ov72_0223ACA8(Work*w){int i,base;int*pos=ov72_0223B46C;u8*win;i=0;base=31;win=w->windows[0];for(;i<3;base+=64,pos++,win+=16,i++){AddWindowParameterized(w->bg,win,4,(u8)*pos,21,8,2,13,(u16)base);}AddWindowParameterized(w->bg,w->window,4,2,19,27,4,13,79);}
