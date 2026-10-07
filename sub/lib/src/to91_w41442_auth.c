#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void *WMSP_WlRequest(void*);
void __VENEER_MIi_CpuCopy16(const void*,void*,u32);
u16 *WMSP_WL_MlmeAuthenticate(void*,const void*,u16,u16);
static inline void InitializeHeader(u16 *packet){packet[0]=0;packet[1]=0;packet[2]=0;packet[3]=0;packet[4]=0;packet[5]=0;packet[6]=4;packet[7]=5;}
u16 *WMSP_WL_MlmeAuthenticate(void *buffer,const void *peer,u16 algorithm,u16 timeout){u16 *packet=buffer,*reply;InitializeHeader(packet);__VENEER_MIi_CpuCopy16(peer,packet+8,6);packet[11]=algorithm;packet[12]=timeout;reply=packet+packet[7]+8;reply[0]=packet[6];reply[1]=6;WMSP_WlRequest(buffer);return reply;}
