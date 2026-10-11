#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Icon{u8 pad[8];void*sprite,*text;}Icon;
typedef struct Work{u8 pad[0x24];void*bg;u8 pad28[0x20];void*msg;u8 pad4c[0x840];void*slots[4];}Work;
extern void*NewString_ReadMsgData(void*,int),*VsRecorder_NewMessageFormat(int),*FrontierCard_NewNameString(void*,int),*String_New(int,int);
extern void ov40_02230DCC(Work*,void*),BufferString(void*,int,void*,int,int,int),StringExpandPlaceholders(void*,void*,void*),String_Delete(void*),MessageFormat_Delete(void*),InitWindow(void*),AddTextWindowTopLeftCorner(void*,void*,int,int,int,int),AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*),TextOBJ_CopyFromBGWindow(void*,void*,void*,int),RemoveWindow(void*);
void ov40_0223B29C(Icon*,Work*,int);
void ov40_0223B29C(Icon*icon,Work*w,int index){u8 window[16];void*text;void*slot=w->slots[index];void*msg=w->msg;if(slot==0)text=NewString_ReadMsgData(msg,8);else{void*format=VsRecorder_NewMessageFormat(109);void*template=NewString_ReadMsgData(w->msg,7);void*name=FrontierCard_NewNameString(w->slots[index],109);ov40_02230DCC(w,name);text=String_New(255,109);BufferString(format,0,name,0,1,2);StringExpandPlaceholders(format,text,template);String_Delete(template);String_Delete(name);MessageFormat_Delete(format);}InitWindow(window);AddTextWindowTopLeftCorner(w->bg,window,20,2,0,0);AddTextPrinterParameterizedWithColor(window,0,text,0,0,255,0xE0D00,0);TextOBJ_CopyFromBGWindow(icon->sprite,icon->text,window,109);String_Delete(text);RemoveWindow(window);}

typedef struct Container{u8 pad[0x114];struct {u8 pad[4];void*sprite,*text;u8 tail[16];} icons[4];}Container;
typedef struct Four{int val[4];}Four;extern const Four ov40_022454C0,ov40_022454D0;
extern void ov40_0222D78C(Work*,int),VsRecorder_CreateTextSprite(Icon*,void*,int),VsRecorder_SetTextSpriteMsg(Icon*,void*,int),ManagedSprite_SetPositionXY(void*,s16,s16),ManagedSprite_SetAnim(void*,int),TextOBJ_SetPosition(void*,int,int),TextOBJ_SetSpritesDrawFlag(void*,int);extern void*ov40_0222D800(Work*,int);
void ov40_0223B374(Work*);
