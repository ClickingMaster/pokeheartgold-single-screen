#ifndef TO52_R3_W05_PRIVATE_H
#define TO52_R3_W05_PRIVATE_H
#include "global.h"
#define W32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define W16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
void *Party_GetMonByIndex(void *, int);
void Pokepic_SetAttr(void *, int, int);
void Pokepic_Delete(void *);
void *ov81_02240F48(void *, u8, void *, int, int, int);
void *ov81_02242CBC(void *, int, int, int, int, int, int);
void ov81_02242D88(void *, int);
void ov81_02241DDC(void *, int);
void ov81_02241E68(void *, int, int, int);
void ov81_02241EDC(void *, int, int, int);
void ov81_02241F50(void *);
#endif
