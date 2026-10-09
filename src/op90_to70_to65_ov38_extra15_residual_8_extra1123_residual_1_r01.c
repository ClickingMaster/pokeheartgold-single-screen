#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    char b[13];
} Template13;
typedef struct {
    int f0;
    int f4;
} Info;
extern Template13 ov38_02224EBC;
Info ov38_022260FC;
extern char ov38_02226104[];
int ov00_021EC9E0(Info *info);
void ov38_022218C0(char *buf, int v);
void ov38_0222189C(char *dst, const char *src, int n);
int ov38_022201C8(char *out, const u8 *in, int len);
int ov38_02220684(char *out);
// @fn ov38_02220684
int ov38_02220684(char *out) {
    Template13 t = ov38_02224EBC;
    if (ov00_021EC9E0(&ov38_022260FC)) {
        if (ov38_022260FC.f0 >= 0 && ov38_022260FC.f0 < 10) {
            t.b[1] = ov38_022260FC.f0 + '0';
        } else if (ov38_022260FC.f0 >= 10 && ov38_022260FC.f0 < 100) {
            ov38_022218C0(t.b, ov38_022260FC.f0);
        }
        if (ov38_022260FC.f0 == 4 || ov38_022260FC.f0 == 8) {
            if (ov38_022260FC.f4 >= 0 && ov38_022260FC.f4 < 10) {
                t.b[3] = ov38_022260FC.f4 + '0';
            }
            ov38_0222189C(t.b + 4, ov38_02226104, 9);
        }
        ov38_022201C8(out, (u8 *)t.b, 13);
        return 1;
    }
    return 0;
}
