// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void (*CleanupFn)(void *, int);
typedef void (*ValueFn)(u32);
typedef void (*PairFn)(void *, void *);
struct ThrowContext { u32 unused0; void *object; CleanupFn cleanup; u32 unused12[3]; u8 *frame; u32 registers[16]; };
struct ExceptionInfo { u32 unused0; void *record; u8 *instruction; };
typedef ThrowContext ThrowState;
typedef ExceptionInfo ExceptionState;
typedef struct CleanupRecord { void *object; u32 unused; CleanupFn cleanup; } CleanupRecord;
char *__PopStackFrame(ThrowContext *, ExceptionInfo *);
void FindExceptionRecord(char *, ExceptionInfo *);
void __SetupFrameInfo(ThrowContext *, ExceptionInfo *);
namespace std { void terminate(); }
char *__DecodeSignedNumber(char *, long *);
char *__DecodeUnsignedNumber(char *, unsigned long *);
#define DS(a, b) (u8 *)__DecodeSignedNumber((char *)(a), (long *)(b))
#define DU(a, b) ((u8 *)__DecodeUnsignedNumber((char *)(a), (unsigned long *)(b)))
static inline u32 Word(const u8 *p) { return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24); }
static inline u32 GetWord(ThrowState *t, const s32 *loc, u32 inReg) { if (!inReg) return *(u32 *)(t->frame + *loc); return t->registers[*loc]; }
static inline u32 GetByte(ThrowState *t, const s32 *loc, u32 inReg) { if (!inReg) return t->frame[*loc]; return t->registers[*loc] & 255u; }
static inline s32 GetShort(ThrowState *t, const s32 *loc, u32 inReg) { if (!inReg) return *(s16 *)(t->frame + *loc); return (s16)t->registers[*loc]; }
typedef struct { long displacement; } Ops1;
typedef struct { s32 object; } Ops2;
typedef struct { s32 condition, object; } Ops3;
typedef struct { s32 object; } Ops4;
typedef struct { s32 object; u32 count, stride; } Ops5;
typedef struct { s32 object, adjust; } Ops6;
typedef struct { s32 object, adjust; } Ops7;
typedef struct { s32 condition, object, adjust; } Ops8;
typedef struct { s32 object, adjust; u32 count, stride; } Ops9;
typedef struct { s32 object; } Ops10;
typedef struct { s32 condition, object; } Ops11;
typedef struct { u32 first; s32 second; } Ops12;
typedef struct { s32 object; } Ops13;
typedef struct { u32 count, unused; s32 distance; } Ops15;
typedef struct { s32 object, adjust, otherAdjust; u32 other; } Ops16;
typedef struct { s32 left, right, leftAdjust, rightAdjust; } Ops17;
typedef struct { s32 first, bytes; u32 stride; } Ops18;
typedef struct { s32 unused; } Ops19;
typedef struct { Ops19 o19; Ops18 o18; Ops17 o17; Ops16 o16; Ops15 o15; Ops13 o13; Ops12 o12; Ops11 o11; Ops10 o10; Ops9 o9; Ops8 o8; Ops7 o7; Ops6 o6; Ops5 o5; Ops4 o4; Ops3 o3; Ops2 o2; Ops1 o1; } Ops;
void UnwindStack(ThrowContext *, ExceptionInfo *, char *);
// @fn _Z11UnwindStackP12ThrowContextP13ExceptionInfoPc
#pragma push
#pragma opt_propagation off
void UnwindStack(ThrowContext *state, ExceptionInfo *ex, char *stop_) {
#pragma exception_terminate
    u8 *stop = (u8 *)stop_;
    Ops o;
    for (;;) {
        u8 *cursor = ex->instruction;
        u8 tag;
        if (!cursor) {
            FindExceptionRecord(__PopStackFrame(state, ex), ex);
            if (!ex->record) std::terminate();
            __SetupFrameInfo(state, ex);
            cursor = ex->instruction;
            if (!cursor) continue;
        }
        tag = *cursor;
        switch (tag & 31u) {
        case 1:
            __DecodeSignedNumber((char *)(cursor + 1), &o.o1.displacement);
            ex->instruction += o.o1.displacement;
            break;
        case 2: {
            u8 *p = DS(cursor + 1, &o.o2.object);
            u8 *obj = state->frame + o.o2.object;
            CleanupFn call = (CleanupFn)Word(p);
            call(obj, -1);
            ex->instruction = p + 4;
            break;
        }
        case 3: {
            u32 condReg = tag & 64u;
            u8 *p = DS(cursor + 1, &o.o3.condition);
            p = DS(p, &o.o3.object);
            {
                CleanupFn call = (CleanupFn)Word(p);
                u8 *next = p + 4;
                if (GetByte(state, &o.o3.condition, condReg)) call(state->frame + o.o3.object, -1);
                ex->instruction = next;
            }
            break;
        }
        case 4: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o4.object);
            CleanupFn call = (CleanupFn)Word(p);
            u8 *next = p + 4;
            call((void *)GetWord(state, &o.o4.object, inReg), -1);
            ex->instruction = next;
            break;
        }
        case 5: {
            u8 *p = DS(cursor + 1, &o.o5.object);
            p = DU(p, &o.o5.count);
            p = DU(p, &o.o5.stride);
            {
                u8 *end;
                CleanupFn call = (CleanupFn)Word(p);
                u8 *next = p + 4;
                u32 n = o.o5.count;
                end = state->frame + o.o5.object + n * o.o5.stride;
                if (n) {
                    do {
                        end -= o.o5.stride;
                        call(end, -1);
                    } while (--n);
                }
                ex->instruction = next;
            }
            break;
        }
        case 6: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o6.object);
            p = DS(p, &o.o6.adjust);
            {
                CleanupFn call = (CleanupFn)Word(p);
                u8 *next = p + 4;
                call((u8 *)GetWord(state, &o.o6.object, inReg) + o.o6.adjust, 0);
                ex->instruction = next;
            }
            break;
        }
        case 7: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o7.object);
            p = DS(p, &o.o7.adjust);
            {
                CleanupFn call = (CleanupFn)Word(p);
                u8 *next = p + 4;
                call((u8 *)GetWord(state, &o.o7.object, inReg) + o.o7.adjust, -1);
                ex->instruction = next;
            }
            break;
        }
        case 8: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o8.condition);
            p = DS(p, &o.o8.object);
            p = DS(p, &o.o8.adjust);
            {
                CleanupFn call = (CleanupFn)Word(p);
                u8 *next = p + 4;
                if (GetShort(state, &o.o8.condition, tag & 64u)) call((u8 *)GetWord(state, &o.o8.object, inReg) + o.o8.adjust, -1);
                ex->instruction = next;
            }
            break;
        }
        case 9: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o9.object);
            p = DS(p, &o.o9.adjust);
            p = DU(p, &o.o9.count);
            p = DU(p, &o.o9.stride);
            {
                u8 *end;
                CleanupFn call = (CleanupFn)Word(p);
                u8 *next = p + 4;
                u32 n;
                if (inReg) end = (u8 *)state->registers[o.o9.object] + o.o9.adjust;
                else end = *(u8 **)(state->frame + o.o9.object) + o.o9.adjust;
                end += o.o9.count * o.o9.stride;
                n = o.o9.count;
                if (n) {
                    do {
                        end -= o.o9.stride;
                        call(end, -1);
                    } while (--n);
                }
                ex->instruction = next;
            }
            break;
        }
        case 10: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o10.object);
            ValueFn call = (ValueFn)Word(p);
            u8 *next = p + 4;
            call(GetWord(state, &o.o10.object, inReg));
            ex->instruction = next;
            break;
        }
        case 11: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o11.condition);
            p = DS(p, &o.o11.object);
            {
                ValueFn call = (ValueFn)Word(p);
                u8 *next = p + 4;
                if (GetByte(state, &o.o11.condition, tag & 64u)) call(GetWord(state, &o.o11.object, inReg));
                ex->instruction = next;
            }
            break;
        }
        case 12: {
            u8 *p;
            if (stop == cursor) return;
            p = DU(cursor + 5, &o.o12.first);
            ex->instruction = DS(p, &o.o12.second);
            break;
        }
        case 13: {
            u8 *p = DS(cursor + 1, &o.o13.object);
            CleanupRecord *rec = (CleanupRecord *)(state->frame + o.o13.object);
            if (rec->cleanup) {
                if (state->object == rec->object) state->cleanup = rec->cleanup;
                else rec->cleanup(rec->object, -1);
            }
            ex->instruction = p;
            break;
        }
        case 15: {
            u8 *p;
            if (stop == cursor) return;
            p = DU(cursor + 1, &o.o15.count);
            p = DU(p, &o.o15.unused);
            p = DS(p, &o.o15.distance);
            ex->instruction = p + o.o15.count * 4u;
            break;
        }
        case 16: {
            u32 inReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o16.object);
            p = DS(p, &o.o16.adjust);
            o.o16.other = Word(p);
            p = DS(p + 4, &o.o16.otherAdjust);
            {
                PairFn call = (PairFn)Word(p);
                u8 *next = p + 4;
                void *left = (u8 *)GetWord(state, &o.o16.object, inReg) + o.o16.adjust;
                void *right = (u8 *)o.o16.other + o.o16.otherAdjust;
                call(left, right);
                ex->instruction = next;
            }
            break;
        }
        case 17: {
            u32 rightReg;
            u32 leftReg = tag & 32u;
            u8 *p = DS(cursor + 1, &o.o17.left);
            p = DS(p, &o.o17.leftAdjust);
            rightReg = *p++ & 32u;
            p = DS(p, &o.o17.right);
            p = DS(p, &o.o17.rightAdjust);
            {
                PairFn call = (PairFn)Word(p);
                u8 *next = p + 4;
                u8 *left = (u8 *)GetWord(state, &o.o17.left, leftReg);
                u8 *right = (u8 *)GetWord(state, &o.o17.right, rightReg);
                call(left + o.o17.leftAdjust, right + o.o17.rightAdjust);
                ex->instruction = next;
            }
            break;
        }
        case 18: {
            u32 firstReg = tag & 32u, bytesReg;
            u8 *p = DS(cursor + 1, &o.o18.first);
            bytesReg = *p++ & 32u;
            p = DS(p, &o.o18.bytes);
            p = DU(p, &o.o18.stride);
            {
                u8 *end;
                CleanupFn call = (CleanupFn)Word(p);
                u8 *next = p + 4;
                if (firstReg) end = (u8 *)state->registers[o.o18.first];
                else end = *(u8 **)(state->frame + o.o18.first);
                u32 bytes = GetWord(state, &o.o18.bytes, bytesReg), n;
                end += bytes;
                n = bytes / o.o18.stride;
                if (n) {
                    do {
                        end -= o.o18.stride;
                        call(end, -1);
                    } while (--n);
                }
                ex->instruction = next;
            }
            break;
        }
        case 19:
            ex->instruction = DS(cursor + 1, &o.o19.unused);
            break;
        default:
            std::terminate();
            break;
        }
        if (tag & 128u) ex->instruction = 0;
    }
}
#pragma pop
