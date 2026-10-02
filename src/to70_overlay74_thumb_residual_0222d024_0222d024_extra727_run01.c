#pragma force_active on
#include "global.h"
extern void Sprite_Delete(void*),ov74_022359BC(void);
typedef struct{u8 p[0x2dc4];void*a,*b,*c,*d,*e;}Work;
void ov74_0222D024(Work*);
void ov74_0222D024(Work*w){if(w->a)Sprite_Delete(w->a);if(w->b)Sprite_Delete(w->b);((volatile Work*)w)->b=0;w->a=((volatile Work*)w)->b;if(w->c)Sprite_Delete(w->c);if(w->d)Sprite_Delete(w->d);if(w->e)Sprite_Delete(w->e);((volatile Work*)w)->e=0;((volatile Work*)w)->d=((volatile Work*)w)->e;w->c=((volatile Work*)w)->d;ov74_022359BC();}
