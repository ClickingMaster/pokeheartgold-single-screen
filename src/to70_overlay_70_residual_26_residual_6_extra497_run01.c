#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "text.h"
extern u32 sGtsGenderSymbolMsgs[];extern u32 ov70_0223F2A0(int,int);extern void ov70_02245084(Window *,String *,int,int,int,u32);
void ov70_0223F2BC(Window *,MsgData *,int,int,int,int,int);void ov70_0223F324(Window *,MsgData *,int,int,int,int,int);
void ov70_0223F2BC(Window *w,MsgData *msg,int n,int show,int a,int b,int c){String *s;if(show||n!=3){s=NewString_ReadMsgData(msg,sGtsGenderSymbolMsgs[n]);if(b>3)ov70_02245084(w,s,b,a,0,ov70_0223F2A0(n,c));else ov70_02245084(w,s,0,a,b,ov70_0223F2A0(n,c));String_Delete(s);}}
void ov70_0223F324(Window *w,MsgData *msg,int n,int show,int a,int b,int c){String *s;if(show||n!=3){s=NewString_ReadMsgData(msg,sGtsGenderSymbolMsgs[n]);AddTextPrinterParameterizedWithColor(w,0,s,a,b,255,ov70_0223F2A0(n,c),NULL);String_Delete(s);}}
