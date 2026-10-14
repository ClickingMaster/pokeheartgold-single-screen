#include "global.h"

#include "wifi_setup.h"

extern int ov13_022459B4[2];
extern u32 gWifiSetupCrc32Table[256];
s32 WifiSetup_NotifyStatus(s32 state);

typedef struct {
    u32 length;
    u8 data[0x4c];
    u32 flags;
} Entry;

typedef struct {
    u32 count;
    Entry entries[64];
} List;

extern char sStr_ESSID_AOSS[];
extern char sStr_MELCO[];
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
void WifiSetup_SetError(int value);
extern u8 ov13_0224CFD8[];
extern u8 ov13_0224D658[];

typedef struct Packet {
    u8 pad[6];
    u16 kind;
    u8 gap[2];
    u16 result;
    u16 flags;
    u8 mode;
    u8 padf;
    u8 address[8];
    u16 length;
    u16 other;
    u8 data[1];
} Packet;

extern u8 gWifiSetupPeerAddress[];
extern void WifiSetup_Free(void *);
extern int WifiSetup_GetError(void);
extern void *WifiSetup_Alloc(int);
extern u8 sWifiSetupSecurityRecordTypes[];

typedef struct {
    u8 type, reserved;
    u16 length;
} Header;

extern s8 ov13_0224CFB8[];
extern u8 gWifiSetupCipherSeed[];
extern u8 ov13_0224DD02[];
u32 _u32_div_f(u32, u32);

int WifiSetup_CheckPrintable(const u8 *data, int length);
int WifiSetup_FindAossAccessPoint(List *list);
int WifiSetup_InitAossConnectInfo(void *out);
void WifiSetup_InitPacketAddresses(int count, u8 *dst, const void *src);
int WifiSetup_SetStatus(int value);
int WifiSetup_HandlePacket(int state, void *packet, int *counter, void *context);
int WifiSetup_HandleJoinReply(int state, void *packet, int *counter, void *context);
int WifiSetup_HandleSettingsReply(int state, void *packet, int *counter, void *context);
int WifiSetup_HandleDoneReply(int state, void *packet, int *counter, void *context);
int WifiSetup_DecryptPacket(Packet *p);
int WifiSetup_CheckPeer(int value, const void *data);
int WifiSetup_CheckReplySequence(const u8 *a, u8 *b);
int WifiSetup_ParseJoinReply(const u8 *src, u8 *dst);
int WifiSetup_ReadLittleEndian(const u8 *src, int length);
int WifiSetup_ParseWepRecord(const u8 *src, u8 *dst);
int WifiSetup_ParseWpaRecord(const u8 *src, u8 *dst);
int ov13_02221F64(const u8 *a, void *dst);
int WifiSetup_ParseSecurityRecords(int id, u8 *p, int remaining, u8 *out, u8 *other);
int WifiSetup_IsEncryptionRequested(int value);
int WifiSetup_SendRequest(int which, void *a, void *b, void *c);
int WifiSetup_SendJoinRequest(void *unused, const u8 *src, void *arg);
int WifiSetup_SendSettingsRequest(void *unused, const u8 *src, void *arg);
int WifiSetup_SendDoneRequest(void *unused, const u8 *src, void *arg);
int WifiSetup_BuildJoinPayload(u8 *dst);
void WifiSetup_WriteBody(int mode, u8 *dst, const void *src, s16 *length, u16 *flag, const void *extra);
void WifiSetup_WritePacketHeader(u8 *dst, int a1, int a2, int a3, s8 a4, s8 a5, const void *src);
void WifiSetup_SendPacket(void *a0, void *a1, int index, void *a3);
int WifiSetup_Encrypt(void *src, void *dst, int size, u8 *checksum, void *seedDst, const void *seedSrc, int seedSize);
int WifiSetup_Decrypt(void *dst, const void *src, int size, int expected, void *seedDst, const void *seedSrc, int seedSize);
void WifiSetup_CipherInit(WifiSetupCipher *state, const u8 *key, u32 keySize, u32 tableSize);
void WifiSetup_CipherXor(WifiSetupCipher *state, u8 *dst, const u8 *src, u32 size);
u8 WifiSetup_CipherNextByte(WifiSetupCipher *state);
u8 WifiSetup_CalcCrc32(const void *data, s32 size);
u32 WifiSetup_UpdateCrc32(u32 crc, const u8 *data, s32 size, s32 init, u32 *table);
void WifiSetup_InitCrc32Table(s32 unused, u32 *table);
s32 WifiSetup_Scramble(void *dst, s32 size, const s8 *key, s32 keySize);
void WifiSetup_ScrambleMakeKey(int start, s8 *out, int length, const s8 *key, int keylen);
void WifiSetup_ScrambleXorHalf(s8 *in, u8 *out, int length);
void WifiSetup_ScrambleSwapHalves(u8 *dst, s32 size, u8 *tmp);

