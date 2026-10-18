#ifndef POKEHEARTGOLD_CHAT_USER_MODE_H
#define POKEHEARTGOLD_CHAT_USER_MODE_H

// The mode of a user in a chat channel, from the '@' (op) and '+' (voice) nick prefixes
enum ChatUserMode {
    CHAT_USER_MODE_NORMAL,
    CHAT_USER_MODE_VOICE,
    CHAT_USER_MODE_OP,
    CHAT_USER_MODE_VOICE_OP,
};

#endif // POKEHEARTGOLD_CHAT_USER_MODE_H
