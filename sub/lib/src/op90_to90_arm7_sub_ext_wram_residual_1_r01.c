#pragma force_active on
#include <nitro/types.h>
#include <nitro/hw/consts_shared.h>
#include <nitro/os/tick.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
typedef struct WMSPStatus {
 u16 state;u8 pad002[10];
 u32 mp;u32 w10;u32 w14;u32 pad018;u32 w1c;u8 pad020[0x10];
 u16 parentSize,childSize;u8 pad034[0x10];
 u16 parentInterval,childInterval;u64 parentTick,childTick;u8 pad058[6];
 u16 x5e,x60;s16 x62,x64;u16 x66;u8 pad068[8];
 u16 x70,x72;u8 *mpBuf[2];u8 pad07c[6];
 u16 x82,x84,x86,x88,x8a,x8c,x8e,x90;u8 pad092[0x1c];
 u16 xae;u8 *rxBuf[2];u16 xb8,xba,xbc,xbe,xc0,xc2;u8 pad0c4[0x32];
 u16 xf6;u8 pad0f8[0x30];
 u8 mac[15][6];u16 x182;u8 pad184[4];u16 aid;u8 bssid[6];u8 pad190[6];
 u16 x196;u32 x198;u8 x19c[80];u8 pad1ec[0xc];
 u8 x1f8[16][16];u8 pad2f8[0x738-0x2f8];
 u64 tick[16];u64 timeout;u8 pad7c0[0xc];u16 x7cc;
} WMSPStatus;
typedef char WMSPStatusCheck[(sizeof(WMSPStatus)==0x7d0)?1:-1];
typedef struct WMSPWork {u8 pad0[0x28];u8 indicateQ[0x30],forwardQ[0x30],requestQ[0x30];u8 pad0b8[0x154c-0xb8];volatile u32 active;WMSPStatus *volatile status;u8 pad1554[0x3c];} WMSPWork;
extern WMSPWork wmspW;
extern u8 wmspMPAckAlarm[],wmspMPIntervalAlarm[];
u32 __VENEER_OS_DisableInterrupts(void);u32 __VENEER_OS_RestoreInterrupts(u32);
int __VENEER_OS_ReceiveMessage(void *,void *,int);int __VENEER_OS_SendMessage(void *,void *,int);void __VENEER_OS_ExitThread(void);
void __VENEER_MI_CpuCopy8(const void *,void *,u32);void __VENEER_MI_CpuFill8(void *,u8,u32);void __VENEER_MIi_CpuCopy16(const void *,void *,u32);void __VENEER_MIi_CpuClear16(u32,void *,u32);
void *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);void __VENEER_WMSP_ReturnResult2Wm9(void *);
u64 __VENEER_OS_GetTick(void);void __VENEER_OS_CancelAlarm(void *);void __VENEER_OS_SetAlarm(void *,u64,void (*)(void *),void *);
void WMSP_CleanSendQueue(u16);void WMSP_CancelVAlarm(void);void WMSP_SetThreadPriorityLow(void);void WMSP_ResetSizeVars(void);
u32 *WMSP_GetInternalRequestBuf(void);u8 WMSP_GetRssi8_06001B5C(u8);void WMSP_AddRssiToList(u8);u16 WMSP_GetAverageLinkLevel(void);int WMSP_CheckMacAddress(void *);
void WmspMaMultiPollAckAlarmCallback(void *);void WmspMPParentIntervalAlarmCallback(void *);void WmspMPChildIntervalAlarmCallback(void *);
void WMSP_SetChildSize(u16);int WMSP_FlushSendQueue(int,u16);void WMSP_ParsePortPacket(u16,u16,void *,u16,void *);void WMSP_RequestResumeMP(void);
void WmspKickMPParent(u16);void WmspKickMPChild(void);void WmspFreeBufOfWL(void *);
void WMSP_IndicateThread(void *);
// @fn WMSP_IndicateThread
void WMSP_IndicateThread(void *arg) {
/*DECL*/
 WMSPWork *w;
 void *received;
 u8 *buf_9;
 u8 *buf_10;
 u8 *buf_11;
 u16 rcv;
 int ok;
 u32 *req;
 int i;
 u16 aid85;
 u32 irq;
 u16 mode;
 u32 minimum;
 u32 seed;
 u32 len;
 int size;
 int wasMP;
 u16 *cb_1;
 u16 *cb_2;
 u16 *cb_3;
 u16 *cb_4;
 u16 *cb_5;
 u16 *cb_6;
 u16 *cb_7;
 u16 *cb_8;
 u16 *cb_9;
 u16 *cb_10;
 u16 *cb_11;
 u16 *cb_12;
 u16 *cb_13;
 u32 level;
 int timeout;
 int flushed;
 int canKick;
 int retry;
 int ack;
 u8 *child;
 int hasData;
 u16 bitmap;
 u16 aid86;
 int cancelled;
 u32 rssi;
 u16 ordinal;
 u16 kind;
 u16 aid84;
 u16 oldFlag;
 u32 missing;
 void *message;
 int j;
 u8 *data;
 u64 last;
 WMSPStatus *cur;
 u16 command;
 int resume;
 u64 tick;
 u32 bit;
 int accept;
 u16 more;
 vu16 *timer;
 u8 mac[6];
 u16 current;
 u16 upper;
 WMSPStatus *st;
 u16 old;
/*ENDDECL*/
 w=&wmspW;
 for(;;) {
  __VENEER_OS_ReceiveMessage((u8 *)w+0x28,&received,1);message=received;
  if(!message){__VENEER_OS_ExitThread();return;}
  kind=H(message,12)&0xff00;command=H(message,12)&0xff;
  if((kind!=0 && kind!=0x100) || !(command&0x80))goto forward;
  if(!wmspW.active)continue;
  st=wmspW.status;mode=st->state;if(mode==1)continue;
  switch(((u16 *)message)[6]) {
  case 0x84:
   cb_1=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_1[0]=0x80;cb_1[1]=0;cb_1[2]=0x13;__VENEER_WMSP_ReturnResult2Wm9(cb_1);break;
  case 0x85:
   if(mode==7 || mode==9) {
    __VENEER_MI_CpuCopy8((u8 *)message+16,mac,6);
    aid85=0;
    for(i=0;i<15;i++) {
     irq=__VENEER_OS_DisableInterrupts();
     if((st->x182&(1<<(i+1))) && mac[0]==st->mac[i][0] && mac[1]==st->mac[i][1] && mac[2]==st->mac[i][2] && mac[3]==st->mac[i][3] && mac[4]==st->mac[i][4] && mac[5]==st->mac[i][5]) {
      aid85=(u16)(i+1);st->x182&=~(1<<aid85);st->x86&=~(1<<aid85);st->tick[aid85]=0;
      __VENEER_MI_CpuFill8(st->mac[i],0,6);__VENEER_OS_RestoreInterrupts(irq);break;
     }
     __VENEER_OS_RestoreInterrupts(irq);
    }
    if(!aid85)break;
    cb_2=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_2[0]=8;cb_2[1]=0;cb_2[4]=9;cb_2[9]=H(message,22);cb_2[8]=aid85;
    __VENEER_MI_CpuCopy8((u8 *)message+16,cb_2+5,6);cb_2[22]=st->parentSize;cb_2[23]=st->childSize;__VENEER_WMSP_ReturnResult2Wm9(cb_2);
    if(st->mp==1)WMSP_CleanSendQueue((u16)(1<<aid85));
   }else {
    wasMP=0;
    irq=__VENEER_OS_DisableInterrupts();
    if(!st->x182){__VENEER_OS_RestoreInterrupts(irq);break;}
    if(st->mp==1){st->mp=0;wasMP=1;WMSP_CancelVAlarm();WMSP_SetThreadPriorityLow();}
    st->x182=0;st->x86=0;st->w14=0;st->w10=0;st->w1c=0;st->x198=0;st->x196=0;
    __VENEER_MI_CpuFill8(st->x19c,0,80);WMSP_ResetSizeVars();st->xc2=0;st->state=3;__VENEER_OS_RestoreInterrupts(irq);
    cb_2=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_2[0]=12;cb_2[1]=0;cb_2[4]=9;cb_2[6]=H(message,22);cb_2[5]=st->aid;
    __VENEER_MI_CpuCopy8(st->bssid,cb_2+8,6);cb_2[11]=st->parentSize;cb_2[12]=st->childSize;__VENEER_WMSP_ReturnResult2Wm9(cb_2);
    if(wasMP)WMSP_CleanSendQueue(1);
   }
   break;
  case 0x86: {
   aid86=H(message,22);
   if(aid86==0 || aid86>=16)break;
   if(!st->xf6) {
    req=WMSP_GetInternalRequestBuf();
    if(!req)ok=0;
    else {req[0]=0x22;__VENEER_MI_CpuCopy8((u8 *)message+16,req+1,6);ok=__VENEER_OS_SendMessage(wmspW.requestQ,req,0);}
    if(!ok){cb_3=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_3[0]=0x80;cb_3[1]=8;cb_3[2]=0x16;cb_3[3]=0x22;__VENEER_WMSP_ReturnResult2Wm9(cb_3);}
    break;
   }
   irq=__VENEER_OS_DisableInterrupts();st->x182|=1<<aid86;st->x86&=~(1<<aid86);
   st->tick[aid86]=__VENEER_OS_GetTick()|1;
   __VENEER_MI_CpuCopy8((u8 *)message+16,st->mac[aid86-1],6);__VENEER_OS_RestoreInterrupts(irq);
   __VENEER_MIi_CpuClear16(1,st->x1f8[aid86],16);
   cb_3=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_3[0]=8;cb_3[1]=0;cb_3[4]=7;__VENEER_MI_CpuCopy8((u8 *)message+16,cb_3+5,6);cb_3[8]=aid86;
   __VENEER_MIi_CpuCopy16((u8 *)message+34,cb_3+10,24);cb_3[22]=st->parentSize;cb_3[23]=st->childSize;__VENEER_WMSP_ReturnResult2Wm9(cb_3);break;
  }
  case 0x87:cb_4=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_4[0]=0x80;cb_4[1]=0;cb_4[2]=0x12;__VENEER_WMSP_ReturnResult2Wm9(cb_4);break;
  case 0x88:cb_5=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_5[0]=0x80;cb_5[1]=0;cb_5[2]=0x11;__VENEER_WMSP_ReturnResult2Wm9(cb_5);break;
  case 0x8b:
   if(st->xc2){cb_6=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_6[0]=12;cb_6[1]=0;cb_6[4]=8;__VENEER_WMSP_ReturnResult2Wm9(cb_6);}break;
  case 0x8c:
   if(st->xc2){cb_7=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_7[0]=8;cb_7[1]=0;cb_7[4]=2;__VENEER_WMSP_ReturnResult2Wm9(cb_7);}break;
  case 0x8d: {
   seed=WMSP_GetRssi8_06001B5C(B(message,31));
   seed^=*(vu16 *)0x027fff98<<1;seed^=seed>>16;*(vu16 *)0x027fff98=(u16)seed;
   if(st->state!=8 && st->state!=10)break;
   if(st->xba!=H(message,0x44)) {
    req=WMSP_GetInternalRequestBuf();
    if(!req)ok=0;
    else {req[0]=0x25;req[1]=1;req[2]=0x8002;ok=__VENEER_OS_SendMessage(wmspW.requestQ,req,0);}
    if(!ok){cb_8=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_8[0]=0x80;cb_8[1]=8;cb_8[2]=0x16;cb_8[3]=0x25;__VENEER_WMSP_ReturnResult2Wm9(cb_8);}
    break;
   }
   if(!wmspW.status->xc2)break;
   cb_8=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_8[0]=0x80;cb_8[1]=0;cb_8[2]=0x10;cb_8[3]=H(message,0x44);cb_8[4]=st->state;cb_8[5]=H(message,22);
   if(cb_8[5]<=128)__VENEER_MIi_CpuCopy16((u8 *)message+0x3c,cb_8+6,(cb_8[5]+1)&~1);
   __VENEER_WMSP_ReturnResult2Wm9(cb_8);break;
  }
  case 0x180: {
   if(!st->w10)break;
   WMSP_AddRssiToList(WMSP_GetRssi8_06001B5C(B(message,31)));st->xbc=WMSP_GetAverageLinkLevel();
   if(WMSP_CheckMacAddress((u8 *)message+0x2e)==1)break;
   if(H(message,22)>0x5e4)break;
   st->xae^=1;buf_9=st->rxBuf[st->xae];
   __VENEER_MI_CpuCopy8((u8 *)message+16,buf_9,(H(message,22)+0x2d)&~1);__VENEER_MI_CpuCopy8((u8 *)message+0x28,buf_9+0x18,6);__VENEER_MI_CpuCopy8((u8 *)message+0x2e,buf_9+0x1e,6);
   cb_9=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_9[0]=0x11;cb_9[1]=0;cb_9[2]=15;*(u8 **)(cb_9+4)=buf_9;__VENEER_WMSP_ReturnResult2Wm9(cb_9);break;
  }
  case 0x182: {
   rssi=WMSP_GetRssi8_06001B5C(B(message,31));if(st->xbe>rssi)st->xbe=(u16)rssi;
   if(!st->mp)break;
   if(st->x60==1)st->x60=0;
   oldFlag=st->x8e;st->x70^=1;buf_10=st->mpBuf[st->x70];len=H(message,22)+0x30;if(st->x72<len)len=st->x72;
   __VENEER_MI_CpuCopy8((u8 *)message+16,buf_10,len);
   irq=__VENEER_OS_DisableInterrupts();cancelled=0;
   if(st->x84==1){cancelled=1;__VENEER_OS_CancelAlarm(wmspMPAckAlarm);}
   st->x84=1;st->x82=H(buf_10,10);more=H(message,24);hasData=(more&0x2000)?1:0;st->x90=(u16)hasData;
   __VENEER_OS_SetAlarm(wmspMPAckAlarm,OS_MilliSecondsToTicks((s64)(((u16)(H(buf_10,10)-H(buf_10,12))+128)<<4))/1024,WmspMaMultiPollAckAlarmCallback,0);
   ack=(more&0x2800)==0x2800;retry=(more&0x6000)==0x6000;if(retry)st->x8a=0;st->x8e=ack?1:0;st->x8c=retry?1:0;
   if(hasData){size=(H(buf_10,44)-102)/4-32;if(size>=0){if(size>512)size=512;if(size!=st->childSize)WMSP_SetChildSize((u16)size);}}
   __VENEER_OS_RestoreInterrupts(irq);
   if(cancelled){if(oldFlag==1)WMSP_FlushSendQueue(cancelled,0);cb_10=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_10[0]=14;cb_10[1]=9;cb_10[2]=13;*(u32 *)(cb_10+4)=0;__VENEER_WMSP_ReturnResult2Wm9(cb_10);}
   if(!hasData){if(H(buf_10,6)>=2)st->x5e=(H(buf_10,0x30)&0x8000)?1:0;break;}
   __VENEER_MI_CpuCopy8((u8 *)message+0x28,buf_10+0x18,6);__VENEER_MI_CpuCopy8((u8 *)message+0x2e,buf_10+0x1e,6);
   if(H(buf_10,6)>=2){H(buf_10,6)-=2;st->x5e=(H(buf_10,0x30)&0x8000)?1:0;cb_10=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_10[0]=14;cb_10[1]=0;cb_10[2]=12;*(u8 **)(cb_10+4)=buf_10;__VENEER_WMSP_ReturnResult2Wm9(cb_10);if(H(buf_10,6))WMSP_ParsePortPacket(0,H(buf_10,0x30),buf_10+0x32,H(buf_10,6),buf_10);}
   else {H(buf_10,6)=0;st->x5e=0;cb_10=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_10[0]=14;cb_10[1]=14;cb_10[2]=12;*(u8 **)(cb_10+4)=buf_10;__VENEER_WMSP_ReturnResult2Wm9(cb_10);}
   if(st->timeout!=0)st->tick[0]=__VENEER_OS_GetTick()|1;
   break;
  }
  case 0x184: {
   resume=0;
   if(!st->mp)break;
   if(H(message,18)!=0 || (st->x66==1 && H(message,16)!=0)){WMSP_RequestResumeMP();break;}
   if(st->x66)st->x66=0;
   buf_11=st->mpBuf[st->x70];len=H(message,22)*H(message,20)+10;if(st->x72<len)len=st->x72;
   __VENEER_MI_CpuCopy8((u8 *)message+16,buf_11,len);
   cur=wmspW.status;minimum=cur->xbe;
   if(H(buf_11,0)==0 && H(message,20)>=1) {
    data=(u8 *)message+26;
    for(j=0;j<H(message,20);j++){level=WMSP_GetRssi8_06001B5C(data[H(message,22)*j+3]);if(level<minimum)minimum=level;}
    cur->xbe=minimum;
   }
   tick=__VENEER_OS_GetTick()|1;missing=H(buf_11,0);child=buf_11+10;
   for(ordinal=0;ordinal<H(buf_11,4);ordinal++,child+=H(buf_11,6)) {
    aid84=H(child,4);rcv=H(child,0);
    if(aid84<1 || aid84>15)continue;
    if(rcv>=2 && rcv!=0xffff) {
     rcv=(u16)(rcv-2);H(child,0)=rcv;st->x86|=1<<aid84;st->tick[aid84]=tick;
     if(rcv)WMSP_ParsePortPacket(aid84,H(child,8),child+10,rcv,buf_11);
    }else if(rcv==0) {
     last=st->tick[aid84];bit=1<<aid84;missing|=1<<aid84;
     if(st->timeout!=0 && last!=0 && tick-last>st->timeout) {
      req=WMSP_GetInternalRequestBuf();st->tick[aid84]=0;
      if(!req)ok=0;
      else {req[0]=0x25;req[1]=bit;req[2]=0x8001;ok=__VENEER_OS_SendMessage(wmspW.requestQ,req,0);}
      if(!ok){cb_11=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_11[0]=0x80;cb_11[1]=8;cb_11[2]=0x16;cb_11[3]=0x25;__VENEER_WMSP_ReturnResult2Wm9(cb_11);}
     }
    }
   }
   WMSP_FlushSendQueue(0,(u16)missing);if(H(buf_11,0))resume=1;
   cb_11=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_11[0]=14;cb_11[1]=0;cb_11[2]=11;*(u8 **)(cb_11+4)=buf_11;__VENEER_WMSP_ReturnResult2Wm9(cb_11);st->x70^=1;
   irq=__VENEER_OS_DisableInterrupts();if(!resume)st->x62--;if(st->x64>0)st->x64--;
   canKick=st->x62>0 && st->x64>0;__VENEER_OS_RestoreInterrupts(irq);
   if(canKick) {
    if(resume==1)bitmap=H(message,16);else bitmap=0xffff;
    if(st->parentInterval){__VENEER_OS_CancelAlarm(wmspMPIntervalAlarm);__VENEER_OS_SetAlarm(wmspMPIntervalAlarm,st->parentTick,WmspMPParentIntervalAlarmCallback,(void *)bitmap);}
    else WmspKickMPParent(bitmap);
   }
   break;
  }
  case 0x185: {
   flushed=0;
   if(!st->mp)break;
   if(!H(message,14)) {
    timer=(vu16 *)0x048080f8;
    timeout=1;old=timer[0];upper=timer[1];current=timer[0];if(old>current)upper=timer[1];
    if((s16)((u16)((upper<<12)|(current>>4))-st->x82)<=0)break;
   }else timeout=0;
   irq=__VENEER_OS_DisableInterrupts();if(!st->x84){__VENEER_OS_RestoreInterrupts(irq);break;}
   st->x84=0;hasData=st->x90;__VENEER_OS_CancelAlarm(wmspMPAckAlarm);__VENEER_OS_RestoreInterrupts(irq);
   retry=st->x8c;accept=1;
   if(retry && (timeout || !(H(message,0x3e)&(1<<st->aid))))accept=0;
   if(retry)st->x8c=0;
   if(st->x8e){st->x8e=0;flushed=WMSP_FlushSendQueue(timeout,accept?1:0);}
   if(!hasData)break;
    cb_12=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_12[0]=14;
    if(timeout)cb_12[1]=9;else if(H(message,0x3e)&(1<<st->aid))cb_12[1]=15;else cb_12[1]=0;
    cb_12[2]=13;*(u32 *)(cb_12+4)=0;
    if(!timeout){cb_12[6]=H(message,28);cb_12[7]=H(message,30);__VENEER_MI_CpuCopy8((u8 *)message+0x28,cb_12+8,6);__VENEER_MI_CpuCopy8((u8 *)message+0x2e,cb_12+11,6);cb_12[14]=H(message,0x3a);cb_12[15]=H(message,0x3c);cb_12[16]=H(message,0x3e);}
    __VENEER_WMSP_ReturnResult2Wm9(cb_12);
   if(hasData) {
    if(flushed==1 || !st->x5e) {
     if(st->childInterval){__VENEER_OS_CancelAlarm(wmspMPIntervalAlarm);__VENEER_OS_SetAlarm(wmspMPIntervalAlarm,st->childTick,WmspMPChildIntervalAlarmCallback,0);}
     else WmspKickMPChild();
    }else{st->x5e=0;st->x60=1;st->x88=0;}
   }
   break;
  }
  case 0x186:
   if(st->x7cc==1 && H(message,16)==32) {
    irq=__VENEER_OS_DisableInterrupts();__VENEER_OS_CancelAlarm(wmspMPAckAlarm);st->x84=0;__VENEER_OS_RestoreInterrupts(irq);
    st->x8a=0;WMSP_FlushSendQueue(1,0);cb_13=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_13[0]=0x80;cb_13[1]=0;cb_13[2]=0x17;cb_13[3]=1;__VENEER_WMSP_ReturnResult2Wm9(cb_13);break;
   }
   req=WMSP_GetInternalRequestBuf();
   if(!req)ok=0;
   else {req[0]=0x25;req[2]=0x8003;if(st->state==9 || st->state==7)req[1]=0x7ffe;else if(st->state==10 || st->state==8)req[1]=1;ok=__VENEER_OS_SendMessage(wmspW.requestQ,req,0);}
   if(!ok){cb_13=__VENEER_WMSP_GetBuffer4Callback2Wm9();cb_13[0]=0x80;cb_13[1]=8;cb_13[2]=0x16;cb_13[3]=0x25;__VENEER_WMSP_ReturnResult2Wm9(cb_13);}
   break;
  default:break;
  }
  WmspFreeBufOfWL(message);continue;
forward:
  __VENEER_OS_SendMessage((u8 *)&wmspW+0x58,message,1);
 }
}
