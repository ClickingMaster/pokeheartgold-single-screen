#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct{void *a;Sprite *b,*c;u8 pad[8];void *d;u8 pad2[8];int x,y;} Work;
extern Sprite *ov96_021E8BAC(void *);
extern void ov96_021EAC0C(Work *,int);
extern void ov96_021E8BF4(u8 *gfx, int index, int screen, void *palette);

void ov96_021EAED4(Work *w,int x,int y,int arg,int flag);
void ov96_021EAF60(void *owner, u8 index, u32 screen, void *palette);

void ov96_021EAED4(Work *w,int x,int y,int arg,int flag){VecFx32 delta,result;Sprite *s;w->x+=x;w->y+=y;delta.x=x<<12;delta.y=y<<12;delta.z=0;s=ov96_021E8BAC(w->a);ov96_021EAC0C(w,arg);VEC_Add(&delta,Sprite_GetMatrixPtr(s),&result);Sprite_SetMatrix(s,&result);if(w->c){VEC_Add(&delta,Sprite_GetMatrixPtr(w->c),&result);Sprite_SetMatrix(w->c,&result);}if(flag&&w->b){VEC_Add(&delta,Sprite_GetMatrixPtr(w->b),&result);Sprite_SetMatrix(w->b,&result);}}
