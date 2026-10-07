#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35648Flags {u16 beacon:1,resBc:1,preamble:1,antenna:1,diversity:1,antennaOther:1,bcnInd:1,nullKey:1,other:8;} W35648Flags;
typedef struct W35648Config {u8 pad[0x22];u16 maxConn;u16 mac[3];u16 retry,enable,mode,rate,auth,wep,key;u16 pad38;W35648Flags flags;u16 active;u8 pad3e[0xe];u16 revision;u8 pad4e[0x36];u16 ssidMask[16];u8 pada4[0x1a];u16 enable2;u8 padc0[2];u16 lost;} W35648Config;
typedef struct W35648Work {u8 pad[0x300];W35648Config config;} W35648Work;
#define W35648_WORK (*(W35648Work *volatile *)0x0380fff4)
int PARAMGET_DiversityReqCmd(u16 *,u16 *);
// @fn PARAMGET_DiversityReqCmd
int PARAMGET_DiversityReqCmd(u16 *req,u16 *out){u16 revision;W35648Work *work;W35648Config *c2;out[1]=3;work=W35648_WORK;revision=work->config.revision;if(revision<16)return 1;out[3]=work->config.flags.diversity;c2=&W35648_WORK->config;out[4]=c2->flags.antennaOther^c2->flags.antenna;return 0;}
