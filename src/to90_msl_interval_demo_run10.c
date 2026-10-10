#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 w[21];
} Registers;

typedef struct {
    u32 w[7];
    Registers regs;
} ThrowContext;

typedef struct {
    u32 w[6];
} ExceptionInfo;

typedef struct {
    ExceptionInfo ex;
    ThrowContext ctx;
} ActionIterator;

extern u32 _Z13CurrentActionPK14ActionIterator(const ActionIterator *);
extern u32 _Z10NextActionP14ActionIterator(ActionIterator *);
extern char *_Z20__DecodeSignedNumberPcPl(char *, s32 *);
void *_Z23FindMostRecentExceptionP12ThrowContextP13ExceptionInfo(ThrowContext *, const ExceptionInfo *);
#pragma force_active on

void *_Z23FindMostRecentExceptionP12ThrowContextP13ExceptionInfo(ThrowContext *context, const ExceptionInfo *info) {
    ActionIterator it;
    s32 offset;
    u32 action;
    u8 *record;
    it.ex.w[0] = info->w[0];
    it.ex.w[1] = info->w[1];
    it.ex.w[2] = info->w[2];
    it.ex.w[3] = info->w[3];
    it.ex.w[4] = info->w[4];
    it.ex.w[5] = info->w[5];
    it.ctx.w[0] = context->w[0];
    it.ctx.w[1] = context->w[1];
    it.ctx.w[2] = context->w[2];
    it.ctx.w[3] = context->w[3];
    it.ctx.w[4] = context->w[4];
    it.ctx.w[5] = context->w[5];
    it.ctx.w[6] = context->w[6];
    it.ctx.regs = context->regs;
    action = _Z13CurrentActionPK14ActionIterator(&it);
    for (;;) {
        switch (action) {
        default:
        case 1:
        case 14:
            return 0;
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
        case 16:
        case 17:
        case 18:
            action = _Z10NextActionP14ActionIterator(&it);
            break;
        case 13:
            _Z20__DecodeSignedNumberPcPl((char *)it.ex.w[2] + 1, &offset);
            record = (u8 *)it.ctx.w[6] + offset;
            context->w[0] = *(u32 *)(record + 4);
            context->w[1] = *(u32 *)record;
            context->w[2] = 0;
            context->w[3] = (u32)record;
            return record;
        }
    }
}
