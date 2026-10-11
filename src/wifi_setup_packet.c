#include "global.h"

#include "wifi_setup_internal.h"

// Swaps the two bytes of a 16-bit value (network byte order).
#define Swap16(value) ((u16)((u8)((value) >> 8) | (((value) << 8) & 0xff00)))

typedef struct WifiSetupApEntry {
    u32 ssidLength;
    char ssid[32];
    u8 unk_24[4];
    u8 bssid[6];
    u16 privacy;
} WifiSetupApEntry;

typedef struct WifiSetupApEntryList {
    u32 count;
    WifiSetupApEntry entries[];
} WifiSetupApEntryList;

typedef struct WifiSetupMacPair {
    u8 first[6];
    u8 second[6];
    u8 prefix[4];
} WifiSetupMacPair;

typedef struct WifiSetupSocketAddress {
    u8 host[4];
    u8 size;
    u8 family;
    u16 port;
    s32 tail;
} WifiSetupSocketAddress;

typedef struct WifiSetupVersion {
    u8 bytes[7];
} WifiSetupVersion;

typedef struct WifiSetupVersionRecord {
    u16 value;
    WifiSetupVersion version;
} WifiSetupVersionRecord;

typedef struct WifiSetupSettings64 {
    u64 words[8];
} WifiSetupSettings64;

extern char ov13_0224E440[]; // SSID of the chosen access point
extern u8 ov13_0224E3E0[6];  // BSSID of the chosen access point
extern u8 ov13_0224E3D8[6];  // own MAC address
extern u8 ov13_0224E464[];
extern u8 ov13_02245B18[4];
extern WifiSetupVersion ov13_02242680;
extern u32 ov13_0224E284[8];
extern u32 ov13_0224E2B0[];
extern u8 ov13_0224E184[];
extern u8 ov13_0224E2E8[][40];
extern u32 ov13_0224E380[18];
extern u8 ov13_0224E0B0[];
extern u8 ov13_0224E0C4[];
extern WifiSetupSettings64 ov13_0224E144;

int ov13_02223AE4(WifiSetupApInfo *list, u32 count);
int ov13_02223B28(int a, int b, int c, u32 d);
u32 ov13_02223E6C(void);
void ov13_02223ED4(void *arg);
int ov13_02224164(const WifiSetupApEntryList *current, const WifiSetupApEntryList *previous, u32 *selected);
int SOC_SendTo(int socket, const void *data, int length, int flags, const void *address);
u32 SOCL_GetHostID(void);
void SOC_U32to4U8(u32 value, void *out);

int ov13_02224330(void);
int ov13_022245C4(int socket, const void *address, const void *unused, const void *data, int length);
int ov13_022245DC(int socket, const void *data, int length);
int ov13_02224618(int socket, int unused, const void *data, int length);
u8 *ov13_02224624(u8 *data, u32 *type, u32 *length);
u8 *ov13_02224694(u8 **cursor, u8 *end, u32 *type, u32 *length);
u8 *ov13_022246E4(u8 *data, u32 *type, u32 *length);
u32 ov13_02224718(u8 *dest, u32 type, u8 *data, u32 length, const void *key);
u8 *ov13_022247DC(u8 *out, int tag, const void *source, int size);
u8 *ov13_0222483C(u8 *dst, u16 type, const void *src, u32 length);
int ov13_02224864(void *data, int *extended);
int ov13_02224938(u8 *data, u32 expectedType, void *dst, int key);
void ov13_02224988(void *dst);
BOOL ov13_02224A24(u8 *mac);
BOOL ov13_02224A30(WifiSetupMacPair *pair);
int ov13_02224B2C(const u8 *data);
int ov13_02224D00(u8 *dst, const char *src, int length);
int ov13_02224DB4(void);

#pragma force_active on
#pragma optimization_level 3

