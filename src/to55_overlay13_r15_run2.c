#include "global.h"
#pragma require_prototypes off

extern u8 ov13_0224DEE0[];
extern u8 ov13_0224DF08[];
extern u8 ov13_0224DF10[];
extern u8 ov13_0224DF30[];
extern u8 ov13_0224E3E8[];
extern u8 ov13_0224E420[];
extern u8 _02108FC0[];
extern u8 _02108FC8[];
extern int WCM_LockApList(int lock);
extern int WCM_CountApList(void);
extern void *WCM_PointApList(u16 index);
extern int WCM_SearchAsync(void *, void *, int);
extern int WCM_DisconnectAsync(void);
extern int WCM_CleanupAsync(void);
extern int WCM_StartupAsync(void *, void (*)(void *));
extern int WCM_ConnectAsync(void *, void *, int);

typedef struct WcmSmall {
    void *work;
    int searchArg;
    void *startupArg;
    int connectArg;
    void *bssid;
    void *ssid;
    int state;
    void (*callback)(int, int);
    void *connectBssid;
    void *connectSsid;
} WcmSmall;

typedef struct WcmLarge {
    u8 pad00[0x28];
    void *connectSsid;
    int state;
    u8 pad30[0x14];
    void *bssid;
    void *ssid;
    void *connectBssid;
    int connectArg;
    u8 pad54[0xc];
    int searchArg;
} WcmLarge;

#define SMALL (*(WcmSmall *)ov13_0224DEE0)
#define LARGE (*(WcmLarge *)ov13_0224DF30)

void ov13_02223114(void *arg);
void ov13_02223770(void *arg);

int ov13_02223A7C(void) {
    switch (LARGE.state) {
    case 5:
        if (WCM_SearchAsync(NULL, NULL, 0) != 3) {
            return 0;
        }
        break;
    case 7:
        if (WCM_DisconnectAsync() != 3) {
            return 0;
        }
        break;
    case 1:
        if (WCM_StartupAsync(*(void **)(ov13_0224DF30 + 8), ov13_02223770) != 3) {
            return 0;
        }
        break;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 8:
    default:
        return 0;
    }
    return 1;
}

int ov13_02223AE4(u8 *dst, int max) {
    int count;
    int i;
    WCM_LockApList(1);
    count = WCM_CountApList();
    if (count > 0) {
        i = 0;
        if (count > 0) {
            do {
                if (i >= max) {
                    break;
                }
                MIi_CpuCopy32(WCM_PointApList((u16)i), (u32 *)dst, 0xc0);
                i++;
                dst += 0xc0;
            } while (i < count);
        }
    }
    WCM_LockApList(0);
    return count;
}

int ov13_02223B28(const u8 *bssid, const u8 *ssid, int len, int searchArg) {
    OSIntrMode irq;
    int i;
    irq = OS_DisableInterrupts();
    LARGE.searchArg = searchArg;
    LARGE.bssid = ov13_0224E3E8;
    if (bssid != NULL) {
        for (i = 0; i < 6; i++) {
            ov13_0224E3E8[i] = *bssid++;
        }
    } else {
        MI_CpuFill8(ov13_0224E3E8, 0xff, 6);
        LARGE.bssid = _02108FC0;
    }
    LARGE.ssid = ov13_0224E420;
    if (ssid != NULL && len > 0 && len < 0x20) {
        for (i = 0; i < len; i++) {
            ov13_0224E420[i] = *ssid++;
        }
        for (; i < 0x20; i++) {
            ov13_0224E420[i] = 0;
        }
    } else {
        MI_CpuFill8(ov13_0224E420, 0xff, 0x20);
        LARGE.ssid = _02108FC8;
    }
    if (LARGE.state == 3) {
        if (WCM_SearchAsync(ov13_0224E3E8, LARGE.ssid, LARGE.searchArg) == 3) {
            LARGE.state = 6;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    } else if (ov13_02223A7C() == 1) {
        LARGE.state = 6;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}
