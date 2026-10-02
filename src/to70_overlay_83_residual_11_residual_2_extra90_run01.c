#pragma force_active on
#include "global.h"
#pragma force_active on
#define PTR(w,o) (*(void**)((w)+(o)))
extern const u16 ov83_02247D12[],ov83_02247F88[],ov83_02247D24[],ov83_02247EE0[];
extern void *NewMsgDataFromNarc(int,int,int,int),*ListMenuItems_New(int,int),*ov83_02247B7C(void*);
extern int ov83_0224777C(void*,int,int);
extern void ListMenuItems_AppendFromMsgData(void*,void*,int,int),DestroyMsgData(void*),ov83_0224755C(void*,int),ov83_02240528(void*,int),ov83_02240664(void*),ov83_02240748(void*),ov83_022407FC(void*),ov83_0224088C(void*),ov83_022408E0(void*,int),ov83_02240984(void*),ov83_0224759C(void*,int,int);
void ov83_02240B54(u8*,int);
void ov83_02240B54(u8*w,int mode){void *msg=NewMsgDataFromNarc(1,27,222,107);int index=ov83_0224777C(PTR(w,0x50c),w[9],1);const u16*table;if(mode==6){w[0x861]=ov83_02247D12[index-1];table=ov83_02247F88;}else{w[0x861]=ov83_02247D24[index-1];table=ov83_02247EE0;}PTR(w,0x4dc)=ListMenuItems_New(w[0x861],107);for(u16 i=0;i<w[0x861];i++)ListMenuItems_AppendFromMsgData(PTR(w,0x4dc),msg,table[i],i);DestroyMsgData(msg);PTR(w,0x85c)=ov83_02247B7C(w);ov83_0224755C(PTR(w,0x77c),1);ov83_02240528(w,mode);ov83_02240664(w);ov83_02240748(w);ov83_022407FC(w);ov83_0224088C(w);ov83_022408E0(w,0);ov83_02240984(w);ov83_0224759C(PTR(w,0x780),48,72);}
