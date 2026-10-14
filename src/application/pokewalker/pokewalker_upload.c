// Pokewalker connection app: the data sent to the Pokewalker. The upload image (graphics the Pokewalker shows,
// with the player's name), the stroll data (the Pokemon, its course, the course's Pokemon and items with their
// pictures), and the conversion of rendered text and pictures to the Pokewalker's 2bpp format.

#include "global.h"

#include "application/pokewalker/pokewalker_app.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0274.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_rtc.h"
#include "heap.h"
#include "math_util.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "text.h"
#include "char_tiles.h"

// Bitmaps of the upload image (asm data).
extern const u8 sPokewalkerUploadImageArrowRightShifted[];
extern const u8 sPokewalkerUploadImageArrowDownShifted[];
extern const u8 sPokewalkerUploadImageArrowRightHollow[];
extern const u8 ov112_021F55B8[];
extern const u8 sPokewalkerUploadImageArrowLeftHollow[];
extern const u8 sPokewalkerUploadImageArrowLeft[];
extern const u8 sPokewalkerUploadImageArrowDownHollow[];
extern const u8 sPokewalkerUploadImageArrowUpHollow[];
extern const u8 ov112_021F5608[];
extern const u8 ov112_021F5618[];
extern const u8 ov112_021F5628[];
extern const u8 ov112_021F5638[];
extern const u8 ov112_021F5648[];
extern const u8 sPokewalkerUploadImageMusicNote[];
extern const u8 ov112_021F5668[];
extern const u8 ov112_021F5678[];
extern const u8 sPokewalkerUploadImageArrowLeftShifted[];
extern const u8 sPokewalkerUploadImageArrowDown[];
extern const u8 ov112_021F56A8[];
extern const u8 sPokewalkerUploadImageArrowRight[];
extern const u8 sPokewalkerUploadImageArrowUp[];
extern const u8 sPokewalkerUploadImageHeart[];
extern const u8 ov112_021F56E8[];
extern const u8 sPokewalkerUploadImagePokeBall[];
extern const u8 ov112_021F5708[];
extern const u8 sPokewalkerUploadImageArrowUpShifted[];
extern const u8 ov112_021F5738[];
extern const u8 ov112_021F5748[];
extern const u8 sPokewalkerUploadImageDigit0[];
extern const u8 sPokewalkerUploadImageDigit2[];
extern const u8 sPokewalkerUploadImageDigit6[];
extern const u8 sPokewalkerUploadImageDigit8[];
extern const u8 ov112_021F57D8[];
extern const u8 sPokewalkerUploadImageSlash[];
extern const u8 sPokewalkerUploadImageColon[];
extern const u8 sPokewalkerUploadImageDigit4[];
extern const u8 sPokewalkerUploadImageDigit1[];
extern const u8 sPokewalkerUploadImageDigit7[];
extern const u8 ov112_021F5898[];
extern const u8 sPokewalkerUploadImageDigit9[];
extern const u8 ov112_021F58D8[];
extern const u8 ov112_021F58F8[];
extern const u8 sPokewalkerUploadImageDigit3[];
extern const u8 sPokewalkerUploadImageDash[];
extern const u8 ov112_021F5958[];
extern const u8 sPokewalkerUploadImageDigit5[];
extern const u8 ov112_021F5998[];
extern const u8 ov112_021F59D8[];
extern const u8 ov112_021F5A18[];
extern const u8 ov112_021F5A58[];
extern const u8 ov112_021F5A98[];
extern const u8 ov112_021F5AD8[];
extern const u8 ov112_021F5B18[];
extern const u8 ov112_021F5B58[];
extern const u8 ov112_021F5B98[];
extern const u8 ov112_021F5BD8[];
extern const u8 ov112_021F5C18[];
extern const u8 ov112_021F5C58[];
extern const u8 ov112_021F5C98[];
extern const u8 ov112_021F5CD8[];
extern const u8 ov112_021F5D18[];
extern const u8 ov112_021F5D58[];
extern const u8 ov112_021F5DB8[];
extern const u8 ov112_021F5E18[];
extern const u8 ov112_021F5E78[];
extern const u8 ov112_021F5ED8[];
extern const u8 ov112_021F5F38[];
extern const u8 ov112_021F5F98[];
extern const u8 ov112_021F5FF8[];
extern const u8 ov112_021F6058[];
extern const u8 ov112_021F60B8[];
extern const u8 ov112_021F6118[];
extern const u8 ov112_021F6178[];
extern const u8 ov112_021F61F8[];
extern const u8 ov112_021F6278[];
extern const u8 ov112_021F62F8[];
extern const u8 ov112_021F6378[];
extern const u8 ov112_021F63F8[];
extern const u8 ov112_021F6498[];
extern const u8 ov112_021F6538[];
extern const u8 ov112_021F65D8[];
extern const u8 ov112_021F6678[];
extern const u8 ov112_021F6718[];
extern const u8 ov112_021F67D8[];
extern const u8 ov112_021F6898[];
extern const u8 ov112_021F6DD8[];
extern const u8 ov112_021F7018[];
extern const u8 ov112_021F70D8[];
extern const u8 ov112_021F71D8[];
extern const u8 ov112_021F72D8[];
extern const u8 ov112_021F7418[];
extern const u8 ov112_021F7558[];
extern const u8 ov112_021F7698[];
extern const u8 ov112_021F77D8[];
extern const u8 ov112_021F7918[];
extern const u8 ov112_021F7A58[];
extern const u8 ov112_021F7B98[];
extern const u8 ov112_021F7D18[];
extern const u8 ov112_021F7E98[];
extern const u8 ov112_021F8018[];
extern const u8 ov112_021F8198[];
extern const u8 ov112_021F8318[];
extern const u8 ov112_021F8498[];
extern const u8 ov112_021F8618[];
extern const u8 ov112_021F8798[];
extern const u8 ov112_021F8918[];
extern const u8 ov112_021F8A98[];
extern const u8 ov112_021F8C18[];
extern const u8 ov112_021F8D98[];
extern const u8 ov112_021F8F18[];
extern const u8 ov112_021F9098[];
extern const u8 ov112_021F9218[];
extern const u8 ov112_021F9398[];
extern const u8 ov112_021F9518[];
extern const u8 ov112_021F9698[];
extern const u8 ov112_021F9818[];
extern const u8 ov112_021F9998[];
extern const u8 ov112_021F9B18[];
extern const u8 ov112_021F9C98[];
extern const u8 ov112_021F9E18[];
extern const u8 ov112_021F9F98[];
extern const u8 ov112_021FA118[];
extern const u8 ov112_021FA298[];
extern const u8 ov112_021FA418[];
extern const u8 ov112_021FA598[];
extern const u8 ov112_021FA718[];
extern const u8 ov112_021FA898[];
extern const u8 ov112_021FAA18[];
extern const u8 ov112_021FAB98[];
extern const u8 ov112_021FAD18[];
extern const u8 ov112_021FAE98[];
extern const u8 ov112_021FB018[];
extern const u8 ov112_021FB198[];
extern const u8 ov112_021FB318[];
extern const u8 ov112_021FB498[];
extern const u8 ov112_021FB618[];
extern const u8 ov112_021FB798[];
extern const u8 ov112_021FB918[];
extern const u8 ov112_021FBA98[];
extern const u8 ov112_021FBC18[];
extern const u8 ov112_021FBD98[];
extern const u8 ov112_021FBF18[];
extern const u8 ov112_021FC098[];
extern const u8 ov112_021FC218[];
extern const u8 ov112_021FC398[];
extern const u8 ov112_021FC518[];
extern const u8 ov112_021FC698[];
extern const u8 ov112_021FC818[];
extern const u8 ov112_021FC998[];
extern const u8 ov112_021FCB18[];
extern const u8 ov112_021FCC98[];
extern const u8 ov112_021FCE18[];
extern const u8 ov112_021FCF98[];
extern const u8 ov112_021FD118[];
extern const u8 ov112_021FD298[];
extern const u8 ov112_021FD418[];
extern const u8 ov112_021FD598[];
extern const u8 ov112_021FD718[];
extern const u8 ov112_021FD898[];
extern const u8 ov112_021FDA18[];
extern const u8 ov112_021FDB98[];
extern const u8 ov112_021FDD18[];
extern const u8 ov112_021FDE98[];
extern const u8 ov112_021FE018[];
extern const u8 ov112_021FE198[];

