#ifndef TO52_R6_W02_PRIVATE_H
#define TO52_R6_W02_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov96EntryInput {
    u32 participant;
    u8 pad04[5];
    u8 kind;
    u8 pad0A[6];
    void *window;
} Ov96EntryInput;

void ov96_021F8980(void *);
void ov96_021F8A50(void *);
void ov96_021F8AFC(void *, u8, void *);
void ov96_021F8DD4(void *);
void G2dRenderer_SetSubSurfaceCoords(void *, fx32, fx32);
void G2dRenderer_SetMainSurfaceCoords(void *, fx32, fx32);
void *PokeathlonCourse_GetParticipantData(void *, u32);
void ov96_021F8C04(void *, void *, void *);
void ov96_021F8BC0(void *, s32, void *);
int ov96_021E8424(u8);
int ov96_021EEDCC(int);
void ov96_021EEA94(void *, u32, u32, u32);
void ov96_021EEB84(void *, void *, u32, u32);
void ov96_021F8C2C(void *);
void ov96_021F8C54(BOOL, BOOL, void *);
void ov96_021F8E94(void *);

void ov96_021F87E0(void *);
void *ov96_021F8830(void *, u32, u32, Ov96EntryInput *);
void ov96_021F8918(void *);

#endif
