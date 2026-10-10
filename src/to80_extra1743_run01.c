#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad0[0x80c];
    void *queue;
} Work;

typedef struct {
    char *name;
    u32 a, b;
} Command;

extern const char ov45_022559B8[];
extern const char ov45_02255918[];
extern const char ov45_02255764[];
extern const char ov45_02255D18[];
extern const char ov45_02255D28[];
extern const char ov45_02255D38[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02249390(void *);
extern void ov00_021FF160(void *, void (*)(void *), Command *);
void ov45_02249460(Work *, const char *, u32, u32);

void ov45_02249460(Work *w, const char *name, u32 a, u32 b) {
    char buffer[224];
    Command command;
    if (!w) {
        __msl_assertion_failed(ov45_022559B8, ov45_02255918, ov45_02255764, 1085);
    }
    if (!name) {
        __msl_assertion_failed(ov45_02255D18, ov45_02255918, ov45_02255764, 1087);
    }
    if (*name == 0) {
        __msl_assertion_failed(ov45_02255D28, ov45_02255918, ov45_02255764, 1087);
    }
    if (strlen(name) >= 128) {
        __msl_assertion_failed(ov45_02255D38, ov45_02255918, ov45_02255764, 1087);
    }
    strncpy(buffer, name, 128);
    buffer[127] = 0;
    command.a = a;
    command.b = b;
    command.name = buffer;
    ov00_021FF160(w->queue, ov45_02249390, &command);
}
