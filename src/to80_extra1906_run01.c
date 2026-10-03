#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 *data;u16 pad;s16 first;u32 extra;} Row;
typedef struct {u32 pad;Row rows[8];u8 index,pad2;u16 selected;} Table;
typedef struct {u8 pad[0x234];Table *table;u8 pad2[0x40c];s32 selected;u8 pad3[0x3a];u16 other;} W;
extern void ov15_021FD574(W *,int,int,int),ov15_021FF4EC(W *,s16,int),ov15_022002B4(W *,int),ov15_021FB14C(W *);
int ov15_021FA4F8(W *);
static inline u16 pairAt(u16*data,int index){return data[index*2];}typedef struct{u16 a,b;} Pair;static inline u16 pairNext(u16*data,int index){return ((Pair*)data)[index].b;}
int ov15_021FA4F8(W *w){Row *rows=w->table->rows;u32 offset=w->table->index*12;Row *row=(Row*)((u8*)rows+offset);ov15_021FD574(w,2,0,0);ov15_021FF4EC(w,row->first,w->selected-8);ov15_022002B4(w,w->selected-8);ov15_021FB14C(w);w->table->selected=pairAt(*(u16**)((u8*)rows+offset),row->first+w->selected-8);w->other=pairNext(*(u16**)((u8*)rows+offset),row->first+w->selected-8);return 4;}
