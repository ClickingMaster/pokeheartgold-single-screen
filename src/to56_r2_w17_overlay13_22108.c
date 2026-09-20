#include "global.h"

typedef struct Ov13State {
    int unk00;
    void *unk04;
    int unk08;
    void *unk0C;
    int unk10;
    void *unk14;
    u8 pad18[0x0C];
    void *unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
} Ov13State;

typedef struct Ov13CipherState {
    u32 unk00;
    u32 unk04;
    u8 *table;
    u32 tableSize;
} Ov13CipherState;

extern Ov13State _0224CF98;
extern s8 ov13_0224CFB8[];
extern u8 ov13_0224DCF8[];
extern u8 ov13_0224DD00[];
extern u8 ov13_0224DD02[];
extern u8 ov13_02245A20[];

int ov13_022208E8(int size);
int ov13_022208F8(void *ptr);
void ov13_022214AC(int value);
int ov13_02222A1C(int value);
int ov13_02222A44(int value);
int ov13_02222AB0(void);
int ov13_02222A9C(const void *data);
void ov13_02222968(void *dst, const void *src, int size);
void ov13_02222978(void *dst, int value, int size);
int ov13_022227A0(const void *data, int size, const void *expected, int expectedSize);
u8 ov13_02222708(const void *data, int size);
int ov13_022229EC(void *a0, void *a1, int a2, int a3, const void *a4, int a5);
int ov13_022226C8(Ov13CipherState *state);
u32 _u32_div_f(u32, u32);

int ov13_02222108(void *unused, const u8 *src, void *arg);
int ov13_0222224C(void *unused, const u8 *src, void *arg);
int ov13_02222328(void *unused, const u8 *src, void *arg);
int ov13_02222394(u8 *dst);
void ov13_02222420(int mode, u8 *dst, const void *src, s16 *length, u16 *flag, const void *extra);
void ov13_02222474(u8 *dst, int a1, int a2, int a3, s8 a4, s8 a5, const void *src);
void ov13_022224CC(void *a0, void *a1, int index, void *a3);
int ov13_02222534(void *a0, void *a1, int size, const void *extra, u8 *seedDst, const void *seedSrc, int seedSize);
int ov13_022225B0(void *dst, const void *src, int size, u8 expected, void *seedDst, const void *seedSrc, int seedSize);
void ov13_02222638(Ov13CipherState *state, const u8 *key, u32 keySize, u32 tableSize);
void ov13_0222269C(Ov13CipherState *state, u8 *dst, const u8 *src, u32 size);

int ov13_02222108(void *unused, const u8 *src, void *arg) {
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
    work = _0224CF98.unk14;
    ov13_02222978(work, 0, 0x5DC);
    payload = (u8 *)ov13_022208E8(0x210);
    if (payload == NULL) {
        ov13_022214AC(2);
        return -1;
    }
    ov13_02222978(payload, 0, 0x210);
    encoded = work + 0x18;
    ov13_02222968(ov13_0224DCF8, src, 8);
    ov13_02222968(data, ov13_0224DCF8, 8);
    header.length = ov13_02222394(payload + 4);
    if (header.length < 0) {
        ov13_022214AC(3);
        if (payload != NULL) {
            ov13_022208F8(payload);
        }
        return -1;
    }
    payload[0] = 0;
    *(u16 *)(payload + 2) = ov13_02222A44((u16)header.length);
    header.length += 4;
    ov13_02222420(0, encoded, payload, &header.length, (u16 *)&header.flag, &header);
    header.flag |= 0x10;
    if (ov13_022227A0(data, 8, ov13_02245A20, 6) != 0) {
        ov13_022214AC(2);
        if (payload != NULL) {
            ov13_022208F8(payload);
        }
        return -1;
    }
    ov13_02222474(work, 1 << 12, header.length, header.flag, header.value, 0x11, data);
    header.length += 0x18;
    ov13_022224CC(work, (void *)(int)header.length, 0xFF, arg);
    if (payload != NULL) {
        ov13_022208F8(payload);
    }
    return 0;
}

