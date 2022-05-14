#include "GameModel.h"
#include <iostream>
#include <cmath>
#include <cstring>

GameModel::GameModel(MQTTClient2 &mqttClient, char myTeam)
{
    this->mqttClient = &mqttClient;
    this->teamID = to_string(myTeam);
    coord_t arco1 = {(CENTER_OF_COURT_X) + 0.45, (CENTER_OF_COURT_Z)}; //verificar coordenadas
    coord_t arco2 = {(CENTER_OF_COURT_X) - 0.45, (CENTER_OF_COURT_Z)};
    this->arcoTeam = (myTeam == '1') ? arco1 : arco2;
    this->arcoOpposite = (myTeam == '1') ? arco2 : arco1;
};

GameModel::~GameModel(){
    // borrar a todos los players xDDDD

};

void GameModel::onMessage(string topic, vector<char> payload)
{
    cout << topic << endl;
}

void GameModel::start(string teamID)
{
    for (int playerNumber = 1; playerNumber <= team.size(); playerNumber++)
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