// The routeImageId column of the course table has its own label.
typedef struct PokewalkerCourseImageColumn {
    u32 routeImageId;
    u8 unk_04[0xBC];
} PokewalkerCourseImageColumn;
extern const PokewalkerCourseImageColumn sPokewalkerCourseRouteImageIds[];
extern const u8 sStrollTextWindowWidths[];
extern const u8 sSpindaSpotMask[]; // where Spinda's spots may be drawn (2bpp picture)
extern const void *sPokewalkerRouteImages[];

void PokewalkerApp_WriteUploadBlocks(u8 *dst);

void PokewalkerApp_BuildUploadImage(PokewalkerApp *app, u8 *dst) {
    Window window;
    u8 *buf = Heap_Alloc(HEAP_ID_154, 0x180);
    AddWindowParameterized(app->bgConfig, &window, 1, 0, 0, 10, 2, 0, 0);
    FillWindowPixelBuffer(&window, 0);
    MI_CpuCopy8(sPokewalkerUploadImageDigit0, dst, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit1, dst + 0x20, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit2, dst + 0x40, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit3, dst + 0x60, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit4, dst + 0x80, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit5, dst + 0xA0, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit6, dst + 0xC0, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit7, dst + 0xE0, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit8, dst + 0x100, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDigit9, dst + 0x120, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageColon, dst + 0x140, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageDash, dst + 0x160, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageSlash, dst + 0x180, 0x20);
    MI_CpuCopy8(ov112_021F5998, dst + 0x1A0, 0x40);
    MI_CpuCopy8(sPokewalkerUploadImageArrowUp, dst + 0x278, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowUpShifted, dst + 0x288, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowUpHollow, dst + 0x298, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowDown, dst + 0x2A8, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowDownShifted, dst + 0x2B8, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowDownHollow, dst + 0x2C8, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowLeft, dst + 0x2D8, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowLeftShifted, dst + 0x2E8, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowLeftHollow, dst + 0x2F8, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowRight, dst + 0x308, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowRightShifted, dst + 0x318, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageArrowRightHollow, dst + 0x328, 0x10);
    MI_CpuCopy8(ov112_021F5898, dst + 0x338, 0x20);
    MI_CpuCopy8(ov112_021F58D8, dst + 0x358, 0x20);
    MI_CpuCopy8(ov112_021F58F8, dst + 0x378, 0x20);
    MI_CpuCopy8(ov112_021F5668, dst + 0x3B8, 0x10);
    MI_CpuCopy8(ov112_021F5638, dst + 0x3C8, 0x8);
    MI_CpuCopy8(sPokewalkerUploadImagePokeBall, dst + 0x1E0, 0x10);
    MI_CpuCopy8(ov112_021F5628, dst + 0x1F0, 0x10);
    MI_CpuCopy8(ov112_021F5648, dst + 0x208, 0x10);
    MI_CpuCopy8(ov112_021F5748, dst + 0x218, 0x10);
    MI_CpuCopy8(ov112_021F5678, dst + 0x228, 0x10);
    MI_CpuCopy8(sPokewalkerUploadImageHeart, dst + 0x238, 0x10);
    MI_CpuCopy8(ov112_021F55B8, dst + 0x248, 0x10);
    MI_CpuCopy8(ov112_021F5608, dst + 0x258, 0x10);
    MI_CpuCopy8(ov112_021F56E8, dst + 0x268, 0x10);
    MI_CpuCopy8(ov112_021F5618, dst + 0x3D0, 0x10);
    MI_CpuCopy8(ov112_021F56A8, dst + 0x3E0, 0x10);
    MI_CpuCopy8(ov112_021F5B98, dst + 0xE10, 0x40);
    MI_CpuCopy8(ov112_021F5CD8, dst + 0xE50, 0x40);
    MI_CpuCopy8(ov112_021F5BD8, dst + 0xE90, 0x40);
    MI_CpuCopy8(ov112_021F5A18, dst + 0xED0, 0x40);
    MI_CpuCopy8(ov112_021F5C98, dst + 0xF10, 0x40);
    MI_CpuCopy8(ov112_021F5AD8, dst + 0xF50, 0x40);
    MI_CpuCopy8(ov112_021F7418, dst + 0x690, 0x140);
    MI_CpuCopy8(ov112_021F7558, dst + 0x7D0, 0x140);
    MI_CpuCopy8(ov112_021F7698, dst + 0x910, 0x140);
    MI_CpuCopy8(ov112_021F77D8, dst + 0xA50, 0x140);
    MI_CpuCopy8(ov112_021F7918, dst + 0xB90, 0x140);
    MI_CpuCopy8(ov112_021F7A58, dst + 0xCD0, 0x140);
    MI_CpuCopy8(ov112_021F5C58, dst + 0x18D0, 0x40);
    MI_CpuCopy8(ov112_021F59D8, dst + 0x1910, 0x40);
    MI_CpuCopy8(ov112_021F6178, dst + 0x1950, 0x80);
    MI_CpuCopy8(ov112_021F5E18, dst + 0x19D0, 0x60);
    MI_CpuCopy8(ov112_021F6DD8, dst + 0x1A30, 0xC0);
    MI_CpuCopy8(ov112_021F5A58, dst + 0x1AF0, 0x40);
    MI_CpuCopy8(ov112_021F5A98, dst + 0x1B30, 0x40);
    MI_CpuCopy8(ov112_021F5B18, dst + 0x1B70, 0x40);
    MI_CpuCopy8(ov112_021F5B58, dst + 0x1BB0, 0x40);
    MI_CpuCopy8(ov112_021F62F8, dst + 0x1BF0, 0x80);
    MI_CpuCopy8(ov112_021F6378, dst + 0x1C70, 0x80);
    MI_CpuCopy8(ov112_021F6718, dst + 0x1CF0, 0xC0);
    MI_CpuCopy8(ov112_021F5738, dst + 0x1DB0, 0x10);
    MI_CpuCopy8(ov112_021F5708, dst + 0x1DC0, 0x10);
    MI_CpuCopy8(ov112_021FE198, dst + 0x1DD0, 0x300);
    MI_CpuCopy8(ov112_021F5D18, dst + 0xF90, 0x40);
    MI_CpuCopy8(ov112_021F5C18, dst + 0x1110, 0x40);
    MI_CpuCopy8(ov112_021F65D8, dst + 0x1150, 0xA0);
    MI_CpuCopy8(ov112_021F61F8, dst + 0x11F0, 0x80);
    MI_CpuCopy8(ov112_021F6278, dst + 0x1270, 0x80);
    MI_CpuCopy8(ov112_021F6538, dst + 0x1270, 0xA0);
    MI_CpuCopy8(ov112_021F70D8, dst + 0x1310, 0x100);
    PokewalkerApp_PrintPlayerName(&window, app);
    PokewalkerApp_ConvertTo2bpp(window.pixelBuffer, 10, 2, buf, 0, 0, 0x50, 0x10);
    MI_CpuCopy8(buf, dst + 0xFD0, 0x140);
    MI_CpuCopy8(ov112_021F67D8, dst + 0x1690, 0xC0);
    MI_CpuCopy8(ov112_021F6898, dst + 0x1750, 0xC0);
    MI_CpuCopy8(ov112_021F7018, dst + 0x1810, 0xC0);
    MI_CpuCopy8(ov112_021F6498, dst + 0x1410, 0xA0);
    MI_CpuCopy8(ov112_021F63F8, dst + 0x14B0, 0xA0);
    MI_CpuCopy8(ov112_021F57D8, dst + 0x1670, 0x20);
    MI_CpuCopy8(ov112_021F5DB8, dst + 0x15B0, 0x60);
    MI_CpuCopy8(ov112_021F6058, dst + 0x1610, 0x60);
    MI_CpuCopy8(ov112_021F5D58, dst + 0x1550, 0x60);
    MI_CpuCopy8(ov112_021F71D8, dst + 0x20D0, 0x100);
    MI_CpuCopy8(ov112_021F5958, dst + 0x21D0, 0x20);
    MI_CpuCopy8(sPokewalkerUploadImageMusicNote, dst + 0x21F0, 0x10);
    MI_CpuCopy8(ov112_021FCF98, dst + 0x22B0, 0x180);
    MI_CpuCopy8(ov112_021F8198, dst + 0x2430, 0x180);
    MI_CpuCopy8(ov112_021F8318, dst + 0x25B0, 0x180);
    MI_CpuCopy8(ov112_021F8498, dst + 0x2730, 0x180);
    MI_CpuCopy8(ov112_021FC998, dst + 0x28B0, 0x180);
    MI_CpuCopy8(ov112_021F8618, dst + 0x2A30, 0x180);
    MI_CpuCopy8(ov112_021F8798, dst + 0x2BB0, 0x180);
    MI_CpuCopy8(ov112_021F8918, dst + 0x2D30, 0x180);
    MI_CpuCopy8(ov112_021FC398, dst + 0x2EB0, 0x180);
    MI_CpuCopy8(ov112_021F8A98, dst + 0x3030, 0x180);
    MI_CpuCopy8(ov112_021F8C18, dst + 0x31B0, 0x180);
    MI_CpuCopy8(ov112_021F8D98, dst + 0x3330, 0x180);
    MI_CpuCopy8(ov112_021F8F18, dst + 0x34B0, 0x180);
    MI_CpuCopy8(ov112_021F9098, dst + 0x3630, 0x180);
    MI_CpuCopy8(ov112_021F9218, dst + 0x37B0, 0x180);
    MI_CpuCopy8(ov112_021F9398, dst + 0x3930, 0x180);
    MI_CpuCopy8(ov112_021FB798, dst + 0x3AB0, 0x180);
    MI_CpuCopy8(ov112_021F9518, dst + 0x3C30, 0x180);
    MI_CpuCopy8(ov112_021F9698, dst + 0x3DB0, 0x180);
    MI_CpuCopy8(ov112_021F9818, dst + 0x3F30, 0x180);
    MI_CpuCopy8(ov112_021FB198, dst + 0x40B0, 0x180);
    MI_CpuCopy8(ov112_021F9998, dst + 0x4230, 0x180);
    MI_CpuCopy8(ov112_021F9B18, dst + 0x43B0, 0x180);
    MI_CpuCopy8(ov112_021F9C98, dst + 0x4530, 0x180);
    MI_CpuCopy8(ov112_021FAB98, dst + 0x46B0, 0x180);
    MI_CpuCopy8(ov112_021F9E18, dst + 0x4830, 0x180);
    MI_CpuCopy8(ov112_021F9F98, dst + 0x49B0, 0x180);
    MI_CpuCopy8(ov112_021FA118, dst + 0x4B30, 0x180);
    MI_CpuCopy8(ov112_021FA298, dst + 0x4CB0, 0x180);
    MI_CpuCopy8(ov112_021FA418, dst + 0x4E30, 0x180);
    MI_CpuCopy8(ov112_021FA598, dst + 0x4FB0, 0x180);
    MI_CpuCopy8(ov112_021FA718, dst + 0x5130, 0x180);
    MI_CpuCopy8(ov112_021FA898, dst + 0x52B0, 0x180);
    MI_CpuCopy8(ov112_021FAA18, dst + 0x5430, 0x180);
    MI_CpuCopy8(ov112_021FAD18, dst + 0x55B0, 0x180);
    MI_CpuCopy8(ov112_021FAE98, dst + 0x5730, 0x180);
    MI_CpuCopy8(ov112_021FB018, dst + 0x58B0, 0x180);
    MI_CpuCopy8(ov112_021FB318, dst + 0x5A30, 0x180);
    MI_CpuCopy8(ov112_021FB498, dst + 0x5BB0, 0x180);
    MI_CpuCopy8(ov112_021FB618, dst + 0x5D30, 0x180);
    MI_CpuCopy8(ov112_021FB918, dst + 0x5EB0, 0x180);
    MI_CpuCopy8(ov112_021FBA98, dst + 0x6030, 0x180);
    MI_CpuCopy8(ov112_021FBC18, dst + 0x61B0, 0x180);
    MI_CpuCopy8(ov112_021FBD98, dst + 0x6330, 0x180);
    MI_CpuCopy8(ov112_021FBF18, dst + 0x64B0, 0x180);
    MI_CpuCopy8(ov112_021FC098, dst + 0x6630, 0x180);
    MI_CpuCopy8(ov112_021FC218, dst + 0x67B0, 0x180);
    MI_CpuCopy8(ov112_021FC518, dst + 0x6930, 0x180);
    MI_CpuCopy8(ov112_021FC698, dst + 0x6AB0, 0x180);
    MI_CpuCopy8(ov112_021FC818, dst + 0x6C30, 0x180);
    MI_CpuCopy8(ov112_021FCB18, dst + 0x6DB0, 0x180);
    MI_CpuCopy8(ov112_021FCC98, dst + 0x6F30, 0x180);
    MI_CpuCopy8(ov112_021FCE18, dst + 0x70B0, 0x180);
    MI_CpuCopy8(ov112_021FD118, dst + 0x7230, 0x180);
    MI_CpuCopy8(ov112_021FD298, dst + 0x73B0, 0x180);
    MI_CpuCopy8(ov112_021FD418, dst + 0x7530, 0x180);
    MI_CpuCopy8(ov112_021FD598, dst + 0x76B0, 0x180);
    MI_CpuCopy8(ov112_021FD718, dst + 0x7830, 0x180);
    MI_CpuCopy8(ov112_021FD898, dst + 0x79B0, 0x180);
    MI_CpuCopy8(ov112_021FDA18, dst + 0x7B30, 0x180);
    MI_CpuCopy8(ov112_021FDB98, dst + 0x7CB0, 0x180);
    MI_CpuCopy8(ov112_021FDD18, dst + 0x7E30, 0x180);
    MI_CpuCopy8(ov112_021FDE98, dst + 0x7FB0, 0x180);
    MI_CpuCopy8(ov112_021FE018, dst + 0x8130, 0x180);
    MI_CpuCopy8(ov112_021F7B98, dst + 0x82B0, 0x180);
    MI_CpuCopy8(ov112_021F7D18, dst + 0x8430, 0x180);
    MI_CpuCopy8(ov112_021F7E98, dst + 0x85B0, 0x180);
    MI_CpuCopy8(ov112_021F8018, dst + 0x8730, 0x180);
    MI_CpuCopy8(ov112_021F72D8, dst + 0x88B0, 0x180);
    MI_CpuCopy8(ov112_021F6678, dst + 0x2210, 0xA0);
    MI_CpuCopy8(ov112_021F5E78, dst + 0x3F0, 0x60);
    MI_CpuCopy8(ov112_021F5ED8, dst + 0x450, 0x60);
    MI_CpuCopy8(ov112_021F5F38, dst + 0x4B0, 0x60);
    MI_CpuCopy8(ov112_021F5F98, dst + 0x510, 0x60);
    MI_CpuCopy8(ov112_021F5FF8, dst + 0x570, 0x60);
    MI_CpuCopy8(ov112_021F6118, dst + 0x5D0, 0x60);
    MI_CpuCopy8(ov112_021F60B8, dst + 0x630, 0x60);
    PokewalkerApp_WriteUploadBlocks(dst + 0x8A30);
    Heap_Free(buf);
    RemoveWindow(&window);
}

