#include "GameModel.h"
#include "data.h"

/////////////////          DESPUES VA A HABER Q BORRAR ESTA FUNCION
/////////////////          NO LA BORREN HASTA NO TENER LISTO EL UPDATE DE MOVIMIENTO XQ ESTO ANDA
void GameModel::testMovement()
{
	setPoint_t destination = team[0]->goToBall(arcoOpposite, { ball[0], ball[2] }, 1.05f);
	// MQTTMessage setpointMessage = {"robot" + teamID + "." + team[0]->robotID + "/pid/setpoint/set",
	//         getArrayFromSetPoint(destination)};
	// mqttClient->publish(setpointMessage.topic, setpointMessage.payload);
	setSetpoint(destination, "1");

	cout << "ESTO DEBERIA MOVER A UN ROBOT" << endl;
	// cout << setpointMessage.topic << endl;   //esto es para ver si esta bien el topic
	cout << ball[0] << "," << ball[2] << endl; // esto es para ver bien las coordenadas de la bocha
	cout << "rotacion: " << destination.rotation << endl;
}
//////////////////

/**
 * @brief
 *
 */
GameModel::GameModel(MQTTClient2& mqttClient, string myTeam)
{
	this->mqttClient = &mqttClient;
	this->teamID = myTeam;
	this->oppTeamID = (myTeam == "1") ? "2" : "1";
	Vector2 arco1 = { 4.5f, 0.0f };
	Vector2 arco2 = { -4.5f, 0.0f };
	arcoTeam = (myTeam == "1") ? arco1 : arco2;
	arcoOpposite = (myTeam == "1") ? arco2 : arco1;
	for (int i = 0; i < 12; i++)
	{
		this->ball[i] = 0;
	}
	this->ball[0] = 1.5;

	Robot enemy1;
	oppTeam.push_back(&enemy1);
	Robot enemy2;
	oppTeam.push_back(&enemy2);
	Robot enemy3;
	oppTeam.push_back(&enemy3);
	Robot enemy4;
	oppTeam.push_back(&enemy4);
	Robot enemy5;
	oppTeam.push_back(&enemy5);
	Robot enemy6;
	oppTeam.push_back(&enemy6);
}

GameModel::~GameModel()
{
	team.clear();
	oppTeam.clear();
	//destroyHeatMap();
}

void GameModel::update()
{
	// for (auto player : team)
	// {
	//     player->update();
	// }
	shootToGoal(team[0]); // para esta entrega solo hace falta esta funcion
}

/**
 * @brief this function recives a message from MQTTclient and analizes the topic
 * @param topic: string declaration of messagge´s topic
 * @param payload: data of the topic
 *
 */
void GameModel::onMessage(string topic, vector<char> payload)
{
	if (!topic.compare("ball/motion/state")) // compare returns 0 if are equal
	{
		memcpy(ball, &payload[0], payload.size());
		// cout << "Ball Pos: " << ball[0] << "," << ball[1] << "," << ball[2] << endl;

		// updateDeltaTime();  //HAY Q HACER ESTOS UPDATES
		update();    //ORIENTARLO SEGUN EL GAMESTATE (PARA LA PROX ENTREGA)

		while (!messagesToSend.empty()) // sends all messages appended to message vector
		{
			MQTTMessage actualMessage = messagesToSend.back();
			mqttClient->publish(actualMessage.topic, actualMessage.payload);
			messagesToSend.pop_back();
		}
	}
	else
		assignMessagePayload(topic, payload); //queremos asignar los valores a los robots segun los topicos
}

/**
 * @brief analizes the topic and delivers the payload to the desired place ¿?

 * @param topic: string declaration of messagge´s topic
 * @param payload: data of the topic
 */
