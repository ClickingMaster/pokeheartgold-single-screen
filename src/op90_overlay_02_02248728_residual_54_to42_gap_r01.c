#pragma force_active on
#include "global.h"
extern u16 LCRandom(void);
typedef struct {u8 species;u8 low1:4,high1:4;u8 low2:3,mid2:2,high2:3;u8 low3:5,high3:3;u8 low4:5,high4:3;u8 f5,f6,f7;u16 low8:3,mid8:10,high8:3;u16 lowa:3,mida:3,higha:10;u16 f12,f14;u8 bit0:1,bits1:2,bits3:2,high16:3;u8 chance;u16 flag;} Filter;
typedef struct{u8 present,group,species,kind,type,primary,secondary,category,detail,subtype,rule6,rule7,tier,level,condition3,condition1,reserved16,kind8,code14,code5,mode;s8 signedRule;u8 kind4,kind8high;u8 reserved24[2];u16 number;}Attributes;
typedef struct{u8 prefix[12];void *save;}Field;
typedef char AttributesNumber[(offsetof(Attributes,number)==26)?1:-1];
extern void *Save_VarsFlags_Get(void *);
extern BOOL Save_VarsFlags_CheckFlagInArray(void *,u16);
extern int ov02_02250628(int,int);
extern int ov02_02250594(int,int);
extern int ov02_022506D4(int,int);
extern int ov02_02250738(int,int);
BOOL ov02_0224F108(Field *,const Filter *,const Attributes *);
// @fn ov02_0224F108
BOOL ov02_0224F108(Field *field,const Filter *f,const Attributes *v) {
 u16 flag;u32 attr;
 if((int)LCRandom()%100>=f->chance)return FALSE;
 flag=f->flag; if(flag && !Save_VarsFlags_CheckFlagInArray(Save_VarsFlags_Get(field->save),flag))return FALSE;
 if(f->low3) {if(f->low3==9) {if(!v->present)return FALSE;} else if(f->low3!=v->group)return FALSE;}
 if(f->species && f->species!=v->species)return FALSE;
 if(f->high2) {if(f->high2==7) {if(v->kind!=2&&v->kind!=3&&v->kind!=4&&v->kind!=5&&v->kind!=8)return FALSE;} else if(f->high2!=v->kind)return FALSE;}
 if(f->lowa) {if(f->lowa==5) {if(v->tier<5)return FALSE;} else if(f->lowa!=v->tier)return FALSE;}
 if(f->high16) {if(f->high16==4) {if(v->level<4)return FALSE;} else if(f->high16!=v->level)return FALSE;}
 if(f->bits1 && f->bits1!=v->type)return FALSE;
 attr=f->bits3;if(attr) {if(attr==3&&!v->condition3)return FALSE;if(attr==1&&!v->condition1)return FALSE;if(attr==2)return FALSE;return FALSE;}
 if(f->low4 && f->low4!=v->primary&&f->low4!=v->secondary)return FALSE;
 if(f->low8 && f->low8!=v->kind8)return FALSE;
 if(f->f14 && f->f14!=v->code14)return FALSE;
 if(f->f5 && f->f5!=v->code5)return FALSE;
 if(f->f12 && f->f12-1!=v->number)return FALSE;
 if(f->mida && f->mida!=v->mode)return FALSE;
 if(f->low1 && !ov02_02250628(f->low1,v->signedRule))return FALSE;
 if(f->high4 && f->high4!=v->kind4)return FALSE;
 if(f->high1 && !ov02_02250594(f->high1,v->category))return FALSE;
 if(f->low2 && f->low2!=v->detail)return FALSE;
 if(f->f6 && !ov02_022506D4(f->f6,v->rule6))return FALSE;
 if(f->f7 && !ov02_02250738(f->f7,v->rule7))return FALSE;
 if(f->mid2 && f->mid2!=v->subtype)return FALSE;
 if(f->high8 && f->high8!=v->kind8high)return FALSE;
 return TRUE;
}