int ov13_0222224C(void *unused, const u8 *src, void *arg) {
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
    work = _0224CF98.unk14;
    ov13_02222978(packet, 0, 8);
    ov13_02222978(work, 0, 0x5DC);
    packet[0] = 2;
    packet[1] = 0;
    *(u16 *)(packet + 2) = ov13_02222A44(4);
    *(u32 *)(packet + 4) = _0224CF98.unk2C;
    *(u32 *)(packet + 4) = ov13_02222A1C(*(u32 *)(packet + 4));
    header.length = 8;
    ov13_02222420(_0224CF98.unk00, work + 0x18, packet, &header.length, (u16 *)&header.flag, &header);
    ov13_02222968(data, src + 8, 8);
    size = ov13_022227A0(data, 8, ov13_02245A20, 6);
    if (size != 0) {
        ov13_022214AC(2);
        return -1;
    }
    ov13_02222474(work, 2 << 12, header.length, header.flag, header.value, 0x11, data);
    header.length += 0x18;
    ov13_022224CC(work, (void *)(int)header.length, 0, arg);
    return 0;
}

int ov13_02222328(void *unused, const u8 *src, void *arg) {
    u8 data[8];
    u8 *work = _0224CF98.unk14;
    int size;

    ov13_02222978(work, 0, 0x5DC);
    ov13_02222968(data, src + 0x10, 8);
    size = ov13_02222A9C(ov13_02245A20);
    ov13_022227A0(data, 8, ov13_02245A20, size);
    ov13_02222474(work, 3 << 12, 0, 0, 0, 0x11, data);
    ov13_022224CC(work, (void *)0x18, 0, arg);
    return 0;
}

int ov13_02222394(u8 *dst) {
    s16 sourceLen;
    s16 secondLen;
    s16 total = 0;
    u8 *second;
    u32 value;

    dst[0] = (s8)ov13_0224CFB8[0x1D];
    dst[1] = 1;
    sourceLen = (s16)_0224CF98.unk28;
    ov13_02222968(dst + 6, _0224CF98.unk24, sourceLen);
    *(u16 *)(dst + 2) = ov13_02222A44((u16)sourceLen);
    secondLen = (s16)(sourceLen + 6);
    secondLen = (s16)(((secondLen + 1) / 2) * 2);
    *(u16 *)(dst + 4) = ov13_02222A44((u16)secondLen);
    total += secondLen;
    second = dst + secondLen;
    dst[secondLen] = 0x60;
    second[1] = 0;
    *(u16 *)(second + 4) = ov13_02222A44(0);
    value = ov13_02222A1C(0xE);
    ov13_02222968(second + 6, &value, 4);
    *(u16 *)(second + 2) = ov13_02222A44(4);
    total += 10;
    return (s16)total;
}

void ov13_02222420(int mode, u8 *dst, const void *src, s16 *length, u16 *flag, const void *extra) {
    if (mode == 1) {
        *flag = 1;
        ov13_02222534(src, dst + 4, *length, extra, dst + 2, ov13_0224DCF8, 8);
        *(u16 *)dst = ov13_02222A44((u16)*length);
        *length += 4;
    } else {
        ov13_02222968(dst, src, *length);
    }
}

void ov13_02222474(u8 *dst, int a1, int a2, int a3, s8 a4, s8 a5, const void *src) {
    *(u16 *)(dst + 0) = ov13_02222A44(1);
    *(u16 *)(dst + 2) = 0;
    *(u16 *)(dst + 4) = 0;
    *(u16 *)(dst + 6) = ov13_02222A44((u16)a1);
    *(u16 *)(dst + 8) = 0;
    *(u16 *)(dst + 10) = ov13_02222A44((u16)a2);
    *(u16 *)(dst + 12) = ov13_02222A44((u16)a3);
    dst[14] = a4;
    dst[15] = a5;
    ov13_02222968(dst + 16, src, 8);
}
