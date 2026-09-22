#include "global.h"

typedef struct Ov74LinkState {
    void *unk00;
    void *unk04;
    u8 pad08[4];
    void *unk0C;
    s32 unk10;
    s32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
} Ov74LinkState;

typedef struct Ov74Peer {
    void *unk00;
    u8 pad04[6];
    u8 unk0A;
    u8 pad0B;
} Ov74Peer;

typedef struct Ov74Global {
    u8 pad00[0x1C];
    u32 unk1C;
} Ov74Global;

extern u32 ov74_02231094(void);
extern u32 ov74_02231064(void);
extern void ov74_02231088(u8 value);
extern void *ov74_0223105C(void);
extern u32 ov74_02231118(void);
extern void ov74_0223113C(u32 value);
extern Ov74Peer *ov74_0223115C(void);
extern Ov74LinkState *ov74_02231184(void);
extern u8 *ov74_022311DC(void);
extern u32 ov74_02230D80(void);
extern u32 ov74_02230E44(void);
extern void ov74_02231724(void);
extern void WM_Finish(void);

u32 ov74_02230E94(void);
void ov74_02230EB4(void);
void ov74_02230EE8(void);
void ov74_02230F14(void *arg0, s32 arg1, s32 arg2);
BOOL ov74_02230F40(void);
BOOL ov74_02230F6C(void);
BOOL ov74_02230F98(void);
BOOL ov74_02230FD4(void);
void ov74_02231008(void);

u32 ov74_02230E94(void) {
    switch (ov74_02231094()) {
    case 1:
        return ov74_02230D80();
    case 2:
        return ov74_02230E44();
    default:
        return 0;
    }
}

void ov74_02230EB4(void) {
    Ov74LinkState *state = ov74_02231184();
    u8 *data = ov74_022311DC();
    if (ov74_02231094() == 1) {
        state->unk18 = 1;
        state->unk19 = 0;
        state->unk00 = data + 8;
        state->unk04 = data + 8;
        state->unk10 = 0;
        state->unk14 = 0;
        state->unk1A = 0x78;
        state->unk1B = 0;
        state->unk1C = 0xFD;
    }
}

void ov74_02230EE8(void) {
    Ov74LinkState *state = ov74_02231184();
    u8 *data = ov74_022311DC();
    state->unk18 = 1;
    state->unk19 = 0;
    state->unk00 = data + 8;
    state->unk04 = data + 8;
    state->unk10 = 0x30;
    state->unk14 = 0x30;
    state->unk1A = 0x78;
    state->unk1B = 0;
    state->unk1C = 0xFE;
}

void ov74_02230F14(void *arg0, s32 arg1, s32 arg2) {
    Ov74LinkState *state = ov74_02231184();
    if (arg2 <= 0xF0) {
        state->unk18 = 2;
        state->unk19 = 3;
        state->unk00 = arg0;
        state->unk04 = arg0;
        state->unk10 = arg1;
        state->unk14 = 0;
        state->unk1A = 0x78;
        state->unk1B = 0;
        state->unk1C = arg2;
    }
}

BOOL ov74_02230F40(void) {
    u32 mode = ov74_02231094();
    u32 status;
    if (mode == 1) {
        goto mode_ok;
    }
    if (mode != 2) {
        goto fail;
    }
mode_ok:
    status = ov74_02231064();
    if (status == 10) {
        goto status_ok;
    }
    if (status != 11) {
        goto fail;
    }
status_ok:
    if (ov74_02231184()->unk19 != 2) {
        goto fail;
    }
    return TRUE;
fail:
    return FALSE;
}

BOOL ov74_02230F6C(void) {
    u32 mode = ov74_02231094();
    u32 status;
    if (mode == 1) {
        goto mode_ok;
    }
    if (mode != 2) {
        goto fail;
    }
mode_ok:
    status = ov74_02231064();
    if (status == 10) {
        goto status_ok;
    }
    if (status != 11) {
        goto fail;
    }
status_ok:
    if (ov74_02231184()->unk19 != 2) {
        goto fail;
    }
    return TRUE;
fail:
    return FALSE;
}

BOOL ov74_02230F98(void) {
    u32 mode = ov74_02231094();
    u32 status;
    Ov74LinkState *state;
    if (mode == 1) {
        goto mode_ok;
    }
    if (mode != 2) {
        goto fail;
    }
mode_ok:
    status = ov74_02231064();
    if (status == 10) {
        goto status_ok;
    }
    if (status != 11) {
        goto fail;
    }
status_ok:
    state = ov74_02231184();
    if (state->unk19 != 0) {
        goto fail;
    }
    if ((((u32 *)state->unk0C)[2] << 16) >> 24) {
        goto fail;
    }
    if (state->unk1B >= 4) {
        goto fail;
    }
    return TRUE;
fail:
    return FALSE;
}

BOOL ov74_02230FD4(void) {
    if (ov74_02231094() == 2) {
        Ov74Peer *peer = ov74_0223115C();
        int i;
        for (i = 0; i < 8; i++, peer++) {
            if (peer->unk00 != NULL && peer->unk0A != 0) {
                return TRUE;
            }
        }
        return FALSE;
    }
    return ov74_02231118();
}

void ov74_02231008(void) {
    u32 state;
    ov74_02231088(12);
    state = ov74_02231064();
    switch (state) {
    case 0:
    case 1:
        WM_Finish();
        ov74_0223113C(1);
        break;
    case 9:
        if (ov74_02231118() == 0) {
            Ov74Global *peer = ov74_0223105C();
            ov74_02231724();
            peer->unk1C = 1;
        }
        break;
    }
}
