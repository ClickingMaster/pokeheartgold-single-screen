#include "global.h"
#include "touchscreen_list_menu.h"
#include "list_menu_items.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern int ov57_0223A0A8(void*,int);extern struct{s32 message,value;}ov57_0223BECC[];extern const TouchscreenListMenuTemplate ov57_0223BD7C;
void ov57_02238C30(BgConfig*,void*,int,void*,int);
static inline int SelectMenuIndices(int indices[4],int kind){int size;switch(kind){case 0:indices[0]=0;indices[1]=4;size=6;indices[2]=1;break;case 1:indices[0]=2;indices[1]=0;indices[2]=4;size=8;indices[3]=1;break;case 2:indices[0]=3;indices[1]=0;indices[2]=4;size=8;indices[3]=1;break;case 3:indices[0]=2;indices[1]=0;indices[2]=4;size=8;indices[3]=1;break;default:break;}return size;}
void ov57_02238C30(BgConfig*bg,void*unused1,int unused2,void*w,int selection){TouchscreenListMenuHeader header;int indices[4];int size,count,i;MsgData*msg;String*text;size=SelectMenuIndices(indices,ov57_0223A0A8(w,selection));count=size/2;P(w,0x1bc)=ListMenuItems_New(count,(enum HeapID)52);msg=NewMsgDataFromNarc((MsgDataLoadType)0,(NarcId)27,11,(enum HeapID)52);for(i=0;i<count;i++){text=NewString_ReadMsgData(msg,ov57_0223BECC[indices[i]].message);ListMenuItems_AddItem(P(w,0x1bc),text,ov57_0223BECC[indices[i]].value);String_Delete(text);}DestroyMsgData(msg);MI_CpuFill8(&header,0,24);header.template=ov57_0223BD7C;header.listMenuItems=P(w,0x1bc);header.bgConfig=bg;header.numWindows=(u8)count;P(w,0x24c)=TouchscreenListMenu_Create(P(w,0x248),&header,(u8)*(u32*)((u8*)w+0x40c),22,(u8)(23-count*3),8,0);}
