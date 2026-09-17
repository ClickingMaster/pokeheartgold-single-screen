#include "battle_controller_opponent_pr_messages_internal.h"

void ov12_02261390(BattleSystem *bsys, void *ctx, void *cmd, void *out) {
    u32 battler = U8(ctx, 0x194);
    if (U8(ctx, 0x195) & 1) {
        if (BattleSystem_GetBattleType(bsys) & 4) {
            U16(out, 2) = 0x3DE;
            U8(out, 1) = 0x1B;
            U32(out, 4) = U8(ctx, 0x194);
            U32(out, 8) = U8(ctx, 0x194) | U8(cmd, 1) << 8;
        } else {
            U16(out, 2) = 0x3DD;
            U8(out, 1) = 0x32;
            U32(out, 4) = U8(ctx, 0x194);
            U32(out, 8) = U8(ctx, 0x194);
            U32(out, 0xC) = U8(ctx, 0x194) | U8(cmd, 1) << 8;
        }
    } else {
        if (!(BattleSystem_GetBattleType(bsys) & 2) && !(BattleSystem_GetBattleType(bsys) & 4)) {
            u16 v = U16(cmd, 2);
            if (v == 0) {
                U16(out, 2) = 0x3D8;
            } else if (v < 25) {
                U16(out, 2) = 0x3D9;
            } else if (v < 50) {
                U16(out, 2) = 0x3DC;
            } else if (v < 75) {
                U16(out, 2) = 0x3DA;
            } else {
                U16(out, 2) = 0x3DB;
            }
        } else {
            U16(out, 2) = 0x3D9;
        }
        U8(out, 1) = 2;
        U32(out, 4) = U8(ctx, 0x194) | U8(cmd, 1) << 8;
    }
}

void ov12_02261464(BattleSystem *bsys, void *ctx, void *cmd, void *out) {
    if (U8(ctx, 0x195) & 1) {
        if (!(BattleSystem_GetBattleType(bsys) & 4)) {
            U16(out, 2) = 0x3CC;
            U8(out, 1) = 0x32;
            U32(out, 4) = U8(ctx, 0x194);
            U32(out, 8) = U8(ctx, 0x194);
            U32(out, 0xC) = U8(ctx, 0x194) | (U8(cmd, 1) << 8);
        } else {
            U16(out, 2) = 0x3CE;
            U8(out, 1) = 0x1B;
            U32(out, 4) = U8(ctx, 0x194);
            U32(out, 8) = U8(ctx, 0x194) | (U8(cmd, 1) << 8);
        }
    } else {
        if (!(BattleSystem_GetBattleType(bsys) & 2) && !(BattleSystem_GetBattleType(bsys) & 4)) {
            u16 v = U16(cmd, 2);
            if (v < 100) {
                U16(out, 2) = 0x3D6;
            } else if (v < 325) {
                U16(out, 2) = 0x3D7;
            } else if (v < 550) {
                U16(out, 2) = 0x3D5;
            } else if (v < 775) {
                U16(out, 2) = 0x3D4;
            } else {
                U16(out, 2) = 0x3D3;
            }
        } else {
            U16(out, 2) = 0x3D3;
        }
        U8(out, 1) = 2;
        U32(out, 4) = U8(ctx, 0x194) | (U8(cmd, 1) << 8);
    }
}

void ov12_02261544(BattleSystem *bsys, void *ctx, void *out) {
    u32 type = BattleSystem_GetBattleType(bsys);
    u32 b1;
    u32 b2;
    if (type & 2) {
        b1 = BattleSystem_GetBattlerFromBattlerType(bsys, 3);
        b2 = BattleSystem_GetBattlerFromBattlerType(bsys, 5);
    } else {
        b1 = BattleSystem_GetBattlerFromBattlerType(bsys, 1);
        b2 = b1;
    }
    if (type & 4) {
        if (type & 0x80) {
            U16(out, 2) = 0x3E0;
            U8(out, 1) = 0x3B;
            U32(out, 4) = b1;
            U32(out, 8) = b1;
            U32(out, 0xC) = b2;
            U32(out, 0x10) = b2;
        } else if (type & 8) {
            U16(out, 2) = 0x3CB;
            U8(out, 1) = 0x1A;
            U32(out, 4) = b1;
            U32(out, 8) = b2;
        } else {
            U16(out, 2) = 0x3CA;
            U8(out, 1) = 8;
            U32(out, 4) = b1;
        }
    } else if ((type & 0x10) || (type & 8)) {
        U16(out, 2) = 0x3E0;
        U8(out, 1) = 0x3B;
        U32(out, 4) = b1;
        U32(out, 8) = b1;
        U32(out, 0xC) = b2;
        U32(out, 0x10) = b2;
    } else {
        U16(out, 2) = 0x3C9;
        U8(out, 1) = 0x1E;
        U32(out, 4) = b1;
        U32(out, 8) = b1;
    }
}

