#ifndef OV90_R18_GAP_PRIVATE_H
#define OV90_R18_GAP_PRIVATE_H
#include "global.h"
void *NARC_New(int, int);
void NARC_Delete(void *);
void GfGfxLoader_GXLoadPalFromOpenNarc(void *, int, int, int, int, int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void *GfGfxLoader_GetScrnDataFromOpenNarc(void *, int, int, void **, int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void ov90_0225A65C(void *, int, int);
void *ov90_02258E54(void *, void *, int, int, int, int, int, int, int);
void ov90_0225B254(void *, void *, u32);
void ov90_0225B59C(void *, void *, void *, void *, void *, int);
void ov90_0225A850(void *, int);
#endif
