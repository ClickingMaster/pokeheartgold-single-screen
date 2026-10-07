#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define G (*(u8*volatile*)0x0380fff4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(u8**)((u8*)(p)+(n)))
#define V(n) (*(volatile u16*)(n))
void WL_WriteByte(void*,u32);void SetParentTbttTxq(void);void WSetPowerState(u32);int CheckFrameTimeout(void*);void ResetTxqPri(u32);void AddTask(u32,u32);void WlIntrTbtt(void);
// @fn WlIntrTbtt
void WlIntrTbtt(void){
 u8*p=G,*s=p+0x344,*c=p+0x31c,*q=p+0x42c,*b;u32 power,i;u8*tx;
 V(0x04808010)=0x4000;
 switch(H(s,12)){
 case 1:{u32 timestamp=V(0x0380fff0);b=P(q,0x80)+0x24+H(s,0x96);WL_WriteByte(b+8,timestamp&255);WL_WriteByte(b+9,(timestamp>>8)&255);if(H(s,14)==1)V(0x04808134)=H(c,0x20)+V(0x04808134)+1;
 {u8*g=G;H(g,0x530)=~H(g,0x52e)|H(g,0x532);}
 {u32 flags=V(0x048080b6);if((flags&0x18)||(flags&6)==2){W(q,0x8c)&=~2u;SetParentTbttTxq();}else W(q,0x8c)|=2;}break;}
 case 2:if(H(s,0x12)==0)V(0x04808134)=0xffff;else V(0x04808134)=H(c,0x20)+V(0x04808134)+1;if(H(s,0x1a)==2)WSetPowerState(2);
 case 3:
 if(H(s,8)!=64)power=1;else{power=0;if(H(s,0x72)==1)power=1;if(H(s,0x14)){if(H(s,0x76)==1||(H(s,0x76)==0&&H(s,0x74)==1))power=1;}}
 if(power)V(0x04808038)|=1;else V(0x04808038)&=~1u;
 if(V(0x04808118)>10)V(0x04808048)=0;
 H(s,0x72)--;if(H(s,0x72)==0)H(s,0x72)=H(s,0x70);
 {u16 old=H(s,0x76);H(s,0x76)=old-1;if(old==0)H(s,0x76)=H(s,0x74)-1;}
 for(i=0;i<2;i++){tx=q+20*i;if(H(tx,0)){b=P(tx,12);if(H(b,8)==0&&CheckFrameTimeout(b)){ResetTxqPri(i);H(P(tx,8),0)=2;AddTask(0,14);H(q,0xae)++;}}}
 V(0x048080ae)=13;break;
 }
}
