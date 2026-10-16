#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

void PPWLobbyPlayer_UpdateState(Session *s) {
    switch (s->state) {
    case 0:
        s->state = (s->systemProfileReceived != 0 && s->userProfileReceived != 0) ? 1 : 0;
        if (s->state == 0) {
            return;
        }
    case 1:
        if (s->unk10 != 0) {
            s->state = 2;
        }
        return;
    case 2:
        if (s->entered != 0) {
            s->state = 3;
        }
        return;
    case 3:
        return;
    }
}

BOOL PPWLobbyPlayers_Add(SessionMgr *m, int id) {
    RTCDate date;
    RTCTime time;
    Session *s;
    volatile InsertResult res2;
    InsertResult res;
    volatile Tag t1;
    volatile Tag t0;
    Session *n = LobbyMem_Alloc(sizeof(Session));
    if (n != NULL) {
        n->vt = &gPPWLobbyPlayerVtable;
        n->channel = 5;
        n->time = 0;
        n->unk10 = 0;
        DWC_GetDateTime(&date, &time);
        n->time = RTC_ConvertDateTimeToSecond(&date, &time);
        n->systemProfileChanged = 1;
        n->userProfileData = 0;
        n->userProfileSize = 0;
        n->userProfileCapacity = 0;
        n->userProfileChanged = 1;
        n->state = 0;
        n->id = id;
        n->systemProfileReceived = 0;
        n->userProfileReceived = 0;
        n->entered = 0;
    }
    s = n;
    PPWLobbyPlayerSet_Insert(&res, &m->list, &s);
    t1.c = 0;
    res2.current = res.current;
    t0.c = 0;
    res2.inserted = res.inserted;
    if ((res.inserted & 0xFF) == 0) {
        if (s != NULL) {
            s->vt->dtor((VObj *)s);
        }
        return TRUE;
    }
    return TRUE;
}

BOOL PPWLobbyPlayers_Remove(SessionMgr *m, int id) {
    NodeIter it;
    Node *hdr = (Node *)&m->list.root;
    it.current = m->list.leftmost;
    while (it.current != hdr) {
        Session *s = ((Node *)it.current)->val;
        if (id == s->id) {
            NodeIter tmp;
            if (s != NULL) {
                s->vt->dtor((VObj *)s);
            }
            PPWLobbyPlayerSet_Erase(&tmp, &m->list, it);
            return TRUE;
        }
        ov45_02236E58(&it);
    }
    return FALSE;
}

Session *PPWLobbyPlayers_Find(SessionMgr *m, int id) {
    volatile FindState a;
    volatile FindState b;
    Node *r;
    BOOL e;
    Node *hdr;
    Node *first;
    if (id == m->self.id) {
        return &m->self;
    }
    first = m->list.leftmost;
    hdr = (Node *)&m->list.root;
    a.id = id;
    b.id = id;
    a.end = hdr;
    b.end = hdr;
    a.it.current = first;
    b.it.current = first;
    while (((NodeIter *)&b.it)->current != hdr && id != ((Node *)((NodeIter *)&b.it)->current)->val->id) {
        ov45_02236E58((NodeIter *)&b.it);
    }
    e = ((NodeIter *)&b.it)->current == (Node *)&m->list.root;
    r = b.it.current;
    if (e) {
        return NULL;
    }
    return r->val;
}
