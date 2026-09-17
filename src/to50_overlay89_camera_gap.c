#include "global.h"

#include "to50_overlay89_camera_gap_internal.h"

typedef struct Ov89AngleMove {
    CameraAngle angle;
    s32 current;
    s16 timer;
    u8 state;
} Ov89AngleMove;

typedef struct Ov89TargetMove {
    CameraAngle angle;
    fx32 phase;
    u8 count;
    u8 state;
} Ov89TargetMove;

int ov89_0225C090(void *unused, u8 *work, Camera *camera) {
    Ov89AngleMove *move = (Ov89AngleMove *)(work + 0x18);
    CameraAngle adjustment = { 0, 0, 0, 0 };
    CameraAngle current;

    switch (move->state) {
    case 0:
        current = Camera_GetAngle(camera);
        move->angle = current;
        move->current = move->angle.y;
        move->state++;
        PlaySE(0x5D5);
    case 1:
        adjustment.y -= 0x1000;
        move->current -= 0x1000;
        Camera_AdjustAnglePos(&adjustment, camera);
        if (move->current <= move->angle.y - 0x2000) {
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer >= 0x10) {
            move->state++;
        }
        break;
    case 3:
        adjustment.y += 0x200;
        move->current += 0x200;
        Camera_AdjustAnglePos(&adjustment, camera);
        if (move->current >= move->angle.y) {
            Camera_SetAnglePos(&move->angle, camera);
            return 1;
        }
        break;
    }
    return 0;
}

int ov89_0225C168(void *unused, u8 *work, Camera *camera) {
    Ov89AngleMove *move = (Ov89AngleMove *)(work + 0x28);
    CameraAngle adjustment = { 0, 0, 0, 0 };
    CameraAngle current;

    switch (move->state) {
    case 0:
        current = Camera_GetAngle(camera);
        move->angle = current;
        move->current = move->angle.y;
        move->state++;
        PlaySE(0x5D5);
    case 1:
        adjustment.y += 0x1000;
        move->current += 0x1000;
        Camera_AdjustAnglePos(&adjustment, camera);
        if (move->current >= move->angle.y + 0x2000) {
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer >= 0x10) {
            move->state++;
        }
        break;
    case 3:
        adjustment.y -= 0x200;
        move->current -= 0x200;
        Camera_AdjustAnglePos(&adjustment, camera);
        if (move->current <= move->angle.y) {
            Camera_SetAnglePos(&move->angle, camera);
            return 1;
        }
        break;
    }
    return 0;
}

int ov89_0225C240(void *unused, u8 *work, Camera *camera) {
    Ov89AngleMove *move = (Ov89AngleMove *)(work + 0x38);
    CameraAngle adjustment = { 0, 0, 0, 0 };
    CameraAngle current;

    switch (move->state) {
    case 0:
        current = Camera_GetAngle(camera);
        move->angle = current;
        move->current = move->angle.x;
        PlaySE(0x5D5);
        move->state++;
    case 1:
        adjustment.x += 0xAAA;
        move->current += 0xAAA;
        Camera_AdjustAnglePos(&adjustment, camera);
        if (move->current >= move->angle.x + 0x2000) {
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer >= 0x10) {
            move->state++;
        }
        break;
    case 3:
        adjustment.x -= 0x200;
        move->current -= 0x200;
        Camera_AdjustAnglePos(&adjustment, camera);
        if (move->current <= move->angle.x) {
            Camera_SetAnglePos(&move->angle, camera);
            return 1;
        }
        break;
    }
    return 0;
}

int ov89_0225C31C(void *unused, u8 *work, Camera *camera) {
    Ov89TargetMove *move = (Ov89TargetMove *)(work + 0x48);
    CameraAngle adjustment = { 0, 0, 0, 0 };
    CameraAngle current;
    fx32 sine;

    switch (move->state) {
    case 0:
        current = Camera_GetAngle(camera);
        move->angle = current;
        move->state++;
        PlaySE(0x5D5);
    case 1:
        move->phase += 0x18000;
        if (move->phase >= 0x168000) {
            move->phase -= 0x168000;
            move->count++;
            if (move->count >= 2) {
                move->state++;
                move->phase = 0;
            }
        }
        sine = GF_SinDegFX32(move->phase);
        adjustment = move->angle;
        adjustment.y = FX_Mul(sine, FX32_ONE);
        Camera_SetAngleTarget(&adjustment, camera);
        break;
    default:
        return 1;
    }
    return 0;
}
