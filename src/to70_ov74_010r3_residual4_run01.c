#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct Work {u32 index;u8 pad[24];u32 frame;void *bg;u8 pad24[0x408];u32 text[19];u8 pad478[0x20];u8 buffer[1];} Work;
typedef struct Mode {u32 v[4];} Mode;
typedef struct Bg {u32 v[7];} Bg;
typedef struct Banks {u32 v[10];} Banks;
extern const Mode _0223B4F8;
extern const Bg _0223B508,_0223B524,_0223B540,_0223B55C;
extern const Banks _0223B578;
extern const u8 ov74_0223C960[];
extern void SetBothScreensModesAndDisable(Mode*);
extern void InitBgFromTemplate(void*,int,Bg*,int);
extern void BgClearTilemapBufferAndCommit(void*,int);
extern void GfGfx_SetBanks(Banks*);
extern void GfGfx_SwapDisplay(void);
extern void ResetAllTextPrinters(void);
extern void LoadFontPal0(int,int,int);
extern void GfGfxLoader_GXLoadPal(int,int,int,int,int,int);
extern void GfGfxLoader_LoadCharData(int,int,void*,int,int,int,int,int);
extern void GfGfxLoader_LoadScrnData(int,int,void*,int,int,int,int,int);
extern void FontID_Alloc(int,int);
extern void BgTilemapRectChangePalette(void*,int,u8,u8,u8,u8,u8);
extern void BgCommitTilemapBufferToVram(void*,int);
extern void LoadUserFrameGfx1(void*,int,int,int,int,int);
extern void LoadUserFrameGfx2(void*,int,int,int,u8,int);
void ov74_02232B18(Work*);
void ov74_02232974(void*);void ov74_02232A48(Work*);void ov74_02232AC8(Work*);
typedef struct Template {u32 v[2];s32 x,y;u32 rest[8];} Template;
extern void Sprite_SetDrawFlag(void*,int);
extern void ov74_02231D48(Template*,void*,void*,int);
extern void *Sprite_CreateAffine(Template*);
extern void Sprite_SetAnimActiveFlag(void*,int),Sprite_SetAnimCtrlSeq(void*,int),Sprite_SetPriority(void*,int);
extern int ov74_02231D70(void*,int,int),ov74_02231D94(void*,int,int),ov74_02231DB8(void*,int,int);
extern void ov74_02231EC4(int,int,int,u8,int,void*);
extern void GfGfx_EngineATogglePlanes(int,int);
extern void ov74_02231A1C(void*,void*,int);
void ov74_02232BD4(Work*);
static inline int Coord(int x){return x<<12;}
void ov74_02232974(void*bg){Mode mode=_0223B4F8;SetBothScreensModesAndDisable(&mode);Bg b0=_0223B508;InitBgFromTemplate(bg,0,&b0,0);BgClearTilemapBufferAndCommit(bg,0);Bg b1=_0223B524;InitBgFromTemplate(bg,1,&b1,0);BgClearTilemapBufferAndCommit(bg,1);Bg b2=_0223B540;InitBgFromTemplate(bg,2,&b2,0);BgClearTilemapBufferAndCommit(bg,2);Bg b3=_0223B55C;InitBgFromTemplate(bg,3,&b3,0);BgClearTilemapBufferAndCommit(bg,3);}
void ov74_02232A48(Work*w){Banks banks=_0223B578;GfGfx_SetBanks(&banks);ov74_02232974(w->bg);gSystem.screensFlipped=1;GfGfx_SwapDisplay();ResetAllTextPrinters();LoadFontPal0(0,480,76);GfGfxLoader_GXLoadPal(113,23,0,0,192,76);GfGfxLoader_LoadCharData(113,26,w->bg,2,0,5120,1,76);FontID_Alloc(2,76);}
void ov74_02232AC8(Work*w){GfGfxLoader_LoadScrnData(113,24,w->bg,2,0,1536,1,76);BgTilemapRectChangePalette(w->bg,2,0,0,32,24,ov74_0223C960[w->index]);BgCommitTilemapBufferToVram(w->bg,2);}

void ov74_02232B18(Work*w){LoadFontPal0(0,448,76);LoadUserFrameGfx1(w->bg,0,1008,14,0,76);LoadUserFrameGfx2(w->bg,0,978,13,(u8)w->frame,76);memset(w->text,0,76);w->text[0]=(u32)w->buffer;w->text[2]=2;w->text[3]=1;w->text[4]=27;w->text[5]=4;w->text[9]=0;w->text[10]=1;w->text[8]=1;w->text[11]=0x1020f;w->text[12]=0xf020f;w->text[14]=0;w->text[15]=0;}
