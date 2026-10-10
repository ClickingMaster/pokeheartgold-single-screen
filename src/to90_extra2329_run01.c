#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
extern long atol(const char *);

typedef struct {
    u8 gap[32];
    char **values;
    int kind;
} Input;

typedef struct {
    u32 a, enabled;
    u8 pad[0x508];
} State;

extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_022563F0[];
extern char ov45_0225680C[];
extern char ov45_02256998[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov00_021EC3D8(void *);
extern void ov45_02251B0C(State *, int, void *, int, void *);
extern void *ov00_021EC3A8(int);
extern char *ov00_021FF3D4(char *);
void ov45_0224F490(State *, Input *);

void ov45_0224F490(State *s, Input *in) {
    int count, j;
    char **items, *text, *token;
    int i;
    if (!s) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_022563F0, 0x1200);
    }
    if (in->kind != 4) {
        __msl_assertion_failed(ov45_0225680C, ov45_022565D0, ov45_022563F0, 0x1206);
    }
    if (in->kind != 4) {
        return;
    }
    count = atol(in->values[1]);
    text = in->values[2];
    if (!s->enabled) {
        return;
    }
    items = ov00_021EC3A8(count * 4);
    if (!items) {
        return;
    }
    token = strtok(text, ov45_02256998);
    i = 0;
    while (i < count && token) {
        items[i] = ov00_021FF3D4(token);
        if (!items[i]) {
            break;
        }
        token = strtok(0, ov45_02256998);
        i++;
    }
    ov45_02251B0C(s, 5, (u8 *)s + 0x510, i, items);
    for (j = 0; j < i; j++) {
        ov00_021EC3D8(items[j]);
    }
    ov00_021EC3D8(items);
}
