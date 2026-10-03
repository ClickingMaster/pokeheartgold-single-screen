#pragma force_active on
#include "global.h"
extern void GfGfx_EngineATogglePlanes(int,int),BgClearTilemapBufferAndCommit(void *,int),BufferIntegerAsString(void *,int,int,int,int,int),ov74_02235414(void *,void *,int,int,int,int),ov74_02235464(void *,int,int,int),ov74_0223546C(void *,int,int),ov74_02235568(void *,void *,int,int,int),MessageFormat_Delete(void *);
extern void *MessageFormat_New(int);
void ov74_0222ACD8(u32 *);
void ov74_0222ACD8(u32 *w){u32 template[21];int id=w[0x99b];void *format;if(id==-1)id=11;GfGfx_EngineATogglePlanes(16,0);BgClearTilemapBufferAndCommit((void *)w[0],0);format=MessageFormat_New(84);BufferIntegerAsString(format,0,w[0x99c],5,2,1);ov74_02235414(template,(u8 *)w+0x2680,1,800,31,3);ov74_02235464(template,24,16,100);ov74_0223546C(template,0,1);template[5]=(u32)format;ov74_02235568((void *)w[0],template,4,4,id);MessageFormat_Delete(format);}
