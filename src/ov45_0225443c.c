#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    int state, active, error;
    u8 pad[0x44];
    u32 key;
    void *unused, *out;
    u32 unused2;
    void *input;
    u32 len;
} Work;

extern Work ov45_0225762C;
extern char *ov45_02257628;
extern char ov45_02257728[];
extern char ov45_022576DC[];
extern char ov45_02257704[];
extern void *ov00_021EC2A8(int, int);
extern void ov00_021EC2EC(int, void *, int);
extern int ov00_021FFC18(void *, void *, int, int);
extern void ov45_02253D88(u32);
extern u32 ov45_02253D9C(void);
extern int ov45_02253D50(int);
extern void ov45_02253DDC(char *, char *, u32, u32, u32, u32);
extern void ov00_021FA0B4(int);
extern unsigned long strtoul(const char *, char **, int);
int ov45_022542F4(u8 *, u8 *, int, u8 *, int);
void ov45_0225443C(int, char *);

void ov45_0225443C(int mode, char *input) {
    char name[21];
    char part[9] = { 0 };
    u32 a, b, c, d;
    ov45_0225762C.error = -1;
    ov45_0225762C.unused = NULL;
    ov45_0225762C.out = NULL;
    ov45_0225762C.unused2 = 0;
    ov45_0225762C.input = NULL;
    ov45_0225762C.len = 0;
    switch (mode) {
    case 0:
        ov45_02257628 = ov45_022576DC;
        break;
    case 1:
        ov45_02257628 = ov45_02257704;
        break;
    case 2:
        ov45_02257628 = ov45_02257728;
        break;
    }
    strncpy(name, input, 20);
    name[20] = 0;
    a = strtoul(strncpy(part, input + 20, 8), NULL, 16);
    b = strtoul(strncpy(part, input + 28, 8), NULL, 16);
    c = strtoul(strncpy(part, input + 36, 8), NULL, 16);
    d = strtoul(strncpy(part, input + 44, 8), NULL, 16);
    ov45_02253DDC(input + 52, name, a, b, c, d);
    ov00_021FA0B4(0);
    ov45_0225762C.state = 3;
    ov45_0225762C.active = 1;
}
