#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 type:4;u16 upper:12;} Mode;
typedef struct {u8 pad[0x2a];Mode mode;u16 other[2];u8 pad2[6];u8 count;u8 pad3[0x25];void *msg;u8 pad4[8];void *string;u8 pad5[0xbc];u8 window[16];} State;
extern void FillWindowPixelBuffer(void *,int),DrawFrameAndWindow2(void *,int,int,int),ReadMsgDataIntoString(void *,int,void *),AddTextPrinterParameterized(void *,int,void *,int,int,int,void *),ScheduleWindowCopyToVram(void *);
extern int ov59_02239E0C(State *,void *,int);
void ov59_02238D90(State *);
#pragma opt_propagation off
void ov59_02238D90(State *s){
 u32 offset=0x128;
 FillWindowPixelBuffer((u8 *)s+offset,15);DrawFrameAndWindow2((u8 *)s+offset,1,1,13);
 {int id=ov59_02239E0C(s,&s->mode,0);ReadMsgDataIntoString(s->msg,id,s->string);}AddTextPrinterParameterized((u8 *)s+offset,1,s->string,0,0,255,0);
 if(s->mode.type==5){ScheduleWindowCopyToVram((u8 *)s+offset);return;}
 if(s->count>1){{int id=ov59_02239E0C(s,&s->other[0],1);ReadMsgDataIntoString(s->msg,id,s->string);}AddTextPrinterParameterized((u8 *)s+offset,1,s->string,0,16,255,0);}
 if(s->count>2){{int id=ov59_02239E0C(s,&s->other[1],2);ReadMsgDataIntoString(s->msg,id,s->string);}AddTextPrinterParameterized((u8 *)s+offset,1,s->string,0,32,255,0);}
 else if((u8)(s->count+255)<=1){ReadMsgDataIntoString(s->msg,122,s->string);AddTextPrinterParameterized((u8 *)s+offset,1,s->string,0,(s->count-1)*16+16,255,0);}
 ScheduleWindowCopyToVram((u8 *)s+offset);
}