// Scans up to 30 times for the access point until ov13_02224164 picks one.
int ov13_02224330(void) {
    char text[32];
    OSAlarm alarm;
    u32 selected;
    int attempt;
    int i;
    int total;
    u32 size;
    WifiSetupApEntryList *out;
    WifiSetupApEntryList *previous;
    int result;
    int running;
    int event;
    int found;
    WifiSetupApInfo *ap;

    result = -1;
    previous = NULL;
    selected = 0;
    size = sizeof(WifiSetupApEntry) * ov13_0224DF30.apListCount + sizeof(WifiSetupApEntryList) + sizeof(WifiSetupApEntry);
    out = ov13_02226CD8(1, size);
    if (out == NULL) {
        goto done;
    }
    previous = ov13_02226CD8(1, size);
    if (previous == NULL) {
        goto done;
    }
    for (attempt = 0; attempt < 30 && !ov13_0224DF30.stop; attempt++) {
        if (ov13_02226CBC() >= ov13_02245A58.deadline) {
            break;
        }
        if (!ov13_02223B28(0, 0, 0, 0x30BFFE)) {
            result = -2;
            goto done;
        }
        OS_CreateAlarm(&alarm);
        OS_SetAlarm(&alarm, 0xFFB10, (OSAlarmHandler)ov13_02223ED4, (void *)19);
        running = TRUE;
        result = 0;
        do {
            OS_Sleep(10);
            if (ov13_02226CBC() >= ov13_02245A58.deadline || ov13_0224DF30.stop) {
                break;
            }
            while ((event = ov13_02223E6C()) != 0) {
                switch (event) {
                case 19:
                    running = FALSE;
                    break;
                case 5:
                    found = ov13_02223AE4(ov13_0224DF30.apList, ov13_0224DF30.apListCount);
                    if (found > result) {
                        result = found;
                        OS_CancelAlarm(&alarm);
                        OS_SetAlarm(&alarm, 0xFFB10, (OSAlarmHandler)ov13_02223ED4, (void *)19);
                    }
                    break;
                case 10:
                    running = FALSE;
                    break;
                case 4:
                case 8:
                case 18:
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                default:
                    running = FALSE;
                    break;
                }
            }
        } while (running);
        OS_CancelAlarm(&alarm);
        while (ov13_02223E6C() != 0) {
        }
        if (ov13_0224DF30.stop) {
            break;
        }
        total = 0;
        if (result >= ov13_0224DF30.apListCount) {
            result = -6;
            goto done;
        }
        ap = ov13_0224DF30.apList;
        i = 0;
        if (result > 0) {
            for (; i < result; i++) {
                memcpy(out->entries[total].ssid, ap->ssid, 32);
                out->entries[total].ssidLength = ap->ssidLength;
                out->entries[total].ssid[ap->ssidLength] = 0;
                out->entries[total].privacy = (ap->capability & 0x10) ? 1 : 0;
                memcpy(out->entries[total].bssid, ap->bssid, 6);
                ap++;
                total++;
            }
        }
        out->count = result;
        if (ov13_0224DF30.state != 1 && ov13_02224164(out, previous, &selected)) {
            WifiSetupApEntry *entry = &out->entries[selected];
            ov13_0224DF30.selection = selected;
            strcpy(ov13_0224E440, entry->ssid);
            memcpy(ov13_0224E3E0, entry->bssid, 6);
            ov13_02226D40(text, (const s8 *)ov13_0224E3E0);
            break;
        }
        memcpy(previous, out, size);
        ov13_0224DF30.state = 2;
        ov13_02226F3C();
    }
    if (attempt >= 30 || ov13_02226CBC() > ov13_02245A58.deadline) {
        result = -3;
    } else if (ov13_0224DF30.stop) {
        result = -8;
    } else {
        result = 1;
    }
done:
    if (out != NULL) {
        ov13_02226CFC(out);
    }
    if (previous != NULL) {
        ov13_02226CFC(previous);
    }
    return result;
}

#pragma optimization_level reset
#pragma force_active reset

int ov13_022245C4(int socket, const void *address, const void *unused, const void *data, int length) {
    int result = SOC_SendTo(socket, data, length, 0, address);
    if (result < 0) {
        result = -4;
    }
    return result;
}

