#pragma force_active on
typedef unsigned int u32;
typedef struct {
    u32 unk0;
    u32 unk4_0 : 7;
    u32 kind : 3;
    u32 orientation : 2;
    u32 unk4_12 : 20;
} MSLFile;
int fwide(MSLFile *f, int mode);
// @fn fwide
int fwide(MSLFile *f, int mode) {
    int result;
    if (f == 0 || f->kind == 0) {
        return 0;
    }
    result = mode;
    switch (f->orientation) {
    case 0:
        if (mode > 0) {
            f->orientation = 2;
        } else if (mode < 0) {
            f->orientation = 1;
        }
        result = mode;
        break;
    case 1:
        result = -1;
        break;
    case 2:
        result = 1;
        break;
    }
    return result;
}
