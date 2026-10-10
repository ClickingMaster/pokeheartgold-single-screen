#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nnsys/g2d/g2d_MultiCellAnimation.h>
#pragma force_active on

typedef struct SharedCell {
    NNSG2dCellAnimation animation;
    u32 initialized;
} SharedCell;

typedef char CellSize[(sizeof(SharedCell) == 0x5c) ? 1 : -1];
typedef char NodeSize[(sizeof(NNSG2dNode) == 0x28) ? 1 : -1];

static inline void TickShared(NNSG2dMultiCellInstance *mc, fx32 delta) {
    const NNSG2dMultiCellData *def = mc->pCurrentMultiCell;
    SharedCell *cells = mc->pCellAnimInstasnces;
    u16 i = 0;
    while (i < def->numCellAnim) {
        NNS_G2dTickCellAnimation(&cells[i].animation, delta);
        def = mc->pCurrentMultiCell;
        i++;
    }
}

static inline void TickNodes(NNSG2dMultiCellInstance *mc, fx32 delta) {
    const NNSG2dMultiCellData *def = mc->pCurrentMultiCell;
    NNSG2dNode *nodes = mc->pCellAnimInstasnces;
    u16 i = 0;
    while (i < def->numNodes) {
        NNS_G2dTickCellAnimation(nodes[i].pContent, delta);
        def = mc->pCurrentMultiCell;
        i++;
    }
}

void NNS_G2dTickMCInstance(NNSG2dMultiCellInstance *, fx32);

void NNS_G2dTickMCInstance(NNSG2dMultiCellInstance *mc, fx32 delta) {
    if (mc->mcType == NNS_G2D_MCTYPE_SHARE_CELLANIM) {
        TickShared(mc, delta);
    } else {
        TickNodes(mc, delta);
    }
}
