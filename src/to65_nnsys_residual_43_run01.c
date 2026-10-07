#pragma force_active on
// clang-format off
#define NNS_G3dGetResDictIdxByName ScratchInlineLookup
#include "global.h"
#undef NNS_G3dGetResDictIdxByName
#include <nitro/code32.h>
// clang-format on
#include "renderer_joint_family_private.h"
#define W(p,o) (*(s32 *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define H(p,o) (*(u16 *)((char *)(p)+(o)))
#define S(p,o) (*(s16 *)((char *)(p)+(o)))
#define B(p,o) (*(u8 *)((char *)(p)+(o)))
#define A(p,o) ((char *)(p)+(o))
typedef struct {u8 pad[26];u16 ids[1];} IndexMap;
#define IH(p,i) (((IndexMap *)(p))->ids[i])
#define MUL(a,b) ((s32)(((s64)(a)*(b))>>12))
extern void *_02110A08,*_02110A04;


extern void FX_DivAsync(fx32,fx32);
extern fx32 FX_GetDivResult(void);
extern void *_02110A14,*_02110A0C,*_02110A10;
extern void GetTexSRTAnm_(void *,u16,int,void *);
extern void *NNSi_G3dGetTexPatAnmFV(void *,u16,u16);
extern void *NNSi_G3dGetTexPatAnmTexNameByIdx(void *,int);
extern void *NNSi_G3dGetTexPatAnmPlttNameByIdx(void *,int);
extern void SetTexParamaters_(void *,void *,void *);
extern int NNS_G3dGetResDictIdxByName(void *,void *);
extern void *NNS_G3dGetResDataByName(void *,void *);
void NNSi_G3dAnmObjInitNsBca(void *out,void *s,void *mdl);
void NNSi_G3dAnmObjInitNsBva(void *out,void *s,void *mdl);
void NNSi_G3dAnmCalcNsBva(void *out,void *s,u32 id);
void NNSi_G3dSendJointSRTBasic(void *s);
void texmtxCalc_flagTRS_(void *out,void *s);
void texmtxCalc_flagRS_(void *out,void *s);
void texmtxCalc_flagTR_(void *out,void *s);
void texmtxCalc_flagTS_(void *out,void *s);
void texmtxCalc_flagTRS___3dsmax(void *out,void *s);
void texmtxCalc_flagRS___3dsmax(void *out,void *s);
void texmtxCalc_flagTR___3dsmax(void *out,void *s);
void texmtxCalc_flagTS___3dsmax(void *out,void *s);
void texmtxCalc_flagTRS___xsi(void *out,void *s);
void texmtxCalc_flagRS___xsi(void *out,void *s);
void texmtxCalc_flagTR___xsi(void *out,void *s);
void texmtxCalc_flagTS___xsi(void *out,void *s);
void texmtxCalc_flagR_(void *out,void *s);
void NNSi_G3dAnmCalcNsBta(void *out,void *s,int id);
void NNSi_G3dAnmCalcNsBtp(void *out,void *s,int id);
void SetPlttParamaters_(void *s,void *name,void *out);
void NNSi_G3dSendJointSRTMaya(void *s);
void NNSi_G3dAnmObjInitNsBma(void *out,void *s,void *mdl);
void NNSi_G3dAnmObjInitNsBta(void *out,void *s,void *mdl);
void NNSi_G3dAnmObjInitNsBtp(void *out,void *s,void *mdl);
extern u8 _021DD420[],_021DD430[],_021DD43C[];
extern void BeginSleep(void *);
extern void EndSleep(void *);
extern void SND_Init(void);
extern void NNSi_SndInitResourceMgr(void);
extern void NNSi_SndCaptureInit(void);
extern void NNSi_SndPlayerInit(void);
void NNS_SndInit(void);
extern void PM_PrependPreSleepCallback(void *);
extern void PM_AppendPostSleepCallback(void *);
typedef void (*TexCalc)(void *,void *);
extern TexCalc _02110C1C[],_02110C3C[],_02110C5C[];
void NNSi_G3dSendTexSRTMaya(void *);
void NNSi_G3dSendTexSRT3dsMax(void *);
void NNSi_G3dSendTexSRTXsi(void *);
extern u32 GetMatColAnmValue_(void *,u32,int);
extern u32 GetMatColAnmuAlphaValue_(void *,u32,int);
void NNSi_G3dAnmCalcNsBma(void *,void *,int);
void NNSi_G3dAnmObjInitNsBca(void *out,void *s,void *mdl) { u32 i;u16 *indices;P(out,8)=s;P(out,0xc)=_02110A08;B(out,0x19)=B(mdl,0x17);MIi_CpuClear16(0,(u16 *)A(out,0x1a),B(out,0x19)*2);indices=(u16 *)A(s,0x14);for(i=0;i<H(s,6);i++)IH(out,i)=((u32)W(s,indices[i])>>24)|0x100; }

void NNSi_G3dAnmCalcNsBca(MdlJointResult *out,const NNSG3dAnmObj *s,u32 id) { void *p=s->resAnm;int frame=s->frame,limit=*(u16*)((u8*)p+4)<<12;if(frame>=limit)frame=limit-1;else if(frame<0)frame=0;getJntSRTAnmResult_(p,id,frame,out); }
