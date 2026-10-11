#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[8];void *a,*b;} Output;
typedef struct {u8 pad[0x24];void *bg;u8 pad2[0x20];void *msg;u8 pad3[0x840];void *entries[4];} Work;
extern void *NewString_ReadMsgData(void *,int);
extern void *VsRecorder_NewMessageFormat(int);
extern void *FrontierCard_NewNameString(void *,int);
extern void ov40_02230DCC(Work *,void *);
extern void *String_New(int,int);
extern void BufferString(void *,int,void *,int,int,int);
extern void StringExpandPlaceholders(void *,void *,void *);
extern void String_Delete(void *);
extern void MessageFormat_Delete(void *);
extern void InitWindow(void *);
extern void AddTextWindowTopLeftCorner(void *,void *,int,int,int,int);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,void *);
extern void TextOBJ_CopyFromBGWindow(void *,void *,void *,int);
extern void RemoveWindow(void *);
void ov40_02240D50(Output *,Work *,int);
void ov40_02240D50(Output *out,Work *w,int idx){void *text;u8 window[16];void *messages=w->msg;if(w->entries[idx]==0)text=NewString_ReadMsgData(messages,8);else{void *fmt=VsRecorder_NewMessageFormat(0x6d);void *pattern=NewString_ReadMsgData(w->msg,7);void *name=FrontierCard_NewNameString(w->entries[idx],0x6d);ov40_02230DCC(w,name);text=String_New(255,0x6d);BufferString(fmt,0,name,0,1,2);StringExpandPlaceholders(fmt,text,pattern);String_Delete(pattern);String_Delete(name);MessageFormat_Delete(fmt);}InitWindow(window);AddTextWindowTopLeftCorner(w->bg,window,20,2,0,0);AddTextPrinterParameterizedWithColor(window,0,text,0,0,255,0xe0d00,0);TextOBJ_CopyFromBGWindow(out->a,out->b,window,0x6d);String_Delete(text);RemoveWindow(window);}