void ov12_022615F0(BattleSystem *bsys, void *ctx, void *cmd, void *out) {
    u32 type = BattleSystem_GetBattleType(bsys);
    u32 battler;
    u32 partner;

    if (U8(ctx, 0x195) & 1) {
        if (type & 2) {
            battler = U8(ctx, 0x194);
            partner = BattleSystem_GetBattlerIdPartner(bsys, battler);
        } else {
            battler = U8(ctx, 0x194);
            partner = battler;
        }
        if (type & 4) {
            if (type & 0x80) {
                U16(out, 2) = 0x3DF;
                U8(out, 1) = 0x3C;
                U32(out, 4) = battler;
                U32(out, 8) = battler;
                U32(out, 0xC) = battler | (U8(cmd, battler + 4) << 8);
                U32(out, 0x10) = partner;
                U32(out, 0x14) = partner;
                U32(out, 0x18) = partner | (U8(cmd, partner + 4) << 8);
            } else if (type & 8) {
                U16(out, 2) = 0x3D0;
                U8(out, 1) = 0x38;
                U32(out, 4) = battler;
                U32(out, 8) = battler | (U8(cmd, battler + 4) << 8);
                U32(out, 0xC) = partner;
                U32(out, 0x10) = partner | (U8(cmd, partner + 4) << 8);
            } else if (type & 2) {
                U16(out, 2) = 0x3CF;
                U8(out, 1) = 0x31;
                U32(out, 4) = battler;
                U32(out, 8) = battler | (U8(cmd, battler + 4) << 8);
                U32(out, 0xC) = partner | (U8(cmd, partner + 4) << 8);
            } else {
                U16(out, 2) = 0x3CE;
                U8(out, 1) = 0x1B;
                U32(out, 4) = battler;
                U32(out, 8) = battler | (U8(cmd, battler + 4) << 8);
            }
        } else if ((type & 0x10) || (type & 8)) {
            U16(out, 2) = 0x3DF;
            U8(out, 1) = 0x3C;
            U32(out, 4) = battler;
            U32(out, 8) = battler;
            U32(out, 0xC) = battler | (U8(cmd, battler + 4) << 8);
            U32(out, 0x10) = partner;
            U32(out, 0x14) = partner;
            U32(out, 0x18) = partner | (U8(cmd, partner + 4) << 8);
        } else if (type & 2) {
            U16(out, 2) = 0x3CD;
            U8(out, 1) = 0x39;
            U32(out, 4) = battler;
            U32(out, 8) = battler;
            U32(out, 0xC) = battler | (U8(cmd, battler + 4) << 8);
            U32(out, 0x10) = partner | (U8(cmd, partner + 4) << 8);
        } else {
            U16(out, 2) = 0x3CC;
            U8(out, 1) = 0x32;
            U32(out, 4) = battler;
            U32(out, 8) = battler;
            U32(out, 0xC) = battler | (U8(cmd, battler + 4) << 8);
        }
        return;
    }

    if (type & 4) {
        u8 side = ov12_0223BFC0(bsys);
        if (type & 8) {
            switch (ov12_0223BFCC(bsys, side)) {
            case 0:
            case 3:
                battler = BattleSystem_GetBattlerFromBattlerType(bsys, 4);
                partner = BattleSystem_GetBattlerFromBattlerType(bsys, 2);
                break;
            case 1:
            case 2:
                battler = BattleSystem_GetBattlerFromBattlerType(bsys, 2);
                partner = BattleSystem_GetBattlerFromBattlerType(bsys, 4);
                break;
            }
        } else if (type & 2) {
            battler = BattleSystem_GetBattlerFromBattlerType(bsys, 2);
            partner = BattleSystem_GetBattlerFromBattlerType(bsys, 4);
        } else {
            battler = BattleSystem_GetBattlerFromBattlerType(bsys, 0);
            partner = battler;
        }
    } else if (type & 8) {
        battler = BattleSystem_GetBattlerIdPartner(bsys, U8(ctx, 0x194));
        partner = U8(ctx, 0x194);
    } else if (type & 2) {
        battler = BattleSystem_GetBattlerFromBattlerType(bsys, 2);
        partner = BattleSystem_GetBattlerFromBattlerType(bsys, 4);
    } else {
        battler = U8(ctx, 0x194);
        partner = battler;
    }

    if (type & 4) {
        if (type & 8) {
            U16(out, 2) = 0x3D1;
            U8(out, 1) = 0x31;
            U32(out, 4) = battler;
            U32(out, 8) = battler | (U8(cmd, battler + 4) << 8);
            U32(out, 0xC) = partner | (U8(cmd, partner + 4) << 8);
        } else if (type & 2) {
            U16(out, 2) = 0x3D2;
            U8(out, 1) = 9;
            U32(out, 4) = battler | (U8(cmd, battler + 4) << 8);
            U32(out, 8) = partner | (U8(cmd, partner + 4) << 8);
        } else {
            U16(out, 2) = 0x3D3;
            U8(out, 1) = 2;
            U32(out, 4) = battler | (U8(cmd, battler + 4) << 8);
        }
    } else if (type & 8) {
        U16(out, 2) = 0x3E1;
        U8(out, 1) = 0x39;
        U32(out, 4) = battler;
        U32(out, 8) = battler;
        U32(out, 0xC) = battler | (U8(cmd, battler + 4) << 8);
        U32(out, 0x10) = partner | (U8(cmd, partner + 4) << 8);
    } else if (type & 2) {
        U16(out, 2) = 0x3D2;
        U8(out, 1) = 9;
        U32(out, 4) = battler | (U8(cmd, battler + 4) << 8);
        U32(out, 8) = partner | (U8(cmd, partner + 4) << 8);
    } else {
        U16(out, 2) = 0x3D3;
        U8(out, 1) = 2;
        U32(out, 4) = battler | (U8(cmd, battler + 4) << 8);
    }
}

