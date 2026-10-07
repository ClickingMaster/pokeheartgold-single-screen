#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct { u32 icon; u16 species, hp, maxHp, item; u8 level, gender, visible, form, status, extra; u8 unused[2]; u32 statusIcon; } Slot;
typedef struct { u8 unused[0x214]; Slot slots[3]; } Work;
u32 Party_GetCount(void *), GetMonData(void *, int, void *), GetMonIconNaixEx(int, int, int), Pokemon_GetIconNaix(void *), GetMonGender(void *), Pokemon_GetStatusIconId(void *);
void *Party_GetMonByIndex(void *, int);
void ov05_0221DF38(Work *, void *, int);
void ov05_0221DF38(Work *work, void *party, int index)
{
 u16 count = Party_GetCount(party);
 u16 i;
 Work *base;
 i = 0;
 base = (Work *)((u8 *)work + index * sizeof(Slot));
 for (; i < 3; i++) {
  Work *slot;
  if (count <= i) {
   u32 icon = GetMonIconNaixEx(0, 0, 0);
   slot = (Work *)((u8 *)base + i * sizeof(Slot));
   slot->slots[0].icon = icon;
  } else {
   void *mon = Party_GetMonByIndex(party, i);
   Work *details;
   slot = (Work *)((u8 *)base + i * sizeof(Slot));
   slot->slots[0].icon = Pokemon_GetIconNaix(mon);
   slot->slots[0].species = GetMonData(mon, 5, NULL);
   if (slot->slots[0].species != 0) {
    details = (Work *)((u8 *)base + i * 24);
    details->slots[0].form = GetMonData(mon, 76, NULL);
    details->slots[0].hp = GetMonData(mon, 163, NULL);
    details->slots[0].maxHp = GetMonData(mon, 164, NULL);
    details->slots[0].level = GetMonData(mon, 161, NULL);
    details->slots[0].item = GetMonData(mon, 6, NULL);
    details->slots[0].status = GetMonData(mon, 162, NULL);
    details->slots[0].extra = GetMonData(mon, 112, NULL);
    if (GetMonData(mon, 176, NULL) == 1) slot->slots[0].visible = 0;
    else slot->slots[0].visible = 1;
    details->slots[0].gender = GetMonGender(mon);
    details->slots[0].statusIcon = (u8)Pokemon_GetStatusIconId(mon);
   }
  }
 }
}
