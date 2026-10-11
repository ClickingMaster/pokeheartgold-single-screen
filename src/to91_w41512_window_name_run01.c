#include "global.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "bg_window.h"
#pragma force_active on
typedef struct Scene {u8 prefix[0x48];MsgData *messages;u8 gap4c[0x88c-0x4c];void *entries[4];u8 gap89c[8];Window window;u8 rest[0x4170-0x8a4-sizeof(Window)];} Scene;
extern MessageFormat *VsRecorder_NewMessageFormat(enum HeapID);
extern String *FrontierCard_NewNameString(const u16 *,enum HeapID);
extern void ov40_02230DCC(void *,void *);
void ov40_022442F0(void *,int,int);
typedef char ActualOwner[(sizeof(Scene)==0x4170)?1:-1];
void ov40_022442F0(void *argument,int messageId,int index) {
 Scene *owner=argument;Window *window=&owner->window;String *text;
 if(messageId==282) {
  const u16 *entry=owner->entries[index];
  MessageFormat *format=VsRecorder_NewMessageFormat((enum HeapID)109);
  String *name,*message;
  text=String_New(255,(enum HeapID)109);
  name=FrontierCard_NewNameString(entry,(enum HeapID)109);
  ov40_02230DCC(owner,name);
  message=NewString_ReadMsgData(owner->messages,messageId);
  BufferString(format,0,name,0,1,2);
  StringExpandPlaceholders(format,text,message);
  String_Delete(name);String_Delete(message);MessageFormat_Delete(format);
 } else text=NewString_ReadMsgData(owner->messages,messageId);
 FillWindowPixelBuffer(window,204);
 AddTextPrinterParameterizedWithColor(window,0,text,0,0,255,0xf0d0c,NULL);
 ScheduleWindowCopyToVram(window);
 String_Delete(text);
}
