#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"
void *Sprite_GetImageProxy(void *);
u32 Boxmon_GetIconNaix(void *);void *GfGfxLoader_GetCharDataFromOpenNarc(void *,u32,int,void **,u32);u32 Boxmon_GetIconPalette(void *);void Heap_Free(void *);void ManagedSprite_SetPaletteOverride(void *,int);void ManagedSprite_SetDrawFlag(void *,int);
void PCBox_LoadSpriteCharsMain(void *,int,void *,u32);void PCBox_LoadSpriteCharsSub(void *,int,void *,u32);void *PCBox_LoadMonIconChars(void *,void *,void **);void PCBox_SetIconImage(void *,void *,int,int);void PCBox_LoadMonIcon(void *,void *,int);int PCBox_GetSlotMonData(void *,int,int,int);
void *PCBox_GetSlotBoxMon(void *,int,int);
void PCBox_RefreshSlotIcon(void *,int,int,int);void PCBox_LoadBoxIcons(void *);
void PCBox_SetSpritePriority(void *,int,int);void PCBox_SetSpriteDrawPriority(void *,int,int);void PCBox_SetIconLayer(void *,u32,int);void ov14_021F31E0(void *,int,u32);
void ManagedSprite_SetAffineOverwriteMode(void *,int);void ManagedSprite_SetAffineScale(void *,u32,u32);void ManagedSprite_SetAffineTranslation(void *,int,int);u32 _fsub(u32,u32);u32 _fadd(u32,u32);
void PCBox_StartReleaseScale(void *);int PCBox_UpdateReleaseShrink(void *);int PCBox_UpdateReleaseGrow(void *);
u32 Party_GetCount(void *);
void *Party_GetMonByIndex(void *,u32);
void *Mon_GetBoxMon(void *);
extern s16 sPCBoxPartySlotPositions[];
void PCBox_LoadPartyIcons(void *);
void PCBox_GetSlotPosition(u32,s16 *,s16 *,int);
void PCBox_SetSpriteVisible(void *,int,int);
void PCBox_SetIconLayer(void *,u32,int);
void ov14_021F31E0(void *,int,u32);
void PCBox_SetSpriteVisible(void *,int,int);
void PCBox_SetSpritePriority(void *,int,int);
void PCBox_SetSpriteDrawPriority(void *,int,int);
void ov14_021F2E8C(void *,void *,void *,int,int,int);
void *NewString_ReadMsgData(void *,int);
void ov14_021F2FC4(void *,void *,void *,int,int);
void String_Delete(void *);
void ov14_021F41DB4(void *,int);
void ListMenuItems_Delete(void *);
int ov14_021F2D90(void *);
int ov14_021F2E74(void *);
#endif
