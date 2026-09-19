#ifndef TO50_OV88_H
#define TO50_OV88_H
#include <string.h>

#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "system.h"
void ov88_02258A64(void *);
void ov88_02258A70(void *, u32);
void ov88_02258ABC(void *, int);
void ov88_02258AF4(void *);
void ov88_02258D64(void *, void *, int);
void ov88_02258D84(void *);
void ov88_02258EA8(void *, void *, void *, u32, u32, u32, int);
void ov88_02258EF8(void *);
BOOL ov88_02258EFC(void *, u32, u32);
void ov88_022590C0(void *, void *, void *, u32, u32, int);
void ov88_022590D4(void *);
BOOL ov88_022590D8(void *, void *);
void ov88_022592EC(void *, void *, int);
void ov88_022593D0(void *);
BOOL ov88_02259404(void *, u32, void *, int);
void ov88_0225967C(void *, void *, int);
void ov88_022597DC(void *);
void ov88_02259818(void *, u32);
BOOL ov88_02258800(OverlayManager *, int *);
BOOL ov88_022588C4(OverlayManager *, u32 *);
BOOL ov88_022589FC(OverlayManager *, int *);
void ov45_0222A520(void *, int);
int ov45_0222A330(void *);
void ov45_0222A4A8(void *);
int ov45_0222D844(void);
#endif
