#pragma force_active on
// Own TU: built with MWCC 2.0/sp1 (per-object MWCCVER override in the Makefile).
#include "global.h"

// A cell whose cellAttr bit 11 is set is followed by its bounding rectangle.
typedef struct CellWithBounds {
    NNSG2dCellData cell;
    s16 maxX;
    s16 maxY;
    s16 minX;
    s16 minY;
} CellWithBounds;

BOOL GF_G2dIsCellVisible(const NNSG2dCellData *cell, const MtxFx32 *mtx, const NNSG2dViewRect *rect);

BOOL GF_G2dIsCellVisible(const NNSG2dCellData *cell_, const MtxFx32 *mtx, const NNSG2dViewRect *rect) {
    const CellWithBounds *cell = (const CellWithBounds *)cell_;
    fx32 dx, dy, y0, y1, x0, x1, tmp, top, bottom, left, right;
    u8 radius;
    const s16 *bounds = &cell->maxX; // maxX, maxY, minX, minY

    radius = (u8)(cell->cell.cellAttr & 0x3F) * 4;
    dx = mtx->_20 - rect->posTopLeft.x;
    dy = mtx->_21 - rect->posTopLeft.y;
    if (((cell->cell.cellAttr >> 11) & 1) == 1) {
        y0 = bounds[3] * FX32_ONE;
        y1 = bounds[1] * FX32_ONE;
        x0 = bounds[2] * FX32_ONE;
        x1 = bounds[0] * FX32_ONE;
    } else {
        y0 = -radius * FX32_ONE;
        y1 = radius * FX32_ONE;
        x0 = y0;
        x1 = y1;
    }
    top = dy + (FX_Mul(y0, mtx->_01) + FX_Mul(y0, mtx->_11));
    bottom = dy + (FX_Mul(y1, mtx->_01) + FX_Mul(y1, mtx->_11));
    left = dx + (FX_Mul(x0, mtx->_00) + FX_Mul(x0, mtx->_10));
    right = dx + (FX_Mul(x1, mtx->_00) + FX_Mul(x1, mtx->_10));
    if (bottom < top) {
        tmp = bottom;
        bottom = top;
        top = tmp;
    }
    if (right < left) {
        tmp = right;
        right = left;
        left = tmp;
    }
    if (bottom > 0 && top < rect->sizeView.y && right > 0 && left < rect->sizeView.x) {
        return TRUE;
    }
    return FALSE;
}
