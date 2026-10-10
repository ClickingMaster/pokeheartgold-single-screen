#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern void UpdateGameInfoElement(void);
extern void MLME_IssueBeaconSendIndication(void);
void WlIntrTxBeaconTask(void);

typedef struct {
    u16 low : 6;
    u16 enabled : 1;
    u16 high : 9;
} W35521Flags;

void WlIntrTxBeaconTask(void) {
    if (R16(WL, 0x3e8)) {
        UpdateGameInfoElement();
    }
    if (((W35521Flags *)(WL + 0x33a))->enabled) {
        MLME_IssueBeaconSendIndication();
    }
}