int WifiSetup_CheckPrintable(const u8 *data, int length) {
    int i;
    for (i = 0; i < length; i++) {
        u8 value = *data++;
        if (value < 0x20 || value > 0x7F) {
            return -1;
        }
    }
    return 0;
}

#pragma force_active on

int WifiSetup_FindAossAccessPoint(List *list) {
    int result = 0;
    int matches = 0;
    int i;
    int count;
    if (list->count == 0) {
        return 5;
    }
    count = list->count;
    if ((u32)count > 64) {
        count = 64;
    }
    for (i = 0; i < count; i++) {
        if (list->entries[i].flags & 1) {
            if (list->entries[i].length == WifiSetup_StrLen(sStr_ESSID_AOSS) && WifiSetup_MemCompare(list->entries[i].data, (const u8 *)sStr_ESSID_AOSS, WifiSetup_StrLen(sStr_ESSID_AOSS)) == 0) {
                matches++;
            }
        }
    }
    if (matches > 1) {
        result = 4;
    }
    if (matches == 0) {
        result = 5;
    }
    return result;
}

#pragma force_active reset

int WifiSetup_InitAossConnectInfo(void *out) {
    u8 *p = out;
    *(u32 *)p = WifiSetup_StrLen(sStr_ESSID_AOSS);
    WifiSetup_MemCopy(p + 4, sStr_ESSID_AOSS, *(u32 *)p);
    *(u32 *)(p + 0x24) = 1;
    *(u32 *)(p + 0x28) = WifiSetup_StrLen(sStr_MELCO);
    if (*(u32 *)(p + 0x28) > 13) {
        return -1;
    }
    WifiSetup_MemCopy(p + 0x2C, sStr_MELCO, *(u32 *)(p + 0x28));
    return 0;
}

void WifiSetup_InitPacketAddresses(int count, u8 *dst, const void *src) {
    int i;
    for (i = 0; i < count; i++) {
        WifiSetup_MemCopy(dst, src, 6);
        *(u16 *)(dst + 6) = WifiSetup_Random();
        *(u16 *)(dst + 6) = WifiSetup_HostToNet16(*(u16 *)(dst + 6));
        dst += 8;
    }
}

int WifiSetup_SetStatus(int value) {
    if (value == -1) {
        ov13_022459B4[1] = value;
        return 0;
    }
    if (ov13_022459B4[1] != value) {
        ov13_022459B4[1] = value;
        return WifiSetup_NotifyStatus(value);
    }
    return 0;
}

int WifiSetup_HandlePacket(int state, void *packet, int *counter, void *context) {
    u8 *header = (u8 *)packet + 0xC;
    int type;

    if (WifiSetup_NetToHost16(U16_AT(header, 0)) < 1) {
        (*counter)++;
        return state;
    }
    if (U8_AT(header, 0xF) != 0x11) {
        (*counter)++;
        return state;
    }
    if (WifiSetup_DecryptPacket((Packet *)((u8 *)packet + 0xC)) > 0) {
        (*counter)++;
        return state;
    }
    type = WifiSetup_NetToHost16(U16_AT(header, 6));
    switch (type) {
    case 0x1010:
        state = WifiSetup_HandleJoinReply(state, packet, counter, context);
        break;
    case 0x2010:
        state = WifiSetup_HandleSettingsReply(state, packet, counter, context);
        break;
    case 0x3010:
        state = WifiSetup_HandleDoneReply(state, packet, counter, context);
        break;
    }
    return state;
}

