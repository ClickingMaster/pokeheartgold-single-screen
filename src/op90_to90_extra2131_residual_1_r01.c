#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 pad[3],busy;void(*callback)(int,void*);u32 *first,*second;void*user;u32 operation,phase;void(*notify)(void);} State;
extern State _021E3778;
int RtcBCD2HEX(u32);
RTCWeek RTC_GetDayOfWeek(const RTCDate*);
BOOL RTCi_WriteRawStatus2Async(void);
void RtcCommonCallback(int,u32,BOOL);
#define DATE (*(volatile u32*)0x027ffde8)
#define TIME (*(volatile u32*)0x027ffdec)
typedef struct {u16 intr1:4;u16 pad1:2;u16 intr2:1;u16 pad2:9;} RtcStatus2;
#define ST (*(RtcStatus2*)0x027ffdea)
#define BITS(v,l,r) (((u32)(v)<<(l))>>(r))
// @fn RtcCommonCallback
void RtcCommonCallback(int tag,u32 data,BOOL error){int result;u8 status;u32 *p;void(*cb)(int,void*);
 if(error){if(_021E3778.phase)_021E3778.phase=0;if(_021E3778.busy)_021E3778.busy=0;cb=_021E3778.callback;if(cb){_021E3778.callback=NULL;cb(6,_021E3778.user);}return;}
 status=(u8)data;
 if((u8)((data&0x7f00)>>8)==0x30){if(_021E3778.notify)_021E3778.notify();return;}
 if(status==0){result=0;switch(_021E3778.operation){
 case 0:p=_021E3778.first;p[0]=RtcBCD2HEX(BITS(DATE,24,24));p[1]=RtcBCD2HEX(BITS(DATE,19,27));p[2]=RtcBCD2HEX(BITS(DATE,10,26));p[3]=RTC_GetDayOfWeek((RTCDate*)p);break;
 case 1:p=_021E3778.first;p[0]=RtcBCD2HEX(BITS(TIME,26,26));p[1]=RtcBCD2HEX(BITS(TIME,17,25));p[2]=RtcBCD2HEX(BITS(TIME,9,25));break;
 case 2:p=_021E3778.first;p[0]=RtcBCD2HEX(DATE&255);p[1]=RtcBCD2HEX(BITS(DATE,19,27));p[2]=RtcBCD2HEX(BITS(DATE,10,26));p[3]=RTC_GetDayOfWeek((RTCDate*)p);{u32 t=BITS(TIME,26,26);p=_021E3778.second;p[0]=RtcBCD2HEX(t);}p[1]=RtcBCD2HEX(BITS(TIME,17,25));p[2]=RtcBCD2HEX(BITS(TIME,9,25));break;
 case 3:case 4:case 5:case 11:case 12:case 13:case 14:case 15:break;
 case 6:p=_021E3778.first;switch(ST.intr1){case 4:p[0]=1;break;default:p[0]=0;break;}break;
 case 7:p=_021E3778.first;if(ST.intr2)p[0]=1;else p[0]=0;break;
 case 8:p=_021E3778.first;p[0]=BITS(TIME,29,29);p[1]=RtcBCD2HEX(BITS(TIME,18,26));p[2]=RtcBCD2HEX(BITS(TIME,9,25));p[3]=0;if(BITS(TIME,24,31))p[3]+=1;if(BITS(TIME,16,31))p[3]+=2;if(BITS(TIME,8,31))p[3]+=4;break;
 case 9:if(_021E3778.phase==0){if(*_021E3778.first==1){if(ST.intr1!=4){_021E3778.phase++;ST.intr1=4;if(!RTCi_WriteRawStatus2Async()){_021E3778.phase=0;result=3;}}}else if(ST.intr1){_021E3778.phase++;ST.intr1=0;if(!RTCi_WriteRawStatus2Async()){_021E3778.phase=0;result=3;}}}else _021E3778.phase=0;break;
 case 10:if(_021E3778.phase==0){if(*_021E3778.first==1){if(!ST.intr2){_021E3778.phase++;ST.intr2=1;if(!RTCi_WriteRawStatus2Async()){_021E3778.phase=0;result=3;}}}else if(ST.intr2){_021E3778.phase++;ST.intr2=0;if(!RTCi_WriteRawStatus2Async()){_021E3778.phase=0;result=3;}}}else _021E3778.phase=0;break;
 default:_021E3778.phase=0;result=4;break;}}
 else{_021E3778.phase=0;switch(status){case 1:result=4;break;case 2:result=5;break;case 3:result=1;break;case 0:case 4:default:result=6;break;}}
 if(_021E3778.phase)return;if(_021E3778.busy)_021E3778.busy=0;cb=_021E3778.callback;if(cb){_021E3778.callback=NULL;cb(result,_021E3778.user);}
}
