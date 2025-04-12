#pragma once
#ifndef INTERFACE_GAME
#define INTERFACE_GAME

// Interface of all playable games
class IGame {
public:
    // The main loop in escaperoom.ino will ALWAYS call run() on any game.
    virtual void run() = 0;
protected:
    virtual void setup() = 0;
    virtual void running() = 0;
    virtual void gameDone() = 0;
};

#endif // INTERFACE_GAME