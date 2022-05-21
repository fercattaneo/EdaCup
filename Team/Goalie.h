#ifndef GOALIE_H
#define GOALIE_H

#include "Players.h"

class Goalie : public Players
{
public:
    Goalie(Vector2 teamGoal);
    ~Goalie();

    void updateAttack();
    void updateDefense(setPoint_t oppAttacker); 

private:
    void intercept(setPoint_t oppAttacker);
};

#endif //GOALIE_H