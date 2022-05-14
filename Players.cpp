#include "Players.h"

using namespace std;

Players::Players(string path)
{
	displayImage = LoadImage(path.c_str());
}

Players::~Players()
{
	UnloadImage(displayImage);
}

void Players::start(MQTTClient2 *mqttClient, string playerNumber)
{
	// depende de la entrega
	this->mqttClient = mqttClient;
	this->robotID = "robot" + teamID + "." + playerNumber.c_str();
}

void Players::setDisplay(string path)
{
	Rectangle selectRectangle = {0, 0, 16, 16};
	Image selectedImage = ImageFromImage(displayImage, selectRectangle);

	const int dataSize = 16 * 16 * 3;
	vector<char> payload(dataSize);
	memcpy(payload.data(), selectedImage.data, dataSize);

	UnloadImage(selectedImage);

	mqttClient->publish(robotID + "/display/lcd/set", payload);
}

// Funcion para trasnformar un float a vector de char.
std::vector<char> Players::getArrayFromFloat(float payload)
{
	std::vector<char> data(sizeof(float));

	// Extraído de: https://www.cplusplus.com/reference/cstring/memcpy/
	memcpy(data.data(), &payload, sizeof(float));

	return data;
}

std::vector<char> Players::getArrayFromSetPoint(setPoint_t payload)
{
	std::vector<char> data(sizeof(setPoint_t));

	// Extraído de: https://www.cplusplus.com/reference/cstring/memcpy/
	memcpy(data.data(), &payload, sizeof(setPoint_t));

	return data;
}


//Funcion para acercarse a la pelota 
void Players::goToBall (coord_t pointF){
	setPoint_t destination ={(pointF), 0};
	//mandar mensaje
	this->mqttClient->publish("robot" + this->teamID + "." + this->robotID + "/pid/setpoint/set",getArrayFromSetPoint(destination));
}

// Función para hacer funcionar el Kricker y el Chipper.
// Estos se activan con la tecla ENTER
/*void Players::setKickerChipper()
{
	class MQTTMessage msj1, msj2, msj3;
	if (IsKeyDown(KEY_ENTER))
	{
		msj1.topic = "robot1/kicker/chargeVoltage/set";
		msj1.payload = getArrayFromFloat(KICKER_CHARGEVOLTAGE);
		publish(msj1.topic, msj1.payload);
		msj2.topic = "robot1/kicker/kick/cmd";
		msj2.payload = getArrayFromFloat(KICKER_KICK);
		publish(msj2.topic, msj2.payload);
		msj3.topic = "robot1/kicker/chip/cmd";
		msj3.payload = getArrayFromFloat(KICKER_CHIP);
		publish(msj3.topic, msj3.payload);
	}
}*/

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

/*
 *	@brief: calculates the coordinate in reference from other 2 and a proportional value
 *   @param: originPos - origin position of object
 *   @param: finalPos - final position of reference
 *   @param: proportion - proportional position [0 = origin ~~ 1 = final]
 *   @return: coordinate calculated
 */
coord_t Players::proportionalPosition(coord_t originPos, coord_t finalPos, float proportion)
{
	coord_t destination;
	destination.x = (finalPos.x - originPos.x) * proportion + originPos.x;
	destination.z = (finalPos.z - originPos.z) * proportion + originPos.z;
	return destination;
}

/*
 *	@brief: calculates the rotation between 2 coordinates
 *   @param: originPos - origin position of object
 *   @param: finalPos - final position of reference
 *   @return: angle in eulerian degrees
 */
float Players::calculateRotation(coord_t originPos, coord_t finalPos)
{
	float deltaX = finalPos.x - originPos.x;
	float deltaZ = finalPos.z - originPos.z;

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

// Funcion para convertir un vector de char a un float
float Players::getFloat(std::vector<char> vec)
{
	float value = 0.0;

	// Extraído de: https://www.cplusplus.com/reference/cassert/assert/
	assert(vec.size() == sizeof(value));

	memcpy(&value, &vec[0], std::min(vec.size(), sizeof(float)));
	return value;
}

void Players::toEnablePlayer(void)
{
	this->enablePlayer = true;
}

void Players::dissablePlayer(void)
{
	this->enablePlayer = false;
}