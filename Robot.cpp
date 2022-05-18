#include "Robot.h"

/* SETTERS */
void Robot::setPosition(Vector3 pos)
{
    position = pos;
}

void Robot::setRotation(Vector3 newRot)
{
    rotation = newRot;
}

void Robot::setSpeed(Vector3 newSpeed)
{
    speed = newSpeed;
}

void Robot::setAngularSpeed(Vector3 newAngSpeed)
{
    angularSpeed = newAngSpeed;
}

/* GETTERS */
Vector3 Robot::getPosition()
{
    return position;
}

Vector3 Robot::getRotation()
{
    return rotation;
}

Vector3 Robot::getSpeed()
{
    return speed;
}

Vector3 Robot::getAngularSpeed()
{
    return angularSpeed;
}