void ov12_02261928(BattleSystem *bsys, void *ctx, void *out) {
    u32 type = BattleSystem_GetBattleType(bsys);
    u32 result = BattleSystem_GetBattleOutcomeFlags(bsys);
    u32 b1, b2;
    if (type & 2) {
        b1 = BattleSystem_GetBattlerFromBattlerType(bsys, 3);
        b2 = BattleSystem_GetBattlerFromBattlerType(bsys, 5);
    } else {
        b1 = BattleSystem_GetBattlerFromBattlerType(bsys, 1);
        b2 = b1;
    }
    switch (result) {
    case 1:
        if (type & 8) {
            U16(out, 2) = 0x312;
            U8(out, 1) = 0x1A;
            U32(out, 4) = b1;
            U32(out, 8) = b2;
        } else {
            U16(out, 2) = 0x311;
            U8(out, 1) = 8;
            U32(out, 4) = b1;
        }
        break;
    case 2:
        if (type & 8) {
            U16(out, 2) = 0x314;
            U8(out, 1) = 0x1A;
            U32(out, 4) = b1;
            U32(out, 8) = b2;
        } else {
            U16(out, 2) = 0x313;
            U8(out, 1) = 8;
            U32(out, 4) = b1;
        }
        break;
    case 3:
        if (type & 8) {
            U16(out, 2) = 0x316;
            U8(out, 1) = 0x1A;
            U32(out, 4) = b1;
            U32(out, 8) = b2;
        } else {
            U16(out, 2) = 0x315;
            U8(out, 1) = 8;
            U32(out, 4) = b1;
        }
        break;
    }
}

void ov12_022619E4(BattleSystem *bsys, void *ctx, void *cmd, void *out) {
    int i;
    int left = 0;
    int right = 0;
    for (i = 0; i < BattleSystem_GetMaxBattlers(bsys); i++) {
        if (MaskOfFlagNo(i) & U8(cmd, 1)) {
            if (BattleSystem_GetFieldSide(bsys, i) != 0) {
                right++;
            } else {
                left++;
            }
        }
    }
    if (left && right) {
        U16(out, 2) = 0x30D;
        U8(out, 1) = 0;
        BattleSystem_SetBattleOutcomeFlags(bsys, 0xC3);
        return;
    }
    if (left) {
        U16(out, 2) = 0x30D;
        U8(out, 1) = 0;
        BattleSystem_SetBattleOutcomeFlags(bsys, 0xC2);
        return;
    }
    if (BattleSystem_GetBattleType(bsys) & 8) {
        U16(out, 2) = 0x318;
        U8(out, 1) = 0x1A;
        U32(out, 4) = BattleSystem_GetBattlerFromBattlerType(bsys, 3);
        U32(out, 8) = BattleSystem_GetBattlerFromBattlerType(bsys, 5);
    } else if (BattleSystem_GetBattleType(bsys) & 2) {
        U16(out, 2) = 0x317;
        U8(out, 1) = 8;
        U32(out, 4) = BattleSystem_GetBattlerFromBattlerType(bsys, 3);
    } else {
        U16(out, 2) = 0x317;
        U8(out, 1) = 8;
        U32(out, 4) = BattleSystem_GetBattlerFromBattlerType(bsys, 1);
    }
    BattleSystem_SetBattleOutcomeFlags(bsys, 0xC1);
}

void ov12_02261AD4(BattleSystem *bsys, void *ctx, void *out) {
    U16(out, 2) = 0x3BC;
    U8(out, 1) = 8;
    if (BattleSystem_GetBattleType(bsys) & 4) {
        u32 v = ov12_0223BFCC(bsys, ov12_0223BFC0(bsys));
        if (v) {
            U32(out, 4) = BattleSystem_GetBattlerFromBattlerType(bsys, 4);
        } else {
            U32(out, 4) = BattleSystem_GetBattlerFromBattlerType(bsys, 2);
        }
    } else {
        U32(out, 4) = U8(ctx, 0x194);
    }
    BattleSystem_SetBattleOutcomeFlags(bsys, 0xC2);
}
