#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 a[4];} Window;
typedef struct {u8 pad[0x1c];void*msg;} Work;
extern void FillWindowPixelBuffer(Window*,int);extern void*NewMsgDataFromNarc(int,int,int,int);extern void*NewString_ReadMsgData(void*,int);extern void DestroyMsgData(void*);extern void AddTextPrinterParameterizedWithColor(Window*,int,void*,int,int,int,int,int);extern u8 GetWindowWidth(Window*);extern void String_SetEmpty(void*);extern void ReadMsgDataIntoString(void*,int,void*);extern void String_Delete(void*);
void ov81_02240AD8(Work*,Window*,int,int,u8,u8,u8,u8,u16,u8);
void ov81_02240AD8(Work*w,Window*win,int x,int y,u8 fg,u8 shadow,volatile u8 bg,u8 font,u16 message,u8 mode){void*msg;void*str;u8 width;int label,color;FillWindowPixelBuffer(win,(u8)bg);msg=NewMsgDataFromNarc(1,27,237,100);str=NewString_ReadMsgData(msg,(u16)message);DestroyMsgData(msg);AddTextPrinterParameterizedWithColor(win,(u8)font,str,x,y,255,(((u32)fg<<24)>>8)|(((u32)shadow<<24)>>16)|bg,0);width=GetWindowWidth(win)-1;label=mode==0?26:27;color=mode==0?0x70800:0x30400;String_SetEmpty(str);if(mode!=2){ReadMsgDataIntoString(w->msg,label,str);AddTextPrinterParameterizedWithColor(win,(u8)font,str,width<<3,y,255,color,0);}String_Delete(str);}
