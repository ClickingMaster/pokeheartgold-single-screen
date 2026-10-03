#pragma force_active on
#include "global.h"
#include "list_menu.h"
#include "msgdata.h"
typedef struct {u8 pad[0x2a04];MsgData *msg;u8 pad2[0x1c0];ListMenu *menu;ListMenuItem *items;} W;
typedef struct {u32 msg,index;} Entry;
extern const ListMenuTemplate ov74_0223C320;
void ov74_0222D11C(W *,Entry *,int,Window *,int);
#pragma force_active on
void ov74_0222D11C(W *w,Entry *entries,int count,Window *window,int above){int i;ListMenuTemplate t;if(w->items)ListMenuItems_Delete(w->items);if(w->menu)DestroyListMenu(w->menu,0,0);w->items=ListMenuItems_New(count,(enum HeapID)85);w->msg=NewMsgDataFromNarc((MsgDataLoadType)0,(NarcId)27,247,(enum HeapID)85);i=0;if(count>0){do{ListMenuItems_AppendFromMsgData(w->items,w->msg,entries->msg,entries->index);i++;entries++;}while(i<count);}DestroyMsgData(w->msg);t=ov74_0223C320;t.items=w->items;t.totalItems=count;t.window=window;{ListMenuTemplate*ptr=&t;w->menu=ListMenuInit(ptr,0,(u16)above,(enum HeapID)85);}}
