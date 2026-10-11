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
void PokewalkerApp_StartSubBgScroll(void *p);
void PokewalkerApp_FadeInMonPicture(void *p);
void PokewalkerApp_StartMessage(void *p, int a, int b, int c);
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

int PokewalkerApp_Link_WaitConfirmCourse(void *p);
int PokewalkerApp_Link_ShowConfirmCourseYesNo(void *p);
void PokewalkerApp_ResetLinkError(void *p);
void PokewalkerApp_PlayTransferSE(int play);
int ov112_021EE0F4(void *p);
int ov112_021EE10C(void *p);
int ov112_021EE1A4(void *p);
int ov112_021EE1BC(void *p);
int ov112_021EE1DC(void *p);
int PokewalkerApp_UpdateLinkProgress(void *p, int value);
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
int PokewalkerApp_Link_HandleConfirmCourseYesNo(void *p);
int PokewalkerApp_Link_PrintInstructions(void *p);
int PokewalkerApp_Link_WaitInstructions(void *p);
int PokewalkerApp_Link_PrintWillConnectWithMon(void *p);
int PokewalkerApp_Link_PrintWillConnect(void *p);
int PokewalkerApp_Link_PrintPointAtCardForStroll(void *p);
int PokewalkerApp_Link_WaitPointAtCardForStroll(void *p);
void PokewalkerApp_ScrollSubBgTask(void *task, void *p);
int PokewalkerApp_Link_WaitPointAtCardForGifts(void *p);
void PokewalkerApp_StartSubBgScroll(void *p);
int ov112_021EDF08(void *p);
int ov112_021EDE1C(void *p);
int ov112_021EE12C(void *p);
int PokewalkerApp_Link_StartReceive(void *p);
int ov112_021ED8DC(void *p);

#endif
