#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x18];u16 move;}Extra;
typedef struct{u8 p[0x224];u8*windows;u32 p228;Extra*extra;u8 p230[0x264-0x230];u16 moves[4];u8 maxpp[4],pp[4];u8 p274[0x7a0-0x274];void*messages;u32 p7a4,p7a8;void*str;u32 p7b0;void*moveMsg;}Work;
extern u8 GetMoveMaxPP(u16,u8);extern u32 FontID_String_GetWidth(int,void*,int);extern void ReadMsgDataIntoString(void*,int,void*),AddTextPrinterParameterizedWithColor(void*,int,void*,u32,u32,u32,u32,void*),sub_0208C8C8(Work*,int,int,int,int,u16,u16,u8,u8,u8);
void sub_0208D884(Work*,int);
// @fn sub_0208D884
void sub_0208D884(Work*w,int slot){int offset;u8*windows;u32 move;u8 maxpp,pp;windows=w->windows;offset=(slot+8)*16;if(slot!=4){move=w->moves[slot];maxpp=w->maxpp[slot];pp=w->pp[slot];}else{move=w->extra->move;pp=maxpp=GetMoveMaxPP(move,0);}ReadMsgDataIntoString(w->moveMsg,move,w->str);AddTextPrinterParameterizedWithColor(windows+offset,0,w->str,1,2,255,0x10200,0);if(move){ReadMsgDataIntoString(w->messages,135,w->str);AddTextPrinterParameterizedWithColor(windows+offset,0,w->str,16,16,255,0x10200,0);sub_0208C8C8(w,slot+8,117,slot+136,slot+141,maxpp,pp,2,60,16);}else{u32 width;ReadMsgDataIntoString(w->messages,153,w->str);width=FontID_String_GetWidth(0,w->str,0);AddTextPrinterParameterizedWithColor(windows+offset,0,w->str,60-(width>>1),16,255,0x10200,0);}}
