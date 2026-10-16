#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef union {
    struct {
        u32 flag : 1;
        u32 rest : 31;
        u8 raw[8];
    };

    char bytes[12];
} String;

typedef struct {
    void *connection;
    int state;
    int userId;
    String nick;
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
extern void LobbyChat_FormatNick(String *, int);
extern void LobbyString_Assign(String *, String *);
extern void LobbyString_Destroy(String *);
extern void LobbyChatManager_GetNick(String *, Work *);
extern void chatRetryWithNickA(void *, char *);
extern void chatRegisterUniqueNickA(void *, int, char *, char *);
void LobbyChat_OnNickError(void *, int);

// Retry with a new nick for the same user ID; errors 3 and 4 register it as a unique nick instead.
void LobbyChat_OnNickError(void *chat, int type) {
    String a, b, c, d, e, f, g, h;
    Work *manager = gLobbyChatManager;
    if (manager && manager->state == 5) {
        return;
    }
    if (type == 1) {
        LobbyChat_FormatNick(&a, gLobbyChatManager->userId);
        LobbyString_Assign(&gLobbyChatManager->nick, &a);
        LobbyString_Destroy(&a);
        LobbyChatManager_GetNick(&b, gLobbyChatManager);
        chatRetryWithNickA(chat, getString(&b));
        LobbyString_Destroy(&b);
    } else if (type == 2) {
        LobbyChat_FormatNick(&c, gLobbyChatManager->userId);
        LobbyString_Assign(&gLobbyChatManager->nick, &c);
        LobbyString_Destroy(&c);
        LobbyChatManager_GetNick(&d, gLobbyChatManager);
        chatRetryWithNickA(chat, getString(&d));
        LobbyString_Destroy(&d);
    } else if ((u32)(type - 3) <= 1) {
        LobbyChat_FormatNick(&e, gLobbyChatManager->userId);
        LobbyString_Assign(&gLobbyChatManager->nick, &e);
        LobbyString_Destroy(&e);
        LobbyChatManager_GetNick(&f, gLobbyChatManager);
        chatRegisterUniqueNickA(chat, 2, getString(&f), sStr_Empty_17);
        LobbyString_Destroy(&f);
    } else if (type == 5) {
        return;
    } else if (type == 6) {
        LobbyChat_FormatNick(&g, gLobbyChatManager->userId);
        LobbyString_Assign(&gLobbyChatManager->nick, &g);
        LobbyString_Destroy(&g);
        LobbyChatManager_GetNick(&h, gLobbyChatManager);
        chatRetryWithNickA(chat, getString(&h));
        LobbyString_Destroy(&h);
    }
}
