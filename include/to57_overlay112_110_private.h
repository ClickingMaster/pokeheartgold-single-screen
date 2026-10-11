#ifndef TO57_NEW45_PRIVATE_H
#define TO57_NEW45_PRIVATE_H

#include "global.h"

extern u8 gSystem[];

BOOL PokewalkerIr_GetChunkProgress(void);
BOOL PokewalkerIr_GetTotalProgress(void);
void PokewalkerApp_SpawnFallingParticle(void *p);
void ov112_021EA60C(void *p);
void PokewalkerApp_PlayTransferSE(int value);
int PokewalkerIr_GetAndClearError(void);
BOOL ov112_021ED610(void *p);
void ov112_021EAA98(void *p, void *q, int value);
void ov112_021ED530(void *p);
void ov112_021ED590(void *p);
void PokewalkerIr_Stop(void);
BOOL ov112_021E7668(void);
void PokewalkerApp_ShowSubBg1(void *p, int value);
int PokewalkerApp_GetTouchedButton(int value);
void PokewalkerApp_SelectMenuItem(void *p, int a, int b, int c);
void PokewalkerApp_ShowMessageBox(void *p, int a, int b);
void ov112_021ED7DC(void *p);
int ov112_021ED66C(void *p);

void ov112_021F30BC(void *p, void *q);
BOOL ov112_021F31BC(void *p);
void ov112_021F3140(void *p, void *q);

void ManagedSprite_SetDrawFlag(void *sprite, int flag);
void GetSpeciesNameIntoArray(u16 species, int heapID, u16 *dest);
void PlaySE(int seqNo);

#endif
