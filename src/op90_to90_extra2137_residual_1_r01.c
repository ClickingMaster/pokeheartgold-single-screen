#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u32 unused; const u8 *input; u8 *output; u32 tail; } Spi;
typedef struct { u8 delay[4]; Spi descriptor; } SpiController;
extern SpiController _021E4088;
extern Spi _021E408C;
extern u8 _021E409C[];
extern u16 _02110FBC;
void CARD_LockBackup(u16 lock_id);
void CARD_UnlockBackup(u16 lock_id);
s32 OS_LockCard(u16 lockID);
s32 OS_UnLockCard(u16 lockID);
void CARDi_SpiWaitWriteByte(Spi *);
void CARDi_SpiWaitReadByte(Spi *);
void CARDi_SpiDummyWait(void);
int CARD_SpiWaitReadRange(u8 *out);
static inline void SetIO(const u8 *in, u8 *out)
{
    _021E4088.descriptor.input = in;
    _021E4088.descriptor.output = out;
}
#define REG_SPI (*(vu16 *)0x40001a0)
#pragma push
#pragma optimization_level 2
// @fn CARD_SpiWaitReadRange
int CARD_SpiWaitReadRange(u8 *out)
{
    int size;
    int i;
    SetIO(_021E409C, out);
    _021E409C[0] = 1;
    out[0] = 0;
    CARD_LockBackup(_02110FBC);
    OS_LockCard(_02110FBC);
    while (REG_SPI & 0x80) { }
    REG_SPI = 0xa042;
    CARDi_SpiWaitWriteByte(&_021E408C);
    CARDi_SpiDummyWait();
    CARDi_SpiWaitReadByte(&_021E408C);
    CARDi_SpiDummyWait();
    size = out[0];
    if ((u32)size > 185) {
        size = 0;
    }
    SetIO(NULL, out);
    i = 0;
    out[0] = 0;
    if (size != 0) {
        for (; i < size; i++) {
            if (i == size - 1) {
                REG_SPI = 0xa002;
            }
            CARDi_SpiWaitReadByte(&_021E408C);
        }
    } else {
        REG_SPI = 0xa002;
        CARDi_SpiWaitReadByte(&_021E408C);
    }
    for (i = 0; i < size; i++) {
        out[i] ^= 0xaa;
    }
    while (REG_SPI & 0x80) { }
    OS_UnLockCard(_02110FBC);
    CARD_UnlockBackup(_02110FBC);
    return size;
}
#pragma pop
