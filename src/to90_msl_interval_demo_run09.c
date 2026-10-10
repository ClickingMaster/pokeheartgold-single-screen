#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 first;
    void *record;
    u8 *cursor;
    u8 pad[12];
    u8 context[1];
} Iterator;

typedef struct {
    s32 jump, c19;

    struct {
        s32 a, b;
        u32 c;
    } c18;

    struct {
        s32 first, third, second, last;
    } c17;

    struct {
        s32 a, b, c;
        u32 raw;
    } c16;

    struct {
        u32 count, flags;
        s32 offset;
    } c15;

    s32 c13;

    struct {
        u32 a;
        s32 b;
    } c12;

    struct {
        s32 a, b;
    } c11;

    s32 c10;

    struct {
        s32 a, b;
        u32 c, d;
    } c9;

    struct {
        s32 a, b, c;
    } c8;

    struct {
        s32 a, b;
    } c7, c6;

    struct {
        s32 a;
        u32 b, c;
    } c5;

    s32 c4;

    struct {
        s32 a, b;
    } c3;

    s32 c2;
} Frame;

extern u8 *_Z15__PopStackFrameP12ThrowContextP13ExceptionInfo(void *, Iterator *);
extern void _Z19FindExceptionRecordPcP13ExceptionInfo(u8 *, Iterator *);
extern void _Z16__SetupFrameInfoP12ThrowContextP13ExceptionInfo(void *, Iterator *);
extern u8 *_Z20__DecodeSignedNumberPcPl(u8 *, s32 *);
extern u8 *_Z22__DecodeUnsignedNumberPcPm(u8 *, u32 *);
#define DS _Z20__DecodeSignedNumberPcPl
#define DU _Z22__DecodeUnsignedNumberPcPm
u32 _Z10NextActionP14ActionIterator(Iterator *);
#pragma force_active on

u32 _Z10NextActionP14ActionIterator(Iterator *it) {
    Frame f;
    u8 *p;
    u32 op;
again:
    p = it->cursor;
    if (!p || (*p & 128)) {
        _Z19FindExceptionRecordPcP13ExceptionInfo(_Z15__PopStackFrameP12ThrowContextP13ExceptionInfo(it->context, it), it);
        if (!it->record) {
            return 255;
        }
        _Z16__SetupFrameInfoP12ThrowContextP13ExceptionInfo(it->context, it);
        if (!it->cursor) {
            goto again;
        }
        goto next;
    }
    switch (*p & 31) {
    case 2:
        p = DS(p + 1, &f.c2);
        it->cursor = p + 4;
        break;
    case 3:
        p = DS(p + 1, &f.c3.a);
        p = DS(p, &f.c3.b);
        it->cursor = p + 4;
        break;
    case 4:
        p = DS(p + 1, &f.c4);
        it->cursor = p + 4;
        break;
    case 5:
        p = DS(p + 1, &f.c5.a);
        p = DU(p, &f.c5.b);
        p = DU(p, &f.c5.c);
        it->cursor = p + 4;
        break;
    case 6:
        p = DS(p + 1, &f.c6.a);
        p = DS(p, &f.c6.b);
        it->cursor = p + 4;
        break;
    case 7:
        p = DS(p + 1, &f.c7.a);
        p = DS(p, &f.c7.b);
        it->cursor = p + 4;
        break;
    case 8:
        p = DS(p + 1, &f.c8.a);
        p = DS(p, &f.c8.b);
        p = DS(p, &f.c8.c);
        it->cursor = p + 4;
        break;
    case 9:
        p = DS(p + 1, &f.c9.a);
        p = DS(p, &f.c9.b);
        p = DU(p, &f.c9.c);
        p = DU(p, &f.c9.d);
        it->cursor = p + 4;
        break;
    case 10:
        p = DS(p + 1, &f.c10);
        it->cursor = p + 4;
        break;
    case 11:
        p = DS(p + 1, &f.c11.a);
        p = DS(p, &f.c11.b);
        it->cursor = p + 4;
        break;
    case 12:
        p = DU(p + 5, &f.c12.a);
        p = DS(p, &f.c12.b);
        it->cursor = p;
        break;
    case 13:
        it->cursor = DS(p + 1, &f.c13);
        break;
    case 15:
        p = DU(p + 1, &f.c15.count);
        p = DU(p, &f.c15.flags);
        p = DS(p, &f.c15.offset);
        it->cursor = p + f.c15.count * 4;
        break;
    case 16:
        p = DS(p + 1, &f.c16.a);
        p = DS(p, &f.c16.b);
        f.c16.raw = (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
        p = DS(p + 4, &f.c16.c);
        it->cursor = p + 4;
        break;
    case 17:
        p = DS(p + 1, &f.c17.first);
        p = DS(p, &f.c17.second);
        p = DS(p + 1, &f.c17.third);
        p = DS(p, &f.c17.last);
        it->cursor = p + 4;
        break;
    case 18:
        p = DS(p + 1, &f.c18.a);
        p = DS(p + 1, &f.c18.b);
        p = DU(p, &f.c18.c);
        it->cursor = p + 4;
        break;
    case 19:
        it->cursor = DS(p + 1, &f.c19);
        break;
    default:
        return 255;
    }
next:
    p = it->cursor;
    op = *p & 31;
    if (op != 1) {
        return op;
    }
    do {
        DS(p + 1, &f.jump);
        it->cursor += f.jump;
        p = it->cursor;
        op = *p & 31;
    } while (op == 1);
    return op;
}
