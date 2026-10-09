#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
struct Pair { s32 key; u32 payload; };
struct Buffer { s32 capacity, count; Pair *data; };
struct Cursor {
    Pair *position;
    explicit Cursor(Pair *p) : position(p) {}
    Cursor() {}
    Pair &operator*() const { return *position; }
    Pair *base() const { return position; }
    Cursor &operator++() { ++position; return *this; }
    Cursor &operator--() { --position; return *this; }
    bool operator==(const Cursor &other) const { return position == other.position; }
};
inline bool operator!=(const Cursor &a, const Cursor &b) { return !(a == b); }
struct ConstructingOut {
    Pair *position;
    Buffer *buffer;
    ConstructingOut(Pair *p, Buffer *b) : position(p), buffer(b) {}
    ConstructingOut() {}
    operator Pair *() const { return position; }
    ConstructingOut &operator++() { ++position; return *this; }
    ConstructingOut &operator--() { --position; return *this; }
};
inline bool Less(const Pair &a, const Pair &b) { return a.key < b.key; }
inline Cursor CopyA(Cursor first, Cursor last, Cursor result) {
    Pair *l = last.base();
    Pair *r = result.base();
    Pair *f = first.base();
    for (; f < l; ++f, ++r) *r = *f;
    return Cursor(r);
}
inline Cursor CopyB(ConstructingOut first, ConstructingOut last, Cursor result) {
    for (; first != last; ++first, ++result) *result = *first;
    return result;
}
extern "C" Cursor ov45_02239DF0(ConstructingOut first1, ConstructingOut last1, Cursor first2, Cursor last2, Cursor result, void *ctx);
// @fn ov45_02239DF0
extern "C" Cursor ov45_02239DF0(ConstructingOut first1, ConstructingOut last1, Cursor first2, Cursor last2, Cursor result, void *ctx) {
    while (first1 != last1 && first2 != last2) {
        if (Less(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    if (first1 == last1) return CopyA(first2, last2, result);
    return CopyB(first1, last1, result);
}
