#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35588;
#define MLME ((M35588 *)(BASE+4+0x400))
void MLME_ReAssTimeOut(void);
// @fn MLME_ReAssTimeOut
void MLME_ReAssTimeOut(void){M35588 *p=MLME;*(u16 *)(p->confirm+4)=7;p->state=99;AddTask(2,4);}
