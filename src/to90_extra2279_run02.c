#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[44];u8 indices[4];}Header;typedef struct{u16 value,count;}Slot;typedef struct{Header*header;u8 pad[56];Slot entries[4][36];u8 p[0x114d-0x27c];u8 selected;}Work;u16 BattleBagMenu_GetItemOnPage(Work*,u32);
u16 BattleBagMenu_GetItemOnPage(Work*w,u32 n){u32 selected=w->selected;u32 index=n+6*w->header->indices[selected];u16 value=w->entries[selected][index].value;if(value==0||w->entries[selected][index].count==0)value=0;return value;}
