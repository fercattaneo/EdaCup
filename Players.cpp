#include "Players.h"

using namespace std;

Players::Players()
{
	
}

Players::~Players()
{
	// UnloadImage(displayImage);
}

void Players::start(string playerNumber)
{
	// depende de la entrega
	robotID = playerNumber.c_str();
}

setPoint_t Players::goToBall (Vector2 oppositeGoal, Vector2 ballPosition, float proportional)
{
	setPoint_t destination;
	destination.coord = proportionalPosition(oppositeGoal,ballPosition, proportional);
	destination.rotation = calculateRotation(oppositeGoal,ballPosition);
	//falta calculo de rotacion
	return destination;
}

setPoint_t Players::kickBallLogic (Vector2 oppositeGoal, Vector2 ballPosition)
{
	Vector2 nearBall = proportionalPosition(oppositeGoal,ballPosition, 1.05);
	setPoint_t result;
	if(isCloseTo({this->position.x, this->position.z},nearBall))
	{
		//ver si estoy en posicion
		if(isCloseTo({this->position.x, this->position.z},ballPosition))
		{
			result = {100,100,100};  //tiene q devolver un setpoint pero esto seria
									 // el comando para q al verlo patee
		}
		else
			result = goToBall (oppositeGoal, ballPosition, 1.0f);
	}
	else
		result = goToBall (oppositeGoal, ballPosition, 1.05f);

	return result;
}

void Players::toEnablePlayer(void)
{
	this->enablePlayer = true;
}

void Players::dissablePlayer(void)
{
	this->enablePlayer = false;
}