#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 p[4];
    u8 *records;
    int count;
} S;

extern int ov38_0222308C(void *, void **);
int ov38_02221468(void *, S *);
extern const char ov38_02225EC0[];
extern void ov38_02221888(void *, int);
extern int ov38_02222F88(void *, const char *, char **);
extern int ov38_022218B4(const char *, int);
extern int ov38_022201A0(int);
extern void ov38_0222189C(void *, const char *, u32);
extern int ov38_022202C0(char *, char *, int);
int ov38_02221494(void *, S *);

int ov38_02221494(void *request, S *s) {
    int result = 0, len, i, row, offset;
    char *text, *header;
    ov38_02221888(s->records, s->count * 176);
    if (ov38_02222F88(request, ov38_02225EC0, &header) > 0 && header[0] == '0' && header[1] == 0) {
        return 1;
    }
    len = ov38_0222308C(request, (void **)&text);
    if (len <= 0) {
        goto done;
    }
    i = 0;
    row = 0;
    if (s->count <= 0) {
        goto done;
    }
    offset = 0;
    do {
        int field, start, cr, n, value;
        u8 *rec;
        if (i >= len) {
            result = 1;
            goto done;
        }
        rec = s->records + offset;
        field = 0;
        start = i;
        cr = 0;
        result = 0;
        while (i < len) {
            int ch = text[i];
            if (ch == 13) {
                cr = 1;
                goto advance;
            }
            if (cr) {
                if (ch == 10) {
                    if (field == 5) {
                        n = i - 1 - start;
                        if (n > 8) {
                            goto done;
                        }
                        value = ov38_022218B4(text + start, n);
                        if (value < 0) {
                            goto done;
                        }
                        *(int *)(rec + 172) = value;
                    }
                    i++;
                    result = field >= 5;
                    break;
                }
            } else if (ch == 9) {
                switch (field) {
                case 0:
                    n = i - start;
                    if ((u32)n > 32) {
                        goto done;
                    }
                    ov38_0222189C(rec, text + start, n);
                    break;
                case 1:
                    n = i - start;
                    if ((u32)ov38_022201A0(n) > 102) {
                        goto done;
                    }
                    ov38_022202C0((char *)(rec + 34), text + start, n);
                    *(u16 *)(rec + 134) = 0;
                    break;
                case 2:
                    n = i - start;
                    if ((u32)n > 10) {
                        goto done;
                    }
                    ov38_0222189C(rec + 136, text + start, n);
                    break;
                case 3:
                    n = i - start;
                    if ((u32)n > 10) {
                        goto done;
                    }
                    ov38_0222189C(rec + 147, text + start, n);
                    break;
                case 4:
                    n = i - start;
                    if ((u32)n > 10) {
                        goto done;
                    }
                    ov38_0222189C(rec + 158, text + start, n);
                    break;
                case 5:
                    n = i - start;
                    if (n > 8) {
                        goto done;
                    }
                    value = ov38_022218B4(text + start, n);
                    if (value < 0) {
                        goto done;
                    }
                    *(int *)(rec + 172) = value;
                    break;
                }
                field++;
                start = i + 1;
            }
            cr = 0;
advance:
            i++;
        }
        row++;
        offset += 176;
    } while (row < s->count);
done:
    return result;
}
