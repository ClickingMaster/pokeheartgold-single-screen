// Merged single-screen view.
//
// The view is built from rectangles cut out of the two DS screens. Usually one
// screen fills it and parts of the other are drawn over it, with chosen graphics
// layers (typically that screen's background) made transparent so only its buttons
// and panels appear. Which pieces are used depends on what the game is doing; see
// pickScene in main.cpp.
#pragma once
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <thread>
#include <vector>
#include <SDL.h>

// Layer classes, derived from the 2D renderer's per-pixel flags.
enum : uint8_t
{
    LAYER_BG0 = 0x01,
    LAYER_BG1 = 0x02,
    LAYER_BG2 = 0x04,
    LAYER_BG3 = 0x08,
    LAYER_OBJ = 0x10,
    LAYER_BACKDROP = 0x20,
    LAYER_3D = 0x40,
};

inline uint8_t LayerClass(uint8_t flag)
{
    if (flag & 0x40) return (flag & 0x80) ? LAYER_OBJ : LAYER_3D;
    if (flag & 0x90) return LAYER_OBJ;
    if (flag & 0x0F) return flag & 0x0F;
    return LAYER_BACKDROP;
}

// One piece of the merged view: a rectangle of one DS screen drawn somewhere in it.
// Positions are in base pixels: the 256x192 area a full screen occupies when the
// view is 4:3. On wider outputs the view extends left and right of that area.
struct Element
{
    int screen;           // 0 = top, 1 = bottom, -1 = a plain rectangle of colour keys[0] (sw x sh)
    int sx, sy, sw, sh;   // source rectangle on that screen
    float dx, dy;         // destination of its top left corner
    float scale;          // destination size per source pixel
    uint8_t drop;         // layer classes that are not drawn
    uint8_t alpha;        // opacity of what is drawn
    bool crisp;           // nearest-neighbour instead of filtered sampling
    // Source pixels at or beyond both of these coordinates are not drawn (cuts out a lower right corner).
    int cutX = 256, cutY = 192;
    // Source pixels of exactly these colours are not drawn (e.g. the ruling of a paper background).
    uint32_t keys[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
    // If set, colours within this distance per channel of keys[0] are not drawn either.
    int keyTolerance = 0;
    // If set, the keys only apply within this many source pixels of the piece's edge (to
    // trim the rim of a rounded button without touching the same colour inside it).
    int keyRing = 0;
    // If set, sprite pixels within this many source pixels of a corner (in both directions)
    // are not drawn: a cursor drawn as sprites at the corners of buttons whose labels are
    // sprites too.
    int dropSpriteCorner = 0;
    // Mirrors the piece left to right / top to bottom (to close a box the game leaves open
    // at the screen's edge with a copy of its other side).
    bool flipX = false, flipY = false;
    // Draws a dark outline around the piece, for a panel that floats over another picture.
    bool border = false;
    uint32_t borderColor = 0x282830;
    int borderWidth = 0;  // in 8ths of a base pixel; 0 = the usual 6
    float borderRadius = 0; // rounds the outline's corners: radius of its inner edge, in base pixels
};

// The columns [x0, x1) a rounded rectangle covers in output row y (pixel centres), or
// x0 >= x1 if none. The rectangle is (rx, ry, rw, rh) with corner radius r.
inline void RoundedRowExtent(int rx, int ry, int rw, int rh, float r, int y, int& x0, int& x1)
{
    x0 = x1 = 0;
    if (y < ry || y >= ry + rh) return;
    float inset = 0;
    const float cy = y + 0.5f;
    float d = -1;
    if (cy < ry + r) d = ry + r - cy;
    else if (cy > ry + rh - r) d = cy - (ry + rh - r);
    if (d > 0) inset = r - std::sqrt(std::max(0.0f, r * r - d * d));
    x0 = rx + (int)(inset + 0.5f);
    x1 = rx + rw - (int)(inset + 0.5f);
}

// Draws one row of an outline of thickness t around (rx, ry, rw, rh) whose inner edge has
// corner radius r: calls fill(xa, xb) for each span of it in output row y.
template <typename F>
inline void OutlineRow(int rx, int ry, int rw, int rh, int t, float r, int y, F fill)
{
    int ox0, ox1, ix0, ix1;
    RoundedRowExtent(rx - t, ry - t, rw + 2 * t, rh + 2 * t, r > 0 ? r + t : 0, y, ox0, ox1);
    if (ox1 <= ox0) return;
    RoundedRowExtent(rx, ry, rw, rh, r, y, ix0, ix1);
    if (ix1 <= ix0) { fill(ox0, ox1); return; }
    if (ix0 > ox0) fill(ox0, ix0);
    if (ox1 > ix1) fill(ix1, ox1);
}

struct Scene
{
    const char* name = "top";
    std::vector<Element> items; // drawn in order, later ones on top
    // Colour behind the pieces: black, or sampled from this pixel of a screen so the
    // parts of the view no piece covers continue that screen's background.
    int bgScreen = -1, bgX = 0, bgY = 0;
    // The first piece is the top screen showing the 3D world: when a widescreen image of
    // that world is available it fills the view, and only the 2D parts of the piece (text
    // boxes and the like) are drawn over it.
    bool wide = false;
    // The first piece is the top screen in a battle: the bars beside it are filled with
    // each row's outermost pixel, darkening outwards, so that the sky, the ground and the
    // HP boxes that run off the screen carry on to the edge of the view instead of
    // stopping at black.
    bool edgeFill = false;
};

// A plain rectangle, for the backing of a group of pieces.
inline Element FillRect(float dx, float dy, int w, int h, uint32_t rgb)
{
    Element e = { -1, 0, 0, w, h, dx, dy, 1.0f, 0, 255, true };
    e.keys[0] = rgb;
    return e;
}

// How dark the edge fill is at the outer edge of the view (in 256ths of full brightness).
enum { EdgeFillOuter = 96 };

// The 3D world drawn across the whole view (see GPU3D_Wide.h), and the brightness the
// game currently applies to its screen (fades), which has to be applied to it as well.
struct WideImage
{
    const uint32_t* pixels = nullptr; // top byte 0 where nothing was drawn
    int w = 0, h = 0;
    int fadeMode = 0;   // 0 none, 1 towards white, 2 towards black
    int fade = 0;       // 0..16
    // Instead of the 3D world across the whole view: the whole top screen widened (see
    // battlewide.h), in base pixels from base x `baseX`, already with its fades. It replaces
    // the first piece, which is not drawn.
    bool placed = false;
    int baseX = 0;
};

struct ScreenImage
{
    const uint32_t* pixels;   // 256x192 XRGB
    const uint8_t* flags;     // 256x192 renderer flags, may be null
};

// Mapping between base pixels and output pixels.
struct MergedView
{
    float unit;       // output pixels per base pixel
    float originX;    // output position of base pixel (0, 0)
    float originY;
    float width;      // width of the whole view in base pixels (256 when the output is 4:3 or narrower)
};

inline MergedView MergedMakeView(int outW, int outH)
{
    MergedView v;
    v.unit = outH / 192.0f;
    if (outW / 256.0f < v.unit) v.unit = outW / 256.0f;
    v.originX = (outW - 256 * v.unit) / 2;
    v.originY = (outH - 192 * v.unit) / 2;
    v.width = outW / v.unit;
    return v;
}

inline Element FullScreen(int screen)
{
    return { screen, 0, 0, 256, 192, 0, 0, 1.0f, 0, 255, true };
}

inline Element Crop(int screen, int sx, int sy, int sw, int sh)
{
    return { screen, sx, sy, sw, sh, 0, 0, 1.0f, 0, 255, false };
}

// Arranges crops as one column (or one row), scaled together to fill the view.
inline Scene Stack(const char* name, std::vector<Element> crops, bool horizontal, float viewWidth)
{
    float w = 0, h = 0;
    for (const Element& e : crops)
    {
        if (horizontal)
        {
            w += e.sw;
            if (e.sh > h) h = (float)e.sh;
        }
        else
        {
            h += e.sh;
            if (e.sw > w) w = (float)e.sw;
        }
    }
    float scale = 192.0f / h;
    if (viewWidth / w < scale) scale = viewWidth / w;
    float x = 128 - w * scale / 2, y = 96 - h * scale / 2;
    Scene sc;
    sc.name = name;
    for (Element e : crops)
    {
        e.scale = scale;
        if (horizontal)
        {
            e.dx = x;
            e.dy = y + (h - e.sh) * scale / 2;
            x += e.sw * scale;
        }
        else
        {
            e.dx = x + (w - e.sw) * scale / 2;
            e.dy = y;
            y += e.sh * scale;
        }
        e.crisp = scale == 1.0f;
        sc.items.push_back(e);
    }
    return sc;
}

// Puts a smaller side panel to the left of a main piece, sized so that together they fill
// the width of a wide view; on a view too narrow for that, stacks the panel above instead.
inline Scene SideBySide(const char* name, Element side, Element mainEl, float viewWidth)
{
    if (viewWidth < 300) return Stack(name, { side, mainEl }, false, viewWidth);
    // The side panel is drawn at 0.7 of the main piece's scale unless there is room for more.
    float sm = viewWidth / (mainEl.sw + 0.7f * side.sw);
    if (192.0f / mainEl.sh < sm) sm = 192.0f / mainEl.sh;
    float ss = (viewWidth - mainEl.sw * sm) / side.sw;
    if (ss > sm) ss = sm;
    float x = 128 - (side.sw * ss + mainEl.sw * sm) / 2;
    side.scale = ss;
    side.dx = x;
    side.dy = 96 - side.sh * ss / 2;
    side.crisp = false;
    mainEl.scale = sm;
    mainEl.dx = x + side.sw * ss;
    mainEl.dy = 96 - mainEl.sh * sm / 2;
    mainEl.crisp = sm == 1.0f;
    Scene sc;
    sc.name = name;
    sc.items = { side, mainEl };
    return sc;
}

// Builds an overlay from whatever is left of a screen once the given layers are
// dropped: finds the bounding box of the remaining pixels (ignoring the corner at and
// beyond cutX, cutY), scales
// it down if it is larger than maxW x maxH, and puts it where it hides the fewest
// pixels of the `protect` layers on the screen underneath. Returns false if nothing
// is left to draw.
inline bool SmartOverlay(const ScreenImage screens[2], int screen, int cutX, int cutY, uint8_t drop,
    float maxW, float maxH, uint8_t protect, uint8_t alpha, Element& out)
{
    const uint8_t* flags = screens[screen].flags;
    if (!flags) return false;
    int x0 = 256, y0 = 192, x1 = -1, y1 = -1;
    for (int y = 0; y < 192; y++)
        for (int x = 0; x < 256; x++)
            if (!(x >= cutX && y >= cutY) && !(LayerClass(flags[y * 256 + x]) & drop))
            {
                if (x < x0) x0 = x;
                if (x > x1) x1 = x;
                if (y < y0) y0 = y;
                if (y > y1) y1 = y;
            }
    if (x1 < x0 || y1 < y0) return false;
    // Snap outwards to a coarse grid so small animations do not move the overlay.
    x0 &= ~7;
    y0 &= ~7;
    x1 = (x1 | 7) < 255 ? (x1 | 7) : 255;
    y1 = (y1 | 7) < 191 ? (y1 | 7) : 191;
    int sw = x1 - x0 + 1, sh = y1 - y0 + 1;

    float fit = 1.0f;
    if (maxW / sw < fit) fit = maxW / sw;
    if (maxH / sh < fit) fit = maxH / sh;

    // Candidate anchors: a 3x3 grid, scored by how much protected content they cover.
    // If every position at full size hides a lot, smaller sizes are tried as well.
    const uint8_t* under = screens[1 - screen].flags;
    float best = 1e30f, bestX = 0, bestY = 0, scale = fit;
    static const float shrink[] = { 1.0f, 0.8f, 0.65f };
    for (float k : shrink)
    {
        float sc = fit * k, w = sw * sc, h = sh * sc;
        for (int ay = 0; ay < 3; ay++)
            for (int ax = 0; ax < 3; ax++)
            {
                float px = (256 - w) * ax / 2, py = (192 - h) * ay / 2;
                float cost = 0;
                if (under)
                    for (int y = (int)py; y < (int)(py + h) && y < 192; y += 2)
                        for (int x = (int)px; x < (int)(px + w) && x < 256; x += 2)
                            if (LayerClass(under[y * 256 + x]) & protect) cost += 1;
                // Mild preferences: larger, to the right, and off the centre.
                cost += (1.0f - k) * 40 + (2 - ax) * 0.5f + (ax == 1 && ay == 1 ? 2.0f : 0.0f);
                if (cost < best)
                {
                    best = cost;
                    bestX = px;
                    bestY = py;
                    scale = sc;
                }
            }
    }
    out = { screen, x0, y0, sw, sh, bestX, bestY, scale, drop, alpha, false, cutX, cutY };
    return true;
}

inline bool MergedKeyed(const Element& el, uint32_t rgb)
{
    if (rgb == el.keys[0] || rgb == el.keys[1] || rgb == el.keys[2] || rgb == el.keys[3]) return true;
    if (!el.keyTolerance) return false;
    int t = el.keyTolerance, k = (int)el.keys[0];
    return std::abs((int)((rgb >> 16) & 0xFF) - ((k >> 16) & 0xFF)) <= t &&
        std::abs((int)((rgb >> 8) & 0xFF) - ((k >> 8) & 0xFF)) <= t && std::abs((int)(rgb & 0xFF) - (k & 0xFF)) <= t;
}

// With `smooth` off every piece is drawn with nearest-pixel sampling (hard edges, and fast);
// with it on, pieces that are not marked crisp are filtered. The rows of the picture are
// shared out between a few threads.
inline void MergedCompose(const Scene& scene, const ScreenImage screens[2], int outW, int outH, std::vector<uint32_t>& out,
    bool smooth = false, const WideImage* wide = nullptr)
{
    uint32_t bg = 0xFF000000;
    if (scene.bgScreen >= 0) bg |= screens[scene.bgScreen].pixels[scene.bgY * 256 + scene.bgX] & 0xFFFFFF;
    const bool useWide = scene.wide && wide && wide->pixels && wide->w > 0 && wide->h > 0;
    out.resize((size_t)outW * outH);
    const MergedView v = MergedMakeView(outW, outH);

    const bool placed = useWide && wide->placed;
    std::vector<int> wcol;
    if (useWide)
    {
        wcol.resize(outW);
        for (int x = 0; x < outW; x++)
            wcol[x] = placed ? (int)std::floor((x + 0.5f - v.originX) / v.unit) - wide->baseX : (int)((int64_t)x * wide->w / outW);
    }

    // Each piece with its place in the output and, for nearest-pixel drawing, the source
    // column of every output column.
    struct Piece
    {
        Element el;
        float s;
        int x0, y0, w, h, xa, xb, border;
        bool nearest;
        std::vector<int> col;
    };
    std::vector<Piece> pieces;
    pieces.reserve(scene.items.size());
    for (const Element& item : scene.items)
    {
        if (placed && &item == &scene.items[0]) continue;
        Piece p;
        p.el = item;
        // Over a widescreen world only the 2D parts of the top screen are drawn.
        if (useWide && !placed && pieces.empty()) p.el.drop |= LAYER_3D;
        p.s = v.unit * p.el.scale;
        p.x0 = (int)(v.originX + p.el.dx * v.unit);
        p.y0 = (int)(v.originY + p.el.dy * v.unit);
        p.w = (int)(p.el.sw * p.s + 0.5f);
        p.h = (int)(p.el.sh * p.s + 0.5f);
        p.border = 0;
        if (p.el.border)
        {
            p.border = (int)(v.unit * (p.el.borderWidth ? p.el.borderWidth : 6) / 8.0f + 0.5f);
            if (p.border < 1) p.border = 1;
        }
        p.nearest = p.el.crisp || !smooth;
        p.xa = p.x0 < 0 ? -p.x0 : 0;
        p.xb = p.x0 + p.w > outW ? outW - p.x0 : p.w;
        if (p.nearest && p.xb > p.xa)
        {
            p.col.resize(p.w);
            for (int x = p.xa; x < p.xb; x++)
            {
                int sx = (int)((x + 0.5f) / p.s);
                if (sx >= p.el.sw) sx = p.el.sw - 1;
                p.col[x] = p.el.sx + (p.el.flipX ? p.el.sw - 1 - sx : sx);
            }
        }
        pieces.push_back(std::move(p));
    }

    auto rows = [&](int ya, int yb) {
        // Background: one colour, or the widescreen 3D picture with the game's fade applied.
        if (!useWide)
        {
            std::fill(out.begin() + (size_t)ya * outW, out.begin() + (size_t)yb * outW, bg);
            const ScreenImage& top = screens[scene.items.empty() || scene.items[0].screen < 0 ? 0 : scene.items[0].screen];
            const int left = (int)v.originX, right = (int)(v.originX + 256 * v.unit);
            if (scene.edgeFill && left > 0)
                for (int y = ya; y < yb; y++)
                {
                    int sy = (int)((y - v.originY) / v.unit);
                    if (sy < 0 || sy >= 192) continue;
                    uint32_t* dst = &out[(size_t)y * outW];
                    for (int side = 0; side < 2; side++)
                    {
                        const uint32_t c = top.pixels[sy * 256 + (side ? 255 : 0)];
                        const int x0 = side ? right : 0, x1 = side ? outW : left;
                        for (int x = x0; x < x1; x++)
                        {
                            // 256 beside the screen, EdgeFillOuter at the edge of the view.
                            const int d = side ? x - right : left - 1 - x, span = side ? outW - right : left;
                            const uint32_t k = 256 - (256 - EdgeFillOuter) * d / (span > 1 ? span - 1 : 1);
                            dst[x] = 0xFF000000 | ((((c >> 16) & 0xFF) * k >> 8) << 16) | ((((c >> 8) & 0xFF) * k >> 8) << 8) | ((c & 0xFF) * k >> 8);
                        }
                    }
                }
        }
        else if (placed)
        {
            for (int y = ya; y < yb; y++)
            {
                uint32_t* dst = &out[(size_t)y * outW];
                const int sy = (int)std::floor((y + 0.5f - v.originY) / v.unit);
                if (sy < 0 || sy >= wide->h)
                {
                    std::fill(dst, dst + outW, 0xFF000000);
                    continue;
                }
                const uint32_t* row = wide->pixels + (size_t)sy * wide->w;
                for (int x = 0; x < outW; x++)
                {
                    const int c = wcol[x];
                    dst[x] = (c >= 0 && c < wide->w) ? (0xFF000000 | row[c]) : 0xFF000000;
                }
            }
        }
        else
        {
            const int f = wide->fade, mode = wide->fadeMode;
            for (int y = ya; y < yb; y++)
            {
                const uint32_t* row = wide->pixels + (size_t)((int64_t)y * wide->h / outH) * wide->w;
                uint32_t* dst = &out[(size_t)y * outW];
                if (!mode || !f)
                    for (int x = 0; x < outW; x++)
                    {
                        uint32_t c = row[wcol[x]];
                        dst[x] = (c >> 24) ? (0xFF000000 | c) : 0xFF000000;
                    }
                else
                    for (int x = 0; x < outW; x++)
                    {
                        uint32_t c = row[wcol[x]];
                        if (!(c >> 24)) c = 0;
                        uint32_t r = (c >> 16) & 0xFF, g = (c >> 8) & 0xFF, b = c & 0xFF;
                        if (mode == 1)
                        {
                            r += (255 - r) * f / 16;
                            g += (255 - g) * f / 16;
                            b += (255 - b) * f / 16;
                        }
                        else
                        {
                            r = r * (16 - f) / 16;
                            g = g * (16 - f) / 16;
                            b = b * (16 - f) / 16;
                        }
                        dst[x] = 0xFF000000 | (r << 16) | (g << 8) | b;
                    }
                // Where something 2D reaches an edge of the top screen (a wipe closing in, a
                // flash, a full-screen cover), it is continued to the edge of the view;
                // otherwise the world would still show beside it.
                const ScreenImage& top = screens[scene.items[0].screen];
                if (top.flags && (scene.items[0].drop & 0x7F) != 0x7F)
                {
                    int sy = (int)((y - v.originY) / v.unit);
                    if (sy >= 0 && sy < 192)
                    {
                        const int left = (int)v.originX, right = (int)(v.originX + 256 * v.unit);
                        if (LayerClass(top.flags[sy * 256]) != LAYER_3D)
                            std::fill(dst, dst + std::min(left, outW), 0xFF000000 | top.pixels[sy * 256]);
                        if (LayerClass(top.flags[sy * 256 + 255]) != LAYER_3D && right < outW)
                            std::fill(dst + std::max(right, 0), dst + outW, 0xFF000000 | top.pixels[sy * 256 + 255]);
                    }
                }
            }
        }

        for (const Piece& p : pieces)
        {
            const Element& el = p.el;
            const int x0 = p.x0, y0 = p.y0, w = p.w, h = p.h;
            if (p.border)
            {
                const int t = p.border;
                const float r = el.borderRadius * v.unit;
                for (int y = std::max(ya, y0 - t); y < std::min(yb, y0 + h + t); y++)
                    OutlineRow(x0, y0, w, h, t, r, y, [&](int xa, int xb) {
                        for (int x = std::max(xa, 0); x < std::min(xb, outW); x++) out[(size_t)y * outW + x] = 0xFF000000 | el.borderColor;
                    });
            }
            if (el.screen < 0)
            {
                // A rectangle of one colour, with its corners rounded like its outline.
                const float r = el.borderRadius * v.unit;
                for (int y = std::max(ya, y0); y < std::min(yb, y0 + h); y++)
                {
                    int xa = x0, xb = x0 + w;
                    if (r > 0) RoundedRowExtent(x0, y0, w, h, r, y, xa, xb);
                    for (int x = std::max(0, xa); x < std::min(outW, xb); x++)
                        out[(size_t)y * outW + x] = 0xFF000000 | el.keys[0];
                }
                continue;
            }
            const ScreenImage& src = screens[el.screen];
            const float s = p.s;

            const int oya = std::max(ya, y0), oyb = std::min(yb, y0 + h);
            if (p.nearest)
            {
                // Nearest-pixel path: one table lookup per output pixel. This is the path used
                // unless smooth scaling is on, and it has to stay cheap at 1440p and above.
                const int xa = p.xa, xb = p.xb;
                if (xb <= xa) continue;
                const int* col = p.col.data();
                const bool keyed = el.keys[0] != 0xFFFFFFFF;
                const bool spriteKey = el.dropSpriteCorner > 0 && src.flags;
                const bool plain = !(el.drop && src.flags) && !keyed && !spriteKey && el.alpha == 255 && el.cutX >= 256;
                for (int oy = oya; oy < oyb; oy++)
                {
                    int sy = (int)((oy - y0 + 0.5f) / s);
                    if (sy >= el.sh) sy = el.sh - 1;
                    if (el.flipY) sy = el.sh - 1 - sy;
                    sy += el.sy;
                    const uint32_t* row = src.pixels + sy * 256;
                    const uint8_t* frow = src.flags ? src.flags + sy * 256 : nullptr;
                    uint32_t* dst = out.data() + (ptrdiff_t)oy * outW + x0;
                    if (plain)
                    {
                        for (int x = xa; x < xb; x++) dst[x] = 0xFF000000 | row[col[x]];
                        continue;
                    }
                    for (int x = xa; x < xb; x++)
                    {
                        int sx = col[x];
                        if (sx >= el.cutX && sy >= el.cutY) continue;
                        if (el.drop && frow && (LayerClass(frow[sx]) & el.drop)) continue;
                        uint32_t c = row[sx];
                        if (spriteKey && LayerClass(frow[sx]) == LAYER_OBJ &&
                            std::min(sx - el.sx, el.sx + el.sw - 1 - sx) < el.dropSpriteCorner &&
                            std::min(sy - el.sy, el.sy + el.sh - 1 - sy) < el.dropSpriteCorner)
                            continue;
                        if (keyed && MergedKeyed(el, c & 0xFFFFFF) &&
                            (!el.keyRing || std::min({ sx - el.sx, el.sx + el.sw - 1 - sx, sy - el.sy, el.sy + el.sh - 1 - sy }) < el.keyRing))
                            continue;
                        if (el.alpha == 255)
                            dst[x] = 0xFF000000 | c;
                        else
                        {
                            uint32_t d = dst[x], al = el.alpha, ia = 255 - al;
                            uint32_t r = (((c >> 16) & 0xFF) * al + ((d >> 16) & 0xFF) * ia) / 255;
                            uint32_t g = (((c >> 8) & 0xFF) * al + ((d >> 8) & 0xFF) * ia) / 255;
                            uint32_t bl = ((c & 0xFF) * al + (d & 0xFF) * ia) / 255;
                            dst[x] = 0xFF000000 | (r << 16) | (g << 8) | bl;
                        }
                    }
                }
                continue;
            }

            for (int oy = oya; oy < oyb; oy++)
            {
                // Filtered sample position in source pixels, centred.
                float fy = (oy - y0 + 0.5f) / s - 0.5f;
                if (fy < 0) fy = 0;
                int iy = (int)fy;
                float wy = fy - iy;
                int iy1 = iy + 1 < el.sh ? iy + 1 : iy;
                for (int x = 0; x < w; x++)
                {
                    int ox = x0 + x;
                    if (ox < 0 || ox >= outW) continue;
                    float fx = (x + 0.5f) / s - 0.5f;
                    if (fx < 0) fx = 0;
                    int ix = (int)fx;
                    float wx = fx - ix;
                    int ix1 = ix + 1 < el.sw ? ix + 1 : ix;

                    float acc[3] = { 0, 0, 0 }, cover = 0;
                    const int px[4] = { ix, ix1, ix, ix1 };
                    const int py[4] = { iy, iy, iy1, iy1 };
                    const float wt[4] = { (1 - wx) * (1 - wy), wx * (1 - wy), (1 - wx) * wy, wx * wy };
                    for (int k = 0; k < 4; k++)
                    {
                        int srcX = el.sx + (el.flipX ? el.sw - 1 - px[k] : px[k]);
                        int srcY = el.sy + (el.flipY ? el.sh - 1 - py[k] : py[k]);
                        if (srcX >= el.cutX && srcY >= el.cutY) continue;
                        int si = srcY * 256 + srcX;
                        if (el.drop && src.flags && (LayerClass(src.flags[si]) & el.drop)) continue;
                        uint32_t c = src.pixels[si];
                        if (MergedKeyed(el, c & 0xFFFFFF)) continue;
                        acc[0] += ((c >> 16) & 0xFF) * wt[k];
                        acc[1] += ((c >> 8) & 0xFF) * wt[k];
                        acc[2] += (c & 0xFF) * wt[k];
                        cover += wt[k];
                    }
                    if (cover <= 0.001f) continue;

                    uint32_t& d = out[(size_t)oy * outW + ox];
                    float a = cover * el.alpha / 255.0f;
                    float dr = ((d >> 16) & 0xFF) * (1 - a) + (acc[0] / cover) * a;
                    float dg = ((d >> 8) & 0xFF) * (1 - a) + (acc[1] / cover) * a;
                    float db = (d & 0xFF) * (1 - a) + (acc[2] / cover) * a;
                    d = 0xFF000000 | ((uint32_t)dr << 16) | ((uint32_t)dg << 8) | (uint32_t)db;
                }
            }
        }
    };

    // Small pictures are not worth starting threads for.
    int threads = (size_t)outW * outH < 400000 ? 1 : (int)std::thread::hardware_concurrency() / 2;
    if (threads < 1) threads = 1;
    if (threads > 6) threads = 6;
    std::vector<std::thread> pool;
    for (int i = 1; i < threads; i++) pool.emplace_back(rows, outH * i / threads, outH * (i + 1) / threads);
    rows(0, outH / threads);
    for (auto& t : pool) t.join();
}

// Maps a position in base pixels to touchscreen coordinates, through whichever
// bottom-screen element is topmost there.
inline bool MergedTouch(const Scene& scene, float bx, float by, int& tx, int& ty)
{
    for (int i = (int)scene.items.size() - 1; i >= 0; i--)
    {
        const Element& el = scene.items[i];
        float lx = (bx - el.dx) / el.scale, ly = (by - el.dy) / el.scale;
        if (lx < 0 || ly < 0 || lx >= el.sw || ly >= el.sh) continue;
        if (el.screen != 1) continue; // top-screen elements and plain rectangles are not touchable but do not block
        tx = el.sx + (int)lx;
        ty = el.sy + (int)ly;
        return true;
    }
    return false;
}
