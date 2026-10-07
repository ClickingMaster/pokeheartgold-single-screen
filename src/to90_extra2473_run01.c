#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "font.h"
#include "text.h"
#include "save_pokegear.h"
#include "player_data.h"
#pragma force_active on
void ov01_021EEE44(u8*,Window*);
void ov01_021EEE44(u8*w,Window*win){FillWindowPixelRect(win,15,0,128,128,16);FillWindowPixelRect(win,15,0,384,128,16);MsgData*msg=NewMsgDataFromNarc((MsgDataLoadType)0,(NarcId)27,191,(enum HeapID)4);MessageFormat*fmt=MessageFormat_New((enum HeapID)4);String*str=String_New(16,(enum HeapID)4);String*tmpl=NewString_ReadMsgData(msg,332);u32 money=PhoneCallPersistentState_MomSavings_BalanceAction(SaveData_GetPhoneCallPersistentState(*(SaveData**)(w+12)),0,0);BufferIntegerAsString(fmt,0,money,6,(PrintingMode)0,1);StringExpandPlaceholders(fmt,str,tmpl);u32 x=128-FontID_String_GetWidth(0,str,0);AddTextPrinterParameterized(win,0,str,x,16,255,NULL);money=PlayerProfile_GetMoney(Save_PlayerData_GetProfile(*(SaveData**)(w+12)));BufferIntegerAsString(fmt,0,money,6,(PrintingMode)0,1);StringExpandPlaceholders(fmt,str,tmpl);x=128-FontID_String_GetWidth(0,str,0);AddTextPrinterParameterized(win,0,str,x,48,255,NULL);String_Delete(tmpl);String_Delete(str);MessageFormat_Delete(fmt);DestroyMsgData(msg);ScheduleWindowCopyToVram(win);}
