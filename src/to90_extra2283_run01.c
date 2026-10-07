#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_window.h"
#pragma force_active on
typedef struct{int type,frame;u32 pad[2];Window*window;u32 unused,height,width,pad2[2],base,bg,palette,pad3,tile,colour;} Descriptor;
u32 ov74_0223547C(Descriptor*,void*);u32 ov74_02235568(BgConfig*,Descriptor*,int,int,void*);
u32 ov74_02235568(BgConfig*bg,Descriptor*d,int x,int y,void*text){u32 result;if(*(u32*)d->window==0){AddWindowParameterized(bg,d->window,(u8)d->bg,(u8)x,(u8)y,(u8)d->height,(u8)d->width,(u8)d->palette,(u16)d->base);result=ov74_0223547C(d,text);}else{if(x!=-1)SetWindowX(d->window,(u8)x);if(y!=-1)SetWindowY(d->window,(u8)y);result=ov74_0223547C(d,text);}if(d->frame==1){switch(d->type){case 0:DrawFrameAndWindow1(d->window,0,(u16)d->tile,(u8)d->colour);break;case 1:DrawFrameAndWindow2(d->window,0,(u16)d->tile,(u8)d->colour);break;case 2:default:CopyWindowToVram(d->window);break;}}return result;}
