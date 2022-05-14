#ifndef _GAMEMODEL_H
#define _GAMEMODEL_H

#include "MQTTClient2.h"
#include <vector>
// #include "Robot.h"
#include "Players.h"

using namespace std;

#define LENGTH_OF_COURT_X 9f // meters
#define LENGTH_OF_COURT_Z 6f

#define CENTER_OF_COURT_X 1.0f // physical coordinates
#define CENTER_OF_COURT_Z 0.85f

enum GameState
{
    START,
    PRE_KICKOFF,
    KICKOFF,
    IN_GAME,
    PRE_FREEKICK,
    FREEKICK,
    PRE_PENALTY,
    PENALTY,
    PAUSE,
    CONTINUE,
    REMOVE_ROBOT,
    ADD_ROBOT,
    ENDED_GAME,
    GOAL
};

typedef struct
{
    string topic;
    vector<float> payload;
} Message;

class GameModel : public MQTTListener
{
public:
    GameModel(MQTTClient2 &mqttClient, char myTeam);
    ~GameModel();

    void onMessage(string topic, vector<char> payload);

    void start(string teamID);
    void suscribeToGameTopics();
    void addPlayer(Players *bot);
    void removePlayer(Players *bot);

    // Funcion para enviar los mensajes
    void sendMessage(vector<Message> Menssages);

    string getTeamID();

    GameState gameState;
    void updateTime(float deltaTime);

private:
    MQTTClient2 *mqttClient;
    vector<Players *> team;

    string teamID;
    float deltaTime;

    coord_t arcoTeam;
    coord_t arcoOpposite;
};

#endif //_GAMEMODEL_H