int WifiSetup_HandleJoinReply(int state, void *packet, int *counter, void *context) {
    u8 *header;
    u8 *body;
    int value;

    if (state != 0) {
        (*counter)++;
        return state;
    }
    header = (u8 *)packet + 0xC;
    body = (u8 *)packet + 0x24;
    if (WifiSetup_CheckReplySequence(context, header + 0x10) < 0) {
        (*counter)++;
        return state;
    }
    if (WifiSetup_NetToHost16(U16_AT(body, 2)) == 0) {
        (*counter)++;
        return state;
    }
    if (body[0] == 7) {
        value = WifiSetup_NetToHost32(U32_AT(body, 4));
        if (value == -2) {
            WifiSetup_SetError(0x14);
        } else {
            value = WifiSetup_NetToHost32(U32_AT(body, 4));
            if (value == -3) {
                WifiSetup_SetError(0x15);
            } else {
                WifiSetup_SetError(0x18);
            }
        }
        return -1;
    }
    if (body[0] != 1) {
        (*counter)++;
        return state;
    }
    value = WifiSetup_ParseJoinReply(body + 4, ov13_0224CFD8);
    if (value < 0) {
        if (value == -2) {
            WifiSetup_SetError(0x16);
            return -1;
        }
        (*counter)++;
        return state;
    }
    gWifiSetupState.unk_00 = WifiSetup_IsEncryptionRequested(WifiSetup_NetToHost16(U16_AT(header, 0xC)));
    *counter = 0;
    return 1;
}

int WifiSetup_HandleSettingsReply(int state, void *packet, int *counter, void *context) {
    u8 *header;
    u8 *body;
    int value;

    if (state != 1) {
        (*counter)++;
        return state;
    }
    header = (u8 *)packet + 0xC;
    body = (u8 *)packet + 0x24;
    if (WifiSetup_CheckReplySequence((u8 *)context + 8, header + 0x10) < 0) {
        (*counter)++;
        return state;
    }
    if (WifiSetup_NetToHost16(U16_AT(body, 2)) == 0) {
        (*counter)++;
        return state;
    }
    if (body[0] == 7) {
        value = WifiSetup_NetToHost32(U32_AT(body, 4));
        if (value == -2) {
            WifiSetup_SetError(0x14);
        } else {
            value = WifiSetup_NetToHost32(U32_AT(body, 4));
            if (value == -3) {
                WifiSetup_SetError(0x15);
            } else {
                WifiSetup_SetError(0x18);
            }
        }
        return -1;
    }
    WifiSetup_MemFill(ov13_0224D658, 0, 0x6A0);
    value = WifiSetup_NetToHost16(U16_AT(header, 0xA));
    if (WifiSetup_ParseSecurityRecords(0, body, value, ov13_0224D658, ov13_0224CFD8) < 0) {
        (*counter)++;
        return state;
    }
    if ((gWifiSetupState.receivedMask & gWifiSetupState.requestedMask) == 0) {
        return state;
    }
    *counter = 0;
    return 2;
}

int WifiSetup_HandleDoneReply(int state, void *packet, int *counter, void *context) {
    u8 *body;
    int value;

    if (state != 2) {
        (*counter)++;
        return state;
    }
    body = (u8 *)packet + 0x24;
    context = (u8 *)context + 0x10;
    packet = (u8 *)packet + 0xC;
    if (WifiSetup_CheckReplySequence(context, (u8 *)packet + 0x10) < 0) {
        (*counter)++;
        return state;
    }
    if (body[0] != 7) {
        (*counter)++;
        return state;
    }
    if (WifiSetup_NetToHost16(U16_AT(body, 2)) == 0) {
        (*counter)++;
        return state;
    }
    value = WifiSetup_NetToHost32(U32_AT(body, 4));
    if (value == 0) {
        return 100;
    }
    value = WifiSetup_NetToHost32(U32_AT(body, 4));
    if (value == -2) {
        WifiSetup_SetError(0x14);
        return -1;
    }
    value = WifiSetup_NetToHost32(U32_AT(body, 4));
    if (value == -3) {
        WifiSetup_SetError(0x15);
        return -1;
    }
    WifiSetup_SetError(0x18);
    return -1;
}

#pragma force_active on

int WifiSetup_DecryptPacket(Packet *p) {
    u8 address[8];
    u16 *body = &p->length;
    int result;
    WifiSetup_MemCopy(address, p->address, 8);
    result = WifiSetup_Scramble(address, 8, sStr_MELCO, WifiSetup_StrLen(sStr_MELCO));
    if (result == -1) {
        WifiSetup_SetError(2);
        return -100;
    }
    result = WifiSetup_CheckPeer(WifiSetup_NetToHost16(p->kind), address);
    if (result == 0) {
        int size;
        void *buffer;
        if (WifiSetup_NetToHost16(p->kind) == 4096) {
            WifiSetup_MemCopy(gWifiSetupPeerAddress, address, 8);
        }
        if ((WifiSetup_NetToHost16(p->flags) & 15) == 0) {
            return 0;
        }
        size = WifiSetup_NetToHost16(*body);
        buffer = WifiSetup_Alloc(size);
        if (buffer == 0) {
            WifiSetup_SetError(2);
            return 100;
        }
        if (WifiSetup_Decrypt(body + 2, buffer, size, p->mode, body + 1, gWifiSetupPeerAddress, 8) < 0) {
            WifiSetup_Free(buffer);
            if (WifiSetup_GetError() == 2) {
                return 100;
            }
            return 200;
        }
        WifiSetup_MemCopy(body, buffer, size);
        p->result = WifiSetup_HostToNet16((u16)size);
        WifiSetup_Free(buffer);
        result = 0;
    }
    return result;
}