// Broadcast to port 0xE601 (byte-swapped 0x01E6).
int ov13_022245DC(int socket, const void *data, int length) {
    WifiSetupSocketAddress address;
    address.size = 8;
    address.family = 2;
    address.tail = -1;
    address.port = 0x1e6;
    SOC_U32to4U8(SOCL_GetHostID(), address.host);
    return ov13_022245C4(socket, &address.size, address.host, data, length);
}

int ov13_02224618(int socket, int unused, const void *data, int length) {
    return ov13_022245DC(socket, data, length);
}

// Packet: type (2), length (2), 2 bytes, payload, 16-bit byte sum. Returns the payload or NULL.
u8 *ov13_02224624(u8 *data, u32 *type, u32 *length) {
    u32 sum = 0;
    u8 *end;
    u8 *p;

    *type = Swap16(*(u16 *)data);
    end = data + 6 + (*length = Swap16(*(u16 *)(data + 2)));
    for (p = data; p < end; p++) {
        sum += *p;
    }
    if ((u16)sum != Swap16(*(u16 *)end)) {
        return NULL;
    }
    return data + 6;
}

// Next record (type, length, value padded to 8 bytes) of a record list.
u8 *ov13_02224694(u8 **cursor, u8 *end, u32 *type, u32 *length) {
    u8 *p = *cursor;
    u32 padded;
    if (p >= end) {
        return NULL;
    }
    *type = Swap16(*(u16 *)p);
    *length = Swap16(*(u16 *)(p + 2));
    p += 4;
    padded = (*length + 11) & ~7;
    padded -= 4;
    *cursor = p + padded;
    return p;
}

u8 *ov13_022246E4(u8 *data, u32 *type, u32 *length) {
    u8 *cursor = data + 8;
    return ov13_02224694(&cursor, cursor + Swap16(*(u16 *)data), type, length);
}

#pragma force_active on

// Builds a packet from a record list, AES-key-wrapped when `key` is given; returns its size.
u32 ov13_02224718(u8 *dest, u32 type, u8 *data, u32 length, const void *key) {
    u8 *end = dest;
    u8 *p;
    u32 sum = 0;
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;
    data[7] = 0;
    *(u16 *)data = Swap16((u16)(length - 8));
    if (key) {
        ov13_02225358(dest + 6, data, length, key, 16);
        length += 8;
    } else {
        memcpy(dest + 6, data, length);
    }
    dest[0] = 0;
    dest[1] = 0;
    dest[2] = 0;
    dest[3] = 0;
    dest[4] = 0;
    dest[5] = 0;
    *(u16 *)dest = Swap16((u16)type);
    *(u16 *)(dest + 2) = Swap16((u16)length);
    end += 6;
    end += length;
    for (p = dest; p < end; p++) {
        sum += *p;
    }
    *(u16 *)end = Swap16((u16)sum);
    return (u32)(end + 2 - dest);
}

#pragma push
#pragma opt_propagation off

// Appends one record; returns the end of it.
u8 *ov13_022247DC(u8 *out, int tag, const void *source, int size) {
    int aligned;
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    *(u16 *)out = Swap16((u16)tag);
    aligned = ((size + 11) & ~7) - 4;
    *(u16 *)(out + 2) = Swap16((u16)size);
    out += 4;
    memset(out, 0, aligned);
    memcpy(out, source, size);
    return out + aligned;
}

#pragma pop
#pragma force_active reset

// A record list holding one record; the list header stores its length.
u8 *ov13_0222483C(u8 *dst, u16 type, const void *src, u32 length) {
    int result;
    int size;
    memset(dst, 0, 8);
    result = (int)ov13_022247DC(dst + 8, type, src, length);
    result -= (int)dst;
    size = result;
    size -= 8;
    *(u16 *)dst = (u16)size;
    return (u8 *)result;
}

#pragma force_active on

