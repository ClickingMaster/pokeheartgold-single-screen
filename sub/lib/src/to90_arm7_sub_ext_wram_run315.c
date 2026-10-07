#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35647Flags {u16 beacon:1,resBc:1,preamble:1,antenna:1,diversity:1,antennaOther:1,bcnInd:1,nullKey:1,other:8;} W35647Flags;
typedef struct W35647Config {u8 pad[0x22];u16 maxConn;u16 mac[3];u16 retry,enable,mode,rate,auth,wep,key;u16 pad38;W35647Flags flags;u16 active;u8 pad3e[0xe];u16 revision;u8 pad4e[0x36];u16 ssidMask[16];u8 pada4[0x1a];u16 enable2;u8 padc0[2];u16 lost;} W35647Config;
typedef struct W35647Work {u8 pad[0x300];W35647Config config;} W35647Work;
#define W35647_WORK (*(W35647Work *volatile *)0x0380fff4)
int PARAMGET_MainAntennaReqCmd(u16 *,u16 *);
// @fn PARAMGET_MainAntennaReqCmd
int PARAMGET_MainAntennaReqCmd(u16 *req,u16 *out){u16 revision;W35647Work *work;out[1]=2;work=W35647_WORK;revision=work->config.revision;if(revision<16)return 1;out[3]=work->config.flags.antenna;return 0;}
