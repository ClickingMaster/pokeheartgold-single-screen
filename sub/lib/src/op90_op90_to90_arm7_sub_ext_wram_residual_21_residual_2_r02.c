#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u8 wmspW[];
typedef struct {u8 pad[0x54c];u8 *iface;u8 *state;} To90W35376Global;
#define G ((To90W35376Global *)(wmspW+0x1000))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern void __VENEER_PM_SetLEDPattern(u32);
typedef struct{u16 command,result,state,port;void*context;u16*packet;u16 length,sender;u8 gap[6];u16 serial;u8 gap2[4];u16 receiver;u8 gap3[30];u16 sendSize,receiveSize;} PortResult;
void WMSP_ParsePortPacket(u16,u16,u16 *,s32,void *);
// @fn WMSP_ParsePortPacket
void WMSP_ParsePortPacket(u16 sender,u16 header,u16 *data,s32 size,void *context) {
 u8 *s=G->state;u16 *input;int initial;u16 *payloadp;
 if(size==0||(u32)size>0x204||sender==H(s,0x188)||sender>=16||(size&1))return;
 input=data;initial=1;
 while(size>0){u16 bits,*extra;int payload;u32 serial=0xffff;int hasdest,hasseq,len;u16 port;
 {u16 *borrowed;if(initial==1){borrowed=&header;initial=0;}else{borrowed=input++;size-=2;}bits=*borrowed;}
 payload=(bits&255)*2;if(payload==0)payload=512;
 hasdest=(bits&0x1000)?1:0;hasseq=(bits&0x800)?1:0;
 len=payload+(hasseq?2:0)+(hasdest?2:0);size-=len;if(size<0)return;
 port=(bits&0xf00)>>8;payloadp=input;extra=(u16 *)((u8 *)payloadp+payload);input=(u16 *)((u8 *)input+len);
 if(hasseq==1){u32 slot=port&7;u16 old=((u16 *)(s+0x1f8+sender*16))[slot];serial=*extra;extra++;
 if(old&1)((u16 *)(s+0x1f8+sender*16))[slot]=serial*2;else{serial=serial*2;if((u16)(old-serial)<0x100)continue;((u16 *)(s+0x1f8+sender*16))[slot]=serial;}}
 if(hasdest==1 && !(*extra&((1U<<H(s,0x188)))))continue;
 if(payload>0){PortResult*out=(PortResult*)__VENEER_WMSP_GetBuffer4Callback2Wm9();out->command=0x82;out->result=0;out->state=0x15;out->port=port;out->context=context;out->packet=payloadp;out->length=payload;out->sender=sender;out->receiver=H(s,0x188);out->serial=serial;
 {u16 a=H(s,0x30),b=H(s,0x32);out->sendSize=H(s,0x188)==0?a:b;out->receiveSize=H(s,0x188)==0?b:a;}
 __VENEER_WMSP_ReturnResult2Wm9(out);}
 }
}
