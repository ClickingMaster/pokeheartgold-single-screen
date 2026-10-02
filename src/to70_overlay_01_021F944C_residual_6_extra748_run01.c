#pragma force_active on
#include "global.h"
extern void*ov01_021FA1F4(void*);extern u32*ov01_021FA1FC(void*);extern int ov01_021FA22C(void*),ov01_021FA234(void*),ov01_021F9FCC(void*,u32,int);extern void GF3dGfxRawResMan_FreeObjById(void*,u32),ov01_021FA4F0(void*,u32),ov01_021FA6A4(void*,u32);
void ov01_021F9E30(void*,void*);
void ov01_021F9E30(void*p,void*w){u32 id;void*mgr=ov01_021FA1F4(w);u32 array=(u32)ov01_021FA1FC(w);u32 offset=(u32)ov01_021FA22C(w)<<2;u32*cur=(u32*)(array+offset);int n=ov01_021FA234(w);do{id=*cur;if(id!=65535&&ov01_021F9FCC(p,id,0)==0){GF3dGfxRawResMan_FreeObjById(mgr,id);ov01_021FA4F0(w,id);ov01_021FA6A4(w,id);*cur=65535;}cur++;n--;}while(n!=0);}
