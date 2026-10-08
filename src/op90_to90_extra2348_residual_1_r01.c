#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 pad[0x50]; u32 ip; } NetState;
extern NetState _021D4620;
extern u8 _021D4B08[];
extern void send_packet(u8*,int,void*,int);
void send_arprequest(u32);
// @fn send_arprequest
void send_arprequest(u32 ip){
 u8 buf[42]; u16 th; u16 tl; u16 sh; u16 sl; u32 own;
 MI_CpuFill8(buf,0,42);
 MI_CpuFill8(buf,0xff,6);
 MI_CpuCopy8(_021D4B08,buf+6,6);
 *(u16*)(buf+12)=0x608;
buf[15]=1;
buf[16]=8;
*(u16*)(buf+18)=0x406;
buf[21]=1;
 MI_CpuCopy8(_021D4B08,buf+22,6);
 th=(u16)(ip>>16);
tl=(u16)ip;
own=_021D4620.ip;
sh=(u16)(own>>16);
sl=(u16)own;
 *(u16*)(buf+28)=(sh>>8)|(sh<<8);
*(u16*)(buf+30)=(sl>>8)|(sl<<8);
*(u16*)(buf+38)=(th>>8)|(th<<8);
*(u16*)(buf+40)=(tl>>8)|(tl<<8);
 send_packet(buf,42,0,0);
}
