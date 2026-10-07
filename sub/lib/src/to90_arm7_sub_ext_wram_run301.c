#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35633Flags {u16 beacon:1,resBc:1,preamble:1,antenna:1,diversity:1,antennaOther:1,bcnInd:1,nullKey:1,other:8;} W35633Flags;
typedef struct W35633Config {u8 pad[0x22];u16 maxConn;u16 mac[3];u16 retry,enable,mode,rate,auth,wep,key;u16 pad38;W35633Flags flags;u16 active;u8 pad3e[0xe];u16 revision;u8 pad4e[0x36];u16 ssidMask[16];u8 pada4[0x1a];u16 enable2;u8 padc0[2];u16 lost;} W35633Config;
typedef struct W35633Work {u8 pad[0x300];W35633Config config;} W35633Work;
#define W35633_WORK (*(W35633Work *volatile *)0x0380fff4)
int PARAMGET_EnableChannelReqCmd(u16 *,u16 *);
// @fn PARAMGET_EnableChannelReqCmd
int PARAMGET_EnableChannelReqCmd(u16 *req,u16 *out){out[1]=3;out[3]=W35633_WORK->config.enable;out[4]=W35633_WORK->config.enable2;return 0;}
