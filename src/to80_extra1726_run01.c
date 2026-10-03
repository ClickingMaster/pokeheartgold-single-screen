#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 p[0x24c];s32 timeout;int error;u8 q[8];u32 length;u8 *buffer;u8 r[4];int cancelled,socket;} Global;extern Global _0222AB84;
extern int SOC_Recv(int,void*,int,int);extern void ov39_0222A544(void*,int,void*);extern void ov39_022298FC(void);extern void ov39_02229910(void);extern void ov39_02229FC0(int);
int ov39_02229E40(void);
int ov39_02229E40(void) {typedef struct {u8 b[4];} Bytes;u64 begin;u32 got=0;union {u32 length;Bytes bytes;} packet;packet.length=-1;begin=OS_GetTick()/523656;for(;;){int n=SOC_Recv(_0222AB84.socket,_0222AB84.buffer+got,_0222AB84.length-got,0);if(n!=-6){if(n<=0){_0222AB84.error=9;return 0;}got+=n;if(got>=4 && packet.length==-1){memcpy(&packet,_0222AB84.buffer,4);}if(got==packet.length && packet.length!=-1){ov39_0222A544(_0222AB84.buffer+4,got-4,_0222AB84.buffer+4);_0222AB84.length=packet.length-4;break;}}if(_0222AB84.timeout!=0){s32 now=OS_GetTick()/523656-begin;if(now>=_0222AB84.timeout){_0222AB84.error=12;return 0;}}ov39_022298FC();if(_0222AB84.cancelled==1){_0222AB84.error=13;ov39_02229910();return 0;}ov39_02229910();ov39_02229FC0(1);}return 1;}
