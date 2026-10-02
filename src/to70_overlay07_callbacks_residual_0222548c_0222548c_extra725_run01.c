#pragma force_active on
#include "global.h"
extern void*ov07_022324D8(void*,int),*ov07_0221C4A8(void*,int);
extern void ov07_02231FE4(void*,void*),ov07_02232020(void*,void*,void*,int*),ov07_02222A44(void*,void*,void*),ov07_0221C410(void*,void(*)(void),void*),ov07_02225444(void);
typedef struct{int a;void*owner;u8 p[20];u8 position[6];s16 y;u8 q[12];int vec[3];int v1,v2,v3;}Work;
void ov07_0222548C(void*);
void ov07_0222548C(void*m){int tmp;Work*w=ov07_022324D8(m,84);void*p;ov07_02231FE4(m,w);ov07_02232020(m,ov07_0221C4A8(m,0),w->position,&tmp);w->y+=8;p=ov07_0221C4A8(m,1);ov07_02222A44(w->vec,p,ov07_0221C4A8(m,2));w->v1/=2;w->v3/=2;ov07_0221C410(w->owner,ov07_02225444,w);}
