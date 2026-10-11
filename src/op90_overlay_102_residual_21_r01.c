#pragma force_active on
#include "global.h"
typedef struct { u16 x, y; } Position;
void *EasyChatApp_GetMessageString(void *, int);
void ov102_021EAC44(void *, void *);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void ov102_021EAD70(void *, void *);
void ov102_021EAD98(void *, void *, void *, u32);
void ov102_021EAC70(void *);
void String_Delete(void *);
u32 ov102_021EAC7C(void *, void *);
u32 EasyChatApp_GetWord(void *, int);
int FontID_String_GetWidth(int, void *, int);
int ov102_021EAB30(u8 *s, void *win);
// @fn ov102_021EAB30
int ov102_021EAB30(u8 *s, void *win) {
    void *str;
    int x, y;
    int count, active;
    u8 *writer;
    Position *pos;
    u32 icon;
    str = EasyChatApp_GetMessageString(*(void **)(s + 4), 35);
    count = 0;
    y = 0;
    active = 1;
    x = 0;
    ov102_021EAC44(s + 0x94, s);
    writer = s;
    pos = (Position *)(s + 0x84);
    do {
        switch (ov102_021EAC7C(s + 0x94, str)) {
        case 0:
            AddTextPrinterParameterizedWithColor(win, 1, str, x, y, 255, 0x3040d, NULL);
            x += FontID_String_GetWidth(1, str, 0);
            break;
        case 1:
            ((Position *)(writer + 0x84))->x = x + 50;
            ((Position *)(writer + 0x84))->y = y + 8;
            ov102_021EAD70(win, pos);
            icon = EasyChatApp_GetWord(*(void **)(s + 4), count);
            if (icon != 0xFFFF) {
                ov102_021EAD98(s, win, pos, icon);
            }
            writer += 4;
            pos++;
            count++;
            x += 100;
            break;
        case 2:
            y += 16;
            x = 0;
            break;
        case 3:
            active = 0;
            break;
        }
    } while (active);
    ov102_021EAC70(s + 0x94);
    String_Delete(str);
    return count;
}
