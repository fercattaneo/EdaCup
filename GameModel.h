#ifndef _GAMEMODEL_H
#define _GAMEMODEL_H

#include "MQTTClient2.h"
#include <vector>
#include <array>
// #include "Robot.h"
#include "Players.h"

using namespace std;

#define LENGTH_OF_COURT_X 9f // meters
#define LENGTH_OF_COURT_Z 6f

#define CENTER_OF_COURT_X 1.0f // physical coordinates
#define CENTER_OF_COURT_Z 0.85f

#define EARTH__GRAVITY 9.80665f

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
    unsigned char value : 6;  //ya vemos
    unsigned char type : 2;  //00 = nada, 01 = bot aliado, 10 = bola, 11 = enemigo
}tile_t;

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

    coord_t getProxPosBall2D (Vector3 ballPosition, Vector3 ballVelocity);

    string getTeamID();

    GameState gameState;
    void updateTime(float deltaTime);

    vector<MQTTMessage> messagesReceived;
    vector<MQTTMessage> messagesToSend;

private:
    string teamID;

    MQTTClient2 *mqttClient;
    float deltaTime;

    vector<Players *> team;
    float ball[12];

    array<tile_t, 540000> heatMap;

    Vector2 arcoTeam;
    Vector2 arcoOpposite;

    void updatePositions();
    
    void startHeatMap();
    void updateHeatMap();
};

#endif //_GAMEMODEL_H