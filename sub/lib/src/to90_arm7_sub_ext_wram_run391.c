#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35591;
#define MLME ((M35591 *)(BASE+4+0x400))
typedef struct {u8 pad[4];u16 flags;} Control35591;
extern void SendMessageToWmDirect(void *,void *);
void IssueMlmeConfirm(void);
// @fn IssueMlmeConfirm
void IssueMlmeConfirm(void){u8 *base=BASE;u8 *p=base+0x17c;{Control35591 *c=(Control35591 *)(base+0x424);c->flags&=~1;}SendMessageToWmDirect(p+0x84,*(void **)(base+0x424));if(*(u16 *)(p+0x8c))AddTask(2,11);}
