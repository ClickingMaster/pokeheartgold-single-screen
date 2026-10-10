#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    int network, type;
    u8 id[9];
    u8 gap;
    u8 ssid[33];
    u8 mac[6];
    u8 end[3];
} Work;

extern int ov00_021EC724(void);
extern int ov00_021FAA40(void);
extern int ov00_021FAA70(void *);
extern u8 *WCM_GetApMacAddress(void);
extern u8 *WCM_GetApEssid(u16 *);
int ov00_021EC9E0(Work *);
#pragma force_active on

static inline int selectType(int flag, u8 ch) {
    return flag ? ch - 48 : 255;
}

static inline BOOL digitType(BOOL digit, u8 ch) {
    if (ch < 48) {
        return digit;
    }
    if (ch <= 57) {
        digit = TRUE;
    }
    return digit;
}

int ov00_021EC9E0(Work *w) {
    struct {
        u16 len;
        u8 id[10];
    } temp;

    OSIntrMode irq;
    u8 *p;
    BOOL digit;
    u8 ch;
    MI_CpuFill8(w, 0, 60);
    w->type = 255;
    if (ov00_021EC724() != 4) {
        return 0;
    }
    w->network = ov00_021FAA40();
    if (w->network == 255 || w->network > 99) {
        w->network = 99;
        return 0;
    }
    irq = OS_DisableInterrupts();
    p = WCM_GetApMacAddress();
    DC_InvalidateRange(p, 6);
    if (!p) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
    MI_CpuCopy8(p, w->mac, 6);
    if (w->network >= 0 && w->network < 3) {
        p = WCM_GetApEssid(&temp.len);
        DC_InvalidateRange(p, 32);
        if (!p) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        MI_CpuCopy8(p, w->ssid, temp.len);
    }
    OS_RestoreInterrupts(irq);
    if (ov00_021FAA70(temp.id)) {
        digit = FALSE;
        ch = temp.id[0];
        if (ch < 48) {
            goto selected;
        }
        if (ch <= 57) {
            digit = TRUE;
        }
selected:
        w->type = selectType(digit, ch);
        MI_CpuCopy8(temp.id + 1, w->id, 9);
    } else {
        w->type = 255;
    }
    return 1;
}
