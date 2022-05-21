#ifndef MIDFIELDER_H_
#define MIDFIELDER_H_

#include "Players.h"

class Midfielder : public Players
{
public:
    Midfielder(Vector2 teamGoal);
    ~Midfielder();

    void updateAttack();
    void updateDefense(setPoint_t oppAttacker); 

private:
};

#endif //MIDFIELDER_H_