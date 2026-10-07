#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 *buf; int w; int h; u8 bpp; u8 pad[3]; int stride; } Canvas;
void ClearLine(Canvas *,u32);
#pragma force_active on
static inline int CharSize(int bpp){return bpp*8*8/8;}
static inline u32 Fill(u32 col,int bpp){u32 v;if(bpp==4){v=col|(col<<4);v|=v<<8;v|=v<<16;}else{v=col|(col<<8);v|=v<<16;}return v;}
// @fn ClearLine
void ClearLine(Canvas *c,u32 col){
int i;u8 *p;if(c->bpp==4){col|=col<<4;col|=col<<8;col|=col<<16;}else{col|=col<<8;col|=col<<16;}{const int cs=CharSize(c->bpp);const int step=cs*c->stride;const int size=cs*c->w;
 for(i=0,p=c->buf;i<c->h;i++){MI_CpuFillFast(p,(int)col,size);p+=step;}}
}
