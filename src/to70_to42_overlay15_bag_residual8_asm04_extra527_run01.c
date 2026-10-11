#pragma force_active on
#include "global.h"
typedef struct {u8 pad[6];s16 item;u8 pad2;u8 type;u8 pad3[2];} Entry;
typedef struct {u32 x;Entry entries[8];u8 selected;} Bag;
typedef struct {u8 pad[0x234];Bag *bag;u8 pad2[0x348-0x238];int field;u8 pad3[0x644-0x34c];void *data;u8 pad4[0x671-0x648];u8 a,b,pad5;int state;} Work;
extern void BagApp_UpdateMoveMarker(Work *);extern void BagApp_LoadPocketItemNames(Work *);extern int BagApp_GetPageItemCount(Work *);extern void BagApp_LoadSubScreenLayout(Work *,int,int,int);extern void BagApp_DrawItemList(Work *,int,int,int);extern void BagApp_LoadPocketObjPalette(Work *,int);extern void BagApp_LoadPocketSubBgPalette(Work *,int,int);extern void BagApp_PrintPageNumber(Work *,int,int,int);extern void BagApp_DrawItemIcons(Work *,Entry *,int,int);extern void BagApp_CloseItemPromptWindow(Work *);extern void BagApp_SetCursorPosition(Work *,void *);
int BagApp_State_EndMoveItem(Work *);
int BagApp_State_EndMoveItem(Work *w){Entry *entry;w->field=0;w->a=0;w->b=0;w->state=0;BagApp_UpdateMoveMarker(w);BagApp_LoadPocketItemNames(w);entry=&w->bag->entries[w->bag->selected];BagApp_LoadSubScreenLayout(w,0,BagApp_GetPageItemCount(w),0);BagApp_DrawItemList(w,entry->item,-1,0);BagApp_LoadPocketObjPalette(w,w->bag->selected);BagApp_LoadPocketSubBgPalette(w,1,w->bag->selected);BagApp_PrintPageNumber(w,entry->type,entry->item,0);BagApp_DrawItemIcons(w,entry,BagApp_GetPageItemCount(w),1);BagApp_CloseItemPromptWindow(w);BagApp_SetCursorPosition(w,w->data);return 1;}
