#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {s8 left;u8 glyph;s8 charWidth;} Width;
typedef struct WidthBlock {u16 first,last;struct WidthBlock *next;Width widths[1];} WidthBlock;
typedef struct {u8 type;s8 lineFeed;u16 fallback;Width defaultWidth;u8 pad;void *glyph;WidthBlock *width;} FontInfo;
typedef u16 (*Split)(const void **);
typedef struct {FontInfo *info;Split split;} Font;
typedef struct {s32 width,height;} Rect;
extern u16 NNS_G2dFontFindGlyphIndex(Font *,u16);
s32 NNSi_G2dFontGetStringWidth(Font *,s32,const void *,const void **);
s32 NNSi_G2dFontGetTextHeight(Font *,s32,const void *);
void NNSi_G2dFontGetTextRect(Rect *,Font *,s32,s32,const void *);
Width *NNS_G2dFontGetCharWidthsFromIndex(Font *,u16);
Width *NNS_G2dFontGetCharWidthsFromIndex(Font *font,u16 index){WidthBlock *block;FontInfo *info=font->info;block=info->width;while(block){if(block->first<=index && index<=block->last)return &block->widths[index-block->first];block=block->next;}return &info->defaultWidth;}
s32 NNSi_G2dFontGetStringWidth(Font *font,s32 spacing,const void *str,const void **next){const void *cursor=str;s32 width=0;Split split=font->split;u16 c=split(&cursor);while(c){if(c==10)break;u16 index=NNS_G2dFontFindGlyphIndex(font,c);if(index==65535)index=font->info->fallback;Width *w=NNS_G2dFontGetCharWidthsFromIndex(font,index);width+=spacing+w->charWidth;c=split(&cursor);}if(next){*next=(c==10)?cursor:0;}if(width>0)width-=spacing;return width;}
#pragma warn_unusedvar off
s32 NNSi_G2dFontGetTextHeight(Font *font,s32 spacing,const void *str){const void *cursor=str;Rect rect={0,0};s32 lines=1;Split split=font->split;u16 c=split(&cursor);while(c){if(c==10)lines++;c=split(&cursor);}return lines*(spacing+font->info->lineFeed)-spacing;}
void NNSi_G2dFontGetTextRect(Rect *out,Font *font,s32 spaceX,s32 spaceY,const void *str){Rect rect={0,0};s32 lines=1;if(str){do{s32 width=NNSi_G2dFontGetStringWidth(font,spaceX,str,&str);if(width>rect.width)rect.width=width;lines++;}while(str);}rect.height=(lines-1)*(spaceY+font->info->lineFeed)-spaceY;*out=rect;}
