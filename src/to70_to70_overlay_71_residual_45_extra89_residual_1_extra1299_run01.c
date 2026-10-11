#pragma force_active on
#include "global.h"
typedef struct Work { void *ctx; int pad,timer,active; char gap[0x4c]; char window[16]; void *text,*expanded; } Work;
extern void *TradeSequence_GetMsgData(void*),*TradeSequence_GetMsgFormat(void*);
extern int TradeSequence_GetTradeMode(void*),ov71_0224B6F0(Work*,int),IsPaletteFadeFinished(void);
extern void ReadMsgDataIntoString(void*,int,void*),StringExpandPlaceholders(void*,void*,void*),AddTextPrinterParameterized(void*,int,void*,int,int,int,void*),DrawFrame2AroundWindow(void*,int,int),CopyWindowToVram(void*),PlayFanfare(int),FillWindowPixelBuffer(void*,int),CopyWindowPixelsToVram_TextMode(void*),ClearFrameAndWindow2(void*,int),BeginNormalPaletteFade(int,int,int,int,int,int,int);
int ov71_0224B57C(Work*,int*);
int ov71_0224B57C(Work*w,int *state) {
 switch(*state) {
 case 0: w->timer=0; ++*state; break;
 case 1: if(++w->timer>10) { void *msg=TradeSequence_GetMsgData(w->ctx); void *format=TradeSequence_GetMsgFormat(w->ctx); int id,next; if(TradeSequence_GetTradeMode(w->ctx)==1) { id=2; next=2; } else { id=5; next=3; }
 ReadMsgDataIntoString(msg,id,w->text); StringExpandPlaceholders(format,w->expanded,w->text); AddTextPrinterParameterized(w->window,1,w->expanded,0,0,255,0); DrawFrame2AroundWindow(w->window,0x6d,2); CopyWindowToVram(w->window); PlayFanfare(0x4a4); w->timer=0; w->active=1; *state=next; } break;
 case 2: if(ov71_0224B6F0(w,60)) { void *msg=TradeSequence_GetMsgData(w->ctx); void *format=TradeSequence_GetMsgFormat(w->ctx); ReadMsgDataIntoString(msg,3,w->text); StringExpandPlaceholders(format,w->expanded,w->text); FillWindowPixelBuffer(w->window,15); AddTextPrinterParameterized(w->window,1,w->expanded,0,0,255,0); CopyWindowPixelsToVram_TextMode(w->window); w->timer=0; ++*state; } break;
 case 3: if(ov71_0224B6F0(w,60)) { ClearFrameAndWindow2(w->window,0); w->timer=0; ++*state; } break;
 case 4: if(++w->timer>10) { BeginNormalPaletteFade(3,0,0,0,16,1,57); ++*state; } break;
 case 5: if(IsPaletteFadeFinished()) return 1; break;
 }
 return 0;
}
