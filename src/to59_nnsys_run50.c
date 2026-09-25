typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define B(p, n) (*(u8 *)((u8 *)(p) + (n)))

typedef struct ResDict {
    u8 revision;
    u8 numEntry;
    u16 size;
    u16 dummy;
    u16 ofsEntry;
} ResDict;
typedef struct EntryHeader {
    u16 sizeUnit;
    u16 ofsName;
    u8 data[4];
} EntryHeader;
static inline void *GetResDataByIdx(ResDict *dict, u32 idx) {
    EntryHeader *hdr;
    if (dict != 0 && idx < dict->numEntry) {
        hdr = (EntryHeader *)((u8 *)dict + dict->ofsEntry);
        return (void *)(&hdr->data[0] + hdr->sizeUnit * idx);
    } else {
        return 0;
    }
}
static inline u8 *GetMatEntry(u8 *resource, u32 idx) {
    u8 *entry;
    if (resource != 0) {
        entry = GetResDataByIdx((ResDict *)(resource + 4), idx);
        if (entry != 0) {
            return resource + W(entry, 0);
        }
    }
    return 0;
}
static inline u8 *GetShapeEntry(u8 *resource, u32 idx) {
    u8 *entry;
    if (resource != 0) {
        entry = GetResDataByIdx((ResDict *)resource, idx);
        if (entry != 0) {
            return resource + W(entry, 0);
        }
    }
    return 0;
}

void NNS_G3dGeSendDL(void *, u32);
void NNSi_G3dFuncSbc_SHP_InternalDefault(void *, u32, void *);
void NNSi_G3dFuncSbc_SHP(void *, u32);
void NNSi_G3dFuncSbc_MAT(void *, u32);

void NNSi_G3dFuncSbc_MAT(void *ctx, u32 command) {
    u32 flags = W(ctx, 8);
    if (!(flags & 0x200)) {
        u8 *sbc = (u8 *)W(ctx, 0);
        u8 idx = sbc[1];
        if ((flags & 1) || !(flags & 8) || idx != B(ctx, 0xad)) {
            u8 *resource = (u8 *)W(ctx, 0xd8);
            u8 *entry = GetMatEntry(resource, idx);
            {
                extern void (*_02110AAC[])(void *, u32, void *, u32);
                u16 kind = H(entry, 0);
                void (*callback)(void *, u32, void *, u32) = _02110AAC[kind];
                callback(ctx, command, entry, idx);
            }
        }
    }
    W(ctx, 0) += 2;
}

void NNSi_G3dFuncSbc_SHP_InternalDefault(void *ctx, u32 unused, void *shape) {
    void (*cb)(void *, void *);
    u32 stage;
    u32 handled;
    if (W(ctx, 0x20) != 0) {
        stage = B(ctx, 0x91);
    } else {
        stage = 0;
    }
    if (stage == 1) {
        W(ctx, 8) &= ~0x40;
        cb = (void (*)(void *, void *))W(ctx, 0x20);
        cb(ctx, cb);
        cb = (void (*)(void *, void *))W(ctx, 0x20);
        if (cb != 0) {
            stage = B(ctx, 0x91);
        } else {
            stage = 0;
        }
        handled = W(ctx, 8) & 0x40;
    } else {
        handled = 0;
    }
    if (!handled && !(W(ctx, 8) & 0x100)) {
        NNS_G3dGeSendDL((u8 *)shape + W(shape, 8), W(shape, 0xc));
    }
    if (stage == 2) {
        W(ctx, 8) &= ~0x40;
        cb = (void (*)(void *, void *))W(ctx, 0x20);
        cb(ctx, cb);
        cb = (void (*)(void *, void *))W(ctx, 0x20);
        if (cb != 0) {
            stage = B(ctx, 0x91);
        } else {
            stage = 0;
        }
    }
    if (stage == 3) {
        W(ctx, 8) &= ~0x40;
        cb = (void (*)(void *, void *))W(ctx, 0x20);
        cb(ctx, cb);
    }
}

void NNSi_G3dFuncSbc_SHP(void *ctx, u32 command) {
    u32 flags = W(ctx, 8);
    if (!(flags & 0x200) && (flags & 1) && !(flags & 2)) {
        u8 *resource;
        u8 *sbc = (u8 *)W(ctx, 0);
        u8 idx = sbc[1];
        resource = (u8 *)W(ctx, 0xdc);
        u8 *entry = GetShapeEntry(resource, idx);
        {
            extern void (*_02110ABC[])(void *, u32, void *, u32);
            u16 kind = H(entry, 0);
            void (*callback)(void *, u32, void *, u32) = _02110ABC[kind];
            callback(ctx, command, entry, idx);
        }
    }
    W(ctx, 0) += 2;
}
