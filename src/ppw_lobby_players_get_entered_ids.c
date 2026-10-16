#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

BOOL PPWLobbyPlayers_IsFull(SessionMgr *m) {
    Vec v;
    BOOL made = FALSE;
    BOOL res = FALSE;
    if (m->list.count + 1 == 20) {
        PPWLobbyPlayers_GetEnteredIds(&v, m);
        made = TRUE;
        if (v.size + 1 == 20) {
            res = TRUE;
        }
    }
    if (made) {
        LobbyU32Vector_Destroy(&v);
    }
    return res;
}

void PPWLobbyPlayers_GetEnteredIds(Vec *out, SessionMgr *m) {
    NodeIter it;
    u32 id;
    Node *hdr;
    out->ptr = NULL;
    out->size = 0;
    out->cap = 0;
    VecReserve(out, m->list.count);
    hdr = (Node *)&m->list.root;
    it.current = m->list.leftmost;
    while (it.current != hdr) {
        Session *s = ((Node *)it.current)->val;
        if (s->entered != 0) {
            id = s->id;
            TouchTag();
            if (out->size < out->cap) {
                out->size++;
                ((u32 *)out->ptr)[out->size - 1] = id;
            } else {
                Tag t2 = { 0 };
                LobbyU32Vector_PushBackSlow(out, &id, t2);
            }
        }
        ov45_02236E58(&it);
    }
}
