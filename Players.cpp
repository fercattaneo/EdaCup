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

// Función para hacer funcionar el Kricker y el Chipper.
// Estos se activan con la tecla ENTER
// void Players::setKickerChipper()
// {
// 	class MQTTMessage msj1, msj2, msj3;
// 	if (IsKeyDown(KEY_ENTER))
// 	{
// 		msj1.topic = "robot1/kicker/chargeVoltage/set";
// 		msj1.payload = getArrayFromFloat(KICKER_CHARGEVOLTAGE);
// 		publish(msj1.topic, msj1.payload);
// 		msj2.topic = "robot1/kicker/kick/cmd";
// 		msj2.payload = getArrayFromFloat(KICKER_KICK);
// 		publish(msj2.topic, msj2.payload);
// 		msj3.topic = "robot1/kicker/chip/cmd";
// 		msj3.payload = getArrayFromFloat(KICKER_CHIP);
// 		publish(msj3.topic, msj3.payload);
// 	}
// }

setPoint_t Players::goToBall (Vector2 oppositeGoal, Vector2 ballPosition){
	setPoint_t destination;
	destination.coord = proportionalPosition(oppositeGoal,ballPosition,1.1f);
	destination.rotation = calculateRotation(oppositeGoal,ballPosition);
	//falta calculo de rotacion
	return destination;
}

// Función para encender el Dribbler. Se enciende con la tecla
// Blockspace y se apaga con la tecla M
/*void Players::setDribbler()
{
	class MQTTMessage msj1;
	if (IsKeyDown(KEY_BACKSPACE))
	{
		msj1.topic = "robot1/dribbler/current/set";
		msj1.payload = getArrayFromFloat(DRIBBLER_CURRECT);
		publish(msj1.topic, msj1.payload);
	}
	if (IsKeyDown(KEY_M))
	{
		msj1.topic = "robot1/dribbler/voltage/set";
		msj1.payload = getArrayFromFloat(0.0F);
		publish(msj1.topic, msj1.payload);
	}
}*/

void Players::toEnablePlayer(void)
{
	this->enablePlayer = true;
}

void Players::dissablePlayer(void)
{
	this->enablePlayer = false;
}