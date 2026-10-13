#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "overlay45_format_buffer_private.h"
#pragma force_active on

typedef struct {
    u32 words[4];

    struct {
        u8 bytes[32];
    } a;

    struct {
        u8 bytes[20];
    } b;
} Packet;

typedef struct {
    int state;
    u8 p[0x68 - 4];
    int active;
    u8 q[8];
    Packet packet;
    u8 r[0x134 - 0xb8];
} Work;

extern Work *volatile _022577C4;

static inline int release(Work *w) {
    if (w->active) {
        w->active = 0;
        return 1;
    }
    return 0;
}

extern void PPWLobby_SendMessage(int, int, int, int, int, void *, int);
extern void LobbyTimerMap_Find(void *, void *, void *);
extern void LobbyTimerMap_Erase(void *, void *, void *);
char *LobbyCharVector_Data(Vec *);
int PPW_LobbyUpdateRecruitInfo(Packet *);
int PPW_LobbyStopRecruit(void);
int LobbyTimerMap_EraseKey(void *, void *);

char *LobbyCharVector_Data(Vec *p) {
    if (p->size == 0) {
        return 0;
    }
    return p->ptr;
}

int PPW_LobbyUpdateRecruitInfo(Packet *p) {
    Work *w = _022577C4;
    int result;
    if (!w) {
        return 4;
    }
    if (w && w->state == 10) {
        return 7;
    }
    if (w && _022577C4->state != 8) {
        return 4;
    }
    w = _022577C4;
    if (!w->active) {
        result = 0;
    } else {
        w->packet.words[0] = p->words[0];
        w->packet.words[1] = p->words[1];
        w->packet.words[2] = p->words[2];
        w->packet.words[3] = p->words[3];
        w->packet.a = p->a;
        w->packet.b = p->b;
        result = 1;
    }
    if (!result) {
        return 4;
    }
    return 0;
}

int PPW_LobbyStopRecruit(void) {
    int value;
    Work *w = _022577C4;
    int result;
    if (!w) {
        return 4;
    }
    if (w && w->state == 10) {
        return 7;
    }
    if (w && _022577C4->state != 8) {
        return 4;
    }
    w = _022577C4;
    result = release(w);
    if (!result) {
        return 4;
    }
    w = _022577C4;
    PPWLobby_SendMessage(*(int *)((u8 *)w + 0x4c), 0, 1, 0, 2, &w->packet, 0x44);
    value = 3;
    LobbyTimerMap_EraseKey((u8 *)_022577C4 + 0x134, &value);
    return 0;
}

int LobbyTimerMap_EraseKey(void *w, void *value) {
    struct {
        void *argument, *out, *found;
    } local;

    LobbyTimerMap_Find(&local.found, w, value);
    if (local.found == (u8 *)w + 4) {
        return 0;
    }
    local.argument = local.found;
    LobbyTimerMap_Erase(&local.out, w, local.argument);
    return 1;
}
