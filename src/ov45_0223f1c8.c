#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define U(p, n) (*(unsigned *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

typedef union {
    struct {
        u32 flag : 1;
        u32 pad : 31;
        u32 len;
        char *data;
    } b;

    struct {
        char head;
        char inlinebuf[11];
    } s;
} Str;

extern void *ov45_022577DC;
extern char ov45_022552DC[];
extern char ov45_022552E4[];
extern char ov45_022552EC[];
extern char ov45_022552F0[];
extern char ov45_022552FC[];
void LobbyString_InitFromCStr(Str *, char *);
void LobbyString_Destroy(Str *);
void LobbyString_InitCopy(void *, void *);
void ov45_022376BC(void *);
void LobbyTimerMap_EraseKey(void *, int *, int);
void LobbyChatManager_Destroy(void *);
void DWC_Free(int, void *, int);
void LobbyChat_RenameUser(void *, Str *, Str *);
void LobbyChat_RemoveChannel(void *, Str *);
void LobbyString_AssignCStr(Str *, char *);
void *LobbyChat_GetChannelCallbacks(void *, Str *);
void LobbyChat_StripChannelPrefix(Str *, void *, Str *);
void LobbyChat_GetFullChannelName(Str *, void *, Str *);
int chatInChannelA(void *, char *);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void *LobbyMem_Alloc(unsigned);
void *ov45_0223EA34(void *, unsigned, char *);
void ov45_022421E4(void *, unsigned, void *);
char *strncpy(char *, const char *, size_t);
int LobbyChat_FindUserIdByNick(void *, Str *);
void *ov45_0223EC64(void *p);
void ov45_0223EC80(void *p, unsigned n);
void *ov45_0223ECB0(void *p);
void LobbyChat_OnRaw(void);
void LobbyChat_OnInvited(void);
void LobbyChat_OnChannelModeChanged(void);
void LobbyChat_OnUserListUpdated(void);
void LobbyChat_OnDisconnected(void);
void LobbyChat_OnKicked(void *a, char *s);
void LobbyChat_OnUserChangedNick(void *a, void *b, char *c, char *d);
void LobbyChat_OnUserModeChanged(void *a, void *b, void *c, unsigned mode);
void LobbyChat_OnTopicChanged(void *a, char *s, void *b);
void LobbyChat_OnConnected(void *a, int n);
void LobbyChat_OnFillInUser(void *a, void *b, char *out);
void ov45_0223FA2C(void *out, void *p);
void ov45_0223FC5C(void *out, void *p);
void ov45_02240F6C(void *out, void *p);
void ov45_02240F7C(void *out, void *p);
int ov45_0223FD6C(void *p);
void *ov45_0223FD74(void *p);
void ov45_0223FD88(void);
void *ov45_0223FDE8(void *c, void *key, void *node, void *best);
void ov45_0223FD8C(void **out, void *tree, void *key);
void LobbyChat_OnEnumChannelsEach(void *a, int n);
void ov45_0223FFE8(void *p, u32 *first, u32 *last);
int ov45_02240850(void);
int LobbyChat_GetLastError(void);
void *LobbyChat_ToErrorCode(void *p);
int LobbyChat_IsInChannel(char *s);
void LobbyChat_OnChannelMessage(void *a, char *s, char *name, void *arg, int mode);
void LobbyChat_OnBroadcastKeyChanged(void *a, char *s, char *name, void *arg, void *extra);

void LobbyChat_OnInvited(void) {
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
}

void LobbyChat_OnChannelMessage(void *a, char *s, char *name, void *arg, int mode) {
    Str x, y, z, t;
    int id;
    void *p;
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
    if (mode != 3) {
        return;
    }
    LobbyString_InitFromCStr(&x, name);
    id = LobbyChat_FindUserIdByNick(ov45_022577DC, &x);
    LobbyString_Destroy(&x);
    LobbyString_InitFromCStr(&y, s);
    p = LobbyChat_GetChannelCallbacks(ov45_022577DC, &y);
    LobbyString_Destroy(&y);
    LobbyString_InitFromCStr(&z, s);
    LobbyChat_StripChannelPrefix(&t, ov45_022577DC, &z);
    ((void (*)(char *, int, void *, void *))P(p, 8))(!t.b.flag ? t.s.inlinebuf : t.b.data, id, arg, P(p, 20));
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&z);
}

void LobbyChat_OnKicked(void *a, char *s) {
    Str t;
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
    LobbyString_InitFromCStr(&t, s);
    LobbyChat_RemoveChannel(ov45_022577DC, &t);
    LobbyString_Destroy(&t);
}
