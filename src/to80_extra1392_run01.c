#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Info {
    int x, state;
    char numbers[256], name[256];
    u8 tail[8];
};

extern char ov00_02217A08[];
extern char ov00_02217A0C[];
extern int ov00_021F0B8C(int, struct Info *);
extern int ov00_021ED4E8(char *, char *, char *, int);
extern unsigned long strtoul(const char *, char **, int);
u8 ov00_021EFCB0(int, u8 *, u8 *, char *);

u8 ov00_021EFCB0(int arg, u8 *a, u8 *b, char *name) {
    struct Info info;
    char temp[4];
    if (ov00_021F0B8C(arg, &info)) {
        if (info.state == 6) {
            if (a) {
                if (ov00_021ED4E8(ov00_02217A08, temp, info.numbers, 47) <= 0) {
                    *a = 0;
                } else {
                    *a = strtoul(temp, 0, 10);
                }
            }
            if (b) {
                if (ov00_021ED4E8(ov00_02217A0C, temp, info.numbers, 47) <= 0) {
                    *b = 0;
                } else {
                    *b = strtoul(temp, 0, 10);
                }
            }
        } else {
            if (a) {
                *a = 0;
            }
            if (b) {
                *b = 0;
            }
        }
        if (name) {
            strcpy(name, info.name);
        }
        return info.state;
    }
    if (a) {
        *a = 0;
    }
    if (b) {
        *b = 0;
    }
    return 0;
}
