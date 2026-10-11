#pragma force_active on
#include "global.h"
#include "to91_bag_choices41560_private.h"
#pragma force_active on
typedef struct{u8 p[6];s16 value;u8 kind,p9[3];}Entry;
typedef struct{u32 p0;Entry entries[8];u8 index,mode;u16 id;}Selection;
typedef struct{u32 p0;u8 windows[3][16];u8 p34[0x234-0x34];Selection*selection;u8 p238[0x2f0-0x238];void*messages,*format;u8 p2f8[0x644-0x2f8];s32 offset;}Work;
extern void FillWindowPixelBuffer(void*,int),BagApp_PrintTMHMMoveInfo(Work*,int),ScheduleWindowCopyToVram(void*),BagApp_ShowTMHMTypeIcons(Work*,int,int),BagApp_LoadInfoPanelTilemap(Work*,int),DrawFrameAndWindow2(void*,int,int,int),BagApp_BufferSlotItemName(Work*,int,int),StringExpandPlaceholders(void*,void*,void*),AddTextPrinterParameterized(void*,int,void*,int,int,int,void*),String_Delete(void*);extern BOOL BagApp_IsBerryOrMulch(u32,u32);extern void*NewString_ReadMsgData(void*,int),*String_New(int,int);

void BagApp_PrintItemSelectedMessage(void*argument,const u8*choices,int count){Work*w=argument;Entry*entry;void*message,*expanded;if(w->selection->entries[w->selection->index].kind==3){FillWindowPixelBuffer(w->windows[1],0);BagApp_PrintTMHMMoveInfo(w,w->selection->id);ScheduleWindowCopyToVram(w->windows[0]);BagApp_ShowTMHMTypeIcons(w,w->selection->id,1);BagApp_LoadInfoPanelTilemap(w,0);}DrawFrameAndWindow2(w->windows[2],1,994,12);FillWindowPixelBuffer(w->windows[2],15);entry=&w->selection->entries[w->selection->index];if(w->selection->mode==6&&BagApp_IsBerryOrMulch(entry->kind,w->selection->id)==0)message=NewString_ReadMsgData(w->messages,106);else message=NewString_ReadMsgData(w->messages,43);expanded=String_New(108,6);BagApp_BufferSlotItemName(w,entry->value+w->offset-8,0);StringExpandPlaceholders(w->format,expanded,message);AddTextPrinterParameterized(w->windows[2],1,expanded,0,0,255,0);String_Delete(expanded);String_Delete(message);ScheduleWindowCopyToVram(w->windows[2]);}
