#include "GameModel.h"
#include <iostream>
#include <cmath>

GameModel::GameModel(MQTTClient2 &mqttClient)
{
    this->mqttClient = &mqttClient;
}

GameModel::~GameModel()
{
    //borrar a todos los players xDDDD

};

void GameModel::start(string teamID)
{
    for(int playerNumber = 1 ; playerNumber <= team.size() ; playerNumber++)
    {
        team[playerNumber]->start(mqttClient, to_string(playerNumber));
    }

    this->teamID = teamID;
    
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
    //mqttClient->subscribe("ball/motion/state"); <-- la recibe listener
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

