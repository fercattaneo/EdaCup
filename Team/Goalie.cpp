#include "Goalie.h"

Goalie::Goalie(Vector2 teamGoal)
{
    fieldRol = GOALIE;
    myGoal = teamGoal;
}

Goalie::~Goalie()
{
    
}

void Goalie::updateDefense(setPoint_t oppAttacker)
{
    /* Condicion de si la pelota está en el area que la vaya a buscar
    float goalX = myGoal.x > 0 ? myGoal.x : -myGoal.x;
    float ballX = ballPosition.x > 0 ? ballPosition.x : ballPosition.x;
    if(ballX< goalX && ballX> 3.5f && ballPosition.z >= -1 && ballPosition.z <= 1 ) //if (((ballPosition.x > myGoal.x && ballPosition.x < -3.5f) || (ballPosition.x < myGoal.x && ballPosition.x > 3.5f)) && ballPosition.z >= -1 && ballPosition.z <= 1)
    {

    }*/
    intercept(oppAttacker);
    if (isCloseTo({ position.x, position.z }, oppAttacker.coord, 3))
    {
        setDribbler(1);
    }
}

void Goalie::intercept(setPoint_t oppAttacker)
{
    float deltaX = position.x - oppAttacker.coord.x;
    float alpha = myGoal.x < 0 ? (oppAttacker.rotation - 90) : (270 - oppAttacker.rotation);
    Vector2 destination = proportionalPosition(myGoal, oppAttacker.coord ,(0.5 / 9) );
    destination.y = oppAttacker.coord.y - tan(alpha) * (deltaX);

    setPosition({ destination.x, 0, destination.y });
}