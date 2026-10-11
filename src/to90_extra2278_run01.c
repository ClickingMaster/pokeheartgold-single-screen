#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[12];int heap;} Parent;typedef struct {u16 item,pad;} Item;typedef struct {u8 pad[0x3c];Item entries[21];} Row;
typedef struct {Parent *parent;u8 pad[0x28];u8 *windows;u8 pad0[0xc];Item rows[30][36];u8 tail[0x31];u8 selected;} Work;
extern void *String_New(int,int);extern void GetItemDescIntoString(void *,u16,u16),String_Delete(void *),ScheduleWindowCopyToVram(void *),AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *);
void BattleBagMenu_PrintItemDescription(Work *,int);
void BattleBagMenu_PrintItemDescription(Work *w,int index){void *window=w->windows+32;void *text=String_New(130,w->parent->heap);GetItemDescIntoString(text,w->rows[w->selected][index].item,(u16)w->parent->heap);AddTextPrinterParameterizedWithColor(window,0,text,4,0,255,0x10200,0);String_Delete(text);ScheduleWindowCopyToVram(window);}
