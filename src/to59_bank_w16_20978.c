#include "to59_bank_w16_private.h"

typedef struct Table4 { struct { int x, y; } v[6]; } Table4;
typedef struct Table7 { struct { int x, y; } v[7]; } Table7;

void ov07_02220978(void *emitter, Work *work) {
    VecFx32 value;
    Param param;
    Table4 t31;
    Table7 t30;
    Table7 t32;
    switch (work->mode14) {
    case 1: ov07_02231B90(work->context, work->indexA, &value); break;
    case 2: ov07_02231B90(work->context, work->indexB, &value); break;
    case 6: ov07_02231BC0(work->context, work->indexA, &value); break;
    case 7: ov07_02231BC0(work->context, work->indexB, &value); break;
    case 10: ov07_02231BF0(work->context, work->indexA, &value); break;
    case 11: ov07_02231BF0(work->context, work->indexB, &value); break;
    case 14: ov07_02231C20(work->context, work->indexA, &value); break;
    case 15: ov07_02231C20(work->context, work->indexB, &value); break;
    case 16: ov07_02231C50(work->context, work->indexA, &value); break;
    case 17: ov07_02231C50(work->context, work->indexB, &value); break;
    case 18: ov07_02231C80(work->context, work->indexA, &value); break;
    case 19: ov07_02231C80(work->context, work->indexB, &value); break;
    case 20: ov07_02231CB0(work->context, work->indexA, &value); break;
    case 21: ov07_02231CB0(work->context, work->indexB, &value); break;
    case 22: ov07_02231CE0(work->context, work->indexA, &value); break;
    case 23: ov07_02231CE0(work->context, work->indexB, &value); break;
    case 24: ov07_02231D10(work->context, work->indexA, &value); break;
    case 25: ov07_02231D10(work->context, work->indexB, &value); break;
    case 26: ov07_02231D40(work->context, work->indexA, &value); break;
    case 27: ov07_02231D40(work->context, work->indexB, &value); break;
    case 8: ov07_02231DA0(work->context, work->indexA, &value); break;
    case 9: ov07_02231DA0(work->context, work->indexB, &value); break;
    case 3:
        BattleAnimSystem_ReadScriptArgs(work->context, &param, 4);
        work->scale = ov07_02221664(work);
        value.x = param.value.x;
        value.y = param.value.y;
        value.z = param.value.z;
        if (param.flag == 1) work->scale = 1;
        value.x *= work->scale;
        value.y *= work->scale;
        break;
    case 4: ov07_02231B90(work->context, work->indexA, &value); ov07_02221734(work, &value); break;
    case 5: ov07_02231B90(work->context, work->indexB, &value); ov07_02221734(work, &value); break;
    case 100: ov07_02231B90(work->context, work->indexA, &value); ov07_022216A8(work, work->indexA, &value); break;
    case 101: ov07_02231B90(work->context, work->indexB, &value); ov07_022216A8(work, work->indexB, &value); break;
    case 12: ov07_02220938(emitter, work, (int)&value); ov07_02221734(work, &value); break;
    case 13: ov07_022208F8(emitter, work, (int)&value); ov07_02221734(work, &value); break;
    case 28: value.x = 0x2ce0; value.y = 0; value.z = 0; break;
    case 33: value.x = -5000; value.y = -6000; value.z = 0; break;
    case 34: ov07_02231B90(work->context, work->indexA, &value); break;
    case 31:
        t31 = *(const Table4 *)ov07_022354E0;
        {
            int n = BattleAnimUtil_GetBattlerType(work->context, work->indexA);
            value.x = t31.v[n].x; value.y = t31.v[n].y;
        }
        value.z = 0;
        break;
    case 30:
        t30 = *(const Table7 *)ov07_02235578;
        {
            int n;
            if (ov07_0221BFC0(work->context) == 1) n = 6;
            else n = BattleAnimUtil_GetBattlerType(work->context, work->indexA);
            value.x = t30.v[n].x; value.y = t30.v[n].y;
        }
        value.z = 0;
        break;
    case 32:
        t32 = *(const Table7 *)ov07_02235540;
        {
            int n;
            if (ov07_0221BFC0(work->context) == 1) n = 6;
            else n = BattleAnimUtil_GetBattlerType(work->context, work->indexA);
            value.x = t32.v[n].x; value.y = t32.v[n].y;
        }
        value.z = 0;
        break;
    default: break;
    }
    work->value = value;
    *(int *)((u8 *)emitter + 0x28) = value.x + (**(int ***)((u8 *)emitter + 0x20))[1];
    *(int *)((u8 *)emitter + 0x2c) = value.y + (**(int ***)((u8 *)emitter + 0x20))[2];
    *(int *)((u8 *)emitter + 0x30) = value.z + (**(int ***)((u8 *)emitter + 0x20))[3];
}
