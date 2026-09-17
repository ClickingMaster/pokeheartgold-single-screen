#ifndef SOL_R40_NIGHT_TRAINER_CARD_MAIN_18_PRIVATE_H
#define SOL_R40_NIGHT_TRAINER_CARD_MAIN_18_PRIVATE_H
#include "global.h"
typedef struct TrainerCardMainState {
    u8 data[0x4000];
} TrainerCardMainState;
int FontID_String_GetWidth(int, void *, int);
void FillWindowPixelBuffer(void *, int);
int AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
void ov51_021E7D44(TrainerCardMainState *, int, int, int);
void ov51_021E76A4(TrainerCardMainState *);
void ov51_021E76EC(TrainerCardMainState *, int);
#endif
