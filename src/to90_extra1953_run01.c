#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define P(p,o) (*(u8**)((u8*)(p)+(o)))
typedef struct{u8 pad[0x1340];u8 childData[15][22];u16 memberSlots[15];u8 pieceData[15][4];u32 callback;u32 states[15];u8 reserved[2];s8 entry[15];u8 count;u16 mask;u8 padding[0x1754-0x1538];u16 pieces[15];}Parent;
typedef struct{u8 pad[0x1d4c];u16 status,member,pending;u8 active;}Group;
extern u8*mbc;extern void MBi_CommChangeParentState(u16,int,void*),MBi_ClearParentPieceBuffer(u16),MBi_CommParentRecvData(void*),MBi_CommParentSendData(void),MB_UpdateGameInfoMember(void*,void*,u16,u16),MB_SendGameInfoBeacon(int,int,int),MBi_CommCallParentError(int,int);extern int MBi_GetGgid(void),MBi_GetTgid(void),MBi_GetAttribute(void);
void MBi_CommParentCallback(int,void*);
static inline int groupOffset(u8 entry,int stride){return stride*entry;}
#pragma force_active on
void MBi_CommParentCallback(int reason,void*data){u16 child;switch(reason){case 17:break;case 21:MBi_CommChangeParentState(0,1,data);break;case 0:child=H(data,16);if(child&&child<16)MBi_CommChangeParentState(child,2,data);break;case 1:child=H(data,16);if(child&&child<16){u16 id;int entry;((Parent*)mbc)->memberSlots[child-1]=0;MI_CpuFill8(((Parent*)mbc)->pieceData[H(data,16)-1],0,4);MI_CpuFill8(((Parent*)mbc)->childData[H(data,16)-1],0,22);MBi_ClearParentPieceBuffer(H(data,16));((Parent*)mbc)->pieces[H(data,16)-1]=0;child=H(data,16);entry=((Parent*)mbc)->entry[child-1];if(entry!=-1){entry=groupOffset((u8)entry,0x5d4);((Group*)(mbc+entry))->member&=~(1<<child);((Group*)(mbc+entry))->pending|=1<<child;((Parent*)mbc)->entry[child-1]=-1;((Group*)(mbc+entry))->status&=~(1<<child);}id=H(data,16);if(((Parent*)mbc)->mask&(1<<id)){((Parent*)mbc)->count--;((Parent*)mbc)->mask&=~(1<<H(data,16));}child=H(data,16);if(((Parent*)mbc)->states[child-1]==8)MBi_CommChangeParentState(child,9,0);MBi_CommChangeParentState(H(data,16),3,data);((Parent*)mbc)->states[H(data,16)-1]=0;}break;case 3:MBi_CommParentRecvData(data);break;case 25:MBi_CommParentSendData();break;case 28:{u8 i;int g,t,a;for(i=0;i<16;i++){int offset=i*0x5d4;if(((Group*)(mbc+offset))->active&&((Group*)(mbc+offset))->pending){MB_UpdateGameInfoMember(mbc+0x186c + offset,mbc+0x1340,((Group*)(mbc+offset))->member,((Group*)(mbc+offset))->pending);((Group*)(mbc+offset))->pending=0;}}g=MBi_GetGgid();t=MBi_GetTgid();a=MBi_GetAttribute();MB_SendGameInfoBeacon(g,t,a);break;}case 255:switch(H(data,2)){case 1:case 4:case 5:case 6:case 8:case 9:MBi_CommCallParentError(0,9);break;case 15:default:MBi_CommCallParentError(0,8);break;}break;case 256:switch(H(data,0)){case 0:case 7:case 8:case 13:case 14:case 15:case 17:case 18:case 21:case 25:case 29:MBi_CommCallParentError(0,9);break;case 28:default:MBi_CommCallParentError(0,8);break;}break;}if(reason==17){void(*callback)(int,int,void*)=(void*)((Parent*)mbc)->callback;MIi_CpuClearFast(0,(u32*)mbc,0x7d00);mbc=0;if(callback)callback(0,12,0);}}
