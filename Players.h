#ifndef PLAYERS_H
#define PLAYERS_H

#include "Robot.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <assert.h>
#include "data.h"

using namespace std;

// Rol of each player
enum PLAYERS_POSITION
{
    GOALIE,
    DEFENSE,
    MIDFIELDER,
    SHOOTER
};

class Players : public Robot
{
public:
    Players();
    ~Players();

    void start(string playerNumber);
    void moveMotors();
    setPoint_t goToBall(Vector2 oppositeGoal, Vector2 ballPosition, float proportional);
    setPoint_t kickBallLogic(Vector2 oppositeGoal, Vector2 ballPosition);

    PLAYERS_POSITION fieldRol;
    void toEnablePlayer(void);
    void dissablePlayer(void);

private:
    bool enablePlayer;
};

#endif // PLAYERS_H