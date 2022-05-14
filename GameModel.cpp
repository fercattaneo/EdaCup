#include "GameModel.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include "data.h"

/////////////////          DESPUES VA A HABER Q BORRAR ESTA FUNCION
void GameModel::testMovement()
{
    setPoint_t destination = team[0]->goToBall(arcoOpposite,{ball[0],ball[2]});
    MQTTMessage setpointMessage = 
        {"robot" + team[0]->teamID + "." + team[0]->robotID + "/pid/setpoint/set",
            getArrayFromSetPoint(destination)};
    mqttClient->publish(setpointMessage.topic, setpointMessage.payload);
    cout << "ESTO DEBERIA MOVER A UN ROBOT" << endl;
}
//////////////////

/**
* @brief
*
*/
GameModel::GameModel(MQTTClient2 &mqttClient, char myTeam)
{
    this->mqttClient = &mqttClient;
    this->teamID = to_string(myTeam);
    Vector2 arco1 = {0.45, 0.0}; //verificar coordenadas
    Vector2 arco2 = {-0.45, 0.0};
    this->arcoTeam = (myTeam == '1') ? arco1 : arco2;
    this->arcoOpposite = (myTeam == '1') ? arco2 : arco1;
};

GameModel::~GameModel(){
    // borrar a todos los players xDDDD

};

/**
* @brief
*
*/
void GameModel::onMessage(string topic, vector<char> payload)
{
    // cout << topic << endl;
    // if (!topic.compare("ball/motion/state"))
    // {
    //     float datos[12]; 
    //     memcpy(datos, &payload[0], std::min(payload.size(),sizeof(datos)));
    //     setPoint_t destination = { {datos[0], datos[2]}, 0 };
    //     mqttClient->publish("robot1.1/pid/setpoint/set", team[0]->getArrayFromSetPoint(destination));
    // }

    ////////////////////
    //guardar los msj recibidos
    MQTTMessage incomingMessage = { topic, payload }; // #
    messagesReceived.push_back(incomingMessage); //#######
    if(!topic.compare("ball/motion/state")) 
    {
        //procesamiento ("analizamos topic", luego transformamos a datos utiles, calculos, mandarlo a messagesToSend)
        while (!messagesToSend.empty())   //sends all messages appended to message vector
        {
            MQTTMessage actualMessage = messagesToSend.back();
            mqttClient->publish(actualMessage.topic, actualMessage.payload);
            messagesToSend.pop_back();
        }
    }
    //LLAMO A FUNCION DE CLASIFICAR TOPICS
}

/**
* @brief
*
*/
void GameModel::start(string teamID)
{
    for (int playerNumber = 1; playerNumber <= team.size(); playerNumber++)
    {
        team[playerNumber]->start(to_string(playerNumber));
    }

    this->teamID = teamID;
    startHeatMap();
}

/**
 * @brief anade un bot al equipo
 *
 * @param bot puntero al bot para anadir al vector
 */
void GameModel::addPlayer(Players *bot)
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

coord_t GameModel::getProxPosBall2D (Vector3 ballPosition, Vector3 ballVelocity)
{
    //calculo de tiro oblicuo, relacion en Y
    // Y = Yo + Vy * time + 1/2 * Ay * time^2
    //hallar tiempo de caida para aproximar X,Z
    // time = (-Vy +- sqrt(Vy*Vy - 4*Yo*1/2*Ay))/(2*1/2*Ay)
    //tomo el time positivo xq es el valido
    coord_t proxPos;
    float discriminante = (ballVelocity.y *ballVelocity.y) - (2 * ballPosition.y * EARTH__GRAVITY);
    if(discriminante >= 0)
    {
        discriminante = sqrt(discriminante);
        float fallingTime = (-ballVelocity.y + discriminante)/(EARTH__GRAVITY);
        if(fallingTime < 0)   //negative time
            fallingTime = (-ballVelocity.y - discriminante)/(EARTH__GRAVITY);

        proxPos.x = ballPosition.x + (ballVelocity.x * fallingTime);
        proxPos.z = ballPosition.z + (ballVelocity.z * fallingTime);
    }
    else //raiz imaginaria
    {
        cout << "Invalid ball position algebra..." << endl;
        proxPos.x = ballPosition.x + (ballVelocity.x * 0.1); //forced aproximation
        proxPos.z = ballPosition.z + (ballVelocity.z * 0.1);
    }
    return proxPos;
}

void GameModel::updatePositions(void)
{
    for(auto teamRobot : team)
    {
        setPoint_t destination = teamRobot->goToBall(arcoOpposite,{ball[0],ball[2]});
        MQTTMessage setpointMessage = 
            {"robot" + teamRobot->teamID + "." + teamRobot->robotID + "/pid/setpoint/set",
                getArrayFromSetPoint(destination)};
        messagesToSend.push_back(setpointMessage);
    }
}

/**
 * @brief empieza el heatmaps con 0s
 * 
 */
void GameModel::startHeatMap()
{
    for(long int cont = 0; cont < 540000 ; cont++)
    {
        heatMap[cont].type = 0;
        heatMap[cont].value = 0;
    }
}

void GameModel::updateHeatMap()
{
       
}

void GameModel::setDisplay(string path, string robotID)
{
    Image displayImage = LoadImage(path.c_str());

	const int dataSize = 16 * 16 * 3;
	vector<char> payload(dataSize);
	memcpy(payload.data(), displayImage.data, dataSize);

	// mqttClient->publish(robotID + "/display/lcd/set", payload);
	MQTTMessage setDisplayMessage = {robotID + "/display/lcd/set", payload};
	messagesToSend.push_back(setDisplayMessage);

    UnloadImage(displayImage);
}