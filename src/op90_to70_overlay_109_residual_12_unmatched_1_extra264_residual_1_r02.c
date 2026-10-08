#pragma force_active on
#include "overlay109_sol_partial_internal.h"
extern const u8 _021E7884[];
void ov109_021E7178(PhotoAlbumApp *, Photo *, int, u8, u8);
void ov109_021E7030(PhotoAlbumApp *app, int bgId, int idx, u8 start);
// @fn ov109_021E7030
void ov109_021E7030(PhotoAlbumApp *app, int bgId, int idx, u8 start) {
    int row;
    int x;
    u8 n;
    for (row = 0; row < 3; row++) {
        for (x = start; x < start + 2; x++) {
            n = _021E7884[row * 4 + x] + idx * 12;
            if (n < app->numPhotos) {
                ov109_021E7178(app, app->entries[n].photo, bgId, x, row);
            } else {
                ov109_021E7178(app, NULL, bgId, x, row);
            }
        }
    }
}
