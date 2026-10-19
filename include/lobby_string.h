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

    u32 words[3];

#ifdef __cplusplus
    Str() {
    }

    u32 size() const {
        return !l.isLong ? s.len : l.size;
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

#ifdef __cplusplus
extern "C" {
#endif

Str *LobbyString_InitCopy(Str *, const Str *);
Str *LobbyString_InitFromCStr(Str *, const char *);
Str *LobbyString_InitSubstring(Str *, Str *, u32, u32);
Str *LobbyString_Destroy(Str *);
Str *LobbyString_Assign(Str *, const Str *);
void LobbyString_AssignCStr(Str *, const char *);
void LobbyString_Append(Str *, void *);
void LobbyString_AppendCStr(Str *, const char *);
Str *LobbyString_Replace(Str *, u32, u32, const Str *);
void LobbyString_ReplaceCStr(Str *, u32, u32, const char *);
Str *LobbyString_ReplaceSubstring(Str *, u32, u32, Str *, u32, u32);
void LobbyString_Reserve(Str *, u32);
int LobbyString_Compare(const Str *, const Str *);
int LobbyString_CompareCStr(const Str *, const char *);
int LobbyString_FindFirstOfChars(Str *, const char *, u32, u32);
void LobbyString_Substr(Str *, Str *, u32, u32);
Str *LobbyString_Concat(Str *, Str *, Str *);
Str *LobbyString_ConcatCStr(Str *, Str *, char *);
void LobbyString_Format(Str *, const char *, ...);
void LobbyString_FormatN(Str *, u32, const char *, ...);
#ifdef __cplusplus
int LobbyString_Equals(const Str &, const Str &);
#else
int LobbyString_Equals(const Str *, const Str *);
#endif

#ifdef __cplusplus
}
#endif

#endif // POKEHEARTGOLD_LOBBY_STRING_H
