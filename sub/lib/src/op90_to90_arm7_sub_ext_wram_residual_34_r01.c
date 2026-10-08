#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {
    u16 type;      /* 0x0 */
    u16 bits;      /* 0x2 */
    u16 count;     /* 0x4 */
    u16 x6;
    u8 bkReg;      /* 0x8 */
    u8 pad9[3];
    u32 xc;        /* 0xc */
} RfInfo;
#define ROOT (*(u8 **)0x0380fff4)
extern const u16 macTxRxRegAdrs[16];
extern void FLASH_Read(u32 addr, u32 size, void *dst);
extern void RF_Write(u32 data);
void InitRF(void);
typedef struct { u8 pad[0x5f8]; RfInfo rf; } RfWork;
// @fn InitRF
void InitRF(void) {
    u32 value;
    u32 data;
    u32 i;
    u32 addr;
    s32 bytes;
    u32 count;
    RfInfo *rf;

    rf = &((RfWork *)ROOT)->rf;
    data = 0;
    for (i = 0; i < 16; i++) {
        FLASH_Read(0x44 + i * 2, 2, &data);
        *(vu16 *)(0x04808000 + macTxRxRegAdrs[i]) = data;
    }
    value = ((u32)rf->bits >> 7) << 8;
    value |= rf->bits & 0x7f;
    *(vu16 *)0x04808184 = value;
    addr = 0xce;
    bytes = ((rf->bits & 0x7f) + 7) / 8;
    count = rf->count;
    if (rf->type == 3) {
        FLASH_Read(count + 0xce, 1, &rf->bkReg);
        for (i = 0; i < count;) {
            value = 0;
            FLASH_Read(addr, 1, &value);
            value |= (i << 8) + 0x50000;
            RF_Write(value);
            i++;
            addr++;
        }
    } else {
        value = 0;
        while (count != 0) {
            FLASH_Read(addr, bytes, &value);
            RF_Write(value);
            if (rf->type == 2 && (value >> 18) == 9) {
                rf->xc = value & ~0x7c00;
            }
            count--;
            addr += bytes;
        }
    }
}
