#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35601;
#define MLME ((M35601 *)(BASE+4+0x400))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
extern int WSetMacAdrs(const void *);
extern int WSetRetryLimit(int);
extern int WSetEnableChannel(int);
extern int WSetMode(int);
extern int WSetRate(int);
extern int WSetWepMode(int);
extern int WSetWepKeyId(int);
extern int WSetWepKey(const void *);
extern int WSetBeaconType(int);
extern int WSetBcSsidResponse(int);
extern int WSetBeaconLostThreshold(int);
extern int WSetActiveZoneTime(int,int);
extern int WSetSsidMask(const void *);
extern int WSetPreambleType(int);
extern int WSetAuthAlgo(int);
int PARAMSET_AllReqCmd(u8 *req,u16 *confirm);
// @fn PARAMSET_AllReqCmd
int PARAMSET_AllReqCmd(u8 *req,u16 *confirm){int r;confirm[1]=1;if(H(BASE,0x34c)!=16)return 1;r=WSetMacAdrs(req+16);r|=WSetRetryLimit(H(req,22));r|=WSetEnableChannel(H(req,24));r|=WSetMode(H(req,28));r|=WSetRate(H(req,30));r|=WSetWepMode(H(req,32));r|=WSetWepKeyId(H(req,34));r|=WSetWepKey(req+36);r|=WSetBeaconType(H(req,116));r|=WSetBcSsidResponse(H(req,118));r|=WSetBeaconLostThreshold(H(req,120));r|=WSetActiveZoneTime(H(req,122),0);r|=WSetSsidMask(req+124);r|=WSetPreambleType(H(req,156));r|=WSetAuthAlgo(H(req,158));return r;}
