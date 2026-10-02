#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {u8 pad[0x284];u32 heap;u8 pad2[0x708-0x288];Sprite *a;u8 pad3;u8 flag;u16 height;void *b;Sprite *c;} Work;
extern Sprite *ov96_021EA214(void *,void *,int,int,u32);extern void *ov96_021EA4D4(void *,void *,int,u32);extern Sprite *ov96_021EA584(void *,void *,int,u32);
Sprite **ov96_021E61D8(Work *,int,void *,void *);
Sprite **ov96_021E61D8(Work *w,int height,void *a,void *b){VecFx32 first,second;w->a=ov96_021EA214(a,b,0,1,w->heap);w->b=ov96_021EA4D4(a,b,0,w->heap);w->c=ov96_021EA584(a,b,0,w->heap);first.z=0;first.x=0x80000;first.y=(height+96)<<12;Sprite_SetMatrix(w->a,&first);w->height=height;second.z=0;second.x=0x80000;second.y=(w->height+72)<<12;Sprite_SetMatrix(w->c,&second);w->flag=0;first.y=(height+130)<<12;Sprite_SetMatrix(w->b,&first);return &w->a;}
