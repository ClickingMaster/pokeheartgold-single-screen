#pragma force_active on
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct {
    u32 handle;
    u32 unk4_0 : 5;
    u32 bufferMode : 2;
    u32 kind : 3;
    u32 unk4_10 : 22;
    u32 state : 3;
    u32 unk8_3 : 29;
    u8 rest[0x4c - 12];
} MSLFile;
extern MSLFile __files[];
int fflush(MSLFile *f);
int __flush_line_buffered_output_files(void);
int __flush_all(void);
// @fn __flush_line_buffered_output_files
int __flush_line_buffered_output_files(void) {
    int result = 0;
    MSLFile *f = &__files[0];
    int i = 1;
    while (f != 0) {
        if (f->kind != 0 && (f->bufferMode & 1) && f->state == 1) {
            if (fflush(f) != 0) {
                result = -1;
            }
        }
        if (i < 3) {
            f = &__files[i++];
        } else {
            f = 0;
        }
    }
    return result;
}
// @fn __flush_all
int __flush_all(void) {
    int result = 0;
    MSLFile *f = &__files[0];
    int i = 1;
    while (f != 0) {
        if (f->kind != 0) {
            if (fflush(f) != 0) {
                result = -1;
            }
        }
        if (i < 3) {
            f = &__files[i++];
        } else {
            f = 0;
        }
    }
    return result;
}
