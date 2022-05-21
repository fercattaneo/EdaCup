#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "Players.h"

class Shooter : public Players
{
public:
    Shooter(Vector2 teamGoal);
    ~Shooter();

    void updateAttack();
    void updateDefense(setPoint_t oppAttacker); 

private:
};

#endif // SHOOTER_H_