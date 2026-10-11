#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Frame control bits (WlFrameCtrlBits) tested as masks.
#define FC_TO_DS     0x100
#define FC_FROM_DS   0x200
#define FC_MORE_FRAG 0x400

// Fragments in a received frame (RX header status bits 4..7).
#define FRAG_COUNT(status) ((int)((status) & 0xF0) / 16)

// Task 9: the next received fragment (WlHeap.bufMan[8]). The first one takes a free reassembly slot and a heap buffer
// for the whole frame; the following ones are copied to their place in it. The last fragment completes the frame,
// which goes on to the management (bufMan[7], task 7) or data (bufMan[5], task 6) frames.
void DefragTask(void) {
    WlDefrag *slot;
    WlRxFrame *frame;
    WlHeap *heap;
    u32 fragCount2;
    int free;
    int fragDiff;
    WlDefrag *slot2;
    WlWork *w;
    u32 total;
    WlRxBuf *frm;
    u32 i;
    WlRxBuf *buf;
    u16 frameCtrl;
    int length2;
    WlDefrag *defrag;
    int fragNo;
    int length;
    u32 fragCount;
    u32 offset;
    u32 i2;
    WlDefrag *defrag2;

    WlDefragKey key;

    heap = &wlWorkPlain->heap;
    frm = (WlRxBuf *)heap->bufMan[8].first;

    if (frm == (WlRxBuf *)0xFFFFFFFF) {
        return;
    }
    if (wlWorkPlain->bss.state != 0x40 || frm->frame.frameLength > 0x5FC) {
        goto done;
    }
    frameCtrl = frm->frame.frameCtrl;
    if (frameCtrl & FC_TO_DS) {
        WSetMacAdrs1(key.addr1, frm->frame.addr3);
        if (frameCtrl & FC_FROM_DS) {
            goto done;
        }
        WSetMacAdrs1(key.addr2, frm->frame.addr2);
    } else {
        WSetMacAdrs1(key.addr1, frm->frame.addr1);
        if (frameCtrl & FC_FROM_DS) {
            WSetMacAdrs1(key.addr2, frm->frame.addr3);
        } else {
            WSetMacAdrs1(key.addr2, frm->frame.addr2);
        }
    }
    *(u16 *)&key.seqCtrl = frm->frame.seqCtrl;
    if ((frameCtrl & FC_MORE_FRAG) && ((WlSeqCtrlBits *)&frm->frame.seqCtrl)->fragNo == 0) {
        // the first fragment: a slot already holding the frame takes the fragments it lacks, else a free slot
        // starts the frame
        free = -1;
        defrag = wlWorkPlain->rxCtrl.defrag;
        for (i = 0; i < 3; i++) {
            slot = &defrag[i];
            if (defrag[i].timer != 0) {
                if (!MatchMacAdrs(slot->key.addr1, key.addr1)) {
                    continue;
                }
                if (!MatchMacAdrs(slot->key.addr2, key.addr2)) {
                    continue;
                }
                if (key.seqCtrl.seqNo != slot->key.seqCtrl.seqNo) {
                    continue;
                }
                fragCount = FRAG_COUNT(frm->frame.status);
                fragDiff = fragCount - slot->key.seqCtrl.fragNo;
                if (fragDiff == 0 || (fragDiff & 0x80000000)) {
                    goto done;
                }
                buf = (WlRxBuf *)defrag[i].buf;
                length = frm->frame.frameLength - buf->frame.frameLength - 24;
                if (length == 0 || (length & 0x80000000)) {
                    goto done;
                }
                __VENEER_MIi_CpuCopy16((u8 *)&frm->frame + sizeof(WlRxFrame) + buf->frame.frameLength, (u8 *)&buf->frame + sizeof(WlRxFrame) + buf->frame.frameLength, length);
                buf->frame.frameLength += length;
                slot->key.seqCtrl.fragNo = fragCount;
                wlWorkPlain->counter[9] += fragCount;
                goto done;
            } else {
                free = i;
            }
        }
        if (free == -1) {
            goto done;
        }
        buf = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 0x622);
        if (buf) {
            slot = &defrag[free];
            __VENEER_MIi_CpuCopy16(&key, &slot->key, sizeof(WlDefragKey));
            defrag[free].timer = 5;
            defrag[free].buf = &buf->header;
            __VENEER_MIi_CpuCopy16(&frm->frame.status, &buf->frame.status, frm->frame.frameLength + 12);
            __VENEER_MI_WaitDma(wlWorkPlain->dmaNo);
            fragCount = FRAG_COUNT(buf->frame.status);
            slot->key.seqCtrl.fragNo = fragCount;
            wlWorkPlain->counter[9] += fragCount;
            buf->frame.frameLength = frm->frame.frameLength - 24;
            defrag[free].fragLength = __VENEER__u32_div_f(buf->frame.frameLength, fragCount);
        } else {
            SetFatalErr(4);
        }
    } else {
        // a following fragment: copy it to the frame of its slot
        w = wlWorkPlain;
        frm->frame.frameLength -= 24;
        defrag2 = w->rxCtrl.defrag;
        for (i2 = 0; i2 < 3; i2++) {
            slot2 = &defrag2[i2];
            if (defrag2[i2].timer == 0) {
                continue;
            }
            if (!MatchMacAdrs(slot2->key.addr1, key.addr1)) {
                continue;
            }
            if (!MatchMacAdrs(slot2->key.addr2, key.addr2)) {
                continue;
            }
            if (key.seqCtrl.seqNo != slot2->key.seqCtrl.seqNo) {
                continue;
            }
            // same sequence number: the difference of the raw sequence controls is the fragment number difference
            // (the slot's key.seqCtrl is addressed by hand: the compiler must load it again here)
            fragNo = *(u16 *)((u8 *)defrag2 + i2 * sizeof(WlDefrag) + 0x10) - *(u16 *)&key.seqCtrl;
            if (fragNo & 0x80000000) {
                goto done;
            }
            offset = fragNo * defrag2[i2].fragLength;
            length2 = frm->frame.frameLength - offset;
            if (length2 == 0) {
                goto done;
            }
            if (!(length2 & 0x80000000)) {
                break;
            }
            goto done;
        }
        if (i2 == 3) {
            goto done;
        }
        frame = &((WlRxBuf *)defrag2[i2].buf)->frame;
        total = frame->frameLength + length2;
        if (total > 0x5E4) {
            ReleaseHeapBuf(&w->heap.bufMan[0], (WlHeapBuf *)((u8 *)frame - 16));
            defrag2[i2].timer = 0;
            goto done;
        }
        __VENEER_MIi_CpuCopy16((u8 *)&frm->frame + sizeof(WlRxFrame) + offset, (u8 *)frame + sizeof(WlRxFrame) + frame->frameLength, length2 + 1);
        frame->frameLength = total;
        fragCount2 = FRAG_COUNT(frm->frame.status);
        defrag2[i2].key.seqCtrl.fragNo += fragCount2 - fragNo;
        wlWorkPlain->counter[9] += fragCount2;
        if (frm->frame.status & 0x100) {
            goto done;
        }
        // the frame is complete
        defrag2[i2].timer = 0;
        frame->status = (frame->status & ~0xF0) + 0x10;
        frame->frameLength += 24;
        switch (frame->status & 0xF) {
        case 8:
            MoveHeapBuf(&w->heap.bufMan[0], &w->heap.bufMan[5], (WlHeapBuf *)((u8 *)frame - 16));
            AddTask(2, 6);
            break;
        case 0:
            MoveHeapBuf(&w->heap.bufMan[0], &w->heap.bufMan[7], (WlHeapBuf *)((u8 *)frame - 16));
            AddTask(1, 7);
            break;
        default:
            ReleaseHeapBuf(&w->heap.bufMan[0], (WlHeapBuf *)((u8 *)frame - 16));
            break;
        }
    }
done:
    ReleaseHeapBuf(&heap->bufMan[8], &frm->header);
    if (heap->bufMan[8].count) {
        AddTask(2, 9);
    }
}