#pragma force_active reset

int WifiSetup_CheckPeer(int value, const void *data) {
    int i;
    int present;
    int result;
    result = 0;
    present = result;
    for (i = 0; i < 6; i++) {
        if (gWifiSetupPeerAddress[i] != 0) {
            present = 1;
            break;
        }
    }
    if (present) {
        if (WifiSetup_MemCompare(gWifiSetupPeerAddress, data, 6) != 0) {
            result = 1;
        }
    } else if (value != 0x1000) {
        result = 2;
    }
    return result;
}

int WifiSetup_CheckReplySequence(const u8 *a, u8 *b) {
    int result = 0;
    WifiSetup_Scramble(b, 8, sStr_MELCO, WifiSetup_StrLen(sStr_MELCO));
    if (WifiSetup_MemCompare(a, b, 6) != 0) {
        result = -1;
    } else {
        int av = WifiSetup_NetToHost16(*(u16 *)(a + 6));
        int bv = WifiSetup_NetToHost16(*(u16 *)(b + 6));
        if (av + 1 != bv) {
            result = -2;
        }
    }
    return result;
}

int WifiSetup_ParseJoinReply(const u8 *src, u8 *dst) {
    const u8 *entry;
    u8 *dst80;
    int length;

    WifiSetup_MemFill(dst, 0, 0x104);
    dst80 = dst;
    entry = src;
    dst80 += 0x80;
    for (;;) {
        length = WifiSetup_NetToHost16(*(const u16 *)(entry + 2));
        if (length <= 0) {
            return -1;
        }
        switch (entry[0]) {
        case 0:
            WifiSetup_MemCopy(dst, entry + 6, length);
            break;
        case 1:
            WifiSetup_MemCopy(dst80, entry + 6, length);
            break;
        case 2:
            WifiSetup_MemCopy(dst + 0x100, entry + 6, length);
            break;
        case 3:
        case 4:
            // called through an int-returning type: with the u16 return type the "<= 0" test becomes "== 0"
            if (((int (*)(int))WifiSetup_NetToHost16)(entry[6]) <= 0) {
                return -2;
            }
            break;
        case 5:
            gWifiSetupState.address = WifiSetup_NetToHost32(WifiSetup_ReadLittleEndian(entry + 6, length));
            break;
        case 6:
            gWifiSetupState.netmask = WifiSetup_NetToHost32(WifiSetup_ReadLittleEndian(entry + 6, length));
            break;
        default:
            return -1;
        }
        if (*(const u16 *)(entry + 4) == 0) {
            break;
        }
        entry = src + WifiSetup_NetToHost16(*(const u16 *)(entry + 4));
    }
    return 0;
}

int WifiSetup_ReadLittleEndian(const u8 *src, int length) {
    int offset = length - 1;
    int value = 0;
    const u8 *p = src + offset;
    int i = value;
    for (; i < length; i++) {
        value = (value << 8) + *p--;
    }
    return value;
}

