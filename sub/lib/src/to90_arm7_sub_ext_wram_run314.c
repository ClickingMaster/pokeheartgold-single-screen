#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35646Flags {u16 beacon:1,resBc:1,preamble:1,antenna:1,diversity:1,antennaOther:1,bcnInd:1,nullKey:1,other:8;} W35646Flags;
typedef struct W35646Config {u8 pad[0x22];u16 maxConn;u16 mac[3];u16 retry,enable,mode,rate,auth,wep,key;u16 pad38;W35646Flags flags;u16 active;u8 pad3e[0xe];u16 revision;u8 pad4e[0x36];u16 ssidMask[16];u8 pada4[0x1a];u16 enable2;u8 padc0[2];u16 lost;} W35646Config;
typedef struct W35646Work {u8 pad[0x300];W35646Config config;} W35646Work;
#define W35646_WORK (*(W35646Work *volatile *)0x0380fff4)
int PARAMGET_MaxConnReqCmd(u16 *,u16 *);
// @fn PARAMGET_MaxConnReqCmd
int PARAMGET_MaxConnReqCmd(u16 *req,u16 *out){out[1]=2;out[3]=W35646_WORK->config.maxConn-1;return 0;}
