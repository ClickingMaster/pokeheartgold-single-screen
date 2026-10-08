#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 pad[0x50]; u32 ip; } NetState;
extern NetState _021D4620; extern u8 _02110690[],_021D4B08[];
extern int calc_checksum(void*,int),is_multicast(u32);extern void send_ether(void*,int,void*,int,u32,int),put_in_buffer(void*,void*,void*,int,void*,int);
void send_ip_frag(u8*,int,void*,int,u32,u32);
// @fn send_ip_frag
void send_ip_frag(u8*p,int bytes,void*data,int length,u32 ip,u32 frag){
 u16 total; u16 f; int c; OSIntrMode saved;
 total=bytes+20+length;
f=frag;
*(u16*)(p-18)=(total>>8)|(total<<8);
*(u16*)(p-14)=(f>>8)|(f<<8);
*(u16*)(p-10)=0;
 c=calc_checksum(p-20,20);
 *(u16*)(p-10)=(c>>8)|(c<<8);
 if(ip!=0x7f000001&&ip!=_021D4620.ip)send_ether(p-20,bytes+20,data,length,ip,0x800);
 if(ip!=0x7f000001&&ip!=_021D4620.ip&&!is_multicast(ip))return;
 MI_CpuCopy8(_02110690,p-28,8);
 saved=OS_DisableInterrupts();
 put_in_buffer(_021D4B08,_021D4B08,p-28,bytes+28,data,length);
 OS_RestoreInterrupts(saved);
}
