#ifndef TRAINER_CARD_MAIN_06A_PRIVATE_H
#define TRAINER_CARD_MAIN_06A_PRIVATE_H
#include "global.h"
extern void *gSystem;
extern int System_GetTouchNew(void);
extern int TouchscreenHitbox_TouchNewIsIn(const void *);
extern void PlaySE(int);
extern const u8 _021E7DB8[];
extern const u8 ov51_021E7DBC[];
extern const u8 ov51_021E7DC0[];
int ov51_021E6AAC(u8 *state, int *changed);
#endif
