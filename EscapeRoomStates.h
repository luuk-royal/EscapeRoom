#pragma once
#ifndef ESCAPE_ROOM_STATUS_H
#define ESCAPE_ROOM_STATUS_H

enum EscapeRoomStatus {
  inMap = 0,
  inGame1 = 1,
  inGame2 = 2,
  inGame3 = 3,
  inGame4 = 4,
  startingScreen = 5,
  endingScreen = 6,
  gameOverScreen = 7
};

struct GamesDone
{
  bool gameOneDone = false;
  bool gameTwoDone = false;
  bool gameThreeDone = false;
  bool gameFourDone = false;
};

enum MoleStatus {
  coolingDown,
  started,
  ended,
};

enum DifficultyLevel {
  easy,
  medium,
  hard,
};

enum GameState {
  initializing,
  inGame,
  done,
  failed, // used to reset the game
};

#endif // ESCAPE_ROOM_STATUS_H