#pragma force_active on
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef long s32;
typedef volatile u16 vu16;
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
u64 OS_GetTick(void);
u8 CARDi_SpiWaitWriteByte(Spi *);
void CARDi_SpiWaitReadByte(Spi *);
int CARD_SpiWaitGetStatus(void);
#define REG_SPI (*(vu16 *)0x40001a0)
#pragma opt_arithtransformation off
// @fn CARD_SpiWaitGetStatus
int CARD_SpiWaitGetStatus(void)
{
    u8 status;
    u64 start;
    u64 d;
    _021E4088.descriptor.input = _021E409C;
    _021E4088.descriptor.output = &status;
    _021E409C[0] = 8;
    CARD_LockBackup(_02110FBC);
    (void)OS_LockCard(_02110FBC);
    start = OS_GetTick();
    do { d = (OS_GetTick() - start) * 64; } while (d * 1000 / 33514 < 60);
    while (REG_SPI & 0x80) { }
    REG_SPI = 0xa042;
    status = CARDi_SpiWaitWriteByte(&_021E408C);
    start = OS_GetTick();
    do { d = (OS_GetTick() - start) * 64; } while (d * 1000 / 33514 < 50);
    REG_SPI = 0xa002;
    CARDi_SpiWaitReadByte(&_021E408C);
    while (REG_SPI & 0x80) { }
    (void)OS_UnLockCard(_02110FBC);
    CARD_UnlockBackup(_02110FBC);
    return status;
}
