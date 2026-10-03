#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{int heap;u8 pad4[4];void*save;u8 padc[4];void*party;u8 pad14[53];u8 count,selected;u8 pad4b[17];void*messages;u8 pad60[8];void*text;u8 pad6c[16];u8 records[6][52];u8 windows[10][16];}W;
extern const int ov59_0223C99C[5];extern void ov59_0223AEB0(W*,int),ReadMsgDataIntoString(void*,int,void*),AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*),ScheduleWindowCopyToVram(void*),ov59_0223BC88(W*,u8);extern void*SaveArray_Party_Get(void*),*Party_GetMonByIndex(void*,int);extern int Party_GetCount(void*);extern u32 GetMonData(void*,int,void*);extern void ov59_0223BA64(void*,void*,void*,u8,int);void ov59_0223AD84(W*),ov59_0223AE20(W*);
void ov59_0223AE20(W*w){int count,i,valid;u8*records;w->party=SaveArray_Party_Get(w->save);count=Party_GetCount(w->party);valid=0;i=valid;if(count>0){records=(u8*)w;records+=0x7c;do{void*mon=Party_GetMonByIndex(w->party,i);if(GetMonData(mon,76,NULL)==0){ov59_0223BA64(records+valid++*52,w->party,mon,(u8)i,w->heap);}i++;}while(i<count);}w->count=valid;}
