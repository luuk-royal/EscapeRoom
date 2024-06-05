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
  endingScreen = 6
};

#endif // ESCAPE_ROOM_STATUS_H