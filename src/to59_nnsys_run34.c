#include <nitro.h>
#include <nitro/code32.h>
#include <nnsys/g2d/g2d_MultiCellAnimation.h>
#include <nnsys/g2d/load/g2d_NAN_load.h>
extern const NNSG2dMultiCellData *NNS_G2dGetMultiCellDataByIdx(const NNSG2dMultiCellDataBank *, u16);
extern void NNS_G2dBindAnimCtrl(NNSG2dAnimController *, const NNSG2dAnimSequence *);
extern const void *NNS_G2dGetAnimCtrlCurrentElement(NNSG2dAnimController *);
extern void SetMCDataToMCInstanceImpl_(NNSG2dMultiCellInstance *, const NNSG2dMultiCellData *, u16);
extern void NNSi_G2dSrtcInitControl(NNSG2dSRTControl *, u32);
extern void NNSi_G2dSrtcSetTrans(NNSG2dSRTControl *, s16, s16);
extern void NNSi_G2dSrtcSetSRTScale(NNSG2dSRTControl *, fx32, fx32);
extern void NNSi_G2dSrtcSetSRTRotZ(NNSG2dSRTControl *, u16);
extern BOOL NNS_G2dSetAnimCtrlCurrentFrame(NNSG2dAnimController *, u16);
extern BOOL NNS_G2dTickAnimCtrl(NNSG2dAnimController *, fx32);
extern void NNS_G2dTickMCInstance(NNSG2dMultiCellInstance *, fx32);
typedef struct MCAnimTrans {
    u16 index;
    u16 dummy;
    s16 x;
    s16 y;
} MCAnimTrans;
typedef struct MCAnimSRT {
    u16 index;
    u16 rot;
    fx32 sx;
    fx32 sy;
    s16 x;
    s16 y;
} MCAnimSRT;

void NNS_G2dSetAnimSequenceToMCAnimation(NNSG2dMultiCellAnimation *anim, const NNSG2dMultiCellAnimSequence *seq) {
    NNS_G2dBindAnimCtrl(&anim->animCtrl, seq);
    anim->totalVideoFrame = 0;
    if (anim->animCtrl.pActiveCurrent->frames == 0) {
        return;
    }
    const void *frame = NNS_G2dGetAnimCtrlCurrentElement(&anim->animCtrl);
    u32 type;
    const NNSG2dMultiCellData *data = NNS_G2dGetMultiCellDataByIdx(anim->pMultiCellDataBank, *(u16 *)frame);
    type = anim->animCtrl.pAnimSequence->animType & 0xff;
    NNSi_G2dSrtcInitControl(&anim->srtCtrl, 1);
    if (type != 0) {
        if (type == 2) {
            NNSi_G2dSrtcSetTrans(&anim->srtCtrl, ((MCAnimTrans *)frame)->x, ((MCAnimTrans *)frame)->y);
        } else {
            NNSi_G2dSrtcSetSRTScale(&anim->srtCtrl, ((MCAnimSRT *)frame)->sx, ((MCAnimSRT *)frame)->sy);
            NNSi_G2dSrtcSetSRTRotZ(&anim->srtCtrl, ((MCAnimSRT *)frame)->rot);
            NNSi_G2dSrtcSetTrans(&anim->srtCtrl, ((MCAnimSRT *)frame)->x, ((MCAnimSRT *)frame)->y);
        }
    }
    SetMCDataToMCInstanceImpl_(&anim->multiCellInstance, data, anim->totalVideoFrame);
}

u16 NNS_G2dGetMCBankNumNodesRequired(const NNSG2dMultiCellDataBank *bank) {
    u16 max = 0;
    u16 i = 0;
    if (bank->numMultiCellData > i) {
        do {
            const NNSG2dMultiCellData *data = NNS_G2dGetMultiCellDataByIdx(bank, i);
            if (data->numNodes > max) {
                max = data->numNodes;
            }
            i++;
        } while (i < bank->numMultiCellData);
    }
    return max;
}
