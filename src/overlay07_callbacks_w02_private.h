#ifndef TO53_W02_PRIVATE_H
#define TO53_W02_PRIVATE_H
#include "global.h"
#include "heap.h"
typedef struct BattleSystem BattleSystem;

void PlaySE(u16);
void StopSE(u16, int);
void GF_AssertFail(void);
void ov07_0221C69C(void);
void ov07_0221F8C8(void *, s8);
void SetAllSEPan(void);
void *ov07_0221FB90(BattleSystem *, enum HeapID, u8);
void *NARC_AllocAndReadWholeMember(u32, u32, u32);
void ov06_0221BAD8(void);
void sub_02015494(void *, void *, void *);
void ov07_022207D0(void *, int, int);
void ov07_02221EEC(void *, void *);
void ov07_0221F9A8(void *, void *, int);

void ov07_0221EF14(void *);
void ov07_0221EF34(void *);
void ov07_0221F084(void);
void ov07_0221F088(void);
void ov07_0221F08C(void);
void ov07_0221F090(void);
void ov07_0221F094(void);
void ov07_0221F398(void);
void ov07_0221F424(void);
void ov07_0221F42C(void);
void ov07_0221F814(void);
void ov07_0221F8AC(void);
u32 ov07_0221F8B0(u32);
u8 ov07_0221F8C4(void *);
u32 ov07_0221FA78(void *);
struct Outer;
u32 ov07_0221FA80(struct Outer *, int);
u32 ov07_0221FA90(struct Outer *, int);
u32 ov07_0221FAA0(struct Outer *, int);
BOOL ov07_0221FAB0(void *);
BOOL ov07_0221FAE8(void *);
u32 ov07_0221FAEC(void *, int);
u32 ov07_0221FAF8(void *, int);
BOOL ov07_0221FB78(void);
u32 ov07_0221FB7C(int, int);
void *ov07_0221FDFC(BattleSystem *, enum HeapID);
u8 ov07_0221FE70(void *);
void ov07_0221FE84(void);
void *ov07_0221FEDC(u32, u32, u32);
void ov07_0221FF2C(void);
void ov07_0221FF74(void);
void ov07_02220800(void *);
void ov07_0222080C(void *);
void ov07_02220818(void *);
void ov07_02220824(void *);
void ov07_02220830(void *);
void ov07_0222083C(void *);
void ov07_02220848(void *);
void ov07_02220854(void *);
void ov07_02220860(void *);
void ov07_0222086C(void *);
void ov07_02220878(void *);
void ov07_022217A4(void *, void *);
BOOL ov07_022217B0(void);
BOOL ov07_02221898(void);
BOOL ov07_0222189C(void);
BOOL ov07_02221AD4(void);
BOOL ov07_02221AD8(void);
BOOL ov07_02221ADC(void);
BOOL ov07_02221AE0(void);
void ov07_02221C74(int, void *, void *);
void ov07_02221CE0(void *, void *);

#endif
