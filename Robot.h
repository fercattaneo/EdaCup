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

protected:
    Vector3 position;
    Vector3 speed;
    Vector3 rotation;
    Vector3 angularSpeed;
};

#endif