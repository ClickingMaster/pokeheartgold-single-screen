#pragma force_active on
#include "global.h"
typedef struct{u8 data[16];} Window;
typedef struct{void*font;Window*window;void*list,*palette,*sprite;volatile int offset;int x,y,one,height,vram,heap;} Template;
typedef struct{int x,y,z;} Resource;
typedef struct{u8 p[0xe0];void*manager,*bg;u8 p2[0x174];void*font;void*text[2];Resource res[1];} Work;
void*NewMsgDataFromNarc(int,int,int,int);
void*NewString_ReadMsgData(void*,int);
void InitWindow(Window*);
void AddTextWindowTopLeftCorner(void*,Window*,int,int,int,int);
void AddTextPrinterParameterizedWithColor(Window*,int,void*,int,int,int,int,void*);
int sub_02013688(Window*,int,int);
void sub_02021AC8(int,int,int,Resource*);
void*SpriteManager_GetSpriteList(void*);
void*SpriteManager_FindPlttResourceProxy(void*,int);
u32 FontID_String_GetWidth(int,void*,int);
void*sub_020135D8(Template*);
void sub_020138E0(void*,int);
void String_Delete(void*);
void DestroyMsgData(void*);
void RemoveWindow(Window*);
void ov57_022387E0(Work*,int,int,int,int);
void ov57_022387E0(Work*w,int index,int x,int y,int pal){void*msg=NewMsgDataFromNarc(0,27,11,52);void*volatile s=NewString_ReadMsgData(msg,index+5);Template t;Window win;int allocated;InitWindow(&win);AddTextWindowTopLeftCorner(w->bg,&win,10,2,0,0);AddTextPrinterParameterizedWithColor(&win,2,s,0,0,255,0xf0d02,0);allocated=sub_02013688(&win,2,52);sub_02021AC8(allocated,1,2,&w->res[index]);t.font=w->font;t.window=&win;t.list=SpriteManager_GetSpriteList(w->manager);t.palette=SpriteManager_FindPlttResourceProxy(w->manager,30000);t.sprite=0;t.offset=w->res[index].y;t.x=x-FontID_String_GetWidth(2,s,0)/2;t.y=y+192;t.one=1;t.height=40;t.vram=2;t.heap=52;w->text[index]=sub_020135D8(&t);sub_020138E0(w->text[index],pal);String_Delete(s);DestroyMsgData(msg);RemoveWindow(&win);}
