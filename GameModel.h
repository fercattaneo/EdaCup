#ifndef _GAMEMODEL_H
#define _GAMEMODEL_H

#include "MQTTClient2.h"
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstring>
#include <deque>
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
    unsigned char value : 6; // ya vemos
    unsigned char type : 2;  // 00 = nada, 01 = bot aliado, 10 = bola, 11 = enemigo
} tile_t;

class GameModel : public MQTTListener
{
public:
    GameModel(MQTTClient2 &mqttClient, string myTeam);
    ~GameModel();

    void onMessage(string topic, vector<char> payload);
    void update();

    void start(void);
    void suscribeToGameTopics();
    void setDisplay(string path, string robotID);
    void addPlayer(Players *bot);
    void removePlayer(Players *bot);
    void voltageKickerChipper(string robotID);
    void setDribbler(string robotID);
    void shootToGoal(Players *player);
    void setChipper(string robotID);
    void setKicker(string robotID);
    void setSetpoint(setPoint_t setpoint, string robotID);

    coord_t getProxPosBall2D(Vector3 ballPosition, Vector3 ballVelocity);
    bool isBallStill(void);

    void testMovement(); // DESPUES HAY Q SACARLA
    string getTeamID();

    GameState gameState;
    void updateTime(float deltaTime);
    void updatePositions();
    void assignMessagePayload(string topic, vector<char> &payload);

    vector<MQTTMessage> messagesToSend;

private:
    string teamID;
    string oppTeamID;

    MQTTClient2 *mqttClient;
    float deltaTime;

    vector<Players *> team;
    vector<Robot *> oppTeam;
    float ball[12];

    array<array<tile_t,450>,600> heatMap;

    Vector2 arcoTeam;
    Vector2 arcoOpposite;

    void startHeatMap();
    void updateHeatMap();
/*
             UNIDAD METROS
       _________________________
      |                         |
      |                         |
      |                         | -3.0 < z < 3.0
      |                         |
      |                         |
      |_________________________|
            -4.5 < x < 4.5

    particion tesela de 0,015, obtenemos 450x600 teselas (270000 bytes = 0.27 mb)

    posibilidades:
    array<array<tile_t,450>,600> heatMap;   <--- con arrays
    
    tile_t** heatMap = new [sizeof(tile_t) * 600];  <--- con matriz con new
    for(int i = 0 ; i < 600 ; i++){
        heatMap[i] = new [450];;
    }
    */    
};

#endif //_GAMEMODEL_H