#pragma force_active on
#include "global.h"
typedef struct {u8 rest:5;u8 first:1;u8 second:1;u8 visible:1;} Flags;
extern void PokedexApp_GetSeenFormAndGender(void*,int,u8*,u8*),PokedexApp_SetSpriteVisible(void*,int,int),PokedexApp_LoadMonPicToSprite(void*,u16,u8,u8,int,int,int),PokedexApp_SetSpritePos(void*,int,s16,s16,int);
extern u8 GetMonPicHeightBySpeciesGenderForm(u16,u8,u8,u8,u32);
void PokedexApp_ShowFormCompareMonPic(u8*,int,int,int);
typedef struct{u8 prefix[0x18a2];u16 species;u8 gap[0x23];Flags flags;} PictureState;
// @fn PokedexApp_ShowFormCompareMonPic
void PokedexApp_ShowFormCompareMonPic(u8 *s,int which,int value,int extra){PictureState*picture=(PictureState*)s;struct{u8 form,gender;}attributes;u8 x,height;int palette;int offset;PokedexApp_GetSeenFormAndGender(s,value,&attributes.form,&attributes.gender);if(!picture->flags.visible){palette=2;offset=0;}else{palette=0;offset=GetMonPicHeightBySpeciesGenderForm(picture->species,attributes.gender,0,attributes.form,0);}if(which==1){x=64;height=offset+120;if(picture->flags.first==1){which=3;PokedexApp_SetSpriteVisible(s,1,0);PokedexApp_SetSpriteVisible(s,which,1);}else{PokedexApp_SetSpriteVisible(s,1,1);PokedexApp_SetSpriteVisible(s,3,0);}picture->flags.first=(u8)(picture->flags.first^1);}else if(which==2){x=192;height=offset+120;if(picture->flags.second==1){which=4;PokedexApp_SetSpriteVisible(s,2,0);PokedexApp_SetSpriteVisible(s,which,1);}else{PokedexApp_SetSpriteVisible(s,2,1);PokedexApp_SetSpriteVisible(s,4,0);}picture->flags.second=(u8)(picture->flags.second^1);}PokedexApp_LoadMonPicToSprite(s,picture->species,attributes.form,attributes.gender,palette,which,extra);PokedexApp_SetSpritePos(s,which,x,height,1);}
