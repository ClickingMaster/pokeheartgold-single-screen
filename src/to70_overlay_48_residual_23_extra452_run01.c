#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Limits{s16 update0,update1,left0,right0,left1,right1;}Limits;typedef struct W{u8 pad[360];u8 windows[3][16];u8 extra[16];}W;extern void AddWindowParameterized(void*,void*,int,u8,u8,u8,u8,int,u16),FillWindowPixelBuffer(void*,int),CopyWindowToVram(void*);void ov48_0225A680(void*,Limits*),ov48_0225A6DC(W*,void**);
static inline void SetWindow(u32 left,u32 right,vu16*regs){regs[0]=255;regs[2]=((left<<8)&0xff00)|(u8)right;}
void ov48_0225A680(void*task,Limits*p){if(p->update0){SetWindow(p->left0,p->right0,(vu16*)0x04001040);p->update0=0;}if(p->update1){SetWindow(p->left1,p->right1,(vu16*)0x04001042);p->update1=0;}}
void ov48_0225A6DC(W*w,void**context){int i;int tile;int y;u8(*window)[16];tile=148;i=0;y=i;window=w->windows;for(;i<3;i++,y+=6,window++){AddWindowParameterized(*context,window,5,0,(u8)y,27,6,14,(u16)tile);tile+=162;FillWindowPixelBuffer(window,0);CopyWindowToVram(window);}AddWindowParameterized(*context,w->extra,5,0,0,27,6,14,148);FillWindowPixelBuffer(w->extra,0);}
