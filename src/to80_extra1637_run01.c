#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct State {
    u8 pad0[4];
    int *socket;
    u8 pad8[5];
    u8 local;
    u8 padE[2];
    void *queue;
    u8 count;
    u8 mode;
    u8 expected;
    u8 active;
    u8 pad18[2];
    u16 port;
    int address;
    int owner;
    int addresses[32];
    u16 ports[32];
    void *connection;
    int flagE8;
    u64 tickEC;
    int peers[32];
    u8 pad174[24];
    int flag18C;
    int flag190;
    u8 flag194;
    u8 pad195[7];
    void *pending;
    int state;
    u8 counter;
    u8 pad1A5[1];
    u8 flag1A6;
    u8 retry;
    u8 pad1A8[12];
    u16 remotePort;
    u8 pad1B6[2];
    int remoteAddress;
    int flag1BC;
    u64 tick1C0;
    int flag1C8;
    u64 tick1CC;
    u8 pad1D4[8];
    int bits;
    u8 pad1E0[32];
    int peer200;
    int peer204;
    int flag208;
    int peer20C;
    u8 pad210[192];
    u8 playerFlags[32];
    u8 pad2F0[220];
    u8 last;
    u8 pad3CD[159];
    void (*callback)(int, void *);
    void *callbackArg;
} State;

typedef struct Aux {
    u8 enabled;
    u8 pad[3];
    u32 threshold;
    int bits8, bitsC;
    u64 tick;
} Aux;

typedef struct Sock {
    u16 family, port;
    int address;
} Sock;

extern Aux *ov00_0221A928;
extern State *ov00_021F955C(void);
extern int ov00_021EE568(void);
extern void ov00_021F7DB4(void);
extern void ov00_021F956C(int);
extern int ov00_021F5624(int, int, int, int, int);
extern void ov00_021F5920(int, int, int);
extern int ov00_021F5B30(void);
extern int ov00_021F0404(int);
extern void ov00_021F4304(int, int, int, int, int *, int);
extern int ov00_021F6B20(int *, int);
extern void ov00_021F5AE0(int, int *);
extern void ov00_021F60AC(int);
extern void ov00_021F5D0C(int, int, int);
extern void ov00_021F63DC(void);
extern int ov00_022139B8(void *, int);
extern void ov00_021F3F5C(int, int, int);
extern int ov00_021F8468(void);
extern void ov00_021F3CF8(int);
extern int ov00_021F82EC(void);
extern void ov00_021F2EE4(int, int);
extern int ov00_021F5FD4(int);
extern void ov00_0220D238(int);
extern int ov00_021F6100(int);
extern int ov00_0220D294(int);
extern void ov00_021F9104(int, int, Sock *, u8 *);
extern int ov00_021F7BA0(int);
extern void ov00_02210CBC(void *);
extern void ov00_021F64C0(int);
extern int ov00_021F6FF8(void);
extern int ov00_021F631C(int);
extern void ov00_021F7128(void);
extern void ov00_021F6EC4(int);
extern int ov00_021F7674(int, int, int);
extern int ov00_021F7CEC(int, int);
extern void ov00_021EE31C(int);
extern void ov00_021F6DF0(void);
extern void ov00_0221019C(void *);
extern void ov00_021F6FC4(void);
int ov00_021F4740(int, int, int, int, int *, int);
#define GET()   ov00_021F955C()
#define ERROR() ov00_021F5B30()