int WifiSetup_ParseWepRecord(const u8 *src, u8 *dst) {
    const u8 *entry = src + 6;
    const u8 *base;
    u8 *dst08 = dst + 8;
    u8 *dstF0 = dst + 0xF0;
    u8 *dstB0 = dst + 0xB0;
    u8 *dst70 = dst + 0x70;
    u8 *dst30 = dst + 0x30;
    u32 length;
    base = entry;

    for (;;) {
        length = WifiSetup_NetToHost16(*(const u16 *)(entry + 2));
        switch (entry[0]) {
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            if (length > 5) {
                return -1;
            }
            break;
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
            if (length > 13) {
                return -1;
            }
            break;
        case 0x15:
        case 0x25:
            if (length > 33) {
                return -1;
            }
            break;
        }
        switch (entry[0]) {
        case 0x10:
        case 0x20:
            WifiSetup_MemCopy(dst30, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x11:
        case 0x21:
            WifiSetup_MemCopy(dst70, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x12:
        case 0x22:
            WifiSetup_MemCopy(dstB0, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x13:
        case 0x23:
            WifiSetup_MemCopy(dstF0, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x15:
        case 0x25:
            if (length != 0 && entry[5 + length] != 0) {
                return -1;
            }
            WifiSetup_MemCopy(dst08, entry + 6, length);
            break;
        default:
            return -1;
        }
        if (*(const u16 *)(entry + 4) == 0) {
            break;
        }
        entry = base + WifiSetup_NetToHost16(*(const u16 *)(entry + 4));
    }
    return 0;
}

int WifiSetup_ParseWpaRecord(const u8 *src, u8 *dst) {
    const u8 *entry = src + 6;
    const u8 *base;
    u8 *dst08 = dst + 8;
    u8 *dst30 = dst + 0x30;
    u32 length;
    base = entry;

    for (;;) {
        length = WifiSetup_NetToHost16(*(const u16 *)(entry + 2));
        switch (entry[0]) {
        case 0x30:
        case 0x40:
            if (length > 0x40) {
                return -1;
            }
            break;
        case 0x35:
        case 0x45:
            if (length > 0x21) {
                return -1;
            }
            break;
        }
        switch (entry[0]) {
        case 0x30:
        case 0x40:
            WifiSetup_MemCopy(dst30, entry + 6, length);
            *(int *)(dst + 4) = length;
            break;
        case 0x35:
        case 0x45:
            if (length != 0 && entry[5 + length] != 0) {
                return -1;
            }
            WifiSetup_MemCopy(dst08, entry + 6, length);
            break;
        default:
            return -1;
        }
        if (*(const u16 *)(entry + 4) == 0) {
            break;
        }
        entry = base + WifiSetup_NetToHost16(*(const u16 *)(entry + 4));
    }
    return 0;
}

int ov13_02221F64(const u8 *a, void *dst) {
    const u8 *item = a + 6;
    int size = WifiSetup_NetToHost16(*(u16 *)(item + 2));
    if (size <= 0) {
        return -1;
    }
    if (item[0] != 0x70) {
        return -1;
    }
    WifiSetup_MemCopy(dst, item + 6, size);
    return 0;
}

#pragma force_active on

int WifiSetup_ParseSecurityRecords(int id, u8 *p, int remaining, u8 *out, u8 *other) {
    int flags = 0;
    if (remaining <= 0) {
        return -2;
    }
    Header *record;
    for (;;) {
        record = (Header *)p;
        if (record->type == sWifiSetupSecurityRecordTypes[id]) {
            break;
        }
        int skip = WifiSetup_NetToHost16(*(u16 *)(p + 2)) + 4;
        remaining -= skip;
        p += skip;
        if (remaining <= 0) {
            return -4;
        }
    }
    u16 encoded = record->length;
    p += 4;
    int size = WifiSetup_NetToHost16(encoded);
    u8 *base = out + id * 0x350;
    u8 *first;
    u32 offset;
    u8 *otherCopy;
    otherCopy = other;
    offset = (id + 3) * 128;
    first = base;
    first += 8;
    do {
        int r;
        switch (p[0]) {
        case 3:
            r = WifiSetup_ParseWepRecord(p, first);
            flags |= 1;
            break;
        case 4:
            r = WifiSetup_ParseWepRecord(p, base + 0x138);
            flags |= 2;
            break;
        case 5:
            r = WifiSetup_ParseWpaRecord(p, base + 0x268);
            flags |= 4;
            break;
        case 6:
            r = WifiSetup_ParseWpaRecord(p, base + 0x2d8);
            flags |= 8;
            break;
        case 10:
            r = ov13_02221F64(p, otherCopy + offset);
            break;
        default:
            r = -3;
            break;
        }
        if (r) {
            return r;
        }
        int skip = WifiSetup_NetToHost16(*(u16 *)(p + 2)) + 4;
        size -= skip;
        p += skip;
    } while (size > 0);
    gWifiSetupState.receivedMask |= flags;
    return 0;
}

#pragma force_active reset

int WifiSetup_IsEncryptionRequested(int value) {
    int result = 0;
    if (value & 0x10) {
        result = 1;
    }
    return result;
}

int WifiSetup_SendRequest(int which, void *a, void *b, void *c) {
    switch (which) {
    case 0:
        WifiSetup_SetStatus(2);
        return WifiSetup_SendJoinRequest(a, b, c);
    case 1:
        WifiSetup_SetStatus(3);
        return WifiSetup_SendSettingsRequest(a, b, c);
    case 2:
        WifiSetup_SetStatus(5);
        return WifiSetup_SendDoneRequest(a, b, c);
    default:
        return -1;
    }
}

int WifiSetup_SendJoinRequest(void *unused, const u8 *src, void *arg) {
    struct Header {
        s8 value;
        u8 pad;
        s16 flag;
        s16 length;
    };

    u8 data[8];
    struct Header header;
    u8 *work;
    u8 *payload;
    u8 *encoded;

    header.value = 0;
    header.length = 0;
    header.flag = 0;
    work = gWifiSetupState.data;
    WifiSetup_MemFill(work, 0, 0x5DC);
    payload = (u8 *)WifiSetup_Alloc(0x210);
    if (payload == NULL) {
        WifiSetup_SetError(2);
        return -1;
    }
    WifiSetup_MemFill(payload, 0, 0x210);
    encoded = work + 0x18;
    WifiSetup_MemCopy(gWifiSetupPeerAddress, src, 8);
    WifiSetup_MemCopy(data, gWifiSetupPeerAddress, 8);
    header.length = WifiSetup_BuildJoinPayload(payload + 4);
    if (header.length < 0) {
        WifiSetup_SetError(3);
        if (payload != NULL) {
            WifiSetup_Free(payload);
        }
        return -1;
    }
    payload[0] = 0;
    *(u16 *)(payload + 2) = WifiSetup_HostToNet16((u16)header.length);
    header.length += 4;
    WifiSetup_WriteBody(0, encoded, payload, &header.length, (u16 *)&header.flag, &header);
    header.flag |= 0x10;
    if (WifiSetup_Scramble(data, 8, sStr_MELCO, 6) != 0) {
        WifiSetup_SetError(2);
        if (payload != NULL) {
            WifiSetup_Free(payload);
        }
        return -1;
    }
    WifiSetup_WritePacketHeader(work, 1 << 12, header.length, header.flag, header.value, 0x11, data);
    header.length += 0x18;
    WifiSetup_SendPacket(work, (void *)(int)header.length, 0xFF, arg);
    if (payload != NULL) {
        WifiSetup_Free(payload);
    }
    return 0;
}

int WifiSetup_SendSettingsRequest(void *unused, const u8 *src, void *arg) {
    struct Header {
        s8 value;
        u8 pad;
        s16 flag;
        s16 length;
        u16 padEnd;
    };

    u8 data[8];
    u8 packet[8];
    struct Header header;
    u8 *work;
    int size;

    header.value = 0;
    header.length = 0;
    header.flag = 0;
    work = gWifiSetupState.data;
    WifiSetup_MemFill(packet, 0, 8);
    WifiSetup_MemFill(work, 0, 0x5DC);
    packet[0] = 2;
    packet[1] = 0;
    *(u16 *)(packet + 2) = WifiSetup_HostToNet16(4);
    *(u32 *)(packet + 4) = gWifiSetupState.requestedMask;
    *(u32 *)(packet + 4) = WifiSetup_HostToNet32(*(u32 *)(packet + 4));
    header.length = 8;
    WifiSetup_WriteBody(gWifiSetupState.unk_00, work + 0x18, packet, &header.length, (u16 *)&header.flag, &header);
    WifiSetup_MemCopy(data, src + 8, 8);
    size = WifiSetup_Scramble(data, 8, sStr_MELCO, 6);
    if (size != 0) {
        WifiSetup_SetError(2);
        return -1;
    }
    WifiSetup_WritePacketHeader(work, 2 << 12, header.length, header.flag, header.value, 0x11, data);
    header.length += 0x18;
    WifiSetup_SendPacket(work, (void *)(int)header.length, 0, arg);
    return 0;
}

int WifiSetup_SendDoneRequest(void *unused, const u8 *src, void *arg) {
    u8 data[8];
    u8 *work = gWifiSetupState.data;
    int size;

    WifiSetup_MemFill(work, 0, 0x5DC);
    WifiSetup_MemCopy(data, src + 0x10, 8);
    size = WifiSetup_StrLen(sStr_MELCO);
    WifiSetup_Scramble(data, 8, sStr_MELCO, size);
    WifiSetup_WritePacketHeader(work, 3 << 12, 0, 0, 0, 0x11, data);
    WifiSetup_SendPacket(work, (void *)0x18, 0, arg);
    return 0;
}

int WifiSetup_BuildJoinPayload(u8 *dst) {
    s16 sourceLen;
    s16 secondLen;
    s16 total = 0;
    u8 *second;
    u32 value;

    dst[0] = (s8)ov13_0224CFB8[0x1D];
    dst[1] = 1;
    sourceLen = (s16)gWifiSetupState.unk_28;
    WifiSetup_MemCopy(dst + 6, gWifiSetupState.unk_24, sourceLen);
    *(u16 *)(dst + 2) = WifiSetup_HostToNet16((u16)sourceLen);
    secondLen = (s16)(sourceLen + 6);
    secondLen = (s16)(((secondLen + 1) / 2) * 2);
    *(u16 *)(dst + 4) = WifiSetup_HostToNet16((u16)secondLen);
    total += secondLen;
    second = dst + secondLen;
    dst[secondLen] = 0x60;
    second[1] = 0;
    *(u16 *)(second + 4) = WifiSetup_HostToNet16(0);
    value = WifiSetup_HostToNet32(0xE);
    WifiSetup_MemCopy(second + 6, &value, 4);
    *(u16 *)(second + 2) = WifiSetup_HostToNet16(4);
    total += 10;
    return (s16)total;
}

void WifiSetup_WriteBody(int mode, u8 *dst, const void *src, s16 *length, u16 *flag, const void *extra) {
    if (mode == 1) {
        *flag = 1;
        WifiSetup_Encrypt(src, dst + 4, *length, extra, dst + 2, gWifiSetupPeerAddress, 8);
        *(u16 *)dst = WifiSetup_HostToNet16((u16)*length);
        *length += 4;
    } else {
        WifiSetup_MemCopy(dst, src, *length);
    }
}

void WifiSetup_WritePacketHeader(u8 *dst, int a1, int a2, int a3, s8 a4, s8 a5, const void *src) {
    *(u16 *)(dst + 0) = WifiSetup_HostToNet16(1);
    *(u16 *)(dst + 2) = 0;
    *(u16 *)(dst + 4) = 0;
    *(u16 *)(dst + 6) = WifiSetup_HostToNet16((u16)a1);
    *(u16 *)(dst + 8) = 0;
    *(u16 *)(dst + 10) = WifiSetup_HostToNet16((u16)a2);
    *(u16 *)(dst + 12) = WifiSetup_HostToNet16((u16)a3);
    dst[14] = a4;
    dst[15] = a5;
    WifiSetup_MemCopy(dst + 16, src, 8);
}

void WifiSetup_SendPacket(void *a0, void *a1, int index, void *a3) {
    u8 packet[8];
    int value;

    WifiSetup_MemFill(packet, 0, sizeof(packet));
    packet[1] = 2;
    *(u16 *)(packet + 2) = WifiSetup_HostToNet16(0x5790);
    value = WifiSetup_HostToNet32(gWifiSetupState.address);
    *(int *)(packet + 4) = value;
    if (index == 0xFF || (s8)ov13_0224CFB8[0x1C] == 0) {
        *(int *)(packet + 4) = -1;
    }
    WifiSetup_SendTo((int)a3, a0, (int)a1, 0, packet, sizeof(packet));
}

int WifiSetup_Encrypt(void *src, void *dst, int size, u8 *checksum, void *seedDst, const void *seedSrc, int seedSize) {
    u16 seed;
    WifiSetupCipher state;

    *checksum = WifiSetup_CalcCrc32(src, size);
    state.table = WifiSetup_Alloc(size);
    if (state.table == NULL) {
        return -1;
    }
    seed = WifiSetup_Random();
    WifiSetup_MemCopy(seedDst, &seed, 2);
    WifiSetup_MemCopy(gWifiSetupCipherSeed, seedDst, 2);
    WifiSetup_MemCopy(ov13_0224DD02, seedSrc, seedSize);
    WifiSetup_CipherInit(&state, gWifiSetupCipherSeed, seedSize + 2, size);
    WifiSetup_CipherXor(&state, dst, src, size);
    WifiSetup_Free(state.table);
    return 0;
}

int WifiSetup_Decrypt(void *dst, const void *src, int size, int expected, void *seedDst, const void *seedSrc, int seedSize) {
    WifiSetupCipher state;

    state.table = WifiSetup_Alloc(size);
    if (state.table == NULL) {
        WifiSetup_SetError(2);
        return -1;
    }
    WifiSetup_MemCopy(gWifiSetupCipherSeed, seedDst, 2);
    WifiSetup_MemCopy(ov13_0224DD02, seedSrc, seedSize);
    WifiSetup_CipherInit(&state, gWifiSetupCipherSeed, seedSize + 2, size);
    WifiSetup_CipherXor(&state, src, dst, size);
    {
        int actual = WifiSetup_CalcCrc32(src, size);
        if (actual != expected) {
            WifiSetup_SetError(0x12);
            WifiSetup_Free(state.table);
            return -1;
        }
    }
    WifiSetup_Free(state.table);
    return 0;
}

void WifiSetup_CipherInit(WifiSetupCipher *state, const u8 *key, u32 keySize, u32 tableSize) {
    u8 *table = state->table;
    u32 i;
    u32 keyIndex;
    u32 value;

    state->y = 0;
    state->x = 0;
    state->size = tableSize;
    for (i = 0; i < tableSize; i++) {
        table[i] = i;
    }
    value = 0;
    keyIndex = 0;
    for (i = 0; i < tableSize; i++) {
        u8 saved = table[i];
        u8 replacement;
        value = (value + key[keyIndex] + saved) % state->size;
        replacement = table[value];
        table[value] = saved;
        table[i] = replacement;
        keyIndex++;
        if (keyIndex >= keySize) {
            keyIndex = 0;
        }
    }
}

void WifiSetup_CipherXor(WifiSetupCipher *state, u8 *dst, const u8 *src, u32 size) {
    u32 i;

    for (i = 0; i < size; i++) {
        // called through an int-returning type: the u8 return type adds a truncation here
        u8 value = ((int (*)(WifiSetupCipher *))WifiSetup_CipherNextByte)(state);
        dst[i] = src[i] ^ value;
    }
}

u8 WifiSetup_CipherNextByte(WifiSetupCipher *state) {
    u8 *table = state->table;
    u8 x = (state->x + 1) % state->size;
    u8 tx = table[x];
    u8 y = (tx + state->y) % state->size;
    u8 ty = table[y];
    state->x = x;
    state->y = y;
    table[y] = tx;
    table[x] = ty;
    return table[(tx + ty) % state->size];
}

u8 WifiSetup_CalcCrc32(const void *data, s32 size) {
    return WifiSetup_UpdateCrc32(~0u, data, size, 0, gWifiSetupCrc32Table) ^ ~0u;
}

u32 WifiSetup_UpdateCrc32(u32 crc, const u8 *data, s32 size, s32 init, u32 *table) {
    s32 i;
    if (init == 0) {
        WifiSetup_InitCrc32Table(init, table);
    }
    for (i = 0; i < size; i++) {
        crc = (crc >> 8) ^ table[(u8)(data[i] ^ crc)];
    }
    return crc;
}

void WifiSetup_InitCrc32Table(s32 unused, u32 *table) {
    u32 value;
    s32 i, j;
    u32 *t = table;
    for (i = 0; i < 256; i++) {
        value = i;
        for (j = 0; j < 8; j++) {
            if (value & 1) {
                value = (value >> 1) ^ 0xEDB88320;
            } else {
                value >>= 1;
            }
        }
        t[i] = value;
    }
}

s32 WifiSetup_Scramble(void *dst, s32 size, const s8 *key, s32 keySize) {
    s32 i;
    u8 *a = WifiSetup_Alloc((s32)(size + ((u32)size >> 31)) >> 1);
    u8 *b;
    if (a == NULL) {
        return -1;
    }
    b = WifiSetup_Alloc(size);
    if (b == NULL) {
        WifiSetup_Free(a);
        return -1;
    }
    for (i = 0; i < 2; i++) {
        WifiSetup_ScrambleMakeKey(i, (s8 *)a, size, key, keySize);
        WifiSetup_ScrambleXorHalf((s8 *)a, dst, size);
        WifiSetup_ScrambleSwapHalves(dst, size, b);
    }
    WifiSetup_Free(a);
    WifiSetup_Free(b);
    return 0;
}

#pragma force_active on

void WifiSetup_ScrambleMakeKey(int start, s8 *out, int length, const s8 *key, int keylen) {
    int half = length / 2, pos = start % keylen, i;
    for (i = 0; i < half; i++) {
        out[i] = i;
        out[i] ^= key[pos++];
        if (pos >= keylen) {
            pos = 0;
        }
    }
}

void WifiSetup_ScrambleXorHalf(s8 *in, u8 *out, int length) {
    int i;
    int half = length / 2;
    for (i = 0; i < half; i++) {
        out[i + half] ^= in[i];
    }
}

#pragma force_active reset

void WifiSetup_ScrambleSwapHalves(u8 *dst, s32 size, u8 *tmp) {
    s32 half = (s32)(size + ((u32)size >> 31)) >> 1;
    WifiSetup_MemCopy(tmp, dst + half, half);
    WifiSetup_MemCopy(tmp + half, dst, half);
    WifiSetup_MemCopy(dst, tmp, size);
}
