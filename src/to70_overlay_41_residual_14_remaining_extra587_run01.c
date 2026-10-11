#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *data;int enabled;} EntryA;
typedef struct {void *data;int enabled;int more;} EntryB;
typedef struct {int dummy;void **a;int maxa,na;void **b;int maxb,nb;} Work;
extern void *GraphicsManager_LoadImage(EntryA*);
extern void *GraphicsManager_LoadPalette(EntryB*);

void ov41_0224683C(Work *w,EntryA *entries,int count);
void ov41_0224689C(Work *w,EntryB *entries,int count);

void ov41_0224683C(Work *w,EntryA *entries,int count){int i=0;u8 *cursor;if(i<count){cursor=(u8*)entries;do{if(w->na>=w->maxa)GF_AssertFail();if(entries->enabled)w->a[w->na]=GraphicsManager_LoadImage((EntryA*)cursor);else w->a[w->na]=0;w->na++;entries++;cursor+=8;i++;}while(i<count);}}
void ov41_0224689C(Work *w,EntryB *entries,int count){int i=0;u8 *cursor;if(i<count){cursor=(u8*)entries;do{if(w->nb>=w->maxb)GF_AssertFail();if(entries->enabled)w->b[w->nb]=GraphicsManager_LoadPalette((EntryB*)cursor);else w->b[w->nb]=0;w->nb++;entries++;cursor+=12;i++;}while(i<count);}}
