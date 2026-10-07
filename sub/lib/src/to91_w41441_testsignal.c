#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void *WMSP_WlRequest(void*);
u16 *WMSP_WL_DevTestSignal(void*,u16,u16,u16,u16);
static inline void InitializeHeader(u16 *packet){packet[0]=0;packet[1]=0;packet[2]=0;packet[3]=0;packet[4]=0;packet[5]=0;packet[6]=777;packet[7]=4;}
u16 *WMSP_WL_DevTestSignal(void *buffer,u16 first,u16 second,u16 third,u16 fourth){u16 *packet=buffer,*reply;InitializeHeader(packet);packet[8]=first;packet[9]=second;packet[10]=third;packet[11]=fourth;reply=packet+packet[7]+8;reply[0]=packet[6];reply[1]=1;WMSP_WlRequest(buffer);return reply;}
