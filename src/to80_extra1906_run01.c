#pragma force_active on
#include "global.h"
#include "to91_bag_choices41560_private.h"
#pragma force_active on
typedef struct {u16 *data;u16 pad;s16 first;u32 extra;} Row;
typedef struct {u32 pad;Row rows[8];u8 index,pad2;u16 selected;} Table;
typedef struct {u8 pad[0x234];Table *table;u8 pad2[0x40c];s32 selected;u8 pad3[0x3a];u16 other;} W;
extern void BagApp_LoadSubScreenLayout(W *,int,int,int),BagApp_DrawSelectedItemEntry(W *,s16,int),BagApp_ShowSelectedItemIcon(W *,int);
int BagApp_State_OpenItemMenu(W *);
static inline u16 pairAt(u16*data,int index){return data[index*2];}typedef struct{u16 a,b;} Pair;static inline u16 pairNext(u16*data,int index){return ((Pair*)data)[index].b;}
int BagApp_State_OpenItemMenu(W *w){Row *rows=w->table->rows;u32 offset=w->table->index*12;Row *row=(Row*)((u8*)rows+offset);BagApp_LoadSubScreenLayout(w,2,0,0);BagApp_DrawSelectedItemEntry(w,row->first,w->selected-8);BagApp_ShowSelectedItemIcon(w,w->selected-8);BagApp_SetupItemMenu(w);w->table->selected=pairAt(*(u16**)((u8*)rows+offset),row->first+w->selected-8);w->other=pairNext(*(u16**)((u8*)rows+offset),row->first+w->selected-8);return 4;}
