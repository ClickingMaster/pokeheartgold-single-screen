#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35631Flags {u16 beacon:1,resBc:1,preamble:1,antenna:1,diversity:1,antennaOther:1,bcnInd:1,nullKey:1,other:8;} W35631Flags;
typedef struct W35631Config {u8 pad[0x22];u16 maxConn;u16 mac[3];u16 retry,enable,mode,rate,auth,wep,key;u16 pad38;W35631Flags flags;u16 active;u8 pad3e[0xe];u16 revision;u8 pad4e[0x36];u16 ssidMask[16];u8 pada4[0x1a];u16 enable2;u8 padc0[2];u16 lost;} W35631Config;
typedef struct W35631Work {u8 pad[0x300];W35631Config config;} W35631Work;
#define W35631_WORK (*(W35631Work *volatile *)0x0380fff4)
extern void WSetMacAdrs1(void *,const void *);
int PARAMGET_MacAdrsReqCmd(u16 *,u16 *);
// @fn PARAMGET_MacAdrsReqCmd
int PARAMGET_MacAdrsReqCmd(u16 *req,u16 *out){out[1]=4;WSetMacAdrs1(out+3,W35631_WORK->config.mac);return 0;}
