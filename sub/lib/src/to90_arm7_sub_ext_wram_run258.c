#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35589;
#define MLME ((M35589 *)(BASE+4+0x400))
void MLME_MeasChanTimeOut(void);
// @fn MLME_MeasChanTimeOut
void MLME_MeasChanTimeOut(void){MLME->state=0x83;AddTask(2,5);}
