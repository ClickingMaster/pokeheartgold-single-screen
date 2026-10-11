#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
int Comm_GetLocalNetId(void);int ov37_021E75E8(void*);void Sprite_SetDrawFlag(void*,int);void FillWindowPixelRect(void*,int,int,int,int,int);void PlayerName_FlatToString(void*,void*);void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*);void CopyWindowToVram(void*);
void ov37_021E7478(u8*windows,int unused,u32 color,u8*w);
void ov37_021E7478(u8*windows,int unused,u32 color,u8*w){int local=Comm_GetLocalNetId();int i;u8*records,*sprites,*names;
 if(ov37_021E75E8(w)){volatile int counter=0;records=w;sprites=w;do{if(!P(records,0x334))Sprite_SetDrawFlag(P(sprites,0x210),0);else Sprite_SetDrawFlag(P(sprites,0x210),1);int next=counter+1;records+=8;sprites+=4;counter=next;if(next>=5)break;}while(1);
 i=0;u8*win=windows;for(;i<5;i++,win+=16)FillWindowPixelRect(win,0,0,0,80,16);
 i=0;names=w;for(;i<5;i++,w+=8,names+=4,windows+=16){if(P(w,0x334)){PlayerName_FlatToString(P(w,0x334),P(names,0x14));if(local==i)AddTextPrinterParameterizedWithColor(windows,1,P(names,0x14),0,0,255,0x3040f,NULL);else AddTextPrinterParameterizedWithColor(windows,1,P(names,0x14),0,0,255,color,NULL);}CopyWindowToVram(windows);}
 }
}
