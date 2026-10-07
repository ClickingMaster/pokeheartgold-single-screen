#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 prefix[20];u8*stack,*frame;u32 regs[16];u8 gap[4];u32 size;u8 pad[4];u16 mask;u8 fp;} Throw;u32 _Z15__PopStackFrameP12ThrowContextP13ExceptionInfo(Throw*,void*);
u32 _Z15__PopStackFrameP12ThrowContextP13ExceptionInfo(Throw*ctx,void*unused){u32 size=ctx->size;u32*cursor=(u32*)(ctx->frame+size-(ctx->fp?16:0));int i;for(i=15;i>=0;i--){if(ctx->mask&(1u<<i))ctx->regs[i]=*--cursor;}ctx->stack=ctx->frame+size;return ctx->regs[14];}
