#ifndef TO50_OV70_4190C_PRIVATE_H
#define TO50_OV70_4190C_PRIVATE_H
#include "global.h"

#include "bg_window.h"
typedef struct Ov70WindowSetupWork {
    BgConfig *bgConfig;
    Window *windows;
    u8 pad08[0x54 - 8];
    u32 bgId;
} Ov70WindowSetupWork;
typedef struct Ov70WindowPair {
    u8 x;
    u8 aux;
} Ov70WindowPair;
typedef struct Ov70WindowTriple {
    u8 x;
    u8 aux;
    u8 unused;
} Ov70WindowTriple;
extern const Ov70WindowPair ov70_02245D60[];
extern const Ov70WindowPair ov70_02245D6E[];
extern const Ov70WindowPair ov70_02245D96[];
extern const Ov70WindowTriple ov70_02245DA2[];
extern const Ov70WindowPair ov70_02245E0E[];
extern const Ov70WindowPair ov70_02245E10[];
void ov70_0224190C(Ov70WindowSetupWork *work, int mode);
#endif
