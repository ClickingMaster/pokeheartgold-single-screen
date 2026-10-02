#pragma force_active on
#include "global.h"
#pragma force_active on
void GetBoxmonSpriteCharAndPlttNarcIds(void *,void *,int,int);
u32 GetBoxMonData(void *,int,void *);
void sub_02014494(int,int,int,int,int,int,int,void *,u32,int,int,int);
void GfGfxLoader_GXLoadPal(int,int,int,int,int,int);
int ov112_021E9290(void *,void *,u16 *,int);
int ov112_021E9290(void *mon,void *buf,u16 *narc,int sub){
 u32 personality,species;
 GetBoxmonSpriteCharAndPlttNarcIds(narc,mon,2,0);
 personality=GetBoxMonData(mon,0,NULL);species=GetBoxMonData(mon,5,NULL);
 sub_02014494(narc[0],narc[1],154,0,0,10,10,buf,personality,0,2,species);
 DC_FlushRange(buf,3200);
 if(sub==0){GX_LoadOBJ(buf,2944,3200);GfGfxLoader_GXLoadPal(narc[0],narc[2],1,128,32,154);}
 else{GXS_LoadOBJ(buf,3136,3200);GfGfxLoader_GXLoadPal(narc[0],narc[2],5,32,32,154);}
 return 0;
}
