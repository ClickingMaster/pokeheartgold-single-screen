#pragma force_active on
#include "global.h"
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
extern Record ov13_0224D660;
extern Record ov13_0224D790;
extern u32 gWifiSetupState[];
void WifiSetup_MemFill(void *dst, int value, u32 size);
void WifiSetup_MemCopy(void *dst, const void *src, u32 size);
int WifiSetup_StrLen(const void *str);
int WifiSetup_CheckPrintable(const void *str, int length);
int WifiSetup_StoreAossResult(Out *out);
// @fn WifiSetup_StoreAossResult
int WifiSetup_StoreAossResult(Out *out) {
    Record *a = &ov13_0224D8C0;
    u8 *dst = out->data;
    Record *c = &ov13_0224D660;
    Record *d = &ov13_0224D790;
    Record *b = &ov13_0224D930;

    if (dst == NULL) {
        return -1;
    }
    out->flags = gWifiSetupState[11] & gWifiSetupState[12];
    WifiSetup_MemFill(dst, 0, 0x154);
    if (out->flags & 1) {
        WifiSetup_MemCopy(dst, c->fields[0], c->length);
        WifiSetup_MemCopy(dst + 6, c->fields[1], c->length);
        WifiSetup_MemCopy(dst + 12, c->fields[2], c->length);
        WifiSetup_MemCopy(dst + 18, c->fields[3], c->length);
        if (WifiSetup_CheckPrintable(c->text, WifiSetup_StrLen(c->text))) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0x18, c->text, WifiSetup_StrLen(c->text));
    }
    if (out->flags & 2) {
        WifiSetup_MemCopy(dst + 0x39, d->fields[0], d->length);
        WifiSetup_MemCopy(dst + 0x47, d->fields[1], d->length);
        WifiSetup_MemCopy(dst + 0x55, d->fields[2], d->length);
        WifiSetup_MemCopy(dst + 0x63, d->fields[3], d->length);
        if (WifiSetup_CheckPrintable(d->text, WifiSetup_StrLen(d->text))) {
            goto fail;
        }
        WifiSetup_MemCopy(dst + 0x71, d->text, WifiSetup_StrLen(d->text));
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
