#include "global.h"

#include "wifi_setup/wifi_setup.h"

typedef struct Record {
    u32 unused;
    int length;
    u8 text[0x28];
    u8 fields[4][0x40];
} Record;

typedef struct Out {
    u16 flags;
    u8 pad[0x114];
    u8 terminator;
    u8 data[0x154];
} Out;

extern Record ov13_0224D8C0;
extern Record ov13_0224D930;
extern Record gWifiSetupWep64Record;
extern Record gWifiSetupWep128Record;

int WifiSetup_CheckPrintable(const void *str, int length);
int WifiSetup_StoreAossResult(Out *out);

// Copy the keys of the requested settings that were received into the result.
int WifiSetup_StoreAossResult(Out *out) {
    Record *a = &ov13_0224D8C0;
    u8 *dst = out->data;
    Record *wep64 = &gWifiSetupWep64Record;
    Record *wep128 = &gWifiSetupWep128Record;
    Record *b = &ov13_0224D930;

    if (dst == NULL) {
        return -1;
    }
    out->flags = gWifiSetupState.requestedMask & gWifiSetupState.receivedMask;
    WifiSetup_MemFill(dst, 0, 0x154);
    if (out->flags & 1) {
        WifiSetup_MemCopy(dst, wep64->fields[0], wep64->length);
        WifiSetup_MemCopy(dst + 6, wep64->fields[1], wep64->length);
        WifiSetup_MemCopy(dst + 12, wep64->fields[2], wep64->length);
        WifiSetup_MemCopy(dst + 18, wep64->fields[3], wep64->length);
        if (WifiSetup_CheckPrintable(wep64->text, WifiSetup_StrLen(wep64->text))) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0x18, wep64->text, WifiSetup_StrLen(wep64->text));
    }
    if (out->flags & 2) {
        WifiSetup_MemCopy(dst + 0x39, wep128->fields[0], wep128->length);
        WifiSetup_MemCopy(dst + 0x47, wep128->fields[1], wep128->length);
        WifiSetup_MemCopy(dst + 0x55, wep128->fields[2], wep128->length);
        WifiSetup_MemCopy(dst + 0x63, wep128->fields[3], wep128->length);
        if (WifiSetup_CheckPrintable(wep128->text, WifiSetup_StrLen(wep128->text))) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0x71, wep128->text, WifiSetup_StrLen(wep128->text));
    }
    if (out->flags & 4) {
        if (WifiSetup_CheckPrintable(a->fields[0], a->length - 1)) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0x92, a->fields[0], a->length);
        if (WifiSetup_CheckPrintable(a->text, WifiSetup_StrLen(a->text))) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0xD2, a->text, WifiSetup_StrLen(a->text));
    }
    if (out->flags & 8) {
        if (WifiSetup_CheckPrintable(b->fields[0], b->length - 1)) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0xF3, b->fields[0], b->length);
        if (WifiSetup_CheckPrintable(b->text, WifiSetup_StrLen(b->text))) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0x133, b->text, WifiSetup_StrLen(b->text));
    }
    out->terminator = 0;
    return 0;
fail:
    WifiSetup_MemFill(dst, 0, 0x154);
    return -1;
}
