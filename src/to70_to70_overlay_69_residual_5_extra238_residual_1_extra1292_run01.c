#pragma force_active on
#include "global.h"
typedef struct{void *items,*callback,*print,*window;u16 total,max;u32 format[3];} Template;
typedef struct{u32 heap;u8 p[0xc010-4];void *bg;u8 p2[0x50];void *menu,*items;} Work;
extern void AddWindow(void *,void *),ListMenuItems_AppendFromMsgData(void *,void *,int,int),DestroyMsgData(void *),ov69_021E68D8(void),DrawFrameAndWindow1(void *,int,int,int),CopyWindowToVram(void *);
extern void *NewMsgDataFromNarc(int,int,int,int),*ListMenuItems_New(u32,int),*ListMenuInit(Template *,int,int,u8);
void ov69_021E6994(Work *,void *,int,Template *,int,u8 *,u32);
void ov69_021E6994(Work *w,void *window,int unused,Template *src,int narc,u8 *ids,u32 count){Template t;void *msg;u32 i;AddWindow(w->bg,window);msg=NewMsgDataFromNarc(0,27,narc,w->heap);w->items=ListMenuItems_New(count,w->heap);for(i=0;i<count;i++)ListMenuItems_AppendFromMsgData(w->items,msg,ids[i],i);DestroyMsgData(msg);t=*src;t.items=w->items;t.total=count;t.window=window;t.callback=ov69_021E68D8;w->menu=ListMenuInit(&t,0,0,w->heap);DrawFrameAndWindow1(t.window,1,0x1d9,7);CopyWindowToVram(window);}