// Reply of type 1: records 1 and 2 must be 1; record 5 >= 1 asks for the extended request.
int ov13_02224864(void *data, int *extended) {
    int type;
    volatile int length;
    int tag;
    int size;
    u8 *volatile cursor;
    u8 *blob = ov13_02224624(data, (u32 *)&type, (u32 *)&length);
    int record1 = 0;
    int record2;
    int record5 = 0;
    const u16 *value;
    int limit;
    cursor = blob;
    record2 = 0;
    if (!blob) {
        return 0;
    }
    if (type != 1) {
        return 0;
    }
    limit = length;
    cursor = blob + 8;
    while ((value = (const u16 *)ov13_02224694((u8 **)&cursor, blob + limit, (u32 *)&tag, (u32 *)&size)) != 0) {
        switch (tag) {
        case 1:
            record1 = Swap16(*value);
            break;
        case 2:
            record2 = Swap16(*value);
            break;
        case 5:
            record5 = Swap16(*value);
            break;
        }
    }
    if (record1 != 1 || record2 != 1) {
        return 0;
    }
    if (record5 >= 1) {
        *extended = 1;
    } else {
        *extended = 0;
    }
    return 1;
}

#pragma force_active reset

// Copies the payload of a packet of `expectedType`, AES-key-unwrapped when `key` is given.
int ov13_02224938(u8 *data, u32 expectedType, void *dst, int key) {
    u32 type, length;
    u8 *payload = ov13_02224624(data, &type, &length);
    if (payload == NULL) {
        return 0;
    }
    if (type != expectedType) {
        return 0;
    }
    if (key) {
        ov13_02225510(dst, payload, length, (const void *)key, 16);
        length -= 8;
    } else {
        memcpy(dst, payload, length);
    }
    return length;
}

#pragma force_active on

// First request (type 2): version records, plus own MAC address in the extended form.
void ov13_02224988(void *dst) {
    WifiSetupVersionRecord record;
    u8 *ptr;
    record.value = 256;
    record.version = ov13_02242680;
    ptr = ov13_022247DC(ov13_02245A58.packetBuffer, 1, &record, 2);
    ptr = ov13_022247DC(ptr, 2, &record, 2);
    if (ov13_0224DF30.extendedRequest) {
        ptr = ov13_022247DC(ptr, 5, &record, 2);
    }
    ptr = ov13_022247DC(ptr, 3, &record.version, 7);
    if (ov13_0224DF30.extendedRequest) {
        ptr = ov13_022247DC(ptr, 4, ov13_0224E3D8, 6);
    }
    ov13_02224718(dst, 2, ov13_0224E464, ptr - ov13_02245A58.packetBuffer + 8, NULL);
}

#pragma force_active reset

BOOL ov13_02224A24(u8 *mac) {
    OS_GetMacAddress(mac);
    return TRUE;
}

#pragma force_active on

// Orders the access point's and our MAC address (the smaller one first).
BOOL ov13_02224A30(WifiSetupMacPair *pair) {
    u8 other[6];
    u8 own[6];
    char text1[32];
    char text2[32];
    u8 *src;

    memcpy(pair->prefix, ov13_02245B18, 4);
    memcpy(other, ov13_0224E3E0, 6);
    other[0] &= 0xFD;
    ov13_02224A24(own);
    memcpy(ov13_0224E3D8, own, 6);
    if (memcmp(other, own, 6) <= 0) {
        memcpy(pair->first, own, 6);
        src = other;
    } else {
        memcpy(pair->first, other, 6);
        src = own;
    }
    memcpy(pair->second, src, 6);
    if (ov13_02245A58.debug) {
        ov13_02226D40(text1, (const s8 *)own);
        ov13_02226D40(text2, (const s8 *)other);
    }
    return TRUE;
}

