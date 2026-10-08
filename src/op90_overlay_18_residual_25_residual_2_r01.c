#pragma force_active on
#include "global.h"
typedef struct {u8 rest:5;u8 first:1;u8 second:1;u8 visible:1;} Flags;
extern void ov18_021F3CA8(void*,int,u8*,u8*),ov18_021F11C0(void*,int,int),ov18_021F1A7C(void*,u16,u8,u8,int,int,int),ov18_021F1294(void*,int,s16,s16,int);
extern u8 GetMonPicHeightBySpeciesGenderForm(u16,u8,u8,u8,u32);
void ov18_021F684C(u8*,int,int,int);
typedef struct{u8 prefix[0x18a2];u16 species;u8 gap[0x23];Flags flags;} PictureState;
// @fn ov18_021F684C
void ov18_021F684C(u8 *s,int which,int value,int extra){PictureState*picture=(PictureState*)s;struct{u8 form,gender;}attributes;u8 x,height;int palette;int offset;ov18_021F3CA8(s,value,&attributes.form,&attributes.gender);if(!picture->flags.visible){palette=2;offset=0;}else{palette=0;offset=GetMonPicHeightBySpeciesGenderForm(picture->species,attributes.gender,0,attributes.form,0);}if(which==1){x=64;height=offset+120;if(picture->flags.first==1){which=3;ov18_021F11C0(s,1,0);ov18_021F11C0(s,which,1);}else{ov18_021F11C0(s,1,1);ov18_021F11C0(s,3,0);}picture->flags.first=(u8)(picture->flags.first^1);}else if(which==2){x=192;height=offset+120;if(picture->flags.second==1){which=4;ov18_021F11C0(s,2,0);ov18_021F11C0(s,which,1);}else{ov18_021F11C0(s,2,1);ov18_021F11C0(s,4,0);}picture->flags.second=(u8)(picture->flags.second^1);}ov18_021F1A7C(s,picture->species,attributes.form,attributes.gender,palette,which,extra);ov18_021F1294(s,which,x,height,1);}
