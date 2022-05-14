#ifndef DATA_H
#define DATA_H
#include <vector>
#include "robot.h"
#include <assert.h>

template <typename T>
std::vector<char> getData(T val)
{
    std::vector<char> data(sizeof(val));

	// Extraído de: https://www.cplusplus.com/reference/cstring/memcpy/
	memcpy(data.data(), &payload, sizeof(T));

	return data;
}
std::vector<char> getArrayFromSetPoint(setPoint_t setpoint);
float getFloat(std::vector<char> vec);
Vector2 proportionalPosition(Vector2 originPos, Vector2 finalPos, float proportion);
float calculateRotation(Vector2 originPos, Vector2 finalPos);

#endif

