#pragma force_active on
#include "global.h"
extern void *String_New(int,int);extern void String_Delete(void *);
extern u32 GetBoxMonData(void *,int,void *);extern u32 CalcBoxMonLevel(void *);
extern void *NewString_ReadMsgData(void *,int),*ReadMsgData_ExpandPlaceholders(void *,void *,int,int);
extern void BufferIntegerAsString(void *,int,int,int,int,int),ReadMsgDataIntoString(void *,int,void *),FillWindowPixelBuffer(void *,int);
extern void ov70_02245084(void *,void *,int,int,int,u32);
extern u32 ov70_02245910[],ov70_022465EC[];
typedef struct {u32 words[4];} Window;
typedef struct {u16 species;u8 gender,level;} Result;
void ov70_0223F508(void *,void *,u8 *,void *,Result *);
void ov70_0223F508(void *msg,void *fmt,u8 *windows,void *mon,Result *out) {
 void *label,*lv,*name,*gender;int g,level;int i;Window *win;
 name=String_New(11,61);gender=String_New(11,61);
 GetBoxMonData(mon,119,name);g=GetBoxMonData(mon,111,0)+1;level=CalcBoxMonLevel(mon);
 label=NewString_ReadMsgData(msg,104);BufferIntegerAsString(fmt,3,level,3,0,1);
 lv=ReadMsgData_ExpandPlaceholders(fmt,msg,108,61);
 if(g!=3)ReadMsgDataIntoString(msg,ov70_02245910[g],gender);
 for(i=0,win=(Window*)windows;i<3;i++,win++)FillWindowPixelBuffer(win,0);
 ov70_02245084(windows,label,0,0,0,0xf0200);
 ov70_02245084(windows+16,name,0,0,0,0x10200);
 ov70_02245084(windows+32,lv,0,0,2,0x10200);
 if(g!=3)ov70_02245084(windows+16,gender,70,0,0,ov70_022465EC[g-1]);
 out->species=GetBoxMonData(mon,5,0);out->gender=g;out->level=level;
 String_Delete(lv);String_Delete(gender);String_Delete(name);String_Delete(label);
}
