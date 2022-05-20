#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "data.h"
#include "Robot.h"
using namespace std;
/*
-HAY QUE PONER EL .JSON EN EL CMAKE
*/


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
    /*Test 1. Cambio de datos 
    *
    */ 
        cout << "Testing change a float to a vector if char..." << endl;
        float data = 1.0
        std::vector<char> result = getDataFromFloat(data);
        cout << result << endl;
        /*if (result == )
        {
            pass();
        }
        else
        {
            fail();
        }*/
    
    //Test 2: Prueba de conversion de dato 2.
    Vector2 coord = {1, -1};
    float rotation = 1.05f
    setPoint_t var = {coord, rotation};
    cout << "TESTS FOR GET ARRAY FROM SETPOINT FUNCTION:" << endl;
    std::vector<char> result = getArrayFromSetPoint(var);
    cout<<result<<endl;
    /*if (result == )
    {
        pass();
    }
    else
    {
        fail();
    }*/

    //Test 3: Otra conversion de dato
    vector<char> test = {'a', 'b', 'c'};
    float result = getFloat(vector<char> vec);
    cout << result << endl;
    /*if (result == )
    {
        pass();
    }
    else
    {
        fail();
    }*/

    //Test 4: Prueba del proporcional entre dos puntos
    Vector2 originPos = {-1,1};
    Vcctor2 finalPos = {1,-1};
    cout << "TESTS FOR PROPORTIONAL POSITION FUNCTION:" << endl;
    cout << "Testing half position...";
    float proportion = 0.5f;
    Vector2 result = proportionalPosition(originPos, finalPos, proportion);
    if (result.compare({0,0}))
    {
        pass();
    }
    else
    {
        fail();
    }

    float proportion = 0f;
    cout << "Testing origin position...";
    Vector2 result = proportionalPosition(originPos, finalPos, proportion);
    if (result.compare(originPos))
    {
        pass();
    }
    else
    {
        fail();
    }

    float proportion = 1.0f;
    cout << "Testing final position...";
    Vector2 result = proportionalPosition(originPos, finalPos, proportion);
    if (result.compare(finalPos))
    {
        pass();
    }
    else
    {
        fail();
    }

    //Test 5: Prueba del calculo de la rotacion del jugador adecuada.
    cout << "Testing rotation..." << endl;
    Vector2 originPos = {1, -1};
    Vector2 finalPos = {-1, 1};
    float result = calculateRotation(originPos, finalPos);
    if (result == -45.0)
    {
        pass();
    }
    else
    {
        fail();
    }

    //Test 5: Prueba que verifica si una coordenada está cerca de otra,
    cout << "Testing if 2 coordenates are close to each other" << endl;
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
    
    //Test 6: Prueba que verifica si el robot y la pelota están en sobre la misma linea. 
    cout << "TESTS FOR SAME LINE FUNCTION:" << endl;
    cout << "Testing positive intermediate...";
    Vector2 originPos = {8,4};
    Vector2 finalPos = {0,0};
    Vector2 mediumPos = {4,2};
    if(sameLine (originPos, finalPos, mediumPos))
    {
        pass();
    }
    else
    {
        fail();
    }

    cout << "Testing negative intermediate...";
    Vector2 mediumPos = {-4,-2};
    if(sameLine (originPos, finalPos, mediumPos))
    {
        pass();
    }
    else
    {
        fail();
    }
    

    return 0;
}
    