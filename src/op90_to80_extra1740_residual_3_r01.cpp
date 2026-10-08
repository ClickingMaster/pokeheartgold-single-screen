#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#include <size_t.h>
inline void *operator new(std::size_t, void *storage) { return storage; }
struct Pair { s32 key; u32 payload; };
struct Buffer { s32 capacity, count; Pair *data; };
struct Cursor {
    Pair *position;
    explicit Cursor(Pair *p) : position(p) {}
    Cursor() {}
    Pair &operator*() const { return *position; }
    Pair *operator->() const { return position; }
    Cursor &operator++() { ++position; return *this; }
    Cursor &operator--() { --position; return *this; }
    Cursor operator+(s32 n) const { Cursor t(*this); t.position += n; return t; }
    s32 operator-(const Cursor &other) const { return (s32)((u32)position - (u32)other.position) / (s32)sizeof(Pair); }
    bool operator==(const Cursor &other) const { return position == other.position; }
};
inline bool operator!=(const Cursor &a, const Cursor &b) { return !(a == b); }
struct ConstructingOut {
    Pair *position;
    Buffer *buffer;
    ConstructingOut(Pair *p, Buffer *b) : position(p), buffer(b) {}
    ConstructingOut() {}
    ConstructingOut operator*() const { return *this; }
    ConstructingOut &operator=(const Pair &value) {
        if (position - buffer->data < buffer->count) *position = value;
        else { if (position) new (position) Pair(value); ++buffer->count; }
        return *this;
    }
    ConstructingOut &operator++() { ++position; return *this; }
    s32 operator-(const ConstructingOut &o) const { return position - o.position; }
};
inline ConstructingOut CopyForward(Cursor first, Cursor last, ConstructingOut output) {
    while (first != last) { *output = *first; ++first; ++output; }
    return output;
}
inline s32 Distance(Cursor first, Cursor last) { return last - first; }
inline void IterSwap(const Cursor &a, const Cursor &b) { Pair t = *a; *a = *b; *b = t; }
inline bool Less(const Pair &a, const Pair &b) { return a.key < b.key; }
extern "C" Cursor ov45_02239DF0(ConstructingOut, ConstructingOut, Cursor, Cursor, Cursor, void *);
extern "C" Cursor ov45_02239BBC(Cursor, Cursor, ConstructingOut, ConstructingOut, Cursor, void *);
extern "C" Cursor ov45_02239B3C(Cursor, Cursor, const Pair &, void *);
extern "C" Cursor ov45_02239ABC(Cursor, Cursor, const Pair &, void *);
extern "C" Cursor ov45_022399A8(Cursor, Cursor, Cursor);
extern "C" void ov45_02239518(Cursor first, Cursor middle, Cursor last, void *ctx, s32 len1, s32 len2, ConstructingOut buf, ConstructingOut bufEnd);
// @fn ov45_02239518
extern "C" void ov45_02239518(Cursor first, Cursor middle, Cursor last, void *ctx, s32 len1, s32 len2, ConstructingOut buf, ConstructingOut bufEnd) {
    Cursor first_cut, second_cut;
    s32 len11, len22;
    if (len1 == 0 || len2 == 0) return;
    if (len1 + len2 == 2) {
        if (Less(*middle, *first)) IterSwap(first, middle);
        return;
    }
    s32 cap = bufEnd - buf;
    if (len1 <= len2 && len1 <= cap) {
        ov45_02239DF0(buf, CopyForward(first, middle, buf), middle, last, first, ctx);
    } else if (len2 <= cap) {
        ov45_02239BBC(first, middle, buf, CopyForward(middle, last, buf), last, ctx);
    } else {
        if (len1 <= len2) {
            len22 = len2 / 2;
            second_cut = middle + len22;
            first_cut = ov45_02239B3C(first, middle, *second_cut, ctx);
            len11 = Distance(first, first_cut);
        } else {
            len11 = len1 / 2;
            first_cut = first + len11;
            second_cut = ov45_02239ABC(middle, last, *first_cut, ctx);
            len22 = Distance(middle, second_cut);
        }
        middle = ov45_022399A8(first_cut, middle, second_cut);
        ov45_02239518(first, first_cut, middle, ctx, len11, len22, buf, bufEnd);
        ov45_02239518(middle, second_cut, last, ctx, len1 - len11, len2 - len22, buf, bufEnd);
    }
}
