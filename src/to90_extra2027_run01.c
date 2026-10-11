#pragma force_active on
#include "global.h"
extern u32 WiFiPlazaObj_GetOwnPlayer(void *),ov49_02258E34(u32),WiFiPlazaMap_GetTileAttr(void *,u16,u16);
extern void WiFiPlazaCamera_ClearFollowObject(void *),ov49_02265980(void *,void *,int,const void *),ov49_0226786C(void *,void *,int,int),ov49_02259130(u32,int),ov49_02266C6C(void *,void *),ov49_02266D04(void *,void *);
extern const u8 ov49_0226A74C[],ov49_0226A750[],ov49_0226A754[];
void ov49_02266B28(void *,void *,int);
#pragma force_active on
void ov49_02266B28(void *context,void *argument,int count){
 u8 *local=argument;u32 packed;struct Coordinate{s16 x,y;} copy,original;
 if(*(u32 *)(local+8)==WiFiPlazaObj_GetOwnPlayer(*(void **)((u8 *)context+4)))WiFiPlazaCamera_ClearFollowObject(*(void **)((u8 *)context+8));
 packed=ov49_02258E34(*(u32 *)(local+8));original.x=(s16)packed;original.y=(s16)(packed>>16);copy=original;
 local[0x965]=WiFiPlazaMap_GetTileAttr(*(void **)((u8 *)context+12),(u16)(copy.x/16),(u16)(copy.y/16))==42;
 switch(count){
 case 1:ov49_02265980(context,local,0,ov49_0226A74C);ov49_0226786C(context,local,0,0);break;
 case 2:ov49_02265980(context,local,0,ov49_0226A74C);ov49_02265980(context,local,1,ov49_0226A750);ov49_0226786C(context,local,0,2);ov49_0226786C(context,local,1,1);break;
 case 3:default:ov49_02265980(context,local,0,ov49_0226A74C);ov49_02265980(context,local,1,ov49_0226A750);ov49_02265980(context,local,2,ov49_0226A754);ov49_0226786C(context,local,0,0);ov49_0226786C(context,local,1,2);ov49_0226786C(context,local,2,1);break;
 }
 local[0x955]=(u8)count;ov49_02259130(*(u32 *)(local+8),0);if(!local[0x965])ov49_02266C6C(context,local);else ov49_02266D04(context,local);
}
