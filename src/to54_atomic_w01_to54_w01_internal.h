#ifndef TO53_W01_PRIVATE_H
#define TO53_W01_PRIVATE_H

#include "global.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void GF_AssertFail(void);
void Heap_Free(void *ptr);
void SysTask_Destroy(void *task);
void ov07_0221BE68(int kind, void *owner, void *func, void *arg, u32 value);
void ov07_0221BEA4(int kind, void *owner, void *task);
void ov07_0221C5C8(void *p);
void ov07_0221DEC0(void *p);
void ov07_0221E664(void *p);
void ov07_0221DD14(void *p, int value);
void *ov07_0221BFC0(void *p);
void *ov07_0221BFD0(void *p);
BOOL ov07_0221C3B0(void *p);
BOOL ov07_0221C3C0(void *p);
BOOL ov07_0221C3DC(void *p);
void ov07_0221C3F4(void *owner, void *func, void *arg, u32 value);
void ov07_0221C42C(void *owner, void *func, void *arg, u32 value);
void ov07_0221C448(void *owner, void *task);
void ov07_0221C458(void *owner, void *task);
u16 ov07_0221C468(void *p);
u16 ov07_0221C470(void *p);
void *ov07_0221C478(void *p);
void *ov07_0221C488(void *p, int i);
void *ov07_0221C494(void *p, int i);
void *ov07_0221C4A0(void *p);

extern u32 ov07_0221C4A8(void *p, int i);
void *ov07_0221C514(void *p);
void *ov07_0221C528(void *p);
void *ov07_0221C530(void *p);
int ov07_0221C5C4(void);
void ov07_0221C624(void *p);
void ov07_0221C62C(void *p);
void ov07_0221D5A8(void);
void ov07_0221D5AC(void);
void ov07_0221DD0C(void);
void ov07_0221DD10(void);
BOOL ov07_0221DEB0(u32 a, u32 b);
int ov07_0221E5E0(void *unused, void *p);
int ov07_0221E5EC(void *unused, void *p);
void *ov07_0221E6C8(void *p);
int ov07_0221E6D0(void);
int ov07_0221E868(void *p);
int ov07_0221EB84(void *p);
int ov07_0221ED00(void *p);

BOOL ov07_0221C634(void *);
BOOL ov07_0221C674(void *);
void ov07_0221C69C(void);
void ov07_0221C6B4(void *);
void ov07_0221C6EC(void *);
void ov07_0221C70C(void *);
void ov07_0221C72C(void *);
void ov07_0221C74C(void *);
void ov07_0221D000(void *);
void ov07_0221D02C(void *);
void ov07_0221D05C(void *);
void ov07_0221D088(void *);
void ov07_0221D0C0(void *);
void ov07_0221D148(void *);
void ov07_0221D214(void *);
void ov07_0221D23C(void *);
void ov07_0221D270(void *);
void ov07_0221D2B0(void *);
void ov07_0221D2E4(void *);
#endif
void PlaySE(u16);
void sub_020061B4(u16, u16, u32);

extern u8 gSystem[];
void ov07_0221BE20(void);
void ov07_0221FF2C(void *);
BOOL ov07_0221F8C4(void *);
int ov07_0223192C(void *, u16);