#pragma force_active on
// Picks one Pokemon of each of the course's three pairs.
void PokewalkerApp_PickRouteMons(PokewalkerMon *mons, u16 *steps, u8 *chances, int courseId, u8 *chosen) {
    int i, j;
    const PokewalkerCourse *course = &sPokewalkerCourses[courseId];
    BASE_STATS *personal;
    SetLCRNGSeed(GF_RTC_TimeToSec());
    for (i = 0; i < 3; i++) {
        int idx = i * 2 + LCRandom() % 2;
        const PokewalkerRouteMon *routeMon;
        routeMon = &course->mons[idx];
        chosen[i] = idx;
        personal = AllocAndLoadMonPersonal_HandleAlternateForm(routeMon->species, routeMon->form, HEAP_ID_154);
        mons[i].species = routeMon->species;
        mons[i].heldItem = routeMon->heldItem;
        for (j = 0; j < MAX_MON_MOVES; j++) {
            mons[i].moves[j] = routeMon->moves[j];
        }
        mons[i].level = routeMon->level;
        mons[i].form = routeMon->form;
        mons[i].gender = routeMon->gender;
        mons[i].unk_0D_7 = 0;
        mons[i].canFlip = GetPersonalAttr(personal, BASE_FLIP);
        steps[i] = routeMon->steps;
        chances[i] = routeMon->chance;
        FreeMonPersonal(personal);
    }
}
#pragma force_active reset

