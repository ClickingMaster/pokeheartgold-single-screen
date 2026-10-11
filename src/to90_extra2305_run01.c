#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 low:7;u8 high:1;} Flag;
typedef struct{u8 gender:3;u8 other:4;u8 high:1;} Gender;
typedef struct{void*mon;u8 pad[0x16-4];Flag flag;Gender gender;u8 pad2[0x50-0x18];} Entry;
typedef struct{u8 pad[12];int heap;} Owner;
typedef struct{Owner*owner;Entry entries[100];u8 pad[0x1fa8-0x1f44];void*messages,*format;u8 pad2[0x2070-0x1fb0];void*window;} State;
extern int sBattlePartyMonNameMsgIds[];
extern void*String_New(int,int),*NewString_ReadMsgData(void*,int),*Mon_GetBoxMon(void*);
extern void BufferBoxMonNickname(void*,int,void*),StringExpandPlaceholders(void*,void*,void*),String_Delete(void*),ScheduleWindowCopyToVram(void*);
extern int FontID_String_GetWidth(int,void*,int),GetWindowWidth(void*);
extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*);
typedef struct {void *volatile text; Entry *volatile entry; void *volatile window;} LocalNames;
static inline void*readSuffix(State*s,LocalNames*n){void*extra=0;if(!n->entry->flag.high){Gender *gender=&n->entry->gender;if(!gender->high){if(gender->gender==0)extra=NewString_ReadMsgData(s->messages,16);else if(gender->gender==1)extra=NewString_ReadMsgData(s->messages,17);}}return extra;}
void BattlePartyMenu_PrintMonNameCentered(State*,int);
void BattlePartyMenu_PrintMonNameCentered(State*s,int index){LocalNames names;names.window=s->window;names.entry=&s->entries[index];names.text=String_New(12,s->owner->heap);void*temp=NewString_ReadMsgData(s->messages,sBattlePartyMonNameMsgIds[index]);void*extra,*name;u8 width,extraWidth,x;int gap;BufferBoxMonNickname(s->format,0,Mon_GetBoxMon(s->entries[index].mon));StringExpandPlaceholders(s->format,names.text,temp);String_Delete(temp);extra=readSuffix(s,&names);width=FontID_String_GetWidth(4,names.text,0);if(extra==0){extraWidth=0;gap=0;}else{extraWidth=FontID_String_GetWidth(0,extra,0);gap=8;}x=(GetWindowWidth(names.window)*8-width-extraWidth-gap)/2;name=names.text;AddTextPrinterParameterizedWithColor(names.window,4,name,x,7,255,0x70809,0);String_Delete(names.text);if(extra){if(names.entry->gender.gender==0)AddTextPrinterParameterizedWithColor(names.window,0,extra,gap+(x+width),8,255,0xa0b00,0);else AddTextPrinterParameterizedWithColor(names.window,0,extra,gap+(x+width),8,255,0xc0d00,0);String_Delete(extra);}ScheduleWindowCopyToVram(names.window);}
