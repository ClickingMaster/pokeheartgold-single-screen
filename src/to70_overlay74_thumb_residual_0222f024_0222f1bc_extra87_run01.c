#pragma force_active on
#include "global.h"
#pragma force_active on
#define WORD(w,o) (*(u32*)((w)+(o)))
#define PTR(w,o) (*(void**)((w)+(o)))
typedef struct Config {u32 other,x,y,width,height,font,color,fill,msg;void(*draw)(void*,void*,int);u32 rest[2];} Config;
extern const Config ov74_0223C700[];
extern void AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16),FillWindowPixelBuffer(void*,u8);
extern void *NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int),*ReadMsgData_ExpandPlaceholders(void*,void*,int,int);
extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*),String_Delete(void*),DestroyMsgData(void*),MessageFormat_Delete(void*),DrawFrameAndWindow2(void*,int,int,int),DrawFrameAndWindow1(void*,int,int,int);
int ov74_0222F1BC(u8*,void*,int,int);
int ov74_0222F1BC(u8*w,void*win,int index,int base){const Config*c=&ov74_0223C700[index];if(*(void**)win==0)AddWindowParameterized(PTR(w,0x29fc),win,2,(u8)c->x,(u8)c->y,(u8)c->width,(u8)c->height,15,(u16)base);FillWindowPixelBuffer(win,(u8)c->fill);if(c->msg){PTR(w,0x2a04)=NewMsgDataFromNarc(1,27,247,85);PTR(w,0x2a00)=MessageFormat_New(85);}WORD(w,0x2b98)=base;PTR(w,0x2b9c)=(void*)c;c->draw(w,win,0x10200);if(c->msg){void*str=ReadMsgData_ExpandPlaceholders(PTR(w,0x2a00),PTR(w,0x2a04),c->msg,85);AddTextPrinterParameterizedWithColor(win,c->font,str,0,0,255,c->color,0);String_Delete(str);DestroyMsgData(PTR(w,0x2a04));MessageFormat_Delete(PTR(w,0x2a00));}if(win==w+0x2bc4)DrawFrameAndWindow2(win,0,19,10);else DrawFrameAndWindow1(win,0,10,14);return base+c->width*c->height;}
