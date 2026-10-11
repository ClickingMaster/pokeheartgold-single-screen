#include "global.h"
extern void *ov08_02225ADC;
extern int ov08_02223368(void *, void *);
extern int BattleDpadCursor_HandleInput(void *);
extern void ov08_0222417C(void *);
extern void PlaySE(int);
extern int ov08_02223CD4(void *, int);
extern void ov08_02224938(void *, int, int);
extern int ov08_02222B8C(void *);
int ov08_02222AF0(u8 *work);
int ov08_02222AF0(u8 *work) {
    int choice = ov08_02223368(work, &ov08_02225ADC);
    if (choice == -1) {
        choice = BattleDpadCursor_HandleInput(*(void **)(work + 0x34));
        if (choice == -2) choice = 1;
    } else {
        ov08_0222417C(work);
    }
    switch (choice) {
    case 0: {
        PlaySE(0x5DD);
        *(u16 *)(*(u8 **)work + 0x1C) = ov08_02223CD4(work, (*(u8 **)work + work[0x114D])[0x27]);
        (*(u8 **)work)[0x1E] = work[0x114D];
        ov08_02224938(work, 15, 0);
        return ov08_02222B8C(work);
    }
    case 1: {
        PlaySE(0x5DD);
        work[0x114B] = 5;
        ov08_02224938(work, 16, 0);
        return 11;
    }
    default:
        return 3;
    }
}
