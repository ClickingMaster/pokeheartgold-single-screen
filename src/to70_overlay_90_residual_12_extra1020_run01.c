#pragma force_active on
#include "global.h"
typedef struct{void *msg,*fmt,*string,*tmp;} Work;
extern void *NewMsgDataFromNarc(int,int,int,int);extern void *MessageFormat_New_Custom(int,int,int);extern void *String_New(int,int);extern void LoadFontPal1(int,int,int);
void ov90_02259184(Work *,int);
void ov90_02259184(Work *w,int heap){w->msg=NewMsgDataFromNarc(0,27,236,heap);w->fmt=MessageFormat_New_Custom(8,64,heap);w->string=String_New(128,heap);w->tmp=String_New(128,heap);LoadFontPal1(0,384,heap);LoadFontPal1(4,384,heap);}