// Settings reply: records 0x201-0x20A.
int ov13_02224B2C(const u8 *data) {
    u8 *start = (u8 *)data + 8;
    u8 *pos = start;
    u8 *value;
    int result = 0;
    u32 bytes = Swap16(*(const u16 *)data);
    u32 type;
    u32 length;
    value = ov13_02224694(&pos, start + bytes, &type, &length);
    while (value) {
        switch (type) {
        case 0x201:
            memset(ov13_0224E284, 0, sizeof(ov13_0224E284));
            memcpy(ov13_0224E284, value, length);
            result = 1;
            break;
        case 0x202:
            ov13_0224E2B0[0] = Swap16(*(u16 *)value);
            break;
        case 0x203: {
            int i = 0;
            u8 *p = ov13_0224E184;
            u16 v = Swap16(*(u16 *)value);
            do {
                *(u32 *)(p + 0x15c) = v;
                p += 40;
                i++;
            } while (i < 4);
            break;
        }
        case 0x204: {
            u8 *p = ov13_0224E184;
            int i = 0;
            u16 v = Swap16(*(u16 *)value);
            do {
                *(u32 *)(p + 0x160) = v;
                p += 40;
                i++;
            } while (i < 4);
            break;
        }
        case 0x205:
            ov13_0224E2B0[1] = Swap16(*(u16 *)value);
            break;
        case 0x206:
        case 0x207:
        case 0x208:
        case 0x209: {
            u8 *out;
            int i;
            memset(ov13_0224E2E8[type - 0x206], 0, 32);
            if (ov13_0224E2B0[12] == 1) {
                out = ov13_0224E2E8[type - 0x206];
                for (i = 0; i < (int)length; i++) {
                    out += ov13_02226D0C((char *)out, *(s8 *)value++);
                }
            } else {
                memcpy(ov13_0224E2E8[type - 0x206], value, length);
            }
            break;
        }
        case 0x20a:
            memset(ov13_0224E380, 0, sizeof(ov13_0224E380));
            memcpy(ov13_0224E380, value, length);
            break;
        }
        value = ov13_02224694(&pos, start + bytes, &type, &length);
    }
    return result;
}

#pragma force_active reset

int ov13_02224D00(u8 *dst, const char *src, int length) {
    int len = length;
    int c;
    int nibble = 0;
    const char *text = src;
    int i = nibble;

    for (; i < len; i++) {
        c = text[i];
        switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            nibble += c - '0';
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            nibble += c - 'a' + 10;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            nibble += c - 'A' + 10;
            break;
        default:
            return 0;
        }
        if ((i % 2) == 0) {
            nibble <<= 4;
        } else {
            dst[i / 2] = nibble;
            nibble = 0;
        }
    }
    return 1;
}

// Copies the received settings into the result block; WEP keys of 5/13/16 characters or
// 10/26/32 hex digits.
int ov13_02224DB4(void) {
    volatile int result = 1;
    int i;
    u8 *src;
    char buf[33];
    char *p;
    u8 *out;
    strcpy((char *)&ov13_0224E09C, (char *)ov13_0224E284);
    switch (*(u32 *)ov13_0224E2B0) {
    case 0:
        *(u32 *)(ov13_0224E0B0 + 0xC) = 0;
        break;
    case 1:
        if (*(u32 *)((u8 *)ov13_0224E2B0 + 4) == 0) {
            result = -7;
            break;
        }
        *(u32 *)(ov13_0224E0B0 + 0x10) = *(u32 *)((u8 *)ov13_0224E2B0 + 4);
        src = (u8 *)ov13_0224E2E8;
        out = ov13_0224E0C4;
        i = 0;
        p = buf;
        for (; i < 4; i++, src += 0x28, out += 0x20) {
            memcpy(buf, src, 0x20);
            buf[32] = 0;
            switch (strlen(buf)) {
            case 0:
                break;
            case 5:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 1;
                memcpy(out, p, 5);
                break;
            case 10:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 1;
                ov13_02224D00(out, p, 10);
                break;
            case 13:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 2;
                memcpy(out, p, 13);
                break;
            case 26:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 2;
                ov13_02224D00(out, p, 26);
                break;
            case 16:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 3;
                memcpy(out, p, 16);
                break;
            case 32:
                *(u32 *)(ov13_0224E0B0 + 0xC) = 3;
                ov13_02224D00(out, p, 32);
                break;
            default:
                result = -7;
                break;
            }
        }
        break;
    case 2:
        *(u32 *)(ov13_0224E0B0 + 0xC) = 4;
        memcpy(&ov13_0224E144, &ov13_0224E380, 64);
        break;
    case 3:
        *(u32 *)(ov13_0224E0B0 + 0xC) = 5;
        memcpy(&ov13_0224E144, &ov13_0224E380, 64);
        break;
    default:
        result = -7;
        break;
    }
    return result;
}
