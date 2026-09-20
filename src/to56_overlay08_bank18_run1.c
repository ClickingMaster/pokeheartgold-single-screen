#include "../include/to56_overlay08_bank18_private.h"
#pragma force_active on

void ov08_02220D90(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x80, 0x48);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x88, 0x50);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x90, 0x50);
}

void ov08_02220DE8(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220B90(rec->flags17, work->sprites[13 + work->data->battler], 0xC6, 0x14);
    ov08_02220BB4(work, rec, ov08_022253F4);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    ov08_02220C08(rec->value1A, work->sprites[6], 0x14, 0x84);
}

void ov08_02220E80(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    u32 i;
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220B90(rec->flags17, work->sprites[13 + work->data->battler], 0xC6, 0x14);
    ov08_02220BB4(work, rec, ov08_022253D4);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    for (i = 0; i < 4; i++) {
        if (rec->moves[i].move != 0) {
            ov08_02220AEC(work, work->sprites[21 + i], 0xB010 + i, rec->moves[i].value4);
            ov08_02220A8C(work->sprites[21 + i], ov08_02225454[i].x, ov08_02225454[i].y);
        }
    }
}

void ov08_02220F58(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220B90(rec->flags17, work->sprites[13 + work->data->battler], 0xC6, 0x14);
    ov08_02220BB4(work, rec, ov08_022253E4);
    ov08_02220A8C(work->sprites[21 + work->data->slot], 0x88, 0x30);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    ov08_02220B3C(work, work->sprites[26], rec->moves[work->data->slot].value5);
    ov08_02220A8C(work->sprites[26], 0x18, 0x58);
}

void ov08_0222101C(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220BB4(work, rec, ov08_022253C4);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    ov08_022213C8(work);
}

void ov08_02221088(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220BB4(work, rec, ov08_022253B4);
    ov08_02220A8C(work->sprites[21 + work->data->slot], 0x88, 0x30);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    if (work->data->slot < 4) {
        ov08_02220B3C(work, work->sprites[26], rec->moves[work->data->slot].value5);
    } else {
        ov08_02220B3C(work, work->sprites[26], GetMoveAttr(work->data->move, MOVEATTR_CLASS));
    }
    ov08_02220A8C(work->sprites[26], 0x18, 0x58);
}

void ov08_0222114C(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220BB4(work, rec, ov08_022253C4);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    ov08_022213C8(work);
}

void ov08_022211B8(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220A8C(work->sprites[21 + work->data->slot], 0x88, 0x48);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    ov08_02221500(work);
}

void ov08_02221230(Ov08Work *work) {
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    ov08_02220A8C(work->sprites[7 + work->data->battler], 0x18, 0x0C);
    ov08_02220C08(rec->value1A, work->sprites[work->data->battler], 0x20, 0x14);
    ov08_02220C3C(rec->enabled2D, work->sprites[32 + work->data->battler], 0x28, 0x14);
    ov08_022213C8(work);
    ov08_02221500(work);
}

void ov08_02221294(Ov08Work *work) {
    u16 i;
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    for (i = 0; i < 4; i++) {
        if (rec->moves[i].move != 0) {
            ov08_02220AEC(work, work->sprites[21 + i], 0xB010 + i, rec->moves[i].value4);
            ov08_02220A8C(work->sprites[21 + i], ov08_0222550C[i].x, ov08_0222550C[i].y);
        }
    }
    if (work->data->move != 0) {
        ov08_02220AEC(work, work->sprites[25], 0xB014, (u8)GetMoveAttr(work->data->move, MOVEATTR_TYPE));
        ov08_02220A8C(work->sprites[25], 0x58, 0xB0);
    }
}

void ov08_02221328(Ov08Work *work) {
    u16 i;
    Ov08BattlerRecord *rec = &work->battlers[work->data->battler];
    for (i = 0; i < 4; i++) {
        if (rec->moves[i].move != 0) {
            ov08_02220AEC(work, work->sprites[21 + i], 0xB010 + i, (u8)(GetMoveAttr(rec->moves[i].move, MOVEATTR_CONTEST_TYPE) + 0x12));
            ov08_02220A8C(work->sprites[21 + i], ov08_0222550C[i].x, ov08_0222550C[i].y);
        }
    }
    if (work->data->move != 0) {
        ov08_02220AEC(work, work->sprites[25], 0xB014, (u8)(GetMoveAttr(work->data->move, MOVEATTR_CONTEST_TYPE) + 0x12));
        ov08_02220A8C(work->sprites[25], 0x58, 0xB0);
    }
}
