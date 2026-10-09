#pragma force_active on
#include "global.h"
typedef struct{u8 value:7,flag:1;}Flags;
typedef struct {u8 bytes[16];} DisplayWindow;
typedef struct {u8 gap0[32];void *messages,*formatter,*string;u8 gap2c[0x24];DisplayWindow windows[96];u8 gap650[0x1b4];void *mon;u8 gap808[6];Flags gender;u8 species,ability,nature;u16 item;u8 gap814[4];u16 stats[7];u8 gap826[2];u16 moves[4];u8 pp[4],maxPP[4];} Display;
#define F(p) ((Flags*)((u8*)(p)+0x80e))
#define B(p,o) (((u8*)(p))[o])
#define U(p,o) (*(u16*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void FillWindowPixelBuffer(void*,int),ScheduleWindowCopyToVram(void*),CopyWindowPixelsToVram_TextMode(void*);void*Mon_GetBoxMon(void*);void BufferBoxMonNickname(void*,int,void*),BufferAbilityName(void*,int,int),BufferNatureName(void*,int,int),BufferItemName(void*,int,int),BufferMoveName(void*,int,int);void ov83_02241DD8(void*,void*,void*,int,int,int,int,u32,int),ov83_022479E4(void*,void*,int,int,int,int,u32,int),ov83_02240C48(void*,int,int,int,int),ov83_02247998(void*,void*,int,int,int,u32,int);u8 GetWindowWidth(void*);u32 FontID_String_GetWidth(int,void*,int);void*NewString_ReadMsgData(void*,int);void String_Delete(void*),StringExpandPlaceholders(void*,void*,void*);
void ov83_022421E0(void*,int);
// @fn ov83_022421E0
#pragma opt_strength_reduction off
void ov83_022421E0(void*arg,int flag){Display*p=arg;u32 i,move;u16 half; u16 center;u8*q,*indexed;void*text,*format;
FillWindowPixelBuffer(&p->windows[18],0);
FillWindowPixelBuffer(&p->windows[19],0);
FillWindowPixelBuffer(&p->windows[21],0);
FillWindowPixelBuffer(&p->windows[23],0);
FillWindowPixelBuffer(&p->windows[25],0);
FillWindowPixelBuffer(&p->windows[27],0);
FillWindowPixelBuffer(&p->windows[29],0);
FillWindowPixelBuffer(&p->windows[31],0);
FillWindowPixelBuffer(&p->windows[33],0);
FillWindowPixelBuffer(&p->windows[35],0);
FillWindowPixelBuffer(&p->windows[37],0);
FillWindowPixelBuffer(&p->windows[39],0);
FillWindowPixelBuffer(&p->windows[40],0);
FillWindowPixelBuffer(&p->windows[41],0);
FillWindowPixelBuffer(&p->windows[42],0);
FillWindowPixelBuffer(&p->windows[43],0);
FillWindowPixelBuffer(&p->windows[44],0);
FillWindowPixelBuffer(&p->windows[45],0);
FillWindowPixelBuffer(&p->windows[46],0);
FillWindowPixelBuffer(&p->windows[47],0);
BufferBoxMonNickname(p->formatter,0,Mon_GetBoxMon(p->mon));ov83_02241DD8(p,&p->windows[18],p->messages,91,0,0,0,0x10200,0);if(p->gender.flag==0){if(p->gender.value==0)ov83_022479E4(&p->windows[19],p->messages,86,0,0,0,0x50600,0);else if(p->gender.value==1)ov83_022479E4(&p->windows[19],p->messages,87,0,0,0,0x30400,0);}ov83_02240C48(p,0,p->species,3,0);ov83_02241DD8(p,&p->windows[21],p->messages,94,0,0,0,0x10200,0);BufferAbilityName(p->formatter,0,p->ability);ov83_02241DD8(p,&p->windows[23],p->messages,75,0,0,0,0x10200,0);BufferNatureName(p->formatter,0,p->nature);ov83_02241DD8(p,&p->windows[25],p->messages,73,0,0,0,0x10200,0);BufferItemName(p->formatter,0,p->item);ov83_02241DD8(p,&p->windows[27],p->messages,71,0,0,0,0x10200,0);ov83_02240C48(p,0,p->stats[0],3,0);ov83_02240C48(p,1,p->stats[1],3,0);ov83_02241DD8(p,&p->windows[29],p->messages,95,GetWindowWidth(&p->windows[29])*8,0,0,0x10200,1);ov83_02240C48(p,0,p->stats[2],3,0);ov83_02241DD8(p,&p->windows[31],p->messages,77,GetWindowWidth(&p->windows[31])*8,0,0,0x10200,1);ov83_02240C48(p,0,p->stats[3],3,0);ov83_02241DD8(p,&p->windows[33],p->messages,81,GetWindowWidth(&p->windows[33])*8,0,0,0x10200,1);ov83_02240C48(p,0,p->stats[4],3,0);ov83_02241DD8(p,&p->windows[35],p->messages,79,GetWindowWidth(&p->windows[35])*8,0,0,0x10200,1);ov83_02240C48(p,0,p->stats[5],3,0);ov83_02241DD8(p,&p->windows[37],p->messages,83,GetWindowWidth(&p->windows[37])*8,0,0,0x10200,1);ov83_02240C48(p,0,p->stats[6],3,0);ov83_02241DD8(p,&p->windows[39],p->messages,85,GetWindowWidth(&p->windows[39])*8,0,0,0x10200,1);{move=0;q=(u8*)p;for(;move<4;move++){BufferMoveName(p->formatter,move,U(q,0x828));ov83_02241DD8(p,&p->windows[move+40],p->messages,move+96,0,0,0,0x10200,0);if(U(q,0x828)==0){void*win=&p->windows[move+44];int width=GetWindowWidth(win);ov83_022479E4(win,p->messages,102,(width*8)/2,0,0,0x10200,2);}else{void*win;text=NewString_ReadMsgData(p->messages,90);half=(u16)(FontID_String_GetWidth(0,text,0)/2);win=&p->windows[move+44];center=(u16)((GetWindowWidth(win)*8)/2);ov83_02247998(win,text,center,0,0,0x10200,2);String_Delete(text);indexed=(u8*)p+move;ov83_02240C48(p,0,B(indexed,0x830),2,0);format=NewString_ReadMsgData(p->messages,101);StringExpandPlaceholders(p->formatter,p->string,format);ov83_02247998(win,p->string,(s32)center-half-FontID_String_GetWidth(0,p->string,0),0,0,0x10200,0);String_Delete(format);ov83_02240C48(p,0,B(indexed,0x834),2,0);ov83_02241DD8(p,win,p->messages,101,(s32)center+half,0,0,0x10200,0);}q+=2;}}if(flag==1){DisplayWindow*w;i=18;w=&p->windows[18];for(;i<=47;i++){ScheduleWindowCopyToVram(w);w++;}}else{CopyWindowPixelsToVram_TextMode(&p->windows[18]);CopyWindowPixelsToVram_TextMode(&p->windows[19]);CopyWindowPixelsToVram_TextMode(&p->windows[21]);CopyWindowPixelsToVram_TextMode(&p->windows[23]);CopyWindowPixelsToVram_TextMode(&p->windows[25]);CopyWindowPixelsToVram_TextMode(&p->windows[27]);CopyWindowPixelsToVram_TextMode(&p->windows[29]);CopyWindowPixelsToVram_TextMode(&p->windows[31]);CopyWindowPixelsToVram_TextMode(&p->windows[33]);CopyWindowPixelsToVram_TextMode(&p->windows[35]);CopyWindowPixelsToVram_TextMode(&p->windows[37]);CopyWindowPixelsToVram_TextMode(&p->windows[39]);CopyWindowPixelsToVram_TextMode(&p->windows[40]);CopyWindowPixelsToVram_TextMode(&p->windows[41]);CopyWindowPixelsToVram_TextMode(&p->windows[42]);CopyWindowPixelsToVram_TextMode(&p->windows[43]);CopyWindowPixelsToVram_TextMode(&p->windows[44]);CopyWindowPixelsToVram_TextMode(&p->windows[45]);CopyWindowPixelsToVram_TextMode(&p->windows[46]);CopyWindowPixelsToVram_TextMode(&p->windows[47]);}}
#pragma opt_strength_reduction reset
