#ifndef ROBOT_H
#define ROBOT_H

#include "raylib.h"
#include <string>

using namespace std;

typedef struct
{
    float x;
    float z;
} coord_t;

typedef struct
{
    Vector2 coord;
    float rotation;
} setPoint_t;

class Robot
{
public:
    string teamID;
    string robotID;

    Vector3 getPosition();
    Vector3 getRotation();
    Vector3 getSpeed();
    Vector3 getAngularSpeed();
    float getKickerCharge();

    void setPosition(Vector3 newPos);
    void setRotation(Vector3 newRot);
    void setSpeed(Vector3 newSpeed);
    void setAngularSpeed(Vector3 newAngSpeed);
    void setPowerLevels(Vector3 powerValues);

protected:
    Vector3 position;
    Vector3 rotation;
    Vector3 speed;
    Vector3 angularSpeed;
    Vector3 powerLevel; // x for consume, y for battery level, z for kicker charge
};

#endif