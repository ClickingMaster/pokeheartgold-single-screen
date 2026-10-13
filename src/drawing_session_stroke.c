#include "global.h"

#include "drawing_session.h"

// The last function of the Union Room drawing session (drawing_session.c); its object is linked after
// the overlay's data (asm/drawing_session_data.s).

// force_active only sets a section flag here; kept so the object stays identical.
#pragma force_active on

// Turns this frame's touch samples into the own stroke: the first and the last sample, dropped when
// either is (0, 0).
void DrawingSession_BuildStroke(DrawingSessionStroke *stroke, TouchOutput *touch, int color, int size) {
    int i;
    if (touch->count) {
        stroke->xs[0] = touch->samples[0].x;
        stroke->ys[0] = touch->samples[0].y;
        // = touch->samples[touch->count - 1], with the offset of samples added after the index
        TouchOutput *last = (TouchOutput *)((TPData *)touch + (touch->count - 1));
        stroke->xs[1] = last->samples[0].x;
        stroke->ys[1] = last->samples[0].y;
        for (i = 0; i < 2; i++) {
            if (stroke->xs[i] + stroke->ys[i] == 0) {
                touch->count = 0;
            }
        }
    }
    if (touch->count >= 2) {
        stroke->count = 2;
    } else {
        stroke->count = touch->count;
    }
    stroke->color = color;
    stroke->size = size;
}
