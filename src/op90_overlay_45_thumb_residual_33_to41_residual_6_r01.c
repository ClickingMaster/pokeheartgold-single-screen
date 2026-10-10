#pragma force_active on
#include "global.h"
#include "assert.h"

typedef struct Ov45Slot {
    int index;
    u32 arg;
    int unk8;
    int unkC;
    u8 pad10[0x20];
    u8 unk30[0x14];
    u16 unk44;
    u16 timeout;
    u32 unk48;
} Ov45Slot;

typedef struct Ov45Global {
    u8 pad0[0x19C];
    u8 unk19C[4];
    u8 pad1A0[0x418];
    Ov45Slot slots[3];
    u16 state;
    u8 unk69E;
    u8 unk69F;
} Ov45Global;

extern Ov45Global *_022577C0;

BOOL ov45_0223021C(Ov45Global *global, int index);
void ov45_02230378(Ov45Slot *slot, void *a1);
void ov45_0223285C(Ov45Slot *slot);
BOOL ov45_0222F74C(void);
u32 ov45_02232580(void);
void ov00_021E5CEC(void *a0, u32 a1, int a2, int a3);
void ov00_021E5CD0(void (*func)(void), void *arg);
void WifiConnection_SetDisconnectCallback(void (*func)(void), void *arg);
void ov45_02230390(void);
void ov45_02230394(void);
BOOL ov45_0222F028(int index, u32 arg);

#pragma opt_propagation off
// @fn ov45_0222F028
BOOL ov45_0222F028(int index, u32 arg) {
    GF_ASSERT(_022577C0 != NULL);
    if (ov45_0223021C(_022577C0, index) == TRUE) {
        return FALSE;
    }
    if (_022577C0->state != 3) {
        return FALSE;
    }
    _022577C0->slots[index].index = index;
    _022577C0->slots[index].arg = arg;
    _022577C0->slots[index].unk8 = 1;
    _022577C0->slots[index].unkC = 0;
    ov45_02230378(&_022577C0->slots[index], _022577C0->unk19C);
    ov45_0223285C(&_022577C0->slots[index]);
    GF_ASSERT(ov45_0222F74C());
    _022577C0->slots[index].unk44 = 1;
    _022577C0->slots[index].unk48 = ov45_02232580();
    _022577C0->slots[index].timeout = 1800;
    _022577C0->state = index;
    _022577C0->unk69E = 1;
    _022577C0->unk69F = 0;
    ov00_021E5CEC(_022577C0->slots[index].unk30, arg, 1, _022577C0->slots[index].timeout / 30);
    ov00_021E5CD0(ov45_02230390, _022577C0);
    WifiConnection_SetDisconnectCallback(ov45_02230394, _022577C0);
    return TRUE;
}
#pragma opt_propagation reset
