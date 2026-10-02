#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 *ov13_02224624(void *,int *,int *);extern const u16 *ov13_02224694(u8 **,u8 *,int *,int *);
static inline u16 swap16(u16 x){s32 value=x;return (u16)((u8)(value>>8)|((value<<8)&0xff00));}
int ov13_02224864(void *,int *);
int ov13_02224864(void *data,int *out){int type;volatile int length;int tag,size;u8 *volatile cursor;u8 *blob=ov13_02224624(data,&type,(int *)&length);int a=0,b,c=0;const u16 *token;int limit;cursor=blob;b=0;if(!blob)return 0;if(type!=1)return 0;limit=length;cursor=blob+8;while((token=ov13_02224694((u8 **)&cursor,blob+limit,&tag,&size))!=0){switch(tag){case 1:a=swap16(*token);break;case 2:b=swap16(*token);break;case 5:c=swap16(*token);break;}}if(a!=1||b!=1)return 0;if(c>=1)*out=1;else *out=0;return 1;}
