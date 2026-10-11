#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern struct {
    u32 words[26];
} gCpsState;

extern u8 gCpsMacAddress[];
extern u32 CPS_SocGetLength(void);
extern u8 *CPS_SocRead(u32 *);
extern void CPS_SocConsume(u32);
extern void OS_YieldThread__(void);
extern int maccmp(void *, void *);
#define SWAP(x) ((u16)(((x) >> 8) | ((x) << 8)))
#define READ(p) (((u32)(u16)(((p)[0] << 8) | (p)[1]) << 16) | (u16)(((p)[2] << 8) | (p)[3]))
int dhcp_analyze_response(u32, int);

int dhcp_analyze_response(u32 id, int attempt) {
    u32 length;
    int timeout = (attempt + 1) * 15;
    u32 start = OS_GetTick() >> 16;
    {
        int result = 0;
        while (((int (*)(void))gCpsState.words[18])() && !result && (int)((u32)(OS_GetTick() >> 16) - start) < timeout) {
            u8 *p, *end, *opt;
            u32 address;
            if (!CPS_SocGetLength()) {
                OS_YieldThread__();
                continue;
            }
            p = CPS_SocRead(&length);
            if (length <= 240 || p[0] != 2) {
                goto consume;
            }
            if ((((u32)SWAP(*(u16 *)(p + 4)) << 16) | SWAP(*(u16 *)(p + 6))) != id) {
                goto consume;
            }
            if (maccmp(p + 28, gCpsMacAddress)) {
                goto consume;
            }
            address = READ(p + 16);
            result = 3;
            end = p + length;
            if (p[236] != 99 || p[237] != 130 || p[238] != 83 || (opt = p + 240, p[239] != 99)) {
                goto consume;
            }
            {
                int code;
                int two = 2, one = 1;
                goto option_test;
option_body:
                if (code == 0) {
                    goto option_test;
                }
                switch (code) {
                case 1:
                    gCpsState.words[7] = READ(opt + 1);
                    break;
                case 3:
                    gCpsState.words[11] = READ(opt + 1);
                    break;
                case 6:
                    if (opt[0] < 8) {
                        gCpsState.words[25] = 0;
                    } else {
                        gCpsState.words[25] = READ(opt + 5);
                    }
                    gCpsState.words[24] = READ(opt + 1);
                    break;
                case 51:
                    gCpsState.words[19] = READ(opt + 1);
                    break;
                case 53:
                    switch (opt[1]) {
                    case 2:
                        result = one;
                        gCpsState.words[13] = address;
                        break;
                    case 5:
                        result = two;
                        gCpsState.words[20] = address;
                        break;
                    }
                    break;
                case 54:
                    gCpsState.words[8] = READ(opt + 1);
                    break;
                }
                opt += opt[0] + 1;
option_test:
                if (opt < end) {
                    code = *opt++;
                    if (code != 255) {
                        goto option_body;
                    }
                }
            }
consume:
            CPS_SocConsume(length);
        }
        return result;
    }
}
