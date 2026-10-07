#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x27];u8 clear[5];s8 indices[5];} Context;
typedef struct {Context *ctx;u8 pad[0x1148];u8 mode,active;s8 delta;u8 gap[5];u8 limits[8];} Work;
void ov08_0222377C(Work *);void ov08_022237C4(Work *);void ov08_02223F94(Work *,int);void ov08_02224A50(Work *,int);int ov08_02222A78(Work *);
int ov08_02222A78(Work *w){s8 value=w->ctx->indices[w->active];w->ctx->clear[w->active]=0;value+=w->delta;if(value>w->limits[w->active])w->ctx->indices[w->active]=0;else if(value<0)w->ctx->indices[w->active]=w->limits[w->active];else w->ctx->indices[w->active]=value;ov08_0222377C(w);ov08_022237C4(w);ov08_02223F94(w,w->mode);ov08_02224A50(w,w->mode);return 2;}
