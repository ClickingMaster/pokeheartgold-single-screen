#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 length,family;u16 port;u32 address;} Address;typedef struct {u8 p[0x40];u16 port;} Config;typedef struct {u8 p[0x244];int timeout;u8 p2[8];int error;u8 p3[0x10];u32 address;int cancelled,socket;} State;
extern Config ov39_0222AD84;extern State _0222AB84;extern int SOC_Connect(int,Address *);extern void ov39_022298FC(void),ov39_02229910(void),ov39_02229FC0(int);int ov39_02229BE4(void);
static inline u16 swap(u16 v){int a=v;return ((a>>8)&255)|((a<<8)&0xff00);}
int ov39_02229BE4(void){Address a;u32 start;int result;a.length=8;a.family=2;a.port=swap(ov39_0222AD84.port);a.address=_0222AB84.address;start=OS_GetTick()/523656;for(;;){result=SOC_Connect(_0222AB84.socket,&a);if(result>=0)break;if(result==-26){if(_0222AB84.timeout!=0){int elapsed=OS_GetTick()/523656-start;if(elapsed>=_0222AB84.timeout){_0222AB84.error=10;return 0;}}}else if(result==-30)break;else{_0222AB84.error=7;return 0;}ov39_022298FC();if(_0222AB84.cancelled==1){_0222AB84.error=13;ov39_02229910();return 0;}ov39_02229910();ov39_02229FC0(1);}return 1;}
