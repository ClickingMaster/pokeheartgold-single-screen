#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { void *buf; int w; int h; u8 bpp; u8 pad[3]; int stride; } Canvas;
void ClearContinuous(Canvas *,u32);
#pragma force_active on

static inline u32 Fill(u32 col,int bpp){u32 v;if(bpp==4){v=col|(col<<4);v|=v<<8;v|=v<<16;}else{v=col|(col<<8);v|=v<<16;}return v;}
static inline int CharSize(int bpp){return bpp*8*8/8;}
// @fn ClearContinuous
void ClearContinuous(Canvas *c,u32 col){if(c->bpp==4){col|=col<<4;col|=col<<8;col|=col<<16;}else{col|=col<<8;col|=col<<16;}MI_CpuFillFast(c->buf,(int)col,c->w*c->h*CharSize(c->bpp));}
