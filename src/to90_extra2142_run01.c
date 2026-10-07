#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void(*Callback)(void*);
typedef struct{u8 pad0[4];u8*status;u8 pad8[8];void*receive;u16 pad14,flag;Callback cb[45];Callback portcb[16];void*portuser[16];u32 bitmap;u16 porttype;} Context;
extern u8 _021E4220[],_021E4270[],_021E4284[],_021E4294[];
void WmClearFifoRecvFlag(void);
int WM_Finish(void);
void WmReceiveFifo(int,u32,BOOL);
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
void WmReceiveFifo(int tag,u32 address,BOOL error){Context*c=*(Context**)(_021E4220+4);u8*p=(u8*)address;u16 state,aid,porttype,field;u8*extra;u16 v1,v2,i;u8*mac;Callback cb;
 if(error)return;DC_InvalidateRange(c->receive,256);if(c->flag==0)DC_InvalidateRange(c->status,2048);if(p!=c->receive)DC_InvalidateRange(p,256);
 if(H(p,0)>=44){if(H(p,0)==128){if(H(p,2)==19)OS_Terminate();if(c->cb[44])c->cb[44](p);}else if(H(p,0)==130){if(c->portcb[H(p,6)]){W(p,28)=(u32)c->portuser[H(p,6)];H(p,34)=c->bitmap;DC_InvalidateRange((void*)W(p,8),H(c->status,114));c->portcb[H(p,6)](p);}}else if(H(p,0)==129){H(p,0)=15;cb=(Callback)W(p,28);if(cb)cb(p);}goto end;}
 if(H(p,0)==14&&(u16)(H(p,4)+0xfff5)<=1&&H(p,2)==0)DC_InvalidateRange((void*)W(p,8),H(c->status,114));
 if(H(p,0)==2&&H(p,2)==0){cb=c->cb[H(p,0)];WM_Finish();if(cb)cb(p);return;}
 if(c->cb[H(p,0)]){c->cb[H(p,0)](p);if(H(_021E4220,0)==0)return;}
 if(H(p,0)!=8&&H(p,0)!=12)goto end;
 if(H(p,0)==8){mac=p+10;state=H(p,8);aid=H(p,16);field=H(p,18);extra=p+20;porttype=0;v1=H(p,44);v2=H(p,46);}else if(H(p,0)==12){state=H(p,8);porttype=H(p,10);field=H(p,12);aid=0;extra=NULL;mac=p+16;v1=H(p,22);v2=H(p,24);}
 if(state!=7&&state!=9&&state!=26)goto end;
 if(state==7)c->bitmap|=1<<aid;else c->bitmap&=~(1<<aid);c->porttype=porttype;
 MI_CpuFill8(_021E4270,0,68);H(_021E4220,80)=130;H(_021E4220,82)=0;H(_021E4220,84)=state;W(_021E4220,88)=0;W(_021E4220,92)=0;H(_021E4220,96)=0;H(_021E4220,98)=aid;H(_021E4220,112)=porttype;H(_021E4220,114)=c->bitmap;H(_021E4220,106)=65535;H(_021E4220,140)=field;
 MI_CpuCopy8(mac,_021E4284,6);if(extra)MIi_CpuCopy16((const u16*)extra,(u16*)_021E4294,24);else MIi_CpuClear16(0,(u16*)_021E4294,24);
 H(_021E4220,144)=porttype==0?v1:v2;H(_021E4220,146)=porttype==0?v2:v1;
 for(i=0;i<16;i++){H(_021E4220,86)=i;if(c->portcb[i]){W(_021E4220,108)=(u32)c->portuser[i];c->portcb[i](_021E4270);}}
 end:DC_InvalidateRange(c->receive,256);WmClearFifoRecvFlag();if(p!=c->receive){H(p,0)|=0x8000;DC_StoreRange(p,256);}
}
