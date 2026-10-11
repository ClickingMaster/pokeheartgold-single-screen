#include "overlay96_course_gauge_r118_private.h"

void ov96_02217FD0(Ov96R118Unit *units) {
    int unitIndex;
    int gaugeIndex;
    for (unitIndex = 0; unitIndex < 4; units++, unitIndex++) {
        Ov96R118Object *object = &units->object;
        Ov96R118Gauge *gauge;
        gaugeIndex = 0;
        gauge = units->gauges;
        do {
            int limit;
            float increment = 0.0f;
            limit = *(int *)((u8 *)gauge->config + 8);
            if (object->flags.bits.field22 == gaugeIndex) {
                if (object->flags.bits.bit28) {
                    object->unk5C--;
                    if (object->unk5C <= 0) {
                        gauge->value = 5 << 14;
                        object->vec20 = object->vec2C;
                        object->flags.bits.bit28 = 0;
                    }
                } else if (gauge->value <= 0) {
                    object->unk5C = 60;
                    object->flags.bits.bit28 = 1;
                    object->flags.bits.bit29 = 0;
                    object->flags.bits.bit30 = 0;
                    PokeathlonCourse_AddMonCounter(object->unk00, object->flags.bits.field20, object->flags.bits.field22, 1, 1);
                }
            } else {
                increment = 819.0f;
            }
            gauge->value = (int)((float)gauge->value + increment);
            if (gauge->value > limit) {
                gauge->value = limit;
            }
            gaugeIndex++;
            gauge++;
        } while (gaugeIndex < 3);
    }
}
