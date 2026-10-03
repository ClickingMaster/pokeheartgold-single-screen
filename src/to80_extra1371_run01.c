#pragma force_active on
#include "global.h"
#pragma force_active on
extern void FillWindowPixelBuffer(void *,int);
extern int AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *);
extern void *String_New(u32,u32);
extern void String_Delete(void *);
extern void *MessageFormat_New_Custom(int,int,u32);
extern void MessageFormat_Delete(void *);
extern void BufferIntegerAsString(void *,int,int,int,int,int);
extern void BufferMonthNameAbbr(void *,int,int);
extern void ReadMsgDataIntoString(void *,int,void *);
extern void StringExpandPlaceholders(void *,void *,void *);
extern int FontID_String_GetWidth(int,void *,int);
extern void CopyU16ArrayToString(void *,const u16 *);
extern u16 GetIGTHours(void *);
extern u8 GetIGTMinutes(void *);
extern void ov51_021E74F4(void *,int,int,int,void *,int,int,int,int);
extern void ov51_021E7540(void *,int,int,int,void *);
typedef struct Wnd {u8 data[16];} Wnd;
typedef struct Card {u8 pad[4];u8 f0:1,live:1,f2:1,score:1,f4:4;u8 p1[3];u16 name[8];void *time;u32 money,points,value;u16 id,hours;u8 p2[2];u8 minutes,year,month,day;} Card;
typedef struct Work {u8 pad[0x33c4];void *messages;u8 p1[8];void *expanded,*numeric;u8 p2[20];void *labels[7];} Work;
void ov51_021E6F18(Work *,Wnd *,Card *);
void ov51_021E6F18(Work *w,Wnd *windows,Card *card){
 void *messages=w->messages;u8 i=0;
 for(i=0;i<7;i++){FillWindowPixelBuffer(windows+i,0);if(i!=3||(i==3&&card->score))AddTextPrinterParameterizedWithColor(windows+i,0,w->labels[i],0,0,255,0x10200,NULL);}
 void *text=String_New(32,25);void *expanded=w->expanded;void *fmt=MessageFormat_New_Custom(6,32,25);void *numeric=w->numeric;int width;
 ov51_021E74F4(windows,96,0,0,numeric,card->id,5,2,255);
 CopyU16ArrayToString(w->expanded,card->name);ov51_021E7540(windows+1,104,0,0,w->expanded);
 BufferIntegerAsString(fmt,5,card->money,6,0,1);ReadMsgDataIntoString(messages,19,text);StringExpandPlaceholders(fmt,expanded,text);width=136-FontID_String_GetWidth(0,expanded,0);
 AddTextPrinterParameterizedWithColor(windows+2,0,expanded,width,0,255,0x10200,NULL);
 if(card->score){BufferIntegerAsString(fmt,5,card->points,3,0,1);ReadMsgDataIntoString(messages,26,text);StringExpandPlaceholders(fmt,expanded,text);width=136-FontID_String_GetWidth(0,expanded,0);AddTextPrinterParameterizedWithColor(windows+3,0,expanded,width,0,255,0x10200,NULL);}
 ov51_021E74F4(windows+4,136,0,0,numeric,card->value,9,1,255);
 if(card->live){BufferIntegerAsString(fmt,0,GetIGTHours(card->time),3,1,1);BufferIntegerAsString(fmt,1,GetIGTMinutes(card->time),2,2,1);ReadMsgDataIntoString(messages,21,text);}
 else{BufferIntegerAsString(fmt,0,card->hours,3,1,1);BufferIntegerAsString(fmt,1,card->minutes,2,2,1);ReadMsgDataIntoString(messages,20,text);}
 StringExpandPlaceholders(fmt,expanded,text);width=224-FontID_String_GetWidth(0,expanded,0);AddTextPrinterParameterizedWithColor(windows+5,0,expanded,width,0,255,0x10200,NULL);
 BufferIntegerAsString(fmt,2,card->year,2,2,1);BufferMonthNameAbbr(fmt,3,card->month);BufferIntegerAsString(fmt,4,card->day,2,2,1);ReadMsgDataIntoString(messages,22,text);StringExpandPlaceholders(fmt,expanded,text);width=224-FontID_String_GetWidth(0,expanded,0);AddTextPrinterParameterizedWithColor(windows+6,0,expanded,width,0,255,0x10200,NULL);
 String_Delete(text);MessageFormat_Delete(fmt);
}
