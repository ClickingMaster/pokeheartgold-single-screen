#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline void *operator new(unsigned long, void *p) {
    return p;
}

struct Elem {
    int key;
    u32 val;
};

struct Buf {
    u32 unk0;
    int count;
    Elem *base;
};

struct Proxy {
    Elem *p;
    Buf *buf;

    Proxy(Elem *p_, Buf *b_)
        : p(p_)
        , buf(b_) {
    }

    void operator=(const Elem &e) {
        if (p - buf->base < buf->count) {
            *p = e;
        } else {
            new (p) Elem(e);
            ++buf->count;
        }
    }
};

struct Out {
    Elem *p;
    Buf *buf;

    Proxy operator*() const {
        return Proxy(p, buf);
    }

    Out &operator++() {
        ++p;
        return *this;
    }
};

struct It {
    Elem *p;

    Elem &operator*() const {
        return *p;
    }

    It &operator++() {
        ++p;
        return *this;
    }

    bool operator==(const It &o) const {
        return p == o.p;
    }
};

inline bool operator!=(const It &a, const It &b) {
    return !(a == b);
}

struct Less {
    bool operator()(const Elem &a, const Elem &b) const {
        return a.key < b.key;
    }
};

inline Out Copy(It first, It last, Out result) {
    for (; first != last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

extern "C" Out LobbySort_Merge(It first1, It last1, It first2, It last2, Out result, Less comp);

extern "C" Out LobbySort_Merge(It first1, It last1, It first2, It last2, Out result, Less comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    if (first1 == last1) {
        return Copy(first2, last2, result);
    }
    return Copy(first1, last1, result);
}
