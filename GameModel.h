#ifndef _GAMEMODEL_H
#define _GAMEMODEL_H

#include "MQTTClient2.h"
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstring>
#include "Players.h"
#include "data.h"

using namespace std;

#define LENGTH_OF_COURT_X 9f // meters
#define LENGTH_OF_COURT_Z 6f

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
    GameModel(MQTTClient2 &mqttClient, string myTeam);
    ~GameModel();

    void onMessage(string topic, vector<char> payload);

    void start(void);
    void suscribeToGameTopics();
    void setDisplay(string path, string robotID);
    void addPlayer(Players *bot);
    void removePlayer(Players *bot);
    void voltageKickerChipper(string robotID);
    void setDribbler(string robotID);
    void shootToGoal(Players* player);
    void setChipper(string robotID);
    void setKicker(string robotID);
    void setSetpoint(setPoint_t setpoint, string robotID);

    coord_t getProxPosBall2D (Vector3 ballPosition, Vector3 ballVelocity);
    bool isBallStill(void);
    
    void testMovement(); //DESPUES HAY Q SACARLA
    string getTeamID();

    GameState gameState;
    void updateTime(float deltaTime);
    void updatePositions();

    vector<MQTTMessage> messagesToSend;

    
private:
    string teamID;
    string oppTeamID;

    MQTTClient2 *mqttClient;
    float deltaTime;

    vector<Players *> team;
    vector<Robot *> oppTeam;
    float ball[12];

    array<tile_t, 540000> heatMap;

    Vector2 arcoTeam;
    Vector2 arcoOpposite;

    void assignMessagePayload(string topic, vector<char> payload);

    void startHeatMap();
    void updateHeatMap();
};

#endif //_GAMEMODEL_H