#ifndef DEFENSE_H_
#define DEFENSE_H_

#include "Players.h"

class Defense : public Players
{
public:
    Defense(Vector2 teamGoal);
    ~Defense();

    void updateAttack();
    void updateDefense(setPoint_t oppAttacker); 

private:
};

#endif  // DEFENSE_H_