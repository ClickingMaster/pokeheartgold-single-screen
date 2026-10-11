#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x27];u8 clear[5];s8 indices[5];} Context;
typedef struct {Context *ctx;u8 pad[0x1148];u8 mode,active;s8 delta;u8 gap[5];u8 limits[8];} Work;
void BattleBagMenu_PrintItemList(Work *);void BattleBagMenu_PrintPageNumber(Work *);void BattleBagMenu_ShowPageSprites(Work *,int);void BattleBagMenu_DrawPageButtons(Work *,int);int BattleBagMenu_ChangeItemPage(Work *);
int BattleBagMenu_ChangeItemPage(Work *w){s8 value=w->ctx->indices[w->active];w->ctx->clear[w->active]=0;value+=w->delta;if(value>w->limits[w->active])w->ctx->indices[w->active]=0;else if(value<0)w->ctx->indices[w->active]=w->limits[w->active];else w->ctx->indices[w->active]=value;BattleBagMenu_PrintItemList(w);BattleBagMenu_PrintPageNumber(w);BattleBagMenu_ShowPageSprites(w,w->mode);BattleBagMenu_DrawPageButtons(w,w->mode);return 2;}
