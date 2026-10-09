#include <nitro/types.h>
#include <nitro/code32.h>
struct ThrowContext { void *object; void *type; void *destructor; u8 pad[12]; u8 *frame; };
struct ExceptionInfo { u8 *base; };
struct ex_specification { u32 pad; int handler; int record; };
struct Record { void *type; void *object; void *destructor; u8 pad[8]; char *location; };
void UnwindStack(ThrowContext *, ExceptionInfo *, char *);
void __TransferControl(ThrowContext *, ExceptionInfo *, char *);
void HandleUnexpected(ThrowContext *c, ExceptionInfo *i, ex_specification *s, char *location);
// @fn _Z16HandleUnexpectedP12ThrowContextP13ExceptionInfoP16ex_specificationPc
void HandleUnexpected(ThrowContext *c, ExceptionInfo *i, ex_specification *s, char *location)
{
#pragma exception_terminate
    Record *r;
    UnwindStack(c, i, location);
    r = (Record *)(c->frame + s->record);
    r->type = c->type;
    r->object = c->object;
    r->destructor = c->destructor;
    r->location = location;
    __TransferControl(c, i, (char *)i->base + s->handler);
}
