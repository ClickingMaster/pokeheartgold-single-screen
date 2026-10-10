#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 words[76];
} Global;

typedef struct {
    u32 addr;
    u8 pad[6];
    u16 tick;
} Entry;

typedef struct {
    OSThread *thread;
    int wait;
    u8 state;
    u8 pad[7];
    int tick;
    u8 pad2[4];
    u16 port, oldport;
    u32 seq, oldseq;
} Socket;
typedef struct Thread Thread;

struct Thread {
    u8 pad[0x68];
    Thread *next;
    u8 pad2[0x38];
    Socket *socket;
};

typedef struct {
    u8 pad[4];
    u16 active;
    u8 pad2[0x26];
    int tick;
    u32 pad3;
    void *buffer;
} Frag;

extern Global _021D4620;
extern u8 _021D4700[];
extern u8 _021D4CCC[];
extern u8 _021D4B4C[];
extern Entry _021D46A0[8];
extern Frag _021D4948[8];
extern void CPS_SocRegister(void *);
extern void CPS_SocUnRegister(void);
extern void set_fixed_ip(void);
extern void reset_network_vars(int);
extern void send_arprequest(void);
extern void CPSi_SslPeriodical(int);
extern void dhcp_release_server(void);
extern int dhcp_discover_server(void);
extern int dhcp_request_server(int *, int);
void scavenger(void *);

void scavenger(void *unused) {
    int timer, arp, first, state;
    int now;
    int i;
    _021D4620.words[17] = 0;
    MI_CpuFill8(_021D4700, 0, 100);
    _021D4620.words[71] = 384;
    _021D4620.words[72] = (u32)_021D4CCC;
    _021D4620.words[74] = 384;
    _021D4620.words[75] = (u32)_021D4B4C;
    CPS_SocRegister(_021D4700);
    arp = 1;
    first = 1;
    timer = 1;
    _021D4620.words[3] = 1;
    state = 0;
    for (;;) {
        OS_Sleep(1000);
        if (_021D4620.words[17]) {
            break;
        }
        now = OS_GetTick() >> 16;
        if (((int (*)(void))_021D4620.words[18])()) {
            if (--timer == 0) {
                if (_021D4620.words[4] & 1) {
                    if (!state) {
                        set_fixed_ip();
                        state = 1;
                    }
                } else {
                    switch (state) {
                    case 0:
                        if (first) {
                            _021D4620.words[3] = 2;
                            first = 0;
                        }
                        if (!dhcp_discover_server() || !dhcp_request_server(&timer, 0)) {
                            set_fixed_ip();
                            state = 3;
                        } else {
                            state = 1;
                        }
                        break;
                    case 1:
                        if (!dhcp_request_server(&timer, 1)) {
                            if ((u32)timer < 60) {
                                state = 2;
                            }
                        }
                        break;
                    case 2:
                        if (dhcp_request_server(&timer, 2)) {
                            state = 1;
                        } else if ((u32)timer < 60) {
                            reset_network_vars(3);
                            arp = 1;
                            timer = 1;
                            state = 0;
                        }
                        break;
                    case 3:
                        break;
                    }
                }
            }
        } else {
            reset_network_vars(1);
            arp = 1;
            timer = 1;
            state = 0;
        }
        for (i = 0; i < 8; i++) {
            if (_021D46A0[i].addr && ((s16)(now - _021D46A0[i].tick) > 957)) {
                _021D46A0[i].addr = 0;
            }
        }
        if (_021D4620.words[11] && !--arp) {
            send_arprequest();
            arp = 105;
        }
        {
            Thread *t = (Thread *)OSi_ThreadInfo.list;
            for (; t; t = t->next) {
                Socket *s = t->socket;
                if (s && s->thread) {
                    if (s->state == 3 && now - s->tick > 39) {
                        s->state = 1;
                        s->port = s->oldport;
                        s->seq = s->oldseq;
                    } else if (s->state == 2 && now - s->tick > 39) {
                        if (s->wait == 1) {
                            s->state = 0;
                            s->wait = 0;
                            OS_WakeupThreadDirect(s->thread);
                        }
                    } else if (s->state != 4 && s->wait == 2) {
                        s->wait = 0;
                        OS_WakeupThreadDirect(s->thread);
                    }
                }
            }
        }
        for (i = 0; i < 8; i++) {
            if (_021D4948[i].active && now - _021D4948[i].tick > 239) {
                ((void (*)(void *))_021D4620.words[16])(_021D4948[i].buffer);
                _021D4948[i].active = 0;
            }
        }
        CPSi_SslPeriodical(now);
        if (_021D4620.words[15]) {
            ((void (*)(void))_021D4620.words[15])();
        }
    }
    if (!(_021D4620.words[4] & 1) && state != 3) {
        dhcp_release_server();
    }
    CPS_SocUnRegister();
    (void)unused;
}
