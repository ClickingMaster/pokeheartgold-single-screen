#include "to50_overlay108_r13_private.h"

void ov108_021E7ADC(SafariAreaCustomizer *work) {
    int group;
    int row;
    int column;
    int tile = 1;
    int x;
    u8 y;

    group = 0;
    while (group < 6) {
        row = 0;
        x = (u8)((group / 3) * 9 + 3);
        y = (group % 3) * 9 + 3;
        while (row < 8) {
            u8 xPos = x;
            column = 0;
            while (column < 8) {
                FillBgTilemapRect(work->bgConfig, 5, tile++, (u8)(y + column), xPos, 1, 1, 0x11);
                column++;
            }
            x++;
            row++;
        }
        group++;
    }
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 5);
}
