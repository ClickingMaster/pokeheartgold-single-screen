#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    void *info;
    u16 (*decode)(const void **);
} Fnt;

typedef struct {
    void *canvas;
    Fnt *font;
    int charSpace, lineSpace;
} TC;

typedef struct {
    s8 x, y;
} Dir;

typedef struct {
    int width, height;
} Rect;

void NNS_G2dMapScrToChar256x16Pltt(u16 *, int, int, int, int, int);
int NNS_G2dCharCanvasDrawChar(void *, Fnt *, int, int, int, u16);
int NNSi_G2dFontGetStringWidth(Fnt *, int, const void *, void *);
int NNSi_G2dFontGetTextHeight(Fnt *, int, const void *);
Rect NNSi_G2dFontGetTextRect(Fnt *, int, int, const void *);
void NNSi_G2dTextCanvasDrawString(TC *, int, int, int, const void *, const void **, Dir);
void NNSi_G2dTextCanvasDrawTextAlign(TC *, int, int, int, int, int, const void *, Dir);
void NNS_G2dMapScrToCharText(u16 *, int, int, int, int, int, int, int);
u32 NNSi_G2dCalcRequiredOBJ(u32, u32);
void NNSi_G2dTextCanvasDrawText(TC *, int, int, int, int, const void *, Dir);
void NNSi_G2dTextCanvasDrawTextRect(TC *, int, int, int, int, int, int, const void *, Dir);

static inline Rect getrect(TC *tc, const void *text) {
    Rect r = NNSi_G2dFontGetTextRect(tc->font, tc->charSpace, tc->lineSpace, text);
    return r;
}

typedef struct {
    u32 signature;
    u16 endian, version;
} FontHeader;

typedef struct {
    u8 pad[8];
    u8 *glyph;
} FontInfo;

BOOL NNSi_G2dGetUnpackedFont(FontHeader *, FontInfo **);
void NNSi_G2dUnpackNFT(FontHeader *);
void *NNS_G2dFindBinaryBlock(FontHeader *, u32);

static inline int signature(FontHeader *p) {
    if (p && p->signature == 0x4e465452) {
        return TRUE;
    }
    return FALSE;
}

static inline int version(FontHeader *p, u16 v) {
    if (p && p->version >= v) {
        return TRUE;
    }
    return FALSE;
}

static inline int valid(FontHeader *p, u16 v) {
    if (p) {
        int ok = (BOOL)(signature(p) != 0);
        if (ok) {
            ok = (BOOL)(version(p, v) != 0);
        }
        return ok;
    }
    return 0;
}

void NNSi_G2dTextCanvasDrawText(TC *tc, int x, int y, int color, int flags, const void *text, Dir dir) {
    Rect size = getrect(tc, text);
    if (flags & 0x10) {
        int d = -(size.width + 1) / 2;
        x += d * dir.x;
        y += d * dir.y;
    } else if (flags & 0x20) {
        int d = -size.width;
        x += d * dir.x;
        y += d * dir.y;
    }
    if (flags & 2) {
        int d = -(size.height + 1) / 2;
        x += d * -dir.y;
        y += d * dir.x;
    } else if (flags & 4) {
        int d = -size.height;
        x += d * -dir.y;
        y += d * dir.x;
    }
    NNSi_G2dTextCanvasDrawTextAlign(tc, x, y, size.width, color, flags, text, dir);
}

void NNSi_G2dTextCanvasDrawTextRect(TC *tc, int x, int y, int width, int height, int color, int flags, const void *text, Dir dir) {
    if (flags & 0x100) {
        int d = height - NNSi_G2dFontGetTextHeight(tc->font, tc->lineSpace, text);
        x += d * -dir.y;
        y += d * dir.x;
    } else if (flags & 0x80) {
        int d = (height + 1) / 2 - (NNSi_G2dFontGetTextHeight(tc->font, tc->lineSpace, text) + 1) / 2;
        x += d * -dir.y;
        y += d * dir.x;
    }
    NNSi_G2dTextCanvasDrawTextAlign(tc, x, y, width, color, flags, text, dir);
}

BOOL NNSi_G2dGetUnpackedFont(FontHeader *header, FontInfo **out) {
    BOOL old = FALSE;
    void *block;
    if (!valid(header, 0x101)) {
        if (!valid(header, 0x100)) {
            OS_Terminate();
        }
        old = TRUE;
    }
    NNSi_G2dUnpackNFT(header);
    block = NNS_G2dFindBinaryBlock(header, 0x46494e46);
    if (!block) {
        *out = NULL;
        return FALSE;
    }
    *out = (FontInfo *)((u8 *)block + 8);
    if (old) {
        (*out)->glyph[7] = 0;
    }
    return TRUE;
}
