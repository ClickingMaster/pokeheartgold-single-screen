#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x24];void *system,*manager;u8 pad2[0x60];void *palette;} W;
extern void SpriteSystem_LoadPaletteBufferFromOpenNarc(void *,int,void *,void *,void *,int,int,int,int,int),SpriteSystem_LoadCharResObjFromOpenNarc(void *,void *,void *,int,int,int,int),SpriteSystem_LoadCellResObjFromOpenNarc(void *,void *,void *,int,int,int),SpriteSystem_LoadAnimResObjFromOpenNarc(void *,void *,void *,int,int,int);
extern void ov93_022609E0(W *),ov93_022610B0(W *,void *),ov93_02261164(W *,void *),ov93_02261BBC(W *,void *),ov93_02260CF8(W *),ov93_02260E1C(W *);
void ov93_0225DA40(W *,void *);
void ov93_0225DA40(W *w,void *narc){SpriteSystem_LoadPaletteBufferFromOpenNarc(w->palette,3,w->system,w->manager,narc,23,0,9,2,10006);SpriteSystem_LoadCharResObjFromOpenNarc(w->system,w->manager,narc,20,0,2,10004);SpriteSystem_LoadCellResObjFromOpenNarc(w->system,w->manager,narc,22,0,10004);SpriteSystem_LoadAnimResObjFromOpenNarc(w->system,w->manager,narc,21,0,10004);ov93_022609E0(w);ov93_022610B0(w,(u8*)w+0x1458);ov93_02261164(w,(u8*)w+0x1468);ov93_02261BBC(w,(u8*)w+0x1560);ov93_02260CF8(w);ov93_02260E1C(w);}
