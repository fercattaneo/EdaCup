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
    Players(string path);
    ~Players();

    void start(MQTTClient2 *mqttClient, string playerNumber);
    void setDribbler();
    void setKickerChipper();
    void moveMotors();
    vector<char> getArrayFromFloat(float payload);
    std::vector<char> getArrayFromSetPoint(setPoint_t payload);
    float getFloat(std::vector<char> vec);
    Vector2 proportionalPosition(Vector2 originPos, Vector2 finalPos, float proportion);
    float calculateRotation(Vector2 originPos, Vector2 finalPos);
    setPoint_t goToBall (Vector2 pointF);

    PLAYERS_POSITION fieldRol;
    void toEnablePlayer(void);
    void dissablePlayer(void);

private:
    MQTTClient2 *mqttClient;
    bool enablePlayer;

    Image displayImage;

    void setDisplay(string path);
};

#endif // PLAYERS_H