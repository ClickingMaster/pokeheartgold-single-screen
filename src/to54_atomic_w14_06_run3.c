#include "to54_atomic_w14_overlay07_callbacks_w14_private.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct DrawTaskData {
    void *owner;
    void *manager;
    u32 unused8;
    SysTask *task;
} DrawTaskData;

typedef struct Counts24 {
    u32 v[6];
} Counts24;
extern const Counts24 ov07_02234BF0;

void ov07_0221DCA8(void *p) {
    u32 index;
    U32_AT(p, 0x18) += 4;
    index = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    U32_AT(p, 0x58 + index * 16) = 0;
}

void ov07_0221DCC0(void *p) {
    u32 value;
    U32_AT(p, 0x18) += 4;
    value = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    ov07_0221D55C(p, value);
}

void ov07_0221DCD4(void *p) {
    u32 offset;
    u32 value;
    U32_AT(p, 0x18) += 4;
    offset = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    value = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    U8_AT(p, offset + 0x6c) = value;
}

void ov07_0221DCF0(void *p) {
    u32 offset;
    u32 value;
    U32_AT(p, 0x18) += 4;
    offset = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    value = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    U8_AT(p, offset + 0x7c) = value;
}
