#pragma force_active on
#include "global.h"
struct ItemSlot;
#pragma force_active on
typedef struct {void *pocket;u16 a,b;u8 index,count;u8 q[2];} Slot;typedef struct {u32 header;Slot slots[8];u8 selected,q;u16 quantity;} Bag;typedef struct {u8 p[0x234];Bag *bag;u8 p2[0x448];u16 item;} Work;
extern BOOL Pocket_TakeItem(struct ItemSlot *,u32,u16,u16,u32);extern void ov15_021F9F08(Work *);extern void ov15_021FA044(u16 *,u16 *,u8);extern void ov15_021FA070(u16 *,u16 *,u8,u32);void ov15_021FBB28(Work *);
void ov15_021FBB28(Work *w){Pocket_TakeItem(w->bag->slots[w->bag->selected].pocket,w->bag->slots[w->bag->selected].count,w->bag->quantity,w->item,6);ov15_021F9F08(w);ov15_021FA044(&w->bag->slots[w->bag->selected].b,&w->bag->slots[w->bag->selected].a,w->bag->slots[w->bag->selected].count);ov15_021FA070(&w->bag->slots[w->bag->selected].b,&w->bag->slots[w->bag->selected].a,w->bag->slots[w->bag->selected].count,6);}
