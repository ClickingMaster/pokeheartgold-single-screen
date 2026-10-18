#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

int LobbyString_ToLowerChar(int c) {
    if (c < 0 || c >= 0x80) {
        return c;
    }
    return __lower_mapC[c];
}

BOOL PPWLobby_EncodeMessage(int type, int mode, u32 value, const void *data, u32 len, Str *out) {
    char buf[0x191];
    Str s0;
    Str s1;
    Str s2;
    Str s3;
    if (type == 0 && len > 400) {
        return FALSE;
    }
    if (type == 1 && len > 300) {
        return FALSE;
    }
    LobbyString_AssignCStr(out, sStr_Empty_16);
    LobbyString_FormatN(&s0, 3, sFmt_Hex_3, 0);
    LobbyString_Append(out, &s0);
    LobbyString_Destroy(&s0);
    LobbyString_AppendCStr(out, sStr_Space_2);
    LobbyString_FormatN(&s1, 2, sFmt_Hex_3, 6);
    LobbyString_Append(out, &s1);
    LobbyString_Destroy(&s1);
    LobbyString_AppendCStr(out, sStr_Space_2);
    switch (type) {
    case 0:
        LobbyString_AppendCStr(out, sStr_S);
        break;
    case 1:
        LobbyString_AppendCStr(out, sStr_B);
        break;
    default:
        return FALSE;
    }
    LobbyString_AppendCStr(out, sStr_Space_2);
    switch (mode) {
    case 1:
        LobbyString_AppendCStr(out, sStr_A);
        break;
    case 0:
        LobbyString_AppendCStr(out, sStr_S);
        break;
    default:
        return FALSE;
    }
    LobbyString_AppendCStr(out, sStr_Space_2);
    LobbyString_FormatN(&s2, 8, sFmt_Hex_3, value);
    LobbyString_Append(out, &s2);
    LobbyString_Destroy(&s2);
    LobbyString_AppendCStr(out, sStr_Space_2);
    LobbyString_AppendCStr(out, sStr__);
    LobbyString_AppendCStr(out, sStr_SpaceSpace);
    switch (type) {
    case 0:
        LobbyString_FormatN(&s3, len, sFmt_Str_3, data);
        LobbyString_Append(out, &s3);
        LobbyString_Destroy(&s3);
        break;
    case 1:
        MI_CpuFill8(buf, 0, sizeof(buf));
        if (DWC_Base64Encode(data, len, buf, sizeof(buf)) == -1) {
            return FALSE;
        }
        LobbyString_AppendCStr(out, buf);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

void LobbyCharVector_PushBackSlow(CharVec *s, const char *c) {
    TouchTag();
    LobbyCharVector_RecommendSize(s, 1);
    LobbyCharVector_AppendFillSlow(s, 1, c);
}

void LobbyString_Append(Str *s, void *arg) {
    LobbyString_Replace(s, StrSize(s), 0, arg);
}

void LobbyString_AppendCStr(Str *s, const char *arg) {
    LobbyString_ReplaceCStr(s, StrSize(s), 0, arg);
}

void LobbyString_AssignCStr(Str *s, const char *arg) {
    LobbyString_ReplaceCStr(s, 0, StrSize(s), arg);
}

#pragma warn_uninitializedvar off
void LobbyString_ReplaceCStr(Str *s, u32 a, u32 b, const char *str) {
    Tag x;
    LobbyString_ReplaceRange(s, a, b, str, str + strlen(str), x);
}

int LobbyString_CompareSubstring(const u8 *s, u32 a, u32 b, const u8 *input) {
    const Str *o = (const Str *)input;
    u32 n;
    const char *p;
    if (StrIsShort(o)) {
        p = o->s.buf;
        n = o->s.len;
    } else {
        p = o->l.ptr;
        n = o->l.size;
    }
    return LobbyString_CompareChars(s, a, b, p, n);
}
