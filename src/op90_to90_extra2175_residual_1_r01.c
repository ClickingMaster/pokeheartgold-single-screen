#pragma force_active on
#include "global.h"
typedef struct {s32 stride,rows;} GridDimensions;
typedef struct Context {u8 prefix[0xad];u8 direction;u8 reserved[0x16];GridDimensions grid;} Context;
void ov01_021F55F4(int,int,int,int,Context*);int ov01_021F4728(int,int,int);int ov01_021F4704(int,int,int);
void ov01_021F54AC(int,int,u8,u8,u8,Context*);
void ov01_021F562C(int,u8,Context*);

// @fn ov01_021F562C
void ov01_021F562C(int pos, u8 kind, Context *c) {
    u32 primary;
    int left;
    int right;
    unsigned int secondary;
    switch (kind) {
    case 4:
        ov01_021F55F4(2, 3, 0, 1, c);
        if (c->direction == 2) {
            left = pos - c->grid.stride;
            primary = 1;
            secondary = 0;
            right = left - 1;
            if (left < 0) {
                left = -1;
                right = -1;
            } else if (right < 0 || !ov01_021F4728(left, right, c->grid.stride)) {
                right = -1;
            }
        } else if (c->direction == 3) {
            left = pos - c->grid.stride;
            right = left + 1;
            primary = 0;
            secondary = 1;
            if (left < 0) {
                left = -1;
                right = -1;
            } else if (!ov01_021F4728(left, right, c->grid.stride)) {
                right = -1;
            }
        } else {
            GF_AssertFail();
        }
        ov01_021F54AC(left, right, primary, secondary, kind, c);
        return;
    case 3:
        ov01_021F55F4(1, 3, 0, 2, c);
        if (c->direction == 1) {
            left = pos - 1;
            right = pos - c->grid.stride - 1;
            primary = 2;
            secondary = 0;
            if (left < 0 || !ov01_021F4728(left, pos, c->grid.stride)) {
                left = -1;
                right = left;
            }
            if (right < 0 || !ov01_021F4704(left, right, c->grid.stride)) {
                right = -1;
            }
        } else if (c->direction == 3) {
            left = pos - 1;
            right = pos + c->grid.stride - 1;
            primary = 0;
            secondary = 2;
            if (left < 0 || !ov01_021F4728(left, pos, c->grid.stride)) {
                left = -1;
                right = left;
            }
            if (c->grid.stride * c->grid.rows <= right || !ov01_021F4704(left, right, c->grid.stride)) {
                right = -1;
            }
        } else {
            GF_AssertFail();
        }
        ov01_021F54AC(left, right, primary, secondary, kind, c);
        return;
    case 1:
        ov01_021F55F4(0, 2, 1, 3, c);
        if (c->direction == 0) {
            left = pos + 1;
            right = pos - c->grid.stride + 1;
            primary = 3;
            secondary = 1;
            if (c->grid.stride * c->grid.rows <= left || !ov01_021F4728(left, pos, c->grid.stride)) {
                right = left = -1;
            }
            if (c->grid.stride * c->grid.rows <= right || !ov01_021F4704(left, right, c->grid.stride)) {
                right = -1;
            }
        } else if (c->direction == 2) {
            left = pos + 1;
            right = pos + c->grid.stride + 1;
            primary = 1;
            secondary = 3;
            if (c->grid.stride * c->grid.rows <= left || !ov01_021F4728(left, pos, c->grid.stride)) {
                right = left = -1;
            }
            if (right < 0 || !ov01_021F4704(left, right, c->grid.stride)) {
                right = -1;
            }
        }
        ov01_021F54AC(left, right, primary, secondary, kind, c);
        return;
    case 2:
        ov01_021F55F4(0, 1, 2, 3, c);
        if (c->direction == 0) {
            left = pos + c->grid.stride;
            right = left - 1;
            primary = 3;
            secondary = 2;
            if (c->grid.stride * c->grid.rows <= left) {
                left = -1;
                right = -1;
            } else if (!ov01_021F4728(left, right, c->grid.stride)) {
                right = -1;
            }
        } else if (c->direction == 1) {
            left = pos + c->grid.stride;
            right = left + 1;
            primary = 2;
            secondary = 3;
            if (c->grid.stride * c->grid.rows <= left) {
                right = left = -1;
            } else if (c->grid.stride * c->grid.rows <= right || !ov01_021F4728(left, right, c->grid.stride)) {
                right = -1;
            }
        } else {
            GF_AssertFail();
        }
        ov01_021F54AC(left, right, primary, secondary, kind, c);
        return;
    case 0:
    default:
        GF_AssertFail();
        return;
    }
}
