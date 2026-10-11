#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    int count;
    char **strings;
    int omitted;
} Strings;

extern void *ov00_021EC3A8(u32);
extern void ov00_021EC3D8(void *);
extern char *ov00_021FF3D4(const char *);
extern const char ov45_022566DC[];
extern int ov45_0224A708(void *, int, int, int, int, int, int, Strings *);
int ov45_0224ABA0(void *, int, int, const char **, int, int, int);

int ov45_0224ABA0(void *ctx, int type, int count, const char **names, int omitted, int a, int b) {
    int i, j;
    Strings *list = ov00_021EC3A8(sizeof(Strings));
    const char *name;
    if (!list) {
        return 0;
    }
    {
        u8 *p = (u8 *)list;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[4] = 0;
        p[5] = 0;
        p[6] = 0;
        p[7] = 0;
        p[8] = 0;
        p[9] = 0;
        p[10] = 0;
        p[11] = 0;
    }
    list->omitted = omitted;
    list->count = count;
    if (omitted) {
        list->count--;
    }
    if (list->count != 0) {
        list->strings = ov00_021EC3A8(list->count * 4);
        if (!list->strings) {
            ov00_021EC3D8(list);
            return 0;
        }
        for (i = 0, j = 0; i < count; i++) {
            name = names[i];
            if (strcmp(name, ov45_022566DC) == 0) {
                continue;
            }
            list->strings[j] = ov00_021FF3D4(name);
            if (!list->strings[j]) {
                for (--j; j >= 0; j--) {
                    ov00_021EC3D8(list->strings[j]);
                }
                ov00_021EC3D8(list->strings);
                ov00_021EC3D8(list);
                return 0;
            }
            j++;
        }
        list->count = j;
    }
    return ov45_0224A708(ctx, 14, type, 0, a, 0, b, list);
}
