#pragma force_active on
#include "global.h"
#pragma force_active on
extern void FillWindowPixelBuffer(void*,int),ScheduleWindowCopyToVram(void*),String_Delete(void*);
extern void *NewString_ReadMsgData(void*,int);
extern void BufferIntegerAsString(void*,int,int,int,int,int),StringExpandPlaceholders(void*,void*,void*);
extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,int);
extern int ov08_02225BE4[];
typedef struct {u16 item,quantity;} Item;typedef struct {Item items[36];} Pocket;
typedef struct {u8 prefix[0x10];void *messages,*formatter,*expanded;u8 gap1c[16];u8 (*windows)[16];u8 gap30[12];Pocket pockets[30];u8 gap111c[49];u8 selected;} Work;
void ov08_02223678(void*w,int index,int kind,int win,int font,int y,int color);
void ov08_02223678(void*w,int index,int kind,int win,int font,int y,int color){Work *state=w;u8 (*windows)[16]=state->windows;void*str;FillWindowPixelBuffer(windows[win],0);
 if(state->pockets[state->selected].items[index].quantity){str=NewString_ReadMsgData(state->messages,ov08_02225BE4[kind*2]);BufferIntegerAsString(state->formatter,0,state->pockets[state->selected].items[index].quantity,3,0,1);StringExpandPlaceholders(state->formatter,state->expanded,str);AddTextPrinterParameterizedWithColor(windows[win],font,state->expanded,0,y,255,color,0);String_Delete(str);}ScheduleWindowCopyToVram(windows[win]);}
