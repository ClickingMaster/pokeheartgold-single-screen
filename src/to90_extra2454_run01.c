#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern char ov00_02218450[];
extern char ov00_02218478[];
extern int ov00_021FF970(int, void *, int, int);
extern int ov00_021FFA60(int);
extern void ov00_0220A86C(void *, int, char *);
extern void ov00_02202990(void *, int, int);
extern void ov00_0220A3BC(void *, char *, char *);
int ov00_02202154(void *, int, void *, int, int *, int *, char *);
#pragma optimize_for_size on

int ov00_02202154(void *p, int sock, void *dst, int size, int *closed, int *read, char *tag) {
    u32 n = ov00_021FF970(sock, dst, size, 0);
    if (n == -1) {
        int err = ov00_021FFA60(sock);
        if (err != -6 && err != -26 && err != -76) {
            if (tag[0] == 'P' && tag[1] == 'R') {
                return 3;
            }
            ov00_0220A86C(p, 5, ov00_02218450);
            ov00_02202990(p, 3, 0);
            return 3;
        }
        *read = 0;
        *closed = 0;
    } else {
        if (n > 0) {
            *read = n;
            *closed = 0;
            goto finish;
        }
        ov00_0220A3BC(p, ov00_02218478, tag);
        *read = 0;
        *closed = 1;
    }
finish:
    return 0;
}
