#pragma force_active on
#include "global.h"
typedef struct { u32 data[4]; } Copy;
typedef struct { void *p; u32 a, b; Copy copied; u32 tex, tex4, pltt; } Obj;
typedef struct { u32 key; Obj *obj; } Entry;
typedef struct { u8 pad[4]; u16 count; u8 pad2[0x16]; Entry *entries; } W;
extern u32 GF3dGfxRawResObj_GetTexKey(void *), GF3dGfxRawResObj_GetTex4x4Key(void *), GF3dGfxRawResObj_GetPlttKey(void *);
Obj *ov01_021F1824(W *w, u32 key, void *p, Copy *copy, u32 a, void *resource, u32 b);
// @fn ov01_021F1824
Obj *ov01_021F1824(W *w, u32 key, void *p, Copy *copy, u32 a, void *resource, u32 b) {
    Obj *obj = NULL;
    u32 count = w->count;
    Entry *entries = w->entries;
    u32 n;
    Entry *e;
    n = count;
    e = entries;
    do {
        if (e->key == key) {
            return e->obj;
        }
        e++;
    } while (--n);
    n = count;
    e = entries;
    do {
        if (e->key == 15) {
            e->key = key;
            obj = e->obj;
            break;
        }
        e++;
    } while (--n);
    if (obj == NULL) {
        GF_AssertFail();
    }
    memset(obj, 0, sizeof(Obj));
    obj->p = p;
    obj->copied = *copy;
    obj->a = a;
    if (resource != NULL) {
        obj->tex = GF3dGfxRawResObj_GetTexKey(resource);
        obj->tex4 = GF3dGfxRawResObj_GetTex4x4Key(resource);
        obj->pltt = GF3dGfxRawResObj_GetPlttKey(resource);
    }
    obj->b = b;
    return obj;
}
