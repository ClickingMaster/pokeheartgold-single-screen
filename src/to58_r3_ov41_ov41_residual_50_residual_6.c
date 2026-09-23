#include "global.h"

void ov41_0224A1EC(void *work, int selection, int mode);

void ov41_0224A264(void *);
void ov41_0224A270(void *);
void ov41_0224A258(void *);
void PlaySE(u16);

typedef struct {
    u32 unused;
    u32 selection;
    void *callbackArg;
    void (*callback)(void *, void *);
} Ov41Callback;

void ov41_0224A1DC(Ov41Callback *work, int selection);

void ov41_0224A1DC(Ov41Callback *work, int selection)
{
    if (selection == work->selection) {
        work->callback(work, work->callbackArg);
    }
}

void ov41_0224A1EC(void *work, int selection, int mode)
{
    int i = 2;
    u8 *entry = (u8 *)work + 0x20;
    for (; i <= 3; i++, entry += 0x10) {
        if (i != selection) {
            ov41_0224A264(entry);
        } else if (mode == 0) {
            ov41_0224A270(entry);
            PlaySE(0x67c);
        } else if (mode == 2) {
            ov41_0224A258(entry);
        } else {
            ov41_0224A270(entry);
        }
    }
}
