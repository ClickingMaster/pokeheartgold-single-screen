#include "overlay39_record_state_r23_private.h"

BOOL ov39_022290A4(Ov39AppR23 *app) {
    if (ov39_02228A70(app->printer40) == 1) {
        return FALSE;
    }
    switch (app->substate94) {
    case 0:
        ov39_02228B04(app, app->unk2C, 0x1A, app->unk90, 0xF0F);
        app->substate94++;
        break;
    case 1:
        ov00_021EC8D8();
        sub_0203946C();
        app->substate94++;
        break;
    case 2:
        WirelessStatusIcon_Destroy();
        ov39_02228B04(app, app->unk2C, 0x1B, app->unk90, 0xF0F);
        app->substate94++;
        break;
    case 3:
        app->timer98++;
        if (app->timer98 > 30) {
            app->state08 = app->state0C;
        }
        break;
    }
    return FALSE;
}

void ov39_0222915C(const Ov39LargeRec *src, Ov39SmallRec *dst) {
    int i;
    MI_CpuFill8(dst, 0, 0x60);
    dst->word00 = src->word00;
    dst->word04 = src->word04;
    dst->word08 = src->word08;
    dst->half0C = src->half0C;
    for (i = 0; i < 8; i++) {
        dst->arr0E[i] = src->arr24[i];
    }
    dst->byte1E = src->byte34;
    dst->byte1F = src->byte35;
    dst->byte20 = src->byte36;
    dst->byte21 = src->byte37;
    dst->word24 = src->word3C;
    dst->half28 = src->half40;
    dst->half2A = src->half42;
    dst->half2C = src->half44;
    dst->half2E = src->half46;
    {
        const u8 *s = src->bytes48;
        Ov39SmallRec *d = dst;
        for (i = 0; i < 10; i++) {
            d->bytes30[0] = s[0];
            d->bytes30[1] = s[1];
            d->bytes30[2] = s[2];
            d->bytes30[3] = s[3];
            s += 4;
            d = (Ov39SmallRec *)((u8 *)d + 4);
        }
    }
    dst->byte58 = src->byte70;
    dst->byte59 = src->byte71;
}

void ov39_02229224(const Ov39SmallRec *src, Ov39LargeRec *dst) {
    int i;
    MI_CpuFill8(dst, 0, FashionEntry_sizeof((void *)src));
    dst->word00 = src->word00;
    dst->word04 = src->word04;
    dst->word08 = src->word08;
    dst->half0C = src->half0C;
    for (i = 0; i < 8; i++) {
        dst->arr24[i] = src->arr0E[i];
    }
    dst->byte34 = src->byte1E;
    dst->byte35 = src->byte1F;
    dst->byte36 = src->byte20;
    dst->byte37 = src->byte21;
    dst->word3C = src->word24;
    dst->half40 = src->half28;
    dst->half42 = src->half2A;
    dst->half44 = src->half2C;
    dst->half46 = src->half2E;
    {
        const u8 *s = src->bytes30;
        Ov39LargeRec *d = dst;
        for (i = 0; i < 10; i++) {
            d->bytes48[0] = s[0];
            d->bytes48[1] = s[1];
            d->bytes48[2] = s[2];
            d->bytes48[3] = s[3];
            s += 4;
            d = (Ov39LargeRec *)((u8 *)d + 4);
        }
    }
    dst->byte70 = src->byte58;
    dst->byte71 = src->byte59;
    {
        Ov39LargeRec *p = dst;
        for (i = 0; i < 11; i++) {
            *(u16 *)((u8 *)p + 0x0E) = 0xFFFF;
            p = (Ov39LargeRec *)((u8 *)p + 2);
        }
    }
    *(u8 *)((u8 *)dst + 0x38) = 0;
}
