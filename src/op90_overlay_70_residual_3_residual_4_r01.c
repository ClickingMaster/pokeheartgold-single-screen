#pragma force_active on
#include "global.h"
typedef struct Packet {
    u32 low;
    u32 high;
} Packet;
extern Packet ov70_0224680C;
extern Packet ov70_02246814;
extern int _02246800[2];
extern u8 ov70_02246304[];
extern u8 ov70_0224693C[];
void GameStats_Start(void);
void GameStats_Shutdown(void);
BOOL ov70_02238360(void *a0, Packet *packet, int size, void *a3, int a4);
void ov70_0223826C(void);

#pragma opt_propagation off
// @fn ov70_0223826C
void ov70_0223826C(void) {
    Packet *src = &ov70_0224680C;
    Packet *d = &ov70_02246814;
    *d = *src;
    GameStats_Start();
    if (ov70_02238360(ov70_02246304, &ov70_02246814, sizeof(Packet), ov70_0224693C, 2)) {
        _02246800[0] = 18;
    } else {
        _02246800[0] = 24;
        _02246800[1] = -13;
        GameStats_Shutdown();
    }
}
#pragma opt_propagation reset