int ov00_021F4740(int event, int peer, int address, int port, int *payload, int length) {
    int count = 0;
    int newPeer;
    Sock sock;
    int words[65];
    if (!GET() || GET()->state == 0) {
        return 1;
    }
    if (ov00_021EE568() == 5 && GET()->state == 1 && peer == GET()->peers[0]) {
        ov00_021F7DB4();
    }
    switch (event) {
    case 1:
    case 11: {
        int response;
        if (GET()->mode != 0) {
            address = payload[1];
            port = (u16)payload[2];
        }
        response = ov00_021F5624(peer, address, port, payload[0], event == 11);
        if (response == 2) {
            GET()->bits = 0;
            ov00_021F5920(peer, address, port);
            if (ERROR()) {
                return 0;
            }
            if (GET()->mode == 2 && GET()->callback) {
                State *state = GET();
                int id = ov00_021F0404(peer);
                state->callback(id, GET()->callbackArg);
            }
            words[0] = GET()->count;
            for (count = 1; count <= GET()->count; count++) {
                words[count] = GET()->peers[count];
            }
            words[count++] = GET()->address;
            words[count++] = GET()->port;
            ov00_021F956C(11);
        } else if (response == 3 && GET()->expected != 0) {
            State *state = GET();
            if (state->count == GET()->expected && GET()->mode == 2) {
                words[0] = 16;
                count = 1;
            }
        }
        if (response != 255) {
            ov00_021F4304(response, peer, address, port, words, count);
            if (ERROR()) {
                return 0;
            }
        }
        break;
    }
    case 2:
        if (GET()->state != 4 || peer != GET()->peer204) {
            break;
        }
        GET()->flag208 = 0;
        GET()->retry = 0;
        GET()->flag1C8 = 0;
        GET()->flag1BC = 0;
        GET()->addresses[0] = (payload + payload[0])[1];
        GET()->ports[0] = (payload + payload[0])[2];
        GET()->remoteAddress = (payload + payload[0])[1];
        GET()->remotePort = (payload + payload[0])[2];
        if (GET()->mode == 1) {
            if (ov00_021F6B20(payload + 1, payload[0])) {
                if (GET()->local) {
                    ov00_021F5AE0(peer, payload);
                }
            } else {
                ov00_021F60AC(peer);
                if (ERROR()) {
                    return 0;
                }
                ov00_021F5D0C(0, 0, peer);
                if (ERROR()) {
                    return 0;
                }
                break;
            }
        }
        if (GET()->mode == 0) {
            if (GET()->local) {
                ov00_021F5AE0(peer, payload);
                ov00_021F63DC();
                if (ERROR()) {
                    return 0;
                }
            }
            ov00_021F956C(6);
            ov00_021F3F5C(0, 0, ov00_022139B8(GET()->connection, 0));
            if (ov00_021F8468()) {
                return 0;
            }
        } else {
            ov00_021F956C(5);
            ov00_021F3CF8(peer);
            if (ov00_021F82EC()) {
                return 0;
            }
        }
        break;
    case 3:
        if (GET()->state != 4 || peer != GET()->peer204) {
            break;
        }
        if (length > 0 && payload[0] == 16) {
            ov00_021F2EE4(13, 0);
            return 0;
        }
        return ov00_021F5FD4(GET()->peer204);
    case 4: {
        State *state;
        if (GET()->state != 4 || peer != GET()->peer204) {
            break;
        }
        state = GET();
        state->tick1CC = OS_GetTick();
        if ((GET()->flag208 && GET()->retry < 16) || GET()->mode == 3) {
            GET()->flag1BC = 1;
            state = GET();
            state->tick1C0 = OS_GetTick();
            if (GET()->mode != 3) {
                GET()->retry++;
            }
        } else {
            GET()->flag208 = 0;
            GET()->retry = 0;
            if (GET()->mode == 0) {
                ov00_021F956C(3);
                GET()->flagE8 = 1;
                state = GET();
                state->tickEC = OS_GetTick();
            } else if (GET()->mode == 1) {
                ov00_021F5D0C(1, 0, 0);
            }
        }
        break;
    }
    case 5:
        if (!GET()->active || peer != GET()->owner) {
            break;
        }
        if (GET()->mode == 2 && GET()->local == 1 && peer == GET()->peers[1]) {
            ov00_0220D238(*GET()->socket);
        }
        if (!ov00_021F6100(peer)) {
            return 0;
        }
        break;
    case 6: {
        int newPort;
        int newAddress;
        newAddress = payload[0];
        newPort = (u16)payload[1];
        int socket;
        State *state;
        if (GET()->state == 1) {
            ov00_021F956C(6);
        } else {
            if (GET()->state != 6 && GET()->state != 11) {
                break;
            }
            if (peer != GET()->owner) {
                break;
            }
        }
        GET()->last = 255;
        state = GET();
        if (state->peers[GET()->local + 1] != peer) {
            state = GET();
            state->peers[GET()->local + 1] = peer;
        }
        sock.address = newAddress;
        sock.port = ((newPort >> 8) & 255) | ((newPort << 8) & 0xff00);
        GET()->flag194 = 1;
        socket = ov00_0220D294(*GET()->socket);
        ov00_021F9104(0, socket, &sock, &GET()->flag194);
        state = GET();
        state->flag18C = 0;
        state->flag190 = 0;
        break;
    }
    case 7: {
        int flag;
        State *state;
        if (GET()->state != 1 || peer != GET()->peers[0]) {
            break;
        }
        newPeer = payload[0];
        flag = payload[1] & 255;
        if (GET()->mode == 1 && !GET()->flag1A6) {
            words[0] = ov00_021F7BA0(newPeer);
            ov00_021F4304(32, peer, address, port, words, 1);
            if (ERROR()) {
                return 0;
            }
        }
        state = GET();
        state->peers[GET()->count + 1] = newPeer;
        state = GET();
        state->playerFlags[GET()->count + 1] = flag;
        ov00_02210CBC(GET()->queue);
        if (GET()->callback) {
            int id;
            state = GET();
            id = ov00_021F0404(newPeer);
            state->callback(id, GET()->callbackArg);
        }
        break;
    }
    case 8: {
        int index;
        State *state;
        if (GET()->state != 1 || peer != GET()->peers[0]) {
            break;
        }
        newPeer = payload[0];
        if (!newPeer) {
            index = payload[1];
            count = payload[2];
            ((State *)(index + (u32)GET()))->playerFlags[0] = count;
            state = GET();
            state->peers[index] = GET()->peer200;
            ov00_021F64C0(3);
            break;
        }
        count = payload[2] & 255;
        index = payload[1];
        if (newPeer == GET()->peers[index] && index == GET()->local - 1) {
            state = GET();
            ov00_021F4304(9, peer, state->addresses[0], GET()->ports[0], &newPeer, 1);
            if (ERROR()) {
                return 0;
            }
            break;
        }
        GET()->peers[index] = newPeer;
        ((State *)(index + (u32)GET()))->playerFlags[0] = count;
        GET()->addresses[index] = payload[3];
        GET()->ports[index] = payload[4];
        GET()->remoteAddress = payload[3];
        GET()->remotePort = payload[4];
        ov00_021F956C(5);
        ov00_021F3CF8(newPeer);
        if (ov00_021F82EC()) {
            return 0;
        }
        GET()->flag1C8 = 0;
        GET()->flag1BC = 0;
        break;
    }
    case 9: {
        State *state;
        if (GET()->state != 13) {
            break;
        }
        state = GET();
        if (payload[0] != state->peers[GET()->counter + 1]) {
            break;
        }
        GET()->counter++;
        ov00_021F64C0(0);
        break;
    }
    case 10:
        if (GET()->state != 1 && GET()->state != 18) {
            break;
        }
        if (GET()->mode == 0 || ov00_021F6B20(payload + 1, payload[0])) {
            GET()->flag208 = payload[1];
            GET()->retry = 0;
        } else {
            GET()->flag208 = 0;
        }
        if (GET()->local) {
            ov00_0220D238(*GET()->socket);
        } else if (ov00_021F6FF8()) {
            return 0;
        }
        break;
    case 12:
        if (peer != GET()->peers[0]) {
            break;
        }
        if (GET()->mode == 0 || GET()->mode == 1) {
            if (!ov00_021F631C(peer)) {
                return 0;
            }
        } else if (GET()->mode == 3) {
            GET()->peer20C = peer;
            ov00_021F7128();
            ov00_021F6EC4(0);
        }
        break;
    case 13:
    case 14:
    case 15:
        if (!ov00_021F7674(peer, event, payload[0])) {
            return 0;
        }
        break;
    case 16: {
        int i;
        if (peer != GET()->peers[0]) {
            return 1;
        }
        for (i = 0; i < length; i++) {
            int index = ov00_021F7CEC(payload[i], 0);
            if (index != 255) {
                ov00_021EE31C(index);
            }
        }
        break;
    }
    case 17: {
        Aux *aux = ov00_0221A928;
        if (aux && aux->enabled && (((OS_GetTick() - aux->tick) << 6) / 0x82ea) >= aux->threshold) {
            words[0] = 1;
        } else {
            words[0] = 0;
        }
        ov00_021F4304(18, peer, address, port, words, 1);
        if (ERROR()) {
            return 0;
        }
        break;
    }
    case 18: {
        int index;
        if (GET()->state != 19) {
            break;
        }
        index = ov00_021F7CEC(peer, 0);
        if (index == 255) {
            break;
        }
        ov00_0221A928->bits8 |= 1 << index;
        if (payload[0]) {
            ov00_0221A928->bitsC |= 1 << index;
        }
        break;
    }
    case 19:
        ov00_021F2EE4(12, 0);
        return 0;
    case 32: {
        int i;
        if (GET()->mode != 1 || GET()->flag1A6) {
            break;
        }
        for (i = 1; i <= GET()->local; i++) {
            if (peer == GET()->peers[i]) {
                if (!payload[0]) {
                    if (GET()->state == 11 || GET()->state == 12) {
                        ov00_021F6DF0();
                        if (ERROR()) {
                            return 0;
                        }
                    }
                    if (GET()->pending) {
                        ov00_0221019C(GET()->pending);
                        GET()->pending = 0;
                    }
                    ov00_021F6FC4();
                } else {
                    State *state = GET();
                    state->bits |= 1 << ov00_021F7CEC(peer, 0);
                }
                break;
            }
        }
        break;
    }
    case 64: {
        int i;
        for (i = 1; i <= GET()->local; i++) {
            if (peer == GET()->peers[i]) {
                ov00_021F4304(65, peer, address, port, 0, 0);
                if (ERROR()) {
                    return 0;
                }
                break;
            }
        }
        break;
    }
    case 65:
        break;
    }
    return 1;
}
