// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_strength_reduction off
#include "to90_vct_ssp_private.h"
typedef char SspExtent[(sizeof(Ssp)==36)?1:-1];
typedef char SspBankExtent[(sizeof(SspBank)==84)?1:-1];
typedef char ControlExtent[(sizeof(Control)==36)?1:-1];
typedef char InitExtent[(sizeof(InitArgs)==32)?1:-1];
static inline void LinkSessions(InitArgs *args){u32 offset,index;index=0;if(args->count-1){offset=0;do{Session *base=_021D6F90.core.free;u8 (*arena)[args->count*sizeof(Session)]=(void*)base;Session *current=(Session*)&(*arena)[offset];index++;current->next=base+index;offset+=sizeof(Session);}while(index<args->count-1);}_021D6F90.core.free[args->count-1].next=NULL;}
s32 vct_init_ssp(InitArgs*);
s32 vct_init_ssp(InitArgs *args){Session *arena=args->data;if(!arena||!args->count||args->count>8)return 0;_021D6F90.core.free=arena;MI_CpuFill8(arena,0,args->count*sizeof(Session));MI_CpuFill8(&_021D6FCC,0,sizeof(Session));MI_CpuFill8(&_021D6FB4,0,sizeof(Session));LinkSessions(args);_021D6F90.core.used=NULL;_021D6F90.core.mask=0;_021D6F90.core.count=0;__vct.flag=0;_021D6F90.core.clock=OS_GetTick;_021D6F90.core.seed=0x5d588b65;_021D6F90.core.add=0x269ec3;return 1;}
