#include "global.h"

#include "bg_window.h"

typedef struct Overlay31State {
    u8 data[0x300];
} Overlay31State;

extern void *NewString_ReadMsgData(void *, int);
extern void String_Delete(void *);
extern int ov31_0225E12C(u8 *, int, int);
extern int ov03_02258120(u8 *, int);
extern void ov31_0225DE00(Overlay31State *, Window *, void *, int);
extern void ov31_0225DE24(void *, void *, Window *, int, int);
extern void ov31_0225E0E4(Overlay31State *, int);
void ov31_0225DD14(Overlay31State *);

void ov31_0225DD14(Overlay31State *state) {
    Window *window;
    u8 *work;
    int msgNo;
    void *string;
    int index;
    u8 *savedWork;
    int count;
    int i;

    i = 0;
    window = (Window *)&state->data[0x84];
    for (; i < 6; i++) {
        FillWindowPixelBuffer(window, 0);
        window++;
    }

    count = (*(u8 **)&state->data[0x14])[0x270] - (*(u8 **)&state->data[0x14])[0x271];
    if (count > 6) {
        count = 6;
    } else if (count < 0) {
        count = 0;
    }

    i = 0;
    if (count > 0) {
        window = (Window *)&state->data[0x84];
        do {
            work = *(u8 **)&state->data[0x14];
            index = i + work[0x271];
            msgNo = (*(u16 **)&work[0x268])[index];
            string = NewString_ReadMsgData(*(void **)&state->data[0x15C], msgNo);
            ov31_0225DE00(state, window, string, i);
            String_Delete(string);
            if (ov31_0225E12C(*(u8 **)&state->data[0x14], index, msgNo)) {
                int value;
                savedWork = *(u8 **)&state->data[0x14];
                value = ov03_02258120(savedWork, (u16)msgNo);
                ov31_0225DE24(*(void **)&state->data[0x154], *(void **)&state->data[0x158], window, value, savedWork[0x283]);
            }
            i++;
            window++;
        } while (i < count);
    }

    window = (Window *)state;
    i = 0;
    window = (Window *)((u8 *)window + 0x84);
    for (; i < 6; i++) {
        ScheduleWindowCopyToVram(window);
        window++;
    }
    ov31_0225E0E4(state, count);
}
