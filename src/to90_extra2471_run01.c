#pragma force_active on
#include "global.h"
#pragma force_active on
extern int SaveMysteryGift_HasAnyGift(const void*,int);
extern void*ReadMsgData_ExpandPlaceholders(void*,void*,int,int);
extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*),String_Delete(void*);
typedef struct{u8 p[0x152];u8 a:3;u8 flag:1;u8 b:4;}Card;
typedef struct{int heap;u8 p[0x29fc];void*fmt,*msg;u8 q[0x198];void*save;u8 r[16];Card*cards[3];int selected;}Work;
int ov74_0222D4E4(Work*,void*,int);
int ov74_0222D4E4(Work*w,void*win,int color){int id,selected=w->selected;if(!(w->cards[selected]->flag))id=40;else if(w->save==0||SaveMysteryGift_HasAnyGift(w->save,selected)==1)id=38;else id=39;{void*text=ReadMsgData_ExpandPlaceholders(w->fmt,w->msg,id,w->heap);AddTextPrinterParameterizedWithColor(win,1,text,0,0,255,color,0);String_Delete(text);}return 1;}
