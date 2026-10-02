#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{void*bg;int narc,chars,palette,screen,width,height,layer,plttSlot,plttSize,extra,heap;}W;typedef struct Screen{u16 width,height;}Screen;extern void ov41_02249FFC(int,int,void*,int,int,int),ov41_0224A04C(int,int,int,int,int,int),ov41_02249F7C(void*,int,Screen*,int,int,int,int,int,int);extern void*GfGfxLoader_GetScrnData(int,int,int,Screen**,int);extern void Heap_Free(void*);void ov41_02249E60(W*,int*,int*);
void ov41_02249E60(W*w,int*outx,int*outy){struct {int h,w;}dims;Screen*s;void*data;int x,y;dims.w=w->width/8;dims.h=w->height/8;ov41_02249FFC(w->narc,w->chars,w->bg,w->layer,w->extra,w->heap);ov41_0224A04C(w->narc,w->palette,w->layer<4?0:4,w->plttSize*32,w->plttSlot*32,w->heap);data=GfGfxLoader_GetScrnData(w->narc,w->screen,0,&s,w->heap);x=s->width/8;y=s->height/8;ov41_02249F7C(w->bg,w->layer,s,x,y,dims.w,dims.h,w->extra,w->plttSize);Heap_Free(data);if(outx)*outx=x;if(outy)*outy=y;}
