#ifndef TO57_NEW45_PRIVATE_H
#define TO57_NEW45_PRIVATE_H

#include "global.h"

extern u8 gSystem[];

BOOL ov112_021E7670(void);
BOOL ov112_021E768C(void);
void ov112_021EA7D0(void *p);
void ov112_021EA60C(void *p);
void ov112_021ED314(int value);
int ov112_021E76A8(void);
BOOL ov112_021ED610(void *p);
void ov112_021EAA98(void *p, void *q, int value);
void ov112_021ED530(void *p);
void ov112_021ED590(void *p);
void ov112_021E7464(void);
BOOL ov112_021E7668(void);
void ov112_021E98E8(void *p, int value);
int ov112_021E9888(int value);
void ov112_021EC440(void *p, int a, int b, int c);
void ov112_021EA10C(void *p, int a, int b);
void ov112_021ED7DC(void *p);
int ov112_021ED66C(void *p);

void ov112_021F30BC(void *p, void *q);
BOOL ov112_021F31BC(void *p);
void ov112_021F3140(void *p, void *q);

void ManagedSprite_SetDrawFlag(void *sprite, int flag);
void GetSpeciesNameIntoArray(u16 species, int heapID, u16 *dest);
void PlaySE(int seqNo);

#endif
