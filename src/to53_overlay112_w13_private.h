#ifndef TO53_R3_W13_PRIVATE_H
#define TO53_R3_W13_PRIVATE_H

#include "global.h"

extern u8 gSystem[];

int TextPrinterCheckActive(u8 id);
int IsSEPlaying(int id);
void PlaySE(int id);
void PokewalkerApp_ShowYesNoMenu(void *p, int value);
void *PokewalkerApp_StartMessageBox(void *p, int a, int b);
void PokewalkerApp_HideCursor(void *p);
int ov112_021E7668(void);
void BgClearTilemapBufferAndCommit(void *bg, int id);
void PokewalkerApp_LoadTilemap(void *p, int a, int b);
void PokewalkerApp_PrintExit(void *p, int a);
void sub_02032674(void *a, void *b, void *c);
void sub_02032650(void *a, u32 b, u16 c);
void ManagedSprite_SetAnimationFrame(void *sprite, u16 frame);
int IsFanfarePlaying(void);
int PokewalkerApp_HandleTwoChoiceMenu(void *p);
int ov112_021E9338(void *p);
void PokewalkerApp_HideSprite(void *p, int index);
void ov112_021ED2F4(void *p);
void ov112_021ED0D0(void *p);
void ov112_021EC460(void *p, int a, int b, int c);
void ov112_021EA5A4(void *p, int index);
void PokewalkerApp_LoadSubBg1Tilemap(void *p);
void *SysTask_CreateOnMainQueue(void *func, void *arg, u32 priority);
void SysTask_Destroy(void *task);
void ov112_021EAAE4(void *p, void *q);
int PokewalkerApp_AgeParticles(void *p);
void FillBgTilemapRect(void *bg, u8 id, u16 fill, u8 x, u8 y, u8 width, u8 height, u8 palette);
void BgCommitTilemapBufferToVram(void *bg, int id);
void PokewalkerIr_Stop(void);
void PlayFanfare(int id);
void ov112_021E7548(void *a, void *b, void *c, void *d);
void PokewalkerApp_StartLink(void *p);
void ov112_021E7620(void *a, void *b);

int ov112_021ECECC(void *p);
int ov112_021ECEEC(void *p);
void ov112_021ED25C(void *p);
void ov112_021ED314(int play);
int ov112_021EE0F4(void *p);
int ov112_021EE10C(void *p);
int ov112_021EE1A4(void *p);
int ov112_021EE1BC(void *p);
int ov112_021EE1DC(void *p);
int ov112_021ED330(void *p, int value);
void ov112_021ED590(void *p);
void ov112_021ED5DC(void *p);
BOOL ov112_021ED610(void *p);
BOOL ov112_021ED640(void *p);
void ov112_021EDAF4(void *p, int frame);
int ov112_021EDCB0(int value);
int ov112_021EDD08(void *p);
int ov112_021EDD30(void *p);
int ov112_021EDD5C(void *p);
int ov112_021EDD80(void *p);
int ov112_021EDDB4(void *p);
int ov112_021EDDE8(void *p);
int ov112_021EE178(void *p);
int ov112_021ECEF8(void *p);
int ov112_021ECF38(void *p);
int ov112_021ECF88(void *p);
int ov112_021ECFD4(void *p);
int ov112_021ED020(void *p);
int ov112_021ED054(void *p);
int ov112_021ED090(void *p);
void ov112_021ED180(void *task, void *p);
int ov112_021ED224(void *p);
void ov112_021ED2F4(void *p);
int ov112_021EDF08(void *p);
int ov112_021EDE1C(void *p);
int ov112_021EE12C(void *p);
int ov112_021ED4D4(void *p);
int ov112_021ED8DC(void *p);

#endif
