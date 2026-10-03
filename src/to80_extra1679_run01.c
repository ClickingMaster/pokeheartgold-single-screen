#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define U16(p,o) (*(u16*)((u8*)(p)+(o)))
#define PTR(p,o) (*(u8**)((u8*)(p)+(o)))
#define SW16(x) (((u16)(x)>>8)|((u16)(x)<<8))
extern u8 _021D4888[],_021D4B32[],_021D4620[];
extern u32 calc_checksum_do(void*,u32,u32);extern int invert_checksum(u16);extern void send_ip(void*,u32,void*,u32,u32,int);
void send_tcp(void*,u32,u8*,int);
void send_tcp(void*data,u32 size,u8*conn,int flags){u8*out;u32 header;u32 sum;
if(conn[8]==0)return;if(PTR(&OSi_ThreadInfo,4)==_021D4888)out=_021D4B32;else out=PTR(conn,0x4c)+34;
header=(flags&2)?24:20;
U16(out,-12)=SW16(U32(_021D4620,0x50)>>16);U16(out,-10)=SW16(U32(_021D4620,0x50));U16(out,-8)=SW16(U32(conn,0x1c)>>16);U16(out,-6)=SW16(U32(conn,0x1c));U16(out,-4)=0x600;U16(out,-2)=SW16(header+size);
U16(out,0)=SW16(U16(conn,10));U16(out,2)=SW16(U16(conn,24));U16(out,4)=SW16(U32(conn,0x28)>>16);U16(out,6)=SW16(U32(conn,0x28));U16(out,8)=SW16(U32(conn,0x24)>>16);U16(out,10)=SW16(U32(conn,0x24));out[12]=(header/4)<<4;out[13]=flags;U16(out,14)=SW16(U32(conn,0x3c)-U32(conn,0x44));U16(out,16)=0;U16(out,18)=0;
if(flags&2){U16(out,20)=SW16((U16(_021D4620,2)+0x2040000U)>>16);U16(out,22)=SW16(U16(_021D4620,2)+0x2040000U);}
sum=calc_checksum_do(out-12,header+12,0);sum=calc_checksum_do(data,size,sum);{int checksum=invert_checksum((u16)sum);U16(out,16)=(checksum>>8)|(checksum<<8);}send_ip(out,header,data,size,U32(conn,0x1c),6);U32(conn,0x28)+=size;if(flags&3)U32(conn,0x28)++;}
