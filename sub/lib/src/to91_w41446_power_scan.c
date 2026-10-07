#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void *WMSP_WlRequest(void*);
void __VENEER_MIi_CpuCopy16(const void*,void*,u32);
u16 *WMSP_WL_MlmePowerManagement(void*,u16,u16,u16);
static inline void InitializePowerHeader(u16 *packet){packet[0]=0;packet[1]=0;packet[2]=0;packet[3]=0;packet[4]=0;packet[5]=0;packet[6]=1;packet[7]=3;}
u16 *WMSP_WL_MlmeScan(void*,u32,void*,u16,void*,u16,void*,u16);
static inline void InitializeScanHeader(u16 *packet){packet[0]=0;packet[1]=0;packet[2]=0;packet[3]=0;packet[4]=0;packet[5]=0;packet[6]=2;packet[7]=31;}
u16 *WMSP_WL_MlmePowerManagement(void *buffer,u16 first,u16 second,u16 third){u16 *packet=buffer,*reply;InitializePowerHeader(packet);packet[8]=first;packet[9]=second;packet[10]=third;reply=packet+packet[7]+8;reply[0]=packet[6];reply[1]=1;WMSP_WlRequest(buffer);return reply;}
u16 *WMSP_WL_MlmeScan(void *buffer,u32 size,void *peer,u16 ssidLength,void *ssid,u16 scanType,void *channels,u16 dwell){u16 *packet=buffer,*reply;InitializeScanHeader(packet);__VENEER_MIi_CpuCopy16(peer,packet+8,6);packet[11]=ssidLength;__VENEER_MIi_CpuCopy16(ssid,packet+12,32);packet[28]=scanType;__VENEER_MIi_CpuCopy16(channels,packet+29,16);packet[37]=dwell;packet[38]=0;reply=packet+packet[7]+8;reply[0]=packet[6];reply[1]=(size>>1)-44;WMSP_WlRequest(packet);return reply;}