void GameModel::assignMessagePayload(string topic, vector<char>& payload)
{
	if (topic.compare(0, 6, "edacup") == 0) // game state messagge
	{
		cout << "EDACUP MESSAGGE" << endl;
		if (topic.compare(7, 10, "preKickOff") == 0)
			gameState = PRE_KICKOFF;
		else if (topic.compare(7, 7, "kickOff") == 0)
			gameState = KICKOFF;
		else if (topic.compare(7, 11, "preFreeKick") == 0)
			gameState = PRE_FREEKICK;
		else if (topic.compare(7, 8, "freeKick") == 0)
			gameState = FREEKICK;
		else if (topic.compare(7, 14, "prePenaltyKick") == 0)
			gameState = PRE_PENALTY;
		else if (topic.compare(7, 11, "penaltyKick") == 0)
			gameState = PENALTY;
		else if (topic.compare(7, 5, "pause") == 0)
			gameState = PAUSE;
		else if (topic.compare(7, 8, "continue") == 0)
			gameState = CONTINUE;
		else if (topic.compare(7, 11, "removeRobot") == 0)
			gameState = REMOVE_ROBOT;
		else if (topic.compare(7, 8, "addRobot") == 0)
			gameState = ADD_ROBOT;
		else
			cout << "FAILURE: INVALID GAMESTATE MESSAGE RECIVED" << endl;
	}
	else if (topic.compare(5, 1, teamID) == 0) // the robot belongs to the team
	{
		 if (topic.compare(9, 6, "motion") == 0)
		 {
		 	int robotIndex = topic[7] - '0';

		 	float payloadToFloat[12];
		 	memcpy(payloadToFloat, &payload[0], payload.size());

		 	team[robotIndex - 1]->setPosition({ payloadToFloat[0], payloadToFloat[1], payloadToFloat[2] });
		 	team[robotIndex - 1]->setSpeed({ payloadToFloat[3], payloadToFloat[4], payloadToFloat[5] });
		 	team[robotIndex - 1]->setRotation({ payloadToFloat[6], payloadToFloat[7], payloadToFloat[8] });
		 	team[robotIndex - 1]->setAngularSpeed({ payloadToFloat[9], payloadToFloat[10], payloadToFloat[11] });
		 }
		 else if (topic.compare(9, 11, "power/state") == 0)
		 {
			 int robotIndex = topic[7] - '0';

			 float payloadToFloat[3];
			 memcpy(payloadToFloat, &payload[0], std::min(payload.size(), sizeof(payloadToFloat)));

			 team[robotIndex - 1]->setPowerLevels({payloadToFloat[0], payloadToFloat[1], payloadToFloat[2]});
		}

	}
	else if (topic.compare(5, 1, oppTeamID) == 0) // the robot belongs to opposite team
	{
		if (topic.compare(9, 6, "motion") == 0)
		{
			int robotIndex = topic[7] - '0';

			float payloadToFloat[12];
			memcpy(payloadToFloat, &payload[0], payload.size());
			
            oppTeam[robotIndex - 1]->setPosition({ payloadToFloat[0], payloadToFloat[1], payloadToFloat[2] });
			oppTeam[robotIndex - 1]->setSpeed({ payloadToFloat[3], payloadToFloat[4], payloadToFloat[5] });
			oppTeam[robotIndex - 1]->setRotation({ payloadToFloat[6], payloadToFloat[7], payloadToFloat[8] });
			oppTeam[robotIndex - 1]->setAngularSpeed({ payloadToFloat[9], payloadToFloat[10], payloadToFloat[11] });
		}
	}
	else // error in the messagge recived or topic processing
	{
		cout << "FAILURE: INVALID MAIN TOPIC MESSAGE RECIVED" << endl;
	}
}

/**
 * @brief
 *
 */
void GameModel::start(void)
{
	cout << "GameModel START" << endl;
	cout << "team size: " << team.size() << endl;
	for (int playerNumber = 0; playerNumber < team.size(); playerNumber++)
	{
		team[playerNumber]->start(to_string(playerNumber + 1));
	}
	cout << "GameModel START ended" << endl;
	// startHeatMap();
}

/**
 * @brief anade un bot al equipo
 *
 * @param bot puntero al bot para anadir al vector
 */
void GameModel::addPlayer(Players* bot)
{
	team.push_back(bot);
}

/**
 * @brief se suscribe a todos los topicos de la simulacion
 */

void GameModel::suscribeToGameTopics()
{
	mqttClient->subscribe("ball/motion/state");
	mqttClient->subscribe("edacup/preKickOff");
	mqttClient->subscribe("edacup/kickOff");
	mqttClient->subscribe("edacup/preFreeKick");
	mqttClient->subscribe("edacup/freeKick");
	mqttClient->subscribe("edacup/prePenaltyKick");
	mqttClient->subscribe("edacup/penaltyKick");
	mqttClient->subscribe("edacup/pause");
	mqttClient->subscribe("edacup/continue");
	mqttClient->subscribe("edacup/removeRobot");
	mqttClient->subscribe("edacup/addRobot");
}

string GameModel::getTeamID()
{
	return teamID;
}

void GameModel::updateTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}

coord_t GameModel::getProxPosBall2D(Vector3 ballPosition, Vector3 ballVelocity)
{
	// calculo de tiro oblicuo, relacion en Y
	//  Y = Yo + Vy * time + 1/2 * Ay * time^2
	// hallar tiempo de caida para aproximar X,Z
	//  time = (-Vy +- sqrt(Vy*Vy - 4*Yo*1/2*Ay))/(2*1/2*Ay)
	// tomo el time positivo xq es el valido
	coord_t proxPos;
	float discriminante = (ballVelocity.y * ballVelocity.y) - (2 * ballPosition.y * EARTH__GRAVITY);
	if (discriminante >= 0)
	{
		discriminante = sqrt(discriminante);
		float fallingTime = (-ballVelocity.y + discriminante) / (EARTH__GRAVITY);
		if (fallingTime < 0) // negative time
			fallingTime = (-ballVelocity.y - discriminante) / (EARTH__GRAVITY);

		proxPos.x = ballPosition.x + (ballVelocity.x * fallingTime);
		proxPos.z = ballPosition.z + (ballVelocity.z * fallingTime);
	}
	else // raiz imaginaria
	{
		cout << "Invalid ball position algebra..." << endl;
		proxPos.x = ballPosition.x + (ballVelocity.x * 0.1); // forced aproximation
		proxPos.z = ballPosition.z + (ballVelocity.z * 0.1);
	}
	return proxPos;
}

