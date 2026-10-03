#pragma force_active on
#include "global.h"
#include "field_ui_8c3e4_3_core_internal.h"
extern int GetWindowWidth(Window*);
extern void BufferAbilityName(void*,int,int);
extern String *NewString_ReadMsgData(MsgData*,int);
extern void StringExpandPlaceholders(void*,String*,String*),String_Delete(String*);
extern MsgData *NewMsgDataFromNarc(int,int,int,int);
extern void DestroyMsgData(MsgData*);
void sub_0208D178(SummaryState*s){String *temp;MsgData *data;
sub_0208C7F8(s,16,111,0,0);
sub_0208C7F8(s,17,112,1,0);
sub_0208C7F8(s,18,113,3,0);
sub_0208C7F8(s,19,114,4,0);
sub_0208C7F8(s,20,115,2,0);
ScheduleWindowCopyToVram((Window*)&s->raw[36]);
ScheduleWindowCopyToVram((Window*)&s->raw[244]);
ScheduleWindowCopyToVram((Window*)&s->raw[260]);
ScheduleWindowCopyToVram((Window*)&s->raw[276]);
ScheduleWindowCopyToVram((Window*)&s->raw[292]);
ScheduleWindowCopyToVram((Window*)&s->raw[308]);
ScheduleWindowCopyToVram((Window*)&s->raw[324]);
ScheduleWindowCopyToVram((Window*)&s->raw[340]);
FillWindowPixelBuffer(PTR(s,0x224,Window)+0,0);
FillWindowPixelBuffer(PTR(s,0x224,Window)+1,0);
FillWindowPixelBuffer(PTR(s,0x224,Window)+2,0);
FillWindowPixelBuffer(PTR(s,0x224,Window)+3,0);
FillWindowPixelBuffer(PTR(s,0x224,Window)+4,0);
FillWindowPixelBuffer(PTR(s,0x224,Window)+5,0);
FillWindowPixelBuffer(PTR(s,0x224,Window)+6,0);
FillWindowPixelBuffer(PTR(s,0x224,Window)+7,0);
sub_0208C8C8(s,0,0x75,0x77,0x76,U16(s,0x254),U16(s,0x256),3,(u8)(GetWindowWidth(PTR(s,0x224,Window))*4),0);
sub_0208C87C(s,120,U16(s,600),3,0);sub_0208C778(s,PTR(s,0x224,Window)+1,0x10200,1);
sub_0208C87C(s,121,U16(s,602),3,0);sub_0208C778(s,PTR(s,0x224,Window)+2,0x10200,1);
sub_0208C87C(s,122,U16(s,604),3,0);sub_0208C778(s,PTR(s,0x224,Window)+3,0x10200,1);
sub_0208C87C(s,123,U16(s,606),3,0);sub_0208C778(s,PTR(s,0x224,Window)+4,0x10200,1);
sub_0208C87C(s,124,U16(s,608),3,0);sub_0208C778(s,PTR(s,0x224,Window)+5,0x10200,1);
BufferAbilityName(PTR(s,0x7A8,void),0,U8(s,0x262));temp=NewString_ReadMsgData(PTR(s,0x7A0,MsgData),0x7D);StringExpandPlaceholders(PTR(s,0x7A8,void),PTR(s,0x7AC,String),temp);String_Delete(temp);sub_0208C778(s,PTR(s,0x224,Window)+6,0x10200,0);data=NewMsgDataFromNarc(1,27,0x2D2,19);ReadMsgDataIntoString(data,U8(s,0x262),PTR(s,0x7AC,String));DestroyMsgData(data);AddTextPrinterParameterizedWithColor(PTR(s,0x224,Window)+7,0,PTR(s,0x7AC,String),3,0,255,0x10200,0);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+0);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+1);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+2);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+3);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+4);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+5);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+6);
ScheduleWindowCopyToVram(PTR(s,0x224,Window)+7);
}
