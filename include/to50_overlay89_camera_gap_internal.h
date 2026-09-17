#ifndef POKEHEARTGOLD_TO50_OVERLAY89_CAMERA_GAP_INTERNAL_H
#define POKEHEARTGOLD_TO50_OVERLAY89_CAMERA_GAP_INTERNAL_H

#include "camera.h"

int GF_SinDegFX32(int angle);
void PlaySE(int se);

int ov89_0225C090(void *unused, u8 *work, Camera *camera);
int ov89_0225C168(void *unused, u8 *work, Camera *camera);
int ov89_0225C240(void *unused, u8 *work, Camera *camera);
int ov89_0225C31C(void *unused, u8 *work, Camera *camera);

#endif
