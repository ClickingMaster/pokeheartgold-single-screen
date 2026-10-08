#pragma force_active on
#include "global.h"
typedef struct Record {
    u32 unused;
    int length;
    u8 text[0x28];
    u8 fields[4][0x40];
} Record;
typedef struct Out {
    u16 flags;
    u8 pad[0x114];
    u8 terminator;
    u8 data[0x154];
} Out;
extern Record ov13_0224D8C0;
extern Record ov13_0224D930;
extern Record ov13_0224D660;
extern Record ov13_0224D790;
extern u32 _0224CF98[];
void ov13_02222978(void *dst, int value, u32 size);
void ov13_02222968(void *dst, const void *src, u32 size);
int ov13_02222A9C(const void *str);
int ov13_022216BC(const void *str, int length);
int ov13_022214C4(Out *out);
// @fn ov13_022214C4
int ov13_022214C4(Out *out) {
    Record *a = &ov13_0224D8C0;
    u8 *dst = out->data;
    Record *c = &ov13_0224D660;
    Record *d = &ov13_0224D790;
    Record *b = &ov13_0224D930;

    if (dst == NULL) {
        return -1;
    }
    out->flags = _0224CF98[11] & _0224CF98[12];
    ov13_02222978(dst, 0, 0x154);
    if (out->flags & 1) {
        ov13_02222968(dst, c->fields[0], c->length);
        ov13_02222968(dst + 6, c->fields[1], c->length);
        ov13_02222968(dst + 12, c->fields[2], c->length);
        ov13_02222968(dst + 18, c->fields[3], c->length);
        if (ov13_022216BC(c->text, ov13_02222A9C(c->text))) {
            goto fail;
        }
        ov13_02222968(dst + 0x18, c->text, ov13_02222A9C(c->text));
    }
    if (out->flags & 2) {
        ov13_02222968(dst + 0x39, d->fields[0], d->length);
        ov13_02222968(dst + 0x47, d->fields[1], d->length);
        ov13_02222968(dst + 0x55, d->fields[2], d->length);
        ov13_02222968(dst + 0x63, d->fields[3], d->length);
        if (ov13_022216BC(d->text, ov13_02222A9C(d->text))) {
            goto fail;
        }
        ov13_02222968(dst + 0x71, d->text, ov13_02222A9C(d->text));
    }
    if (out->flags & 4) {
        if (ov13_022216BC(a->fields[0], a->length - 1)) {
            goto fail;
        }
        ov13_02222968(dst + 0x92, a->fields[0], a->length);
        if (ov13_022216BC(a->text, ov13_02222A9C(a->text))) {
            goto fail;
        }
        ov13_02222968(dst + 0xD2, a->text, ov13_02222A9C(a->text));
    }
    if (out->flags & 8) {
        if (ov13_022216BC(b->fields[0], b->length - 1)) {
            goto fail;
        }
        ov13_02222968(dst + 0xF3, b->fields[0], b->length);
        if (ov13_022216BC(b->text, ov13_02222A9C(b->text))) {
            goto fail;
        }
        ov13_02222968(dst + 0x133, b->text, ov13_02222A9C(b->text));
    }
    out->terminator = 0;
    return 0;
fail:
    ov13_02222978(dst, 0, 0x154);
    return -1;
}