// A Pokemon of one of the course's advantage types finds the course's Pokemon in 75% of the steps.
void PokewalkerApp_ApplyAdvantageTypes(const u8 *types, BoxPokemon *boxMon, u16 *steps) {
    int type1 = GetBoxMonData(boxMon, MON_DATA_TYPE_1, NULL);
    int type2 = GetBoxMonData(boxMon, MON_DATA_TYPE_2, NULL);
    int i;
    BOOL found = FALSE;
    for (i = 0; i < 3; i++) {
        if (type1 == types[i] || type2 == types[i]) {
            found = TRUE;
        }
    }
    if (found) {
        for (i = 0; i < 3; i++) {
            steps[i] = steps[i] * 75 / 100;
        }
    }
}

#pragma force_active on
void PokewalkerApp_BuildStrollData(PokewalkerStrollData *data, PokewalkerApp *app) {
    Window windows[15];
    u8 chosen[4];
    u8 *file;
    u32 i;
    u8 courseId;
    u8 *buf;
    int j;
    u16 *p;
    BASE_STATS *personal;
    String *string;

    courseId = app->courseId;
    buf = Heap_Alloc(HEAP_ID_154, 0x180);
    for (i = 0; i < NELEMS(windows); i++) {
        AddWindowParameterized(app->bgConfig, &windows[i], 1, 0, 0, sStrollTextWindowWidths[i], 2, 0, 0);
        FillWindowPixelBuffer(&windows[i], 0);
    }
    data->mon.species = app->strollMonSpecies;
    AcquireBoxMonLock(app->strollMon);
    for (j = 0, p = &data->mon.species; j < MAX_MON_MOVES; j++, p++) {
        p[2] = GetBoxMonData(app->strollMon, MON_DATA_MOVE1 + j, NULL);
    }
    data->mon.level = GetBoxMonData(app->strollMon, MON_DATA_LEVEL, NULL);
    data->mon.form = GetBoxMonData(app->strollMon, MON_DATA_FORM, NULL);
    data->mon.unk_0D_7 = 0;
    data->mon.isShiny = BoxMonIsShiny(app->strollMon);
    data->friendship = GetBoxMonData(app->strollMon, MON_DATA_FRIENDSHIP, NULL);
    GetBoxMonData(app->strollMon, MON_DATA_NICKNAME, data->nickname);
    ReleaseBoxMonLock(app->strollMon, TRUE);
    personal = AllocAndLoadMonPersonal(data->mon.species, HEAP_ID_154);
    data->mon.canFlip = GetPersonalAttr(personal, BASE_FLIP);
    FreeMonPersonal(personal);
    data->mon.gender = GetBoxMonGender(app->strollMon);
    data->routeImageId = sPokewalkerCourseRouteImageIds[courseId].routeImageId - 1;
    string = NewString_ReadMsgData(app->msgData, msg_0274_00091 + app->courseId);
    CopyStringToU16Array(string, data->courseName, 21);
    String_Delete(string);
    PokewalkerApp_PickRouteMons(data->routeMons, data->routeMonSteps, data->routeMonChances, courseId, chosen);
    PokewalkerApp_ApplyAdvantageTypes(sPokewalkerCourses[courseId].advantageTypes, app->strollMon, data->routeMonSteps);
    PokewalkerApp_PrintStrollTexts(windows, app, &sPokewalkerCourses[courseId], chosen);
    for (j = 0; j < 3; j++) {
        file = GfGfxLoader_LoadFromNarc(NARC_a_2_4_8, PokewalkerApp_GetSpeciesIconFile(data->routeMons[j].species), TRUE, HEAP_ID_154, TRUE);
        MI_CpuCopy8(file, &data->routeMonIcons[j * 0x180], 0x180);
        Heap_Free(file);
    }
    file = GfGfxLoader_LoadFromNarc(NARC_a_2_5_6, PokewalkerApp_GetPictureFile(data->routeMons[2].species, data->routeMons[2].gender, 0), TRUE, HEAP_ID_154, TRUE);
    MI_CpuCopy8(file, data->routeMonPicture, 0x600);
    Heap_Free(file);
    for (j = 0; j < 3; j++) {
        PokewalkerApp_ConvertTo2bpp(windows[10 + j].pixelBuffer, 10, 2, buf, 0, 0, 80, 16);
        MI_CpuCopy8(buf, &data->routeMonNameImages[j * 0x140], 0x140);
    }
    for (j = 0; j < 10; j++) {
        data->routeItems[j] = sPokewalkerCourses[courseId].items[j].item;
        data->routeItemSteps[j] = sPokewalkerCourses[courseId].items[j].steps;
        data->routeItemChances[j] = sPokewalkerCourses[courseId].items[j].chance;
    }
    MI_CpuCopy8(sPokewalkerRouteImages[sPokewalkerCourseRouteImageIds[courseId].routeImageId], data->routeImage, 0xC0);
    PokewalkerApp_ConvertTo2bpp(windows[13].pixelBuffer, 10, 2, buf, 0, 0, 80, 16);
    MI_CpuCopy8(buf, data->courseNameImage, 0x140);
    file = GfGfxLoader_LoadFromNarc(NARC_a_2_4_8, PokewalkerApp_GetBoxMonIconFile(app->strollMon), TRUE, HEAP_ID_154, TRUE);
    MI_CpuCopy8(file, data->monIcon, 0x180);
    Heap_Free(file);
    GF_ASSERT(app->strollMonSpecies != SPECIES_NONE);
    file = GfGfxLoader_LoadFromNarc(NARC_a_2_5_6, PokewalkerApp_GetPictureFile(app->strollMonSpecies, app->strollMonGender, app->strollMonForm), TRUE, HEAP_ID_154, TRUE);
    if (app->strollMonSpecies == SPECIES_SPINDA) {
        PokewalkerApp_DrawSpindaSpotsOnPicture(file, sSpindaSpotMask, GetBoxMonData(app->strollMon, MON_DATA_PERSONALITY, NULL));
    }
    MI_CpuCopy8(file, data->monPicture, 0x600);
    Heap_Free(file);
    PokewalkerApp_ConvertTo2bpp(windows[14].pixelBuffer, 10, 2, buf, 0, 0, 80, 16);
    MI_CpuCopy8(buf, data->nicknameImage, 0x140);
    for (j = 0; j < 10; j++) {
        PokewalkerApp_ConvertTo2bpp(windows[j].pixelBuffer, 12, 2, buf, 0, 0, 96, 16);
        MI_CpuCopy8(buf, &data->routeItemNameImages[j * 0x180], 0x180);
    }
    for (i = 0; i < NELEMS(windows); i++) {
        RemoveWindow(&windows[i]);
    }
    Heap_Free(buf);
}
#pragma force_active reset

