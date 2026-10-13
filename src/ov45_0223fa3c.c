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

extern Work *volatile ov45_022577DC;
extern char ov45_02255304[];
extern void ov45_022429E0(String *, void *);
extern void LobbyString_Assign(String *, String *);
extern void LobbyString_Destroy(String *);
extern void ov45_0223FC5C(String *, Work *);
extern void chatRetryWithNickA(void *, char *);
extern void chatRegisterUniqueNickA(void *, int, char *, char *);
void ov45_0223FA3C(void *, int);

void ov45_0223FA3C(void *out, int state) {
    String a, b, c, d, e, f, g, h;
    Work *w = ov45_022577DC;
    if (w && w->state == 5) {
        return;
    }
    if (state == 1) {
        ov45_022429E0(&a, ov45_022577DC->source);
        LobbyString_Assign(&ov45_022577DC->text, &a);
        LobbyString_Destroy(&a);
        ov45_0223FC5C(&b, ov45_022577DC);
        chatRetryWithNickA(out, getString(&b));
        LobbyString_Destroy(&b);
    } else if (state == 2) {
        ov45_022429E0(&c, ov45_022577DC->source);
        LobbyString_Assign(&ov45_022577DC->text, &c);
        LobbyString_Destroy(&c);
        ov45_0223FC5C(&d, ov45_022577DC);
        chatRetryWithNickA(out, getString(&d));
        LobbyString_Destroy(&d);
    } else if ((u32)(state - 3) <= 1) {
        ov45_022429E0(&e, ov45_022577DC->source);
        LobbyString_Assign(&ov45_022577DC->text, &e);
        LobbyString_Destroy(&e);
        ov45_0223FC5C(&f, ov45_022577DC);
        chatRegisterUniqueNickA(out, 2, getString(&f), ov45_02255304);
        LobbyString_Destroy(&f);
    } else if (state == 5) {
        return;
    } else if (state == 6) {
        ov45_022429E0(&g, ov45_022577DC->source);
        LobbyString_Assign(&ov45_022577DC->text, &g);
        LobbyString_Destroy(&g);
        ov45_0223FC5C(&h, ov45_022577DC);
        chatRetryWithNickA(out, getString(&h));
        LobbyString_Destroy(&h);
    }
}
