#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35643Flags {u16 beacon:1,resBc:1,preamble:1,antenna:1,diversity:1,antennaOther:1,bcnInd:1,nullKey:1,other:8;} W35643Flags;
typedef struct W35643Config {u8 pad[0x22];u16 maxConn;u16 mac[3];u16 retry,enable,mode,rate,auth,wep,key;u16 pad38;W35643Flags flags;u16 active;u8 pad3e[0xe];u16 revision;u8 pad4e[0x36];u16 ssidMask[16];u8 pada4[0x1a];u16 enable2;u8 padc0[2];u16 lost;} W35643Config;
typedef struct W35643Work {u8 pad[0x300];W35643Config config;} W35643Work;
#define W35643_WORK (*(W35643Work *volatile *)0x0380fff4)
int PARAMGET_PreambleTypeReqCmd(u16 *,u16 *);
// @fn PARAMGET_PreambleTypeReqCmd
int PARAMGET_PreambleTypeReqCmd(u16 *req,u16 *out){out[1]=2;out[3]=W35643_WORK->config.flags.preamble;return 0;}
