#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35645Flags {u16 beacon:1,resBc:1,preamble:1,antenna:1,diversity:1,antennaOther:1,bcnInd:1,nullKey:1,other:8;} W35645Flags;
typedef struct W35645Config {u8 pad[0x22];u16 maxConn;u16 mac[3];u16 retry,enable,mode,rate,auth,wep,key;u16 pad38;W35645Flags flags;u16 active;u8 pad3e[0xe];u16 revision;u8 pad4e[0x36];u16 ssidMask[16];u8 pada4[0x1a];u16 enable2;u8 padc0[2];u16 lost;} W35645Config;
typedef struct W35645Work {u8 pad[0x300];W35645Config config;} W35645Work;
#define W35645_WORK (*(W35645Work *volatile *)0x0380fff4)
extern u16 BBP_Read(int);
int PARAMGET_CCAModeEDThReqCmd(u16 *,u16 *);
// @fn PARAMGET_CCAModeEDThReqCmd
int PARAMGET_CCAModeEDThReqCmd(u16 *req,u16 *out){out[1]=4;out[3]=BBP_Read(0x13);out[4]=BBP_Read(0x35);out[5]=BBP_Read(0x2e);return 0;}
