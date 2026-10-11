#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include <stddef.h>
typedef struct {void*bg;int layer,axis,pos,target,speed;u16 ticks,done;} ScrollWork;
typedef struct {void*owner,*base,*args,*view;Window windows[2];void*sprite0,*sprite1,*sprite2,*text;u8 other40[32];ScrollWork scroll;u8 gap7c[16];u32 y,row,tail;} Work;
typedef char StateSize[sizeof(Work)==0x98?1:-1];
typedef char ScrollOffset[offsetof(Work,scroll)==0x60?1:-1];
typedef char PositionOffset[offsetof(Work,y)==0x8c?1:-1];
extern void ov102_021EC090(u8 *,int);extern u32 EasyChatApp_GetCategoryWordCount(void *);extern void ov102_021EC13C(u8*,int,u32);extern void ov102_021EC298(ScrollWork*,void*,int,int,int,int);
void ov102_021EBFA0(void *,int);

// @fn ov102_021EBFA0
void ov102_021EBFA0(void *input, int change) {
    Work *w = (Work *)input;
    int i;
    u8 y;
    u32 row;
    u8 nexty;
    u32 newY;
    u32 nextrow;
    int count, pixels, rows;
    ov102_021EC090((u8 *)w, change);
    rows = change * 2;
    row = w->row;
    nextrow = row + rows;
    pixels = change * 24;
    nexty = w->y + pixels;
    newY = nexty;
    if (change < 0) {
        y = nexty;
        row = nextrow;
        count = -rows;
        rows = -rows;
    } else {
        y = w->y + 120;
        row += 10;
        count = rows;
        if (rows + row > EasyChatApp_GetCategoryWordCount(w->base)) count--;
    }
    for (i = 0; i < count; i++) {
        ov102_021EC13C((u8 *)w, (int)(row + i), y);
        if (i & 1) y += 24;
    }
    w->y = newY;
    w->row = nextrow;
    CopyWindowPixelsToVram_TextMode(w->windows);
    ov102_021EC298(&w->scroll, w->view, 2, 1, pixels, rows);
}
