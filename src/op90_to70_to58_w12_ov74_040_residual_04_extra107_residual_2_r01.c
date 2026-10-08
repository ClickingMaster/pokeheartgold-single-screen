#pragma force_active on
#include "global.h"
#include "heap.h"
extern void *sub_02035754(int);
extern void CRYPTO_RC4Init(void *, const void *, u32);
extern void CRYPTO_RC4Encrypt(void *, const void *, u32, void *);
extern u16 MATH_CalcCRC16(const MATHCRC16Table *, const void *, u32);
typedef union { u8 bytes[8]; u16 words[4]; } Key8;
void ov74_02235F58(u8 *src, void *dst, enum HeapID heap);
// @fn ov74_02235F58
void ov74_02235F58(u8 *src, void *dst, enum HeapID heap) {
    MATHCRC16Table *table;
    u16 crc;
    const u8 *mac;
    u16 key[4];
    u8 *bytes;
    u16 last;
    int i;
    void *rc4;
    table = Heap_Alloc(heap, sizeof(MATHCRC16Table));
    MATHi_CRC16InitTableRev(table, 0xA001);
    crc = MATH_CalcCRC16(table, src, 80);
    Heap_Free(table);
    mac = sub_02035754(0);
    bytes = (u8 *)key;
    bytes[0] = mac[4];
    bytes[1] = mac[5];
    bytes[2] = mac[6];
    bytes[3] = mac[7];
    bytes[4] = mac[8];
    bytes[5] = mac[9];
    key[3] = key[1];
    key[1] = crc;
    last = 0xD679;
    for (i = 0; i < 4; i++) {
        *(u16 *)bytes ^= last;
        last = *(u16 *)bytes;
        bytes += 2;
    }
    rc4 = Heap_Alloc(heap, 260);
    CRYPTO_RC4Init(rc4, key, 8);
    CRYPTO_RC4Encrypt(rc4, src + 80, 856, dst);
    Heap_Free(rc4);
}