void PokewalkerApp_PrintPlayerName(Window *window, PokewalkerApp *app) {
    String *name = PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(app->saveData), HEAP_ID_154);
    FontID_Alloc(5, HEAP_ID_154);
    AddTextPrinterParameterizedWithColor(window, 5, name, 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 1, 0), NULL);
    FontID_Release(5);
    String_Delete(name);
}

#pragma force_active on
// Prints the course's item names (windows 0-9), the chosen Pokemon's names (10-12), the course name (13) and
// the stroll Pokemon's nickname (14).
void PokewalkerApp_PrintStrollTexts(Window *windows, PokewalkerApp *app, const PokewalkerCourse *course, const u8 *chosen) {
    MsgData *speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_154);
    String *string;
    int i;

    FontID_Alloc(5, HEAP_ID_154);
    for (i = 0; i < 10; i++) {
        string = NewString_ReadMsgData(app->msgDataItemNames, course->items[i].item);
        AddTextPrinterParameterizedWithColor(&windows[i], 5, string, 2, 1, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 1, 0), NULL);
        String_Delete(string);
    }
    for (i = 0; i < 3; i++) {
        string = NewString_ReadMsgData(speciesNames, course->mons[chosen[i]].species);
        AddTextPrinterParameterizedWithColor(&windows[i + 10], 5, string, 2, 1, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 1, 0), NULL);
        String_Delete(string);
    }
    string = NewString_ReadMsgData(app->msgData, msg_0274_00091 + app->courseId);
    AddTextPrinterParameterizedWithColor(&windows[13], 5, string, 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 1, 0), NULL);
    String_Delete(string);
    string = String_New(12, HEAP_ID_154);
    GetBoxMonData(app->strollMon, MON_DATA_NICKNAME_STRING, string);
    AddTextPrinterParameterizedWithColor(&windows[14], 5, string, 2, 1, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 1, 0), NULL);
    String_Delete(string);
    FontID_Release(5);
    DestroyMsgData(speciesNames);
}
#pragma force_active reset

