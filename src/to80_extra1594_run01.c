#pragma force_active on
#include "global.h"
#define P(d,n) ((char*)(d)+(n))
#define W(d,n) (*(void**)P(d,n))
#define B(d,n) (*(u8*)P(d,n))
#define H(d,n) (*(u16*)P(d,n))
#define S(d,n) (*(s16*)P(d,n))
#define F(d,n) (*(float*)P(d,n))
typedef struct Vector{int x,y,z;} Vector;
void*PokeathlonCourse_GetHeapAllocPtr4(void*);void*PokeathlonCourse_GetDataCopyArea(void*);u8 *ov96_021E8A20(void *ptr);
void ov96_021FAFFC(void*,void*);void*PokeathlonCourseSpritePoolSprite_GetSprite(void*);void PokeathlonCourseSpritePoolSprite_Show(void*,int,int);int ov96_021E5F24(void*);
void Sprite_SetAffineScale(void*,const Vector*);void Sprite_SetAnimCtrlSeq(void*,int);Vector*Sprite_GetMatrixPtr(void*);void Sprite_SetMatrix(void*,const Vector*);Vector*Sprite_GetScalePtr(void*);void ov96_021FB994(void*);
void ov96_021FB0F4(void*);
void ov96_021FB0F4(void*course){void*copy;void*d=PokeathlonCourse_GetHeapAllocPtr4(course);copy=(void *)ov96_021E8A20(P(PokeathlonCourse_GetDataCopyArea(course),0xf0));
 if(B(d,0x3c7))H(d,0x3c4)++;
 if(!B(d,0x3c6)){
 if(H(d,0x3c4)>=150){u8 i;H(d,0x3c4)=0;B(d,0x3c6)=1;ov96_021FAFFC(d,copy);
 for(i=0;i<3;i++){char*entry=P(d,i*0x6c);void*sprite=PokeathlonCourseSpritePoolSprite_GetSprite(W(entry,0x128));u8 a,b;Vector scale;
 PokeathlonCourseSpritePoolSprite_Show(W(entry,0x128),1,1);
 a=(ov96_021E5F24(course)*3+i)/2;b=(ov96_021E5F24(course)*3+i)%2;b=(u8)((B(P(copy,a),0x1c)>>(b*4))&15);
 GF_ASSERT(b>0&&b<=12);F(entry,0x144)=1.5f;
 scale.x=(int)(4096.0f*F(entry,0x144));scale.y=(int)(4096.0f*F(entry,0x144));scale.z=(int)(4096.0f*F(entry,0x144));
 H(entry,0x148)=b;Sprite_SetAffineScale(sprite,&scale);Sprite_SetAnimCtrlSeq(sprite,b-1);
 }
 }
 }else if(H(d,0x3c4)>=30){u8 i;H(d,0x3c4)=0;ov96_021FAFFC(d,copy);
 for(i=0;i<3;i++){char*entry=P(d,i*0x6c);void*sprite=PokeathlonCourseSpritePoolSprite_GetSprite(W(entry,0x128));u8 a,b;
 a=(ov96_021E5F24(course)*3+i)/2;b=(ov96_021E5F24(course)*3+i)%2;b=(u8)((B(P(copy,a),0x1c)>>(b*4))&15);
 GF_ASSERT(b>0&&b<=12);
 if(B(entry,0xe8)){Vector matrix=*Sprite_GetMatrixPtr(sprite);matrix.y=0x138000;Sprite_SetMatrix(sprite,&matrix);Sprite_SetAnimCtrlSeq(sprite,b+11);}
 if(b!=S(entry,0x148)){H(entry,0x148)=b;if(!B(entry,0xe8)){Vector scale;F(entry,0x144)=1.5f;scale.x=(int)(4096.0f*F(entry,0x144));scale.y=(int)(4096.0f*F(entry,0x144));scale.z=(int)(4096.0f*F(entry,0x144));Sprite_SetAffineScale(sprite,&scale);Sprite_SetAnimCtrlSeq(sprite,b-1);}}
 }
 }
 {char*entry;u8 i;for(i=0;i<3;i++){entry=P(d,i*0x6c);void*sprite=PokeathlonCourseSpritePoolSprite_GetSprite(W(entry,0x128));Vector scale;
 if(Sprite_GetScalePtr(sprite)->x>4096)F(entry,0x144)=(float)((double)F(entry,0x144)-0.1);else F(entry,0x144)=1.0f;
 scale.x=(int)(4096.0f*F(entry,0x144));scale.y=(int)(4096.0f*F(entry,0x144));scale.z=(int)(4096.0f*F(entry,0x144));Sprite_SetAffineScale(sprite,&scale);
 }}
 ov96_021FB994(d);
}
