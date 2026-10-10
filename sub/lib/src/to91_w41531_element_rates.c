#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 WL_ReadByte(const void *);
extern const u16 RateElement2Bit[];
void WElement2RateSet(const u8 *element, u16 *out);

typedef struct {
    u16 basic, supported;
} Rates40815;

static inline void PublishRate09(Rates40815 *r, u32 rate) {
    u32 index = (rate & 127) - 1;
    u32 bit;
    if (index < 120 && (bit = RateElement2Bit[index]) != 255) {
        r->supported |= 1u << bit;
        if (rate & 128) {
            r->basic |= 1u << bit;
        }
    } else {
        r->supported |= 0x8000;
        if (rate & 128) {
            r->basic |= 0x8000;
        }
    }
}

typedef struct {
    u8 kind, count;
    u8 rates[];
} RateWire09;

void WElement2RateSet(const u8 *element, u16 *out) {
    const u8 *cursor;
    u32 i, count;
    const RateWire09 *wire;
    Rates40815 *r;
    wire = (const RateWire09 *)element;
    r = (Rates40815 *)out;
    r->basic = 0;
    r->supported = 0;
    count = WL_ReadByte(&wire->count);
    cursor = wire->rates;
    for (i = 0; i < count; i++) {
        PublishRate09(r, WL_ReadByte(cursor + i));
    }
}