#pragma force_active on
// Loads a Pokemon's front picture (80x80) into OBJ VRAM of the main (screen 0) or sub screen.
int PokewalkerApp_LoadMonPicture(BoxPokemon *boxMon, void *buffer, PokepicTemplate *template, int screen) {
    u32 personality, species;
    GetBoxmonSpriteCharAndPlttNarcIds(template, boxMon, 2, FALSE);
    personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL);
    species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    CharTiles_LoadMonObjBlocksFromNarc((NarcId) template->narcID, template->charDataID, HEAP_ID_154, 0, 0, 10, 10, buffer, personality, FALSE, 2, species);
    DC_FlushRange(buffer, 0xC80);
    if (screen == 0) {
        GX_LoadOBJ(buffer, 0xB80, 0xC80);
        GfGfxLoader_GXLoadPal((NarcId) template->narcID, template->palDataID, GF_PAL_LOCATION_MAIN_OBJ, (enum GFPalSlotOffset)0x80, 0x20, HEAP_ID_154);
    } else {
        GXS_LoadOBJ(buffer, 0xC40, 0xC80);
        GfGfxLoader_GXLoadPal((NarcId) template->narcID, template->palDataID, GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)0x20, 0x20, HEAP_ID_154);
    }
    return 0;
}
#pragma force_active reset

