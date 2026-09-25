#include "global.h"

typedef struct Ov74Entry {
    void *ptr;
    u16 pad4;
    u16 id;
    u8 flag8;
    u8 pad9;
    u8 flagA;
    u8 padB;
} Ov74Entry;

extern Ov74Entry *ov74_0223115C(void);
extern u8 *ov74_02231054(void);
extern u8 *ov74_022310C4(void);
extern void *ov74_02231100(void);
extern u16 WM_GetAllowedChannel(void);
extern u16 WM_GetDispersionBeaconPeriod(void);
extern u16 WM_GetNextTgid(void);
extern u16 WM_GetDispersionScanPeriod(void);

void ov74_0222FEA0(u16 id);
int ov74_0222FEC8(void);
BOOL ov74_0222FF28(void);
void ov74_0222FF68(void);
void ov74_0222FF80(void);
void ov74_0222FFAC(void);

void ov74_0222FEA0(u16 id) {
    Ov74Entry *entry = ov74_0223115C();
    int i;
    for (i = 0; i < 8; i++, entry++) {
        if (entry->ptr != NULL && id == entry->id) {
            entry->flag8 = 0;
            entry->flagA = 0;
        }
    }
}

int ov74_0222FEC8(void) {
    u8 *work = (u8 *)ov74_0223115C();
    u16 flags = *(u16 *)(work + 0x62);
    if (flags & 1) {
        *(u16 *)(work + 0x62) &= ~1;
        return 1;
    }
    if (flags & 0x40) {
        *(u16 *)(work + 0x62) &= ~0x40;
        return 7;
    }
    if (flags & 0x1000) {
        *(u16 *)(work + 0x62) &= ~0x1000;
        return 13;
    }
    return 0;
}

BOOL ov74_0222FF28(void) {
    u8 *work = (u8 *)ov74_0223115C();
    u16 allowed;
    work[0x60] = 1;
    allowed = WM_GetAllowedChannel();
    *(u16 *)(work + 0x62) = allowed;
    if (*(u16 *)(work + 0x62) == 0 || *(u16 *)(work + 0x62) == 0x8000) {
        return FALSE;
    }
    work[0x61] = 0x66;
    work[0x64] = 0;
    return TRUE;
}

void ov74_0222FF68(void) {
    u8 *work = ov74_02231054();
    *(u16 *)(work + 0x18) = WM_GetDispersionBeaconPeriod();
    *(u16 *)(work + 0x0c) = WM_GetNextTgid();
}

void ov74_0222FF80(void) {
    u8 *work = ov74_022310C4();
    *(void **)work = ov74_02231100();
    *(u16 *)(work + 4) = *(u16 *)(ov74_02231054() + 0x32);
    *(u16 *)(work + 6) = WM_GetDispersionScanPeriod();
    work[8] = 0xFF;
    work[9] = 0xFF;
    work[10] = 0xFF;
    work[11] = 0xFF;
    work[12] = 0xFF;
    work[13] = 0xFF;
}
