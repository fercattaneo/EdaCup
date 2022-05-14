#include "data.h"
#include <iostream>
#include <deque>

using namespace std;

vector<char> getArrayFromSetPoint(setPoint_t setpoint)
{

	vector<char> payload(12);

	*((float*)&payload[0]) = setpoint.coord.x;
	*((float*)&payload[4]) = setpoint.coord.y;
	*((float*)&payload[8]) = setpoint.rotation;

	return payload;
}

float getFloat(vector<char> vec)
{
	float value = 0.0;

	// Extraído de: https://www.cplusplus.com/reference/cassert/assert/
	assert(vec.size() == sizeof(value));

	memcpy(&value, &vec[0], min(vec.size(), sizeof(float)));
	return value;
}

/*
 *	@brief: calculates the coordinate in reference from other 2 and a proportional value
 *   @param: originPos - origin position of object
 *   @param: finalPos - final position of reference
 *   @param: proportion - proportional position [0 = origin ~~ 1 = final]
 *   @return: coordinate calculated
 */
Vector2 proportionalPosition(Vector2 originPos, Vector2 finalPos, float proportion)
{
	Vector2 destination;
	destination.x = (finalPos.x - originPos.x) * proportion + originPos.x;
	destination.y = (finalPos.y - originPos.y) * proportion + originPos.y;
	return destination;
}

/*
 *	@brief: calculates the rotation between 2 coordinates
 *   @param: originPos - origin position of object
 *   @param: finalPos - final position of reference
 *   @return: angle in eulerian degrees
 */
float calculateRotation(Vector2 originPos, Vector2 finalPos)
{
	float deltaX = finalPos.x - originPos.x;
	float deltaZ = finalPos.y - originPos.y;

	if (deltaX == 0 && deltaZ == 0)
	{
		cout << "Same Position delivered" << endl;
		return 0;
	}
	if (deltaZ == 0)
	{
		cout << "Invalid Angle, aprox to 90°" << endl;
		return 90;
	}
	if (deltaX == 0)
	{
		cout << "Invalid Angle, aprox to 0°" << endl;
		return 0;
	}

	float angle = 1 / (std::tan(deltaX / deltaZ)); // angulo en radianes
	angle = angle * (180 / PI);					   // conversion a grados sexagecimales
	return angle;

	// creo que para los robots el angulo va a tener q ser el q recibe pero negativo
	// correccion: angulo - 90, no negativo xq es en referencia al eje z
	// ver bien despues in-game
}

void separateString (string topic,  deque<string>cadena)
{
	char * ptrString = (char*)&topic;
	string myString = "";
	while(*ptrString != '/0')
	{
		if(*ptrString != '/') 
		{
			myString += to_string(*ptrString);  //sumo el caracter al string
		}
		else     //separo el string
		{			
			cadena.push_front(myString);  //despues hacer pop__back para calsificarlos mejor
			myString = "";
		}
		ptrString++;   //voy al prox caracter
	}
}
