#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x1e448];void *format,*msg;u8 pad2[0x800];int value,page;s8 names[16][5];} Work;
extern void *NewString_ReadMsgData(void *,int);extern void PokewalkerApp_PrintString(Work *,int,void *,int,u32);extern void String_Delete(void *);extern void BufferIntegerAsString(void *,int,int,int,int,int);extern int ov112_021EC7B8(Work *);extern void *ReadMsgData_ExpandPlaceholders(void *,void *,int,int);extern void ov112_021EC8A4(Work *,int,int);
void ov112_021EC7D0(Work *);
void ov112_021EC7D0(Work *w){int i;void *text;for(i=0;i<5;i++){s8 id=*(s8 *)((u32)w+i+w->page*5+0x1ec58);if(id>=0)text=NewString_ReadMsgData(w->msg,id+91);else text=NewString_ReadMsgData(w->msg,118);PokewalkerApp_PrintString(w,i,text,4,0xf0100);String_Delete(text);}BufferIntegerAsString(w->format,10,w->page+1,1,0,1);BufferIntegerAsString(w->format,11,ov112_021EC7B8(w),1,0,1);text=ReadMsgData_ExpandPlaceholders(w->format,w->msg,81,154);PokewalkerApp_PrintString(w,7,text,0,0x10e00);String_Delete(text);ov112_021EC8A4(w,w->page,w->value);}
