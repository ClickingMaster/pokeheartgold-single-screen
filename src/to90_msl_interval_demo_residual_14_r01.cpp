#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on

struct ex_specification {
    unsigned long regions;
    unsigned long specs;
    long pcoffset;
    char *data;
};

struct CatchInfo {
    void *location;
    char *typeinfo;
    void *dtor;
    void *sublocation;
    long pointercopy;
    char *stacktop;
};

namespace std {
void unexpected();
void terminate();
class bad_exception;
}

char *__DecodeUnsignedNumber(char *p, unsigned long *n);
char *__DecodeSignedNumber(char *p, long *n);
int IsInSpecification(char *type, ex_specification *spec);

extern "C" {
extern char _021116F4[], _0211170C[];
extern void *_021116E8[];
void __throw(char *type, void *location, void *dtor);
void __unexpected(CatchInfo *catchinfo);
void *_ZNSt13bad_exceptionD1Ev(void *);
}

struct BadException {
    void **vtable;
};

// @fn __unexpected
extern "C" void __unexpected(CatchInfo *catchinfo) {
    ex_specification spec;
    char *unexp = catchinfo->stacktop;
#pragma exception_magic
    try {
        std::unexpected();
    } catch (...) {
        char *p = __DecodeUnsignedNumber(unexp + 1, &spec.regions);
        p = __DecodeUnsignedNumber(p, &spec.specs);
        p = __DecodeSignedNumber(p, &spec.pcoffset);
        spec.data = p;
        if (IsInSpecification(((CatchInfo *)&__exception_magic)->typeinfo, &spec)) {
            throw;
        }
        if (IsInSpecification(_021116F4, &spec)) {
            BadException e;
            e.vtable = _021116E8;
            __throw(_0211170C, &e, (void *)_ZNSt13bad_exceptionD1Ev);
        }
    }
    std::terminate();
}