BOOL PokewalkerApp_IsStrollStateNonzero(PokewalkerApp *app) {
    u16 unk0;
    u16 unk2;
    Pokewalker_GetStrollState(app->pokewalker, &unk0, &unk2);
    return unk0 != 0;
}

#pragma force_active on
// Color index (0-15) of pixel (x, y) of 4bpp tile data that is widthTiles tiles wide.
int PokewalkerApp_GetPixel4bpp(const u8 *pixels, int widthTiles, int x, int y) {
    int tile = (s32)((u32)(y / 8) * (u32)widthTiles + (u32)(x / 8));
    int pixel = (s32)((u32)(x % 8) + ((u32)(y % 8) << 3));
    if ((pixel & 1) == 1) {
        return pixels[(s32)((u32)(pixel / 2) + (u32)tile * 32)] >> 4;
    }
    return pixels[(s32)((u32)(pixel / 2) + (u32)tile * 32)] & 0xF;
}
#pragma force_active reset

#pragma force_active on
// Converts the (x, y, width, height) area of 4bpp tile data to the Pokewalker's 2bpp format: columns of 8
// pixels, two bytes each (bit plane 1, then bit plane 0). Returns the number of bytes written.
int PokewalkerApp_ConvertTo2bpp(const u8 *src, int widthTiles, int unused, u8 *dst, int x, int y, int width, int height) {
    int i, j, k;
    int pos = 0;

    MIi_CpuClearFast(0, (u32 *)dst, (width * height) / 8 * 2);

    for (i = 0; i < height / 8; i++) {
        for (j = 0; j < width; j++) {
            u8 *out = dst + pos;
            for (k = 0; k < 8; k++) {
                int color = PokewalkerApp_GetPixel4bpp(src, widthTiles, j + x, y + k + i * 8);
                out[0] |= (color >> 1) << k;
                out[1] |= (color & 1) << k;
            }
            pos += 2;
        }
    }
    return pos;
}
#pragma force_active reset
