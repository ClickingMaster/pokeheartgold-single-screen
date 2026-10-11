#ifndef TO51_R3_W05_PRIVATE_H
#define TO51_R3_W05_PRIVATE_H

#include "global.h"

typedef struct Ov49R59Small {
    u8 pad00[8];
    s16 counter;
    u8 pad0A[2];
    void *object;
} Ov49R59Small;

void WiFiPlazaObj_GetPosition(void *object, VecFx32 *position);
void WiFiPlazaObj_SetPosition(void *object, const VecFx32 *position);
BOOL ov49_022613AC(Ov49R59Small *work);

#endif
