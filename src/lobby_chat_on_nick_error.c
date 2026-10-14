#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef union {
    struct {
        u32 flag : 1;
        u32 rest : 31;
        u8 raw[8];
    };

    char bytes[12];
} String;

typedef struct {
    int a, state;
    void *source;
    String text;
} Work;

static inline char *getString(String *s) {
    char *p;
    if (s->flag == 0) {
        p = &s->bytes[1];
    } else {
        p = *(char **)&s->raw[4];
    }
    return p;
}

extern Work *volatile gLobbyChatManager;
extern char sStr_Empty_17[];
extern void LobbyChat_FormatNick(String *, void *);
extern void LobbyString_Assign(String *, String *);
extern void LobbyString_Destroy(String *);
extern void LobbyChatManager_GetNick(String *, Work *);
extern void chatRetryWithNickA(void *, char *);
extern void chatRegisterUniqueNickA(void *, int, char *, char *);
void LobbyChat_OnNickError(void *, int);

void LobbyChat_OnNickError(void *out, int state) {
    String a, b, c, d, e, f, g, h;
    Work *w = gLobbyChatManager;
    if (w && w->state == 5) {
        return;
    }
    if (state == 1) {
        LobbyChat_FormatNick(&a, gLobbyChatManager->source);
        LobbyString_Assign(&gLobbyChatManager->text, &a);
        LobbyString_Destroy(&a);
        LobbyChatManager_GetNick(&b, gLobbyChatManager);
        chatRetryWithNickA(out, getString(&b));
        LobbyString_Destroy(&b);
    } else if (state == 2) {
        LobbyChat_FormatNick(&c, gLobbyChatManager->source);
        LobbyString_Assign(&gLobbyChatManager->text, &c);
        LobbyString_Destroy(&c);
        LobbyChatManager_GetNick(&d, gLobbyChatManager);
        chatRetryWithNickA(out, getString(&d));
        LobbyString_Destroy(&d);
    } else if ((u32)(state - 3) <= 1) {
        LobbyChat_FormatNick(&e, gLobbyChatManager->source);
        LobbyString_Assign(&gLobbyChatManager->text, &e);
        LobbyString_Destroy(&e);
        LobbyChatManager_GetNick(&f, gLobbyChatManager);
        chatRegisterUniqueNickA(out, 2, getString(&f), sStr_Empty_17);
        LobbyString_Destroy(&f);
    } else if (state == 5) {
        return;
    } else if (state == 6) {
        LobbyChat_FormatNick(&g, gLobbyChatManager->source);
        LobbyString_Assign(&gLobbyChatManager->text, &g);
        LobbyString_Destroy(&g);
        LobbyChatManager_GetNick(&h, gLobbyChatManager);
        chatRetryWithNickA(out, getString(&h));
        LobbyString_Destroy(&h);
    }
}
