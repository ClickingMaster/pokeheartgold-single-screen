#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void NNSi_G2dInitializeNode(NNSG2dNode *,NNSG2dNodeType);
void NNS_G2dInitAnimCtrl(NNSG2dAnimController *);
void NNSi_G2dSrtcInitControl(NNSG2dSRTControl *,NNSG2dSRTControlType);
void NNS_G2dInitMCAnimation(NNSG2dMultiCellAnimation *mc,NNSG2dNode *nodes,NNSG2dCellAnimation *cells,u16 count,const NNSG2dCellAnimBankData *bank,const NNSG2dCellDataBank *cellBank,const NNSG2dMultiCellDataBank *mcBank){u16 i=0;mc->multiCellInstance.mcType=NNS_G2D_MCTYPE_DONOT_SHARE_CELLANIM;mc->multiCellInstance.pCurrentMultiCell=NULL;mc->multiCellInstance.pAnimDataBank=bank;mc->multiCellInstance.pCellAnimInstasnces=nodes;for(;i<count;i++){NNSi_G2dInitializeNode(&nodes[i],NNS_G2D_NODETYPE_CELL);nodes[i].pContent=&cells[i];NNS_G2dInitCellAnimation(&cells[i],NNS_G2dGetAnimSequenceByIdx(bank,0),cellBank);}NNS_G2dInitAnimCtrl(&mc->animCtrl);mc->pMultiCellDataBank=mcBank;NNSi_G2dSrtcInitControl(&mc->srtCtrl,NNS_G2D_SRTCONTROLTYPE_SRT);mc->totalVideoFrame=0;}
