#pragma force_active on
#include "global.h"
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void *GfGfxLoader_LoadFromOpenNarc(void*,int,int,int,void*);void GF3dRender_AllocAndLoadTexResources(void*);void GF3dRender_BindModelSet(void*,void*);void NARC_ReadWholeMember(void*,int,void*);void ov108_021E9198(void*,void*,void*,u32);void ov108_021E91F8(void*,void*);void Camera_SetDistance(s32,void*);
typedef struct {u16 distance;s16 x,y,z;} Position;extern const Position ov108_021EA9E4[];
static inline s32 roundS(s16 n){float f;if(n>0)f=0.5f+(float)(n*4096);else f=(float)(n*4096)-0.5f;return(s32)f;}
static inline s32 roundU(u16 n){float f;if(n)f=0.5f+(float)(n*4096);else f=(float)(n*4096)-0.5f;return(s32)f;}
void ov108_021E8F00(u8*w,int index,u8*entry);
static inline u8*getValue(u8*dict,u32 index){if(dict && index<B(dict,1))return dict+H(dict,6)+4;return NULL;}
static inline void*resolve(u8*set){if(set){u8*value=getValue(set+8,0);if(value)return set+U(value,0);}return NULL;}
// @fn ov108_021E8F00
void ov108_021E8F00(u8*w,int index,u8*entry){
 struct {u8 flag,type,pad[6];u32 ids[4];} header;
 u8*obj=w+0x24;void*blob=GfGfxLoader_LoadFromOpenNarc(P(w,0x18),B(entry,0),0,U(w,0x14),NULL);P(w,0x24)=blob;
 u8*set=(u8*)NNS_G3dGetMdlSet(blob);P(obj,4)=set;
 P(obj,8)=resolve(set);
 P(obj,12)=NNS_G3dGetTex(P(obj,0));if(P(obj,12)){GF3dRender_AllocAndLoadTexResources(P(obj,12));GF3dRender_BindModelSet(P(obj,0),P(obj,12));}
 NNS_G3dRenderObjInit((NNSG3dRenderObj*)(obj+16),P(obj,8));DC_FlushAll();H(obj,0xb4)=1;
 NARC_ReadWholeMember(P(w,0x1c),B(entry,0),&header);
 if(!header.flag)goto placement;if(header.type==8)goto placement;{int i;for(i=0;i<4;i++){u32 id=header.ids[i];if(id==0xffffffff)break;ov108_021E9198(w,obj,obj+0x64+20*H(obj,0xb6),id);ov108_021E91F8(obj,obj+0x64+20*H(obj,0xb6));H(obj,0xb6)++;}}
 placement:;const Position*pos=&ov108_021EA9E4[index];U(w,0xdc)=roundS(pos->x);U(w,0xe0)=roundS(pos->y);U(w,0xe4)=roundS(pos->z);Camera_SetDistance(roundU(pos->distance),P(w,0));
}
