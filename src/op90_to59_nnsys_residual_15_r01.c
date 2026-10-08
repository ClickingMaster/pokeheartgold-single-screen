#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct AnimFrame { void *content; u16 frames; u16 pad; } AnimFrame;
typedef struct AnimSeq { u16 numFrames; u16 loopStart; u32 type; u32 mode; AnimFrame *frames; } AnimSeq;
typedef struct Attr { u32 v; } Attr;
typedef struct AttrSeq { u16 num; u16 pad; void *p4; Attr *attrs; } AttrSeq;
typedef struct AttrBank { u16 num; u16 pad; AttrSeq *seqs; } AttrBank;
typedef struct ExData { u32 a, b; AttrBank bank; } ExData;
typedef struct AnimBank { u16 numSeq; u16 numTotalFrames; AnimSeq *seqs; AnimFrame *frames; void *content; u32 pad; ExData *ex; } AnimBank;
void NNS_G2dUnpackNAN(AnimBank *bank);
static inline void UnpackAttrSeq_(AttrSeq *s, u32 base) {
    u32 m;
    for (m = 0; m < s->num; m++) {
        s->attrs[m].v += base;
    }
}
// @fn NNS_G2dUnpackNAN
void NNS_G2dUnpackNAN(AnimBank *bank) {
    bank->seqs = (AnimSeq *)((u32)bank->seqs + (u32)bank);
    bank->frames = (AnimFrame *)((u32)bank->frames + (u32)bank);
    bank->content = (void *)((u32)bank->content + (u32)bank);
    {
        void *content; u16 i; u16 j; AnimFrame *frames; AnimSeq *seqs;
        content = bank->content;
seqs = bank->seqs;
frames = bank->frames;
        for (i = 0; i < bank->numSeq; i++) {
            seqs[i].frames = (AnimFrame *)((u32)seqs[i].frames + (u32)frames);
            for (j = 0; j < seqs[i].numFrames; j++) {
                seqs[i].frames[j].content = (void *)((u32)content + (u32)seqs[i].frames[j].content);
            }
        }
    }
    if (bank->ex) {
        AttrBank *ab;
        u32 k;
        bank->ex = (ExData *)((u32)bank->ex + (u32)bank);
        ab = &bank->ex->bank;
        ab->seqs = (AttrSeq *)((u32)ab->seqs + (u32)ab);
        for (k = 0; k < ab->num; k++) {
            u32 m;
            AttrSeq *s = &ab->seqs[k];
            s->p4 = (void *)((u32)s->p4 + (u32)ab);
            s->attrs = (Attr *)((u32)s->attrs + (u32)ab);
            for (m = 0; m < s->num; m++) {
                *(u32 *)((u8 *)s->attrs + m * sizeof(Attr)) += (u32)ab;
            }
        }
    }
}
