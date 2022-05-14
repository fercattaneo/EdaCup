#ifndef ROBOT_H
#define ROBOT_H

#include <raylib.h>

#include <string>

using namespace std;

typedef struct
{
    float x;
    float y;
    float z;
} vector3_t;

class Robot
{
public:
    string teamID;
    string robotID;

protected:
    vector3_t position;
    vector3_t speed;
    vector3_t rotation;
    vector3_t angularSpeed;
};

#endif