#include "to58_r2_ov07_mid0_private.h"
void ov07_022227A8(R2Motion *m, int x, int y, int frames, int duration);
int ov07_022227D8(R2Motion *m);

void ov07_022227A8(R2Motion *m, int x, int y, int frames, int duration) {
    if (m == NULL) GF_AssertFail();
    m->duration = frames;
    m->value8 = frames;
    m->valueC = duration;
    m->currentX = 0;
    m->currentY = x;
    m->deltaX = y;
    m->deltaY = -x;
    *(s32 *)((u8 *)m + 0x20) = -y;
    m->x = 0;
    m->y = 0;
}

int ov07_022227D8(R2Motion *m) {
    if (m == NULL) GF_AssertFail();
    if (m->valueC != 0) {
        m->duration++;
        if (m->duration >= m->value8) {
            m->duration = 0;
            int oldX = m->deltaY;
            m->deltaY = m->x;
            if (oldX == 0) m->x = 0;
            else m->x = -oldX;
            int oldY = *(s32 *)((u8 *)m + 0x20);
            *(s32 *)((u8 *)m + 0x20) = m->y;
            if (oldY == 0) m->y = 0;
            else m->y = -oldY;
            m->currentX++;
            if (m->currentX >= 4) {
                m->currentX = 0;
                m->valueC--;
            }
        }
        return 1;
    }
    return 0;
}
