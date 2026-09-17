#ifndef POKEHEARTGOLD_R40_UNK_02088B08_PRIVATE_H
#define POKEHEARTGOLD_R40_UNK_02088B08_PRIVATE_H

#include "global.h"

int IsPaletteFadeFinished(void);
void sub_0208E444(void *data);
void sub_02089C50(void *data);
void sub_02089E30(void *data, int direction);
int sub_02089E98(void *data);
void sub_0208A2C0(void *data, int direction);
int sub_0208A2E0(void *data, int direction);
void sub_0208ADB8(void *data, int state);
int sub_0208ADCC(void *data);
int sub_0208AEB4(void *data);
int sub_0208AEC4(void *data);
int sub_0208B044(void *data, u8 state);
int sub_0208B0B0(void *data, int direction, u8 state);
BOOL PlaySE(u16 sequence);
extern u8 gSystem[];
int sub_02088B08(void *data);
int sub_02088B40(void *data);

typedef struct SummaryInputWork {
    u8 padding[0x7BF];
    u8 lowNibble : 4;
    u8 highNibble : 4;
} SummaryInputWork;

#endif
