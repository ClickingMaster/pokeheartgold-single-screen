#include "global.h"

extern u8 _021FF9E0[];
extern u8 _021FF500[];
extern u8 ov112_021FFA18[];
extern u8 ov112_021FFA20[];
extern u8 ov112_021FFAA4[];
extern u8 ov112_021FFB24[];

typedef struct State {
    u8 byte0[0x10];
    u32 count;
    u32 callback14;
    u32 flag18;
} State;

extern void CARD_SpiWaitInit(void);
extern void CARD_SetSpiWriteWaitCycles(u32);
extern void CARD_SetSpiReadWaitCycles(u32);
extern void CARD_SpiWaitReadRange(void *);
extern void CARD_SpiWaitWriteRange(void *, u32);
extern u64 OS_GetTick(void);
extern void OS_Halt(void);
extern void ov112_021E5A14(int);
extern void ov112_021E59B4(void *, int, int, int, u32);

u16 ov112_021E5900(u8 *, u32);
void ov112_021E5938(void);
void ov112_021E594C(void);
void ov112_021E5964(void);
u32 ov112_021E59A8(void);
void ov112_021E59B4(void *, int, int, int, u32);
void ov112_021E5A5C(void);
void ov112_021E5A68(void *, int, int, int);
void ov112_021E5A80(void);
BOOL ov112_021E5A84(u32);
BOOL ov112_021E5AD0(void);
BOOL ov112_021E5B28(u32);
void ov112_021E5D38(void *);
void ov112_021E5D44(void *);
void ov112_021E5D50(void *);
void ov112_021E5D5C(u8);
void ov112_021E5D68(void *);
void ov112_021E5E18(void *);
void ov112_021E5E28(void);
void ov112_021E5EB8(void *, u32);
void ov112_021E5EC4(void *, u32, int);
void ov112_021E6134(void);


void ov112_021E5E18(void *p) {
    if (*(void **)(ov112_021FFAA4 + 0x14) == NULL)
        *(void **)(ov112_021FFAA4 + 0x14) = p;
}
