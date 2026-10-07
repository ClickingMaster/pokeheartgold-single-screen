#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35590;
#define MLME ((M35590 *)(BASE+4+0x400))
extern int MLME_IssueBeaconLostIndication(const void *);
void MLME_BeaconLostTask(void);
// @fn MLME_BeaconLostTask
void MLME_BeaconLostTask(void){MLME_IssueBeaconLostIndication(BASE+0x3c6);}
