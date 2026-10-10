#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 w[28];
} ThrowContext;

typedef struct {
    u32 w[6];
} ExceptionInfo;

typedef struct {
    s32 offset;
    u32 type, destination;
    s32 adjust;
} CatchInfo;

extern void _Z19FindExceptionRecordPcP13ExceptionInfo(char *, ExceptionInfo *);
extern void _ZSt9terminatev(void);
extern void _Z16__SetupFrameInfoP12ThrowContextP13ExceptionInfo(ThrowContext *, ExceptionInfo *);
extern void *_Z23FindMostRecentExceptionP12ThrowContextP13ExceptionInfo(ThrowContext *, ExceptionInfo *);
extern u8 *_Z20FindExceptionHandlerP12ThrowContextP13ExceptionInfoPl(ThrowContext *, ExceptionInfo *, s32 *);
extern char *_Z22__DecodeUnsignedNumberPcPm(char *, u32 *);
extern char *_Z20__DecodeSignedNumberPcPl(char *, s32 *);
extern void _Z11UnwindStackP12ThrowContextP13ExceptionInfoPc(ThrowContext *, ExceptionInfo *, u8 *);
extern void _Z14SetupCatchInfoP12ThrowContextll(ThrowContext *, s32, s32);
extern void _Z17__TransferControlP12ThrowContextP13ExceptionInfoPc(ThrowContext *, ExceptionInfo *, char *);
void _Z14__ThrowHandlerP12ThrowContext(ThrowContext *);
#pragma force_active on

void _Z14__ThrowHandlerP12ThrowContext(ThrowContext *context) {
    ExceptionInfo ex;
    CatchInfo info;
    u8 *handler;
    char *next;
    _Z19FindExceptionRecordPcP13ExceptionInfo((char *)context->w[4], &ex);
    if (ex.w[1] == 0) {
        _ZSt9terminatev();
    }
    _Z16__SetupFrameInfoP12ThrowContextP13ExceptionInfo(context, &ex);
    if (context->w[0] != 0) {
        context->w[3] = 0;
    } else {
        context->w[3] = (u32)_Z23FindMostRecentExceptionP12ThrowContextP13ExceptionInfo(context, &ex);
        if (context->w[3] == 0) {
            _ZSt9terminatev();
        }
    }
    handler = _Z20FindExceptionHandlerP12ThrowContextP13ExceptionInfoPl(context, &ex, &info.offset);
    info.type = (u32)handler[1] | ((u32)handler[2] << 8) | ((u32)handler[3] << 16) | ((u32)handler[4] << 24);
    next = _Z22__DecodeUnsignedNumberPcPm((char *)handler + 5, &info.destination);
    _Z20__DecodeSignedNumberPcPl(next, &info.adjust);
    _Z11UnwindStackP12ThrowContextP13ExceptionInfoPc(context, &ex, handler);
    _Z14SetupCatchInfoP12ThrowContextll(context, info.adjust, info.offset);
    _Z17__TransferControlP12ThrowContextP13ExceptionInfoPc(context, &ex, (char *)ex.w[0] + info.destination);
}
