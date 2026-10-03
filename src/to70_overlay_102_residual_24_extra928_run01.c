#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x7c];void*text;} Work;
extern void ov102_021EAD48(int,s16*);extern void GetECWordIntoStringByIndex(int,void*);extern u32 FontID_String_GetWidth(int,void*,int);extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*);

void ov102_021EAD98(Work*w,void*window,int position,int word);

void ov102_021EAD98(Work*w,void*window,int position,int word){s16 pos[2];if(word!=65535){ov102_021EAD48(position,pos);GetECWordIntoStringByIndex(word,w->text);FontID_String_GetWidth(1,w->text,0);AddTextPrinterParameterizedWithColor(window,1,w->text,pos[0],pos[1],255,0x5060e,0);}}
