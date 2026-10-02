#pragma force_active on
#include "global.h"
typedef struct {u8 pad[6];s16 item;u8 pad2;u8 type;u8 pad3[2];} Entry;
typedef struct {u32 x;Entry entries[8];u8 selected;} Bag;
typedef struct {u8 pad[0x234];Bag *bag;u8 pad2[0x348-0x238];int field;u8 pad3[0x644-0x34c];void *data;u8 pad4[0x671-0x648];u8 a,b,pad5;int state;} Work;
extern void ov15_021FB114(Work *);extern void ov15_021F9F08(Work *);extern int ov15_021FA074(Work *);extern void ov15_021FD574(Work *,int,int,int);extern void ov15_021FF364(Work *,int,int,int);extern void ov15_02200030(Work *,int);extern void ov15_021FD404(Work *,int,int);extern void ov15_021FF6BC(Work *,int,int,int);extern void ov15_02200140(Work *,Entry *,int,int);extern void ov15_021FED24(Work *);extern void ov15_021FFECC(Work *,void *);
int ov15_021FB060(Work *);
int ov15_021FB060(Work *w){Entry *entry;w->field=0;w->a=0;w->b=0;w->state=0;ov15_021FB114(w);ov15_021F9F08(w);entry=&w->bag->entries[w->bag->selected];ov15_021FD574(w,0,ov15_021FA074(w),0);ov15_021FF364(w,entry->item,-1,0);ov15_02200030(w,w->bag->selected);ov15_021FD404(w,1,w->bag->selected);ov15_021FF6BC(w,entry->type,entry->item,0);ov15_02200140(w,entry,ov15_021FA074(w),1);ov15_021FED24(w);ov15_021FFECC(w,w->data);return 1;}
