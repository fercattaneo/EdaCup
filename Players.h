#ifndef PLAYERS_H
#define PLAYERS_H

#include "Robot.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include "MQTTClient2.h"
#include <assert.h>
//#include "GameModel.h"

using namespace std;

typedef struct
{
    float x;
    float z;
} coordinate_t;

typedef struct
{
    coordinate_t coord;
    float rotation;
} setPoint_t;

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
    Players(string path);
    ~Players();

    void start(MQTTClient2 *mqttClient, string playerNumber);
    void setDribbler();
    void setKickerChipper();
    void moveMotors();
    vector<char> getArrayFromFloat(float payload);
    float getFloat(std::vector<char> vec);
    coordinate_t proportionalPosition(coordinate_t originPos, coordinate_t finalPos, float proportion);
    float calculateRotation(coordinate_t originPos, coordinate_t finalPos);

    PLAYERS_POSITION fieldRol;
    void toEnablePlayer(void);
    void dissablePlayer(void);

private:
    MQTTClient2 *mqttClient;
    bool enablePlayer;

    string teamID;
    string playerId; // robot
    Image displayImage;

    void setDisplay(string path);
};

#endif // PLAYERS_H