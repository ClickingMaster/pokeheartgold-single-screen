#ifndef TO50_OV59_R10_GAP4_PRIVATE_H
#define TO50_OV59_R10_GAP4_PRIVATE_H
#include "global.h"

#include "bg_window.h"
typedef struct Ov59R10Gap4Work {
    enum HeapID heap;
    u8 raw[0x50];
    BgConfig *bgConfig;
} Ov59R10Gap4Work;
extern const GraphicsModes ov59_0223C978;
extern const BgTemplate ov59_0223C9B0;
extern const BgTemplate ov59_0223C9CC;
extern const BgTemplate ov59_0223C9E8;
extern const BgTemplate ov59_0223CA04;
void ov59_0223B414(void);
void ov59_0223B434(Ov59R10Gap4Work *);
#endif
