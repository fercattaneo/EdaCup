#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "data.h"
#include "Robot.h"
using namespace std;


int fail()
{
    cout << "FAIL" << endl;
    return 1;
}

int pass()
{
    cout << "PASS" << endl;
    return 0;
}


int main (void) 
{
    cout << "                   __TESTING__" << endl;

    //Test 1: Prueba del proporcional entre dos puntos
    Vector2 originPos = {-1,1};
    Vector2 finalPos = {1,-1};
    cout << "TESTS FOR PROPORTIONAL POSITION FUNCTION:" << endl;
    cout << "Testing half position...";
    float proportion = 0.5f;
    Vector2 result4 = proportionalPosition(originPos, finalPos, proportion);
    if (result4.x == 0 && result4.y == 0 )
    {
        pass();
    }
    else
    {
        fail();
    }

    float proportion2 = 0.0f;
    cout << "Testing origin position...";
    Vector2 result5 = proportionalPosition(originPos, finalPos, proportion2);
    if (result5.x == originPos.x && result5.y == originPos.y)
    {
        pass();
    }
    else
    {
        fail();
    }

    float proportion3 = 1.0f;
    cout << "Testing final position...";
    Vector2 result6 = proportionalPosition(originPos, finalPos, proportion3);
    if (result6.x == finalPos.x && result6.y == finalPos.y)
    {
        pass();
    }
    else
    {
        fail();
    }

    //Test 2: Prueba del calculo de la rotacion del jugador adecuada.
    cout << "Testing rotation...";
    Vector2 originPos3 = {1, -1};
    Vector2 finalPos3 = {-1, 1};
    float result7 = calculateRotation(originPos3, finalPos3);
    if (result7 <= -225.0f && result7 >= -225.1f)//ESTO NO FUNCIONA NUNCA
    {
        pass();
    }
    else
    {
        fail();
    }

    //Test 3: Prueba que verifica si una coordenada está cerca de otra,
    cout << "Testing if 2 coordenates are close to each other ";
    Vector2 originCoord = {1,1};
    Vector2 destinationCoord = {1,0};
    float nearRange = 2;
    if (isCloseTo(originCoord, destinationCoord, nearRange))
    {
        pass();
    }
    else
    {
        fail();
    }
    
    //Test 4: Prueba que verifica si el robot y la pelota están en sobre la misma linea. 
    cout << "TESTS FOR SAME LINE FUNCTION:" << endl;
    cout << "Testing positive intermediate...";
    Vector2 originPos4 = {8,4};
    Vector2 finalPos4 = {0,0};
    Vector2 mediumPos = {4,2};
    if(sameLine (originPos4, finalPos4, mediumPos))
    {
        pass();
    }
    else
    {
        fail();
    }

    cout << "Testing negative intermediate...";
    Vector2 mediumPos3 = {-4, -2};
    if(sameLine (originPos4, finalPos4, mediumPos3))
    {
        pass();
    }
    else
    {
        fail();
    }
    

    return 0;
}
    