#include "data.h"

using namespace std;

vector<char> getArrayFromSetPoint(setPoint_t setpoint)
{
	vector<char> payload(12);

	*((float *)&payload[0]) = setpoint.coord.x;
	*((float *)&payload[4]) = setpoint.coord.y;
	*((float *)&payload[8]) = setpoint.rotation;

	return payload;
}

/*
* @brief gets a float from a char vector
* @param vec: sequence of bytes
* @return float
*/
float getFloat(vector<char> vec)
{
	float value = 0.0;

	// Extraído de: https://www.cplusplus.com/reference/cassert/assert/
	assert(vec.size() == sizeof(value));

	memcpy(&value, &vec[0], min(vec.size(), sizeof(float)));
	return value;
}

/*
 * @brief: calculates the coordinate in reference from other 2 and a proportional value
 * @param: originPos - origin position of object
 * @param: finalPos - final position of reference
 * @param: proportion - proportional position [0 = origin ~~ 1 = final]
 * @return: coordinate calculated
 */
Vector2 proportionalPosition(Vector2 originPos, Vector2 finalPos, float proportion)
{
	Vector2 destination;
	destination.x = (finalPos.x - originPos.x) * proportion + originPos.x;
	destination.y = (finalPos.y - originPos.y) * proportion + originPos.y;
	
	return destination;
}

/*
 * @brief: calculates the rotation between 2 coordinates
 * @param: originPos - origin position of object
 * @param: finalPos - final position of reference
 * @return: angle in eulerian degrees
 */
float calculateRotation(Vector2 originPos, Vector2 finalPos)
{
	float deltaX = finalPos.x - originPos.x;
	float deltaZ = finalPos.y - originPos.y;

	if (deltaX == 0 && deltaZ == 0)
	{
		cout << "Same Position delivered" << endl;
		return 180;
	}
	if (deltaZ == 0)
	{
		// cout << "Invalid Angle, aprox to -90°" << endl;
		return 270;
	}
	if (deltaX == 0)
	{
		// cout << "Invalid Angle, aprox to 180°" << endl;
		return 180;
	}

	float angle = 1 / (std::tan(deltaX / deltaZ)); // angulo en radianes
	angle = angle * (180 / PI);					   // conversion a grados sexagecimales
	return angle + 180;

	// creo que para los robots el angulo va a tener q ser el q recibe pero negativo
	// correccion: angulo - 90, no negativo xq es en referencia al eje z
	// ver bien despues in-game
}

/*
* @brief separates a string in a vector of strings divided by "/"
* @param topic: original string
* @param cadena: sequence of strings returned/appended
*/
void separateString(string topic, vector<string> &cadena)
{
	cadena.clear();

	size_t pos = 0;
	string space_delimiter = "/";
	
    while ((pos = topic.find(space_delimiter)) != string::npos) 
	{
        cadena.push_back(topic.substr(0, pos));
        topic.erase(0, pos + space_delimiter.length());
    }
	cadena.push_back(topic);

    // for (const auto &str : cadena) {
    //     cout << str << endl;
    // }
}

/*
 * Recives 2 coordinates and if the origin coord is close to destination returns true
 */
bool isCloseTo(Vector2 originCoord, Vector2 destinationCoord)
{
	float nearRange = 0.01;
	if ((originCoord.x) <= (destinationCoord.x + nearRange) &&
		(originCoord.x) >= (destinationCoord.x - nearRange) &&
		(originCoord.y) <= (destinationCoord.y + nearRange) &&
		(originCoord.y) >= (destinationCoord.y - nearRange))
		return true;
	else
		return false;
}