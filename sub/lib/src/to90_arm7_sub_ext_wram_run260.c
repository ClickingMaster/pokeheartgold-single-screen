#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35592;
#define MLME ((M35592 *)(BASE+4+0x400))
extern void *AllocateHeapBuf(void *,u32);extern void SetFatalErr(int);extern void WSetMacAdrs1(void *,const void *);extern void SendMessageToWmDirect(void *,void *);
int MLME_IssueAuthIndication(const void *mac,int reason);
// @fn MLME_IssueAuthIndication
int MLME_IssueAuthIndication(const void *mac,int reason){u8 *p=AllocateHeapBuf(BASE+0x188,24);if(!p){SetFatalErr(1);return 0;}*(u16 *)(p+12)=132;*(u16 *)(p+14)=4;WSetMacAdrs1(p+16,mac);*(u16 *)(p+22)=reason;SendMessageToWmDirect(BASE+0x188,p);return 1;}
