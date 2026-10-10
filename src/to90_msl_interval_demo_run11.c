#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define P(p, n)  (*(void **)((u8 *)(p) + (n)))
#define W(p, n)  (*(u32 *)((u8 *)(p) + (n)))
#define SW(p, n) (*(s32 *)((u8 *)(p) + (n)))
#define H(p, n)  (*(u16 *)((u8 *)(p) + (n)))
#define B(p, n)  (*(u8 *)((u8 *)(p) + (n)))

typedef struct {
    u32 count;
    u8 pad[8];
    u8 *types;
} Spec;

typedef struct {
    u32 value;
} Tag;

int __throw_catch_compare(char *, Tag, int *);
int _Z17IsInSpecificationPcP16ex_specification(char *, Spec *);

int _Z17IsInSpecificationPcP16ex_specification(char *name, Spec *s) {
    Tag type;
    int result;
    u8 *p = s->types;
    u32 i;
    for (i = 0; i < s->count; i++, p += 4) {
        type.value = p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
        if (__throw_catch_compare(name, type, &result)) {
            return 1;
        }
    }
    return 0;
}
