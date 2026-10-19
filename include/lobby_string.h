#ifndef POKEHEARTGOLD_LOBBY_STRING_H
#define POKEHEARTGOLD_LOBBY_STRING_H

// The lobby string (overlay 45): up to 10 characters are kept in the string itself, longer ones in an allocated
// buffer. Bit 0 of the first byte tells which form the string has.

#include <nitro/types.h>

typedef union Str {
    struct {
        u32 isLong : 1;
        u32 capacity : 31;
        u32 size;
        char *ptr;
    } l;

    struct {
        u8 isLong : 1;
        u8 len : 7;
        char buf[11];
    } s;

#ifdef __cplusplus
    Str() {
    }

    const char *c_str() const {
        return !l.isLong ? s.buf : l.ptr;
    }
#endif
} Str;

static inline BOOL StrIsShort(const Str *s) {
    return !s->l.isLong;
}

static inline u32 StrSize(Str *s) {
    return StrIsShort(s) ? s->s.len : s->l.size;
}

static inline const char *StrData(Str *s) {
    return StrIsShort(s) ? s->s.buf : s->l.ptr;
}

#endif // POKEHEARTGOLD_LOBBY_STRING_H
