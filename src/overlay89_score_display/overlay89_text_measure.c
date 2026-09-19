#include "private.h"

void ov89_0225A368(int font, void *string, int *widthOut, int *tilesOut)
{
    int width = FontID_String_GetWidth(string, font, 0);
    int tiles = width / 8;
    if (FX_ModS32(width, 8) != 0) {
        tiles++;
    }
    *widthOut = width;
    *tilesOut = tiles;
}
