#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8**)(0x0380fff4))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(u8**)((u8*)(p)+(o)))
#define IO(a) (*(volatile u16*)(a))
typedef void(*W37030Fn)(void*,u32);
#define F(p,o) (*(W37030Fn*)((u8*)(p)+(o)))
extern void TxqEndData(void*,u32);
extern void TxqEndManCtrl(void*,u32);
extern void TxqEndPsPoll(void*,u32);
extern void TxqEndBroadCast(void*,u32);
extern void __VENEER_MIi_CpuClear32(u32,void*,u32);
extern void MakeBeaconFrame(void);
void InitTxCtrl(void);
// @fn InitTxCtrl
void InitTxCtrl(void){
u8*base=ROOT;u8*q=base+0x42c;u8*state=base+0x344;u8*cam=base+0x31c;
__VENEER_MIi_CpuClear32(0,q,0xb0);
H(base,0x42c)=0;W(q,12)=0;H(q,20)=0;W(q,32)=0;H(q,40)=0;W(q,52)=0;H(q,0xa2)=0xffff;H(q,0xa4)=0xffff;
switch(H(state,12)){
case 0:
W(q,8)=0x04804170;W(q,28)=0x04804028;W(q,48)=0x04804000;
F(q,16)=TxqEndData;F(q,36)=TxqEndManCtrl;F(q,56)=TxqEndPsPoll;
IO(0x04804024)=0xb6b8;IO(0x04804026)=0x1d46;IO(0x0480416c)=0xb6b8;IO(0x0480416e)=0x1d46;IO(0x04804790)=0xb6b8;IO(0x04804792)=0x1d46;
H(state,0x8a)=8;IO(0x048080ae)=1;break;
case 1:
W(q,8)=0x04804aa0;W(q,28)=0x04804958;W(q,48)=0x04804334;
F(q,16)=TxqEndData;F(q,36)=TxqEndManCtrl;F(q,56)=TxqEndBroadCast;
W(q,0x80)=0x04804238;W(q,0x44)=0x04804000;
IO(0x04804234)=0xb6b8;IO(0x04804236)=0x1d46;IO(0x04804330)=0xb6b8;IO(0x04804332)=0x1d46;IO(0x04804954)=0xb6b8;IO(0x04804956)=0x1d46;
IO(0x04804a9c)=0xb6b8;IO(0x04804a9e)=0x1d46;IO(0x048050c0)=0xb6b8;IO(0x048050c2)=0x1d46;
H(state,0x8a)=0x208;W(q,0x80)=0x04804238;MakeBeaconFrame();break;
case 2:
W(q,8)=0x048045d8;W(q,28)=0x04804490;W(q,48)=0x04804468;
F(q,16)=TxqEndData;F(q,36)=TxqEndManCtrl;F(q,56)=TxqEndPsPoll;
W(q,0x58)=0x04804000;W(q,0x6c)=0x04804234;
IO(0x04804230)=0xb6b8;IO(0x04804232)=0x1d46;IO(0x04804464)=0xb6b8;IO(0x04804466)=0x1d46;IO(0x0480448c)=0xb6b8;IO(0x0480448e)=0x1d46;
IO(0x048045d4)=0xb6b8;IO(0x048045d6)=0x1d46;IO(0x04804bf8)=0xb6b8;IO(0x04804bfa)=0x1d46;
H(state,0x8a)=0x108;IO(0x048080ae)=13;break;
case 3:
W(q,8)=0x04804170;W(q,28)=0x04804028;W(q,48)=0x04804000;
F(q,16)=TxqEndData;F(q,36)=TxqEndManCtrl;F(q,56)=TxqEndPsPoll;
IO(0x04804024)=0xb6b8;IO(0x04804026)=0x1d46;IO(0x0480416c)=0xb6b8;IO(0x0480416e)=0x1d46;IO(0x04804790)=0xb6b8;IO(0x04804792)=0x1d46;
H(state,0x8a)=0x108;IO(0x048080ae)=13;break;
}
if(H(cam,24))H(state,0x8a)|=0x4000;
}
