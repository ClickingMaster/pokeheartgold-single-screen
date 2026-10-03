#pragma force_active on
#include "global.h"
typedef struct{void*p;u8 pad[0x20];u8 context[0x10];u8 flag;}G;
typedef struct{u8 pad0[0x78];u8 a[0x10],b[0x14],c[0x14];u8 pad1[0xe0];u8 x[0x10],y[0x44];int z,pad2,mode;}S;
typedef struct{int state;u8 pad0[0x24];int old;int*current;void*task;S*s1,*s2;G*g;int flag;}W;
extern int IsPaletteFadeFinished(void);extern void ov92_0225DA2C(S*),sub_020180F8(void*,void*),SysTask_Destroy(void*),ov92_0225D9A8(S*,void*,int),ov92_0225D9B4(S*,int),sub_020180BC(void*,void*,void*,int,int,void*),sub_020181D4(S*,void*),sub_02018198(void*,int),sub_020182A0(S*,int),ov92_022630E8(void*),ov92_02260428(S*,int,int,int,int,float,int),sub_020182A8(S*,int,int,int);
extern const int ov92_02263CD0[][2],ov92_02263CD4[][2];
#define CLEAN() ov92_0225DA2C(a);ov92_0225DA2C(b);sub_020180F8(a->b,w->g->context);sub_020180F8(a->c,w->g->context);sub_020180F8(b->b,w->g->context);sub_020180F8(b->c,w->g->context)
void ov92_0225F530(void*,W*);
void ov92_0225F530(void*task,W*w){S*a=w->s1,*b=w->s2;int n,idx;if(!IsPaletteFadeFinished()||w->g->flag==1){CLEAN();w->task=0;SysTask_Destroy(task);return;}n=*w->current;idx=n-1;if(idx<0||idx>4)idx=0;
switch(w->state){case 0:ov92_0225D9A8(a,w->g->p,ov92_02263CD0[idx][0]);ov92_0225D9B4(a,w->flag);w->state++;return;
case 1:sub_020180BC(a->b,a->a,w->g->p,39,113,w->g->context);sub_020180BC(a->c,a->a,w->g->p,40,113,w->g->context);sub_020181D4(a,a->b);sub_020181D4(a,a->c);sub_02018198(a->b,0);sub_02018198(a->c,0);sub_020182A0(a,0);a->mode=0;ov92_022630E8(a->x);ov92_022630E8(a->y);ov92_02260428(a,0,0,5,5,0.8f,0);ov92_02260428(a,0,0,-5,-5,0.8f,0);w->state++;return;
case 2:ov92_0225D9A8(b,w->g->p,ov92_02263CD4[idx][0]);ov92_0225D9B4(b,w->flag);w->state++;return;
case 3:sub_020180BC(b->b,b->a,w->g->p,41,113,w->g->context);sub_020180BC(b->c,b->a,w->g->p,42,113,w->g->context);sub_020181D4(b,b->b);sub_020181D4(b,b->c);sub_02018198(b->b,0);sub_02018198(b->c,0);sub_020182A0(b,1);b->mode=1;ov92_022630E8(b->x);ov92_022630E8(b->y);ov92_02260428(b,0,0,5,5,0.8f,0);ov92_02260428(b,0,0,-5,-5,0.8f,0);if(w->flag){sub_020182A8(a,0,-32768,0);sub_020182A8(b,0,-32768,0);a->z=-32768;b->z=-32768;}w->state++;return;
case 4:if(!b->mode){sub_020182A0(a,1);sub_020182A0(b,0);a->mode=1;w->state++;}return;
case 5:if(!n)w->state=255;else if(w->old!=n){CLEAN();w->state=0;}w->old=*w->current;return;
default:CLEAN();w->task=0;SysTask_Destroy(task);break;}}
