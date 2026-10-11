#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(w,n) (*(void**)((u8*)(w)+(n)))
#define U(w,n) (*(u32*)((u8*)(w)+(n)))
typedef struct{u8 pad[20];int mode,id;u8 tail[8];}Item;
extern int ov40_0222C15C(void*);
extern void ov40_0222D294(void*,s16*,s16*),OrbitCursor_SetPosition(void*,s16,s16),OrbitCursor_SetTarget(void*,s16,s16),ov40_0222BF80(void*,int),sub_02087A84(void*,u32,u32),ov40_0222C434(void*),OrbitCursor_SetVisible(void*,int),ov40_0222DFE8(void*,int),ov40_0222DE40(void*);
void ov40_0222C23C(u32,int,void*);
void ov40_0222C23C(u32 action,int reason,void*w){struct{s16 y,x;}pos;if(reason!=0)return;U(w,0x6e4)=action;if(!ov40_0222C15C(w))return;if(action>4)return;ov40_0222D294(P(w,0x5fc+action*40),&pos.x,&pos.y);OrbitCursor_SetPosition(P(w,0x6f0),(s16)(pos.x+16),pos.y);OrbitCursor_SetTarget(P(w,0x6f0),(s16)(pos.x+16),pos.y);U(w,0x86c)=((Item*)P(w,0x818))[U(w,0x6e4)].id;ov40_0222BF80(w,((Item*)P(w,0x818))[U(w,0x6e4)].mode);if(((Item*)P(w,0x818))[U(w,0x6e4)].mode==2)sub_02087A84(P(w,0x868),0,0);else{ov40_0222C434(w);OrbitCursor_SetVisible(P(w,0x6f0),1);}if(U(w,0x86c)==999){ov40_0222DFE8(w,193);return;}if(U(w,0x86c)==9999){ov40_0222DFE8(w,301);return;}if(U(w,0x89c)){ov40_0222DE40(w);U(w,0x89c)=0;}}
