#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
int LobbyByteVector_Data(int *);

typedef union {
    struct {
        u32 flag : 1;
        u32 rest : 31;
        u8 raw[8];
    };

    char bytes[12];
} String;

typedef struct {
    int mode;
    String text;
    int a, b;
} Result;

typedef struct {
    int state, error;
    u8 p[0xd0 - 8];
    int mode;
    String text;
    int a, b;
} Work;

extern Work *volatile _022577C4;

static inline int badMode(Work *w) {
    if (w) {
        if (w->mode == 5) {
            return 0;
        }
        return 1;
    }
    return 0;
}

extern u8 ov45_02254D5C[];
extern u8 ov45_02254D1C[];
extern u8 ov45_02254D3C[];
extern void PPWLobby_OnPlayerJoined(void);
extern void PPWLobby_OnPlayerLeft(void);
extern void PPWLobby_OnMessage(void);
extern void PPWLobby_OnChannelDataChanged(void);
extern void PPWLobby_OnTopicChanged(void);
extern void PPWLobby_OnEnterSubChannel(void);
extern void PPWLobby_GetChannelBaseName(String *);
extern void PPWLobby_MakeChannelName(String *, int, String *);
extern void LobbyString_Destroy(String *);
extern void LobbyString_InitCopy(String *, String *);
extern void LobbyString_Assign(String *, String *);
extern int LobbyChat_EnterChannelAsync(char *, int, void *, void *, void (*)(void), int);

static inline void initResult(Result *r, int mode, String *s) {
    r->mode = mode;
    LobbyString_InitCopy(&r->text, s);
    r->a = 0;
    r->b = 0;
}

int PPW_LobbyJoinSubChannelAsync(int);

int LobbyByteVector_Data(int *p) {
    if (p[1] == 0) {
        return 0;
    }
    return p[0];
}