void GameModel::updatePositions(void)
{
	for (auto teamRobot : team)
	{
		setPoint_t destination = teamRobot->goToBall(arcoOpposite, { ball[0], ball[2] }, 1.05f);
		MQTTMessage setpointMessage =
		{ "robot" + teamRobot->teamID + "." + teamRobot->robotID + "/pid/setpoint/set",
		 getArrayFromSetPoint(destination) };
		messagesToSend.push_back(setpointMessage);
	}
}

// /**
//  * @brief empieza el heatmaps con 0s
//  *
//  */
// void GameModel::startHeatMap()
// {
// 	for (int contx = 0; contx < 600; contx++)
// 		for (int contz = 0; contz < 450; contz++)
// 	{
// 		heatMap[contx][contz].type = 0;
// 		heatMap[contx][contz].value = 0;
// 	}
// }

// void GameModel::updateHeatMap()
// {
// 	int coordX;
// 	int coordZ;

// 	startHeatMap();

// 	for (auto i : team)
// 	{
// 		coordX = (int) (1000/15 * (4.5F + i->getPosition().x));
// 		coordZ = (int) (1000/15 * (3.0F + i->getPosition().z));

// 		heatMap[AT(coordX, coordZ)].type = 1;
// 	}

// 	coordX = (int)100 * (4.5F + ball[0]);
// 	coordZ = (int)100 * (3.0F + ball[2]);

// 	for (int contx = 0; contx < 5; contx++)
// 		for (int conty = 0; conty < 5; conty++)
// 			heatMap[AT(coordX, coordZ)].type = 2;
// }

void GameModel::setDisplay(string path, string robotID)
{
	Image displayImage = LoadImage(path.c_str());
	ImageFormat(&displayImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8); // a Robot10.png no le cambia el formato

	const int dataSize = 16 * 16 * 3;
	vector<char> payload(dataSize);
	memcpy(payload.data(), displayImage.data, dataSize);

	MQTTMessage setDisplayMessage = { robotID + "/display/lcd/set", payload };
	messagesToSend.push_back(setDisplayMessage);

	UnloadImage(displayImage);
}

void GameModel::voltageKickerChipper(string robotID)
{
	float voltage = 200.0f;
	vector<char> payload = getDataFromFloat(voltage);
	MQTTMessage setKicker = { "robot" + teamID + "." + robotID + "/kicker/chargeVoltage/set", payload };
	messagesToSend.push_back(setKicker);
}

void GameModel::setDribbler(string robotID)
{
	float voltage = 100.0f;
	vector<char> payload = getDataFromFloat(voltage);
	MQTTMessage setKicker = {  "robot" + teamID + "." + robotID + "/dribbler/voltage/set", payload };
	messagesToSend.push_back(setKicker);
}

void GameModel::setKicker(string robotID)
{
	float potencia = 0.8;
	vector<char> payload = getDataFromFloat(potencia);
	MQTTMessage setKicker = { "robot" + teamID + "." + robotID + "/kicker/kick/cmd", payload };
	messagesToSend.push_back(setKicker);
}

void GameModel::setChipper(string robotID)
{
	float potencia = 0.8;
	vector<char> payload = getDataFromFloat(potencia);
	MQTTMessage setKicker = { "robot" + teamID + "." + robotID + "/kicker/chip/cmd", payload };
	messagesToSend.push_back(setKicker);
}

void GameModel::shootToGoal(Players* player)
{
	setPoint_t placeInCourt = player->kickBallLogic(arcoOpposite, { ball[0], ball[2] });
	setPoint_t kickValue = { 100, 100, 100 };
    
	if ((placeInCourt.coord.x == kickValue.coord.x) &&
		(placeInCourt.coord.y == kickValue.coord.y) &&
		(placeInCourt.rotation == kickValue.rotation)) // comparacion de igualdad de setpoints
	{
		//setChipper(player->robotID);
		if(player->getKickerCharge() >= 160)
			setKicker(player->robotID);	
		else
			voltageKickerChipper(player->robotID); // este orden por el pop_back del vector	
	}
	else // mover hasta el setpoint indicado
		setSetpoint(placeInCourt, player->robotID);
}

bool GameModel::isBallStill(void)
{
	if ((ball[3] == 0) && (ball[4] == 0) && (ball[5] == 0))
		return true;
	else
		return false;
}

/**
 * @brief agrega un setpoint de RobotID a messagesToSend
 *
 * @param setpoint
 * @param robotID
 */
void GameModel::setSetpoint(setPoint_t setpoint, string robotID)
{
	MQTTMessage setpointMessage = { "robot" + teamID + "." + robotID + "/pid/setpoint/set",
								   getArrayFromSetPoint(setpoint) };
	messagesToSend.push_back(setpointMessage);
}