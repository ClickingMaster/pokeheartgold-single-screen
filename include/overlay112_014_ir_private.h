#ifndef TO55_W02_PRIVATE_H
#define TO55_W02_PRIVATE_H

#include "global.h"

typedef struct IrRecord {
    u32 field00;
    u32 field04;
    u16 field08;
    u16 field0A;
    u32 field0C;
    u8 pad10[0x4B];
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 flagsRest : 6;
    u8 pad5C;
    u8 field5D;
} IrRecord;

typedef struct IrCoreState {
    u8 pad00[0x14];
    u32 field14;
    u32 field18;
    u8 pad1C[4];
    IrRecord *field20;
    IrRecord *field24;
    void *field28;
    u8 pad2C[0x10];
    u64 tick3C;
    u32 field44;
    u32 field48;
} IrCoreState;

typedef struct IrPacketState {
    u8 pad00[0x1C];
    u8 command1C;
    u8 command1D;
    u8 pad1E[2];
    u32 field20;
    u32 field24;
    u32 field28;
    u32 field2C;
    u16 field30;
    u16 field32;
    u16 pad34;
    u16 field36;
    u16 pad38;
    u16 field3A;
} IrPacketState;

extern IrCoreState ov112_021FFAA4;
extern IrPacketState ov112_021FFB24;
extern OSMutex ov112_021FFB08;
extern OSMessageQueue ov112_021FFB20;
extern OSMessage ov112_021FFAC0[];
extern OSThread ov112_021FFBD8;
extern u8 ov112_021FFB64[];

void ov112_021E5964(void);
void ov112_021E5A5C(void);
void ov112_021E5A68(void *, int, int, int);
int ov112_021E5B98(void);
void ov112_021E5D38(void *);
void ov112_021E5D44(void *);
void ov112_021E5D50(void *);
void ov112_021E5D5C(u8);
void ov112_021E5E18(int);
void ov112_021E5E28(void);
void ov112_021E5E48(void);

int CARD_SpiWaitGetStatus(void);
void CARD_SetSpiWriteWaitCycles(u32);
void CARD_SetSpiReadWaitCycles(u32);

void ov112_021E6A6C(int);
void ov112_021E6BDC(void);
void ov112_021E6BF8(void);
void ov112_021E6F60(void *, void *, int);
void ov112_021E7398(void *);
void ov112_021E73C8(void *, u32, u32);

#endif
