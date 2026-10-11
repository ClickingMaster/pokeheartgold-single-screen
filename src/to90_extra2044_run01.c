#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern int ov80_02237D8C(int),sub_0205C1F0(int),sub_0205C268(int),FrontierSave_GetStat(void*,int,int),Comm_GetLocalNetId(void);extern void*Save_Frontier_GetStatic(void*),*Save_PlayerData_GetProfile(void*),*sub_02034818(int);extern void ov80_02233594(void*,void*,void*,int);void ov80_02233490(void*,void*);
void ov80_02233490(void*w,void*d){void*f;int n,a,b;if(!ov80_02237D8C(B(d,16))){GF_ASSERT(P(w,0xa8));f=Save_Frontier_GetStatic(P(d,4));n=sub_0205C1F0(B(d,16));a=FrontierSave_GetStat(f,n,sub_0205C268(sub_0205C1F0(B(d,16))));ov80_02233594(w,P(w,0xa8),Save_PlayerData_GetProfile(P(d,4)),a);}else{GF_ASSERT(P(w,0xa8));GF_ASSERT(P(w,0xac));if(!Comm_GetLocalNetId()){f=Save_Frontier_GetStatic(P(d,4));n=sub_0205C1F0(B(d,16));a=FrontierSave_GetStat(f,n,sub_0205C268(sub_0205C1F0(B(d,16))));b=H(d,0xa1c);}else{a=H(d,0xa1c);f=Save_Frontier_GetStatic(P(d,4));n=sub_0205C1F0(B(d,16));b=FrontierSave_GetStat(f,n,sub_0205C268(sub_0205C1F0(B(d,16))));}ov80_02233594(w,P(w,0xa8),sub_02034818(0),a);ov80_02233594(w,P(w,0xac),sub_02034818(1),b);}}
