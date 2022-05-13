#include "MQTTClient2.h"
#include "Players.h"
#include "GameModel.h"
#include "MQTTListenChild.h"
#include "Robot.h"

include "Players.h"#include <iostream>

using namespace std;

void subscribeGameTopics (MQTTClient2 &client);
void subscribeRobotTopics (MQTTClient2 &client, char team);

int main (int argc, char *argv[])
{
    string IMAGES_PATH = "../Resources/";
    if(argc < 2)
    {
        cout << "ERROR: invalid paramenters -> [program] [team number]" <<endl;
        return 1;
    }
    else if(*argv[1]!='1' || *argv[1]!='2')
    {
        cout << "ERROR: invalid team number, must be 1 or 2" <<endl;
        return 1;
    }

    MQTTClient2 client;
    if (!client.connect("controller", "localhost", 1883, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Could not connect." << endl;
        return 1;
    }

    char myTeam = *argv[1] - '0';
    GameModel gameModel(client);

    Players player1(IMAGES_PATH + "bola1_16x16.png");
    gameModel.addPlayer(&player1);

    gameModel.suscribeToGameTopics();
    subscribeRobotTopics(client, myTeam);
    
    MQTTListenChild listener;
    client.setListener(&listener);
    client.run();
    cout << "Game MQTT subscription ended..." << endl;
    cout << "Game STARTING..." << endl;

    bool enableLoop = true;
    while (enableLoop)      //game transition
    {
        float deltaTime = GetFrameTime();
        gameModel.updateTime(deltaTime);

        switch (gameModel.gameState)
        {
            case START:
                
                break;

            case PRE_KICKOFF:
                
                break;
                
            case KICKOFF:
                
                break;

            case IN_GAME:
                
                break;

            case PRE_FREEKICK:
                
                break;

            case FREEKICK:
                
                break;

            case PRE_PENALTY:
                
                break;

            case PENALTY:
                
                break;

            case PAUSE:
                
                break;

            case CONTINUE:
                
                break;

            case REMOVE_ROBOT:
                
                break;

            case ADD_ROBOT:
                
                break;

            case GOAL:
                
                break;

            case ENDED_GAME:
                    enableLoop = false;
                break;

            default:
                break;
        }
    }
    
    cout << "Game ended successfully." << endl;
    return 0;
}

void subscribeRobotTopics (MQTTClient2 &client, char team)
{
    string myTeam, oppositeTeam;
    if(team == '1')
    {
        myTeam = "1";
        oppositeTeam = "2";
    }
    else //team == 2
    {
        myTeam = "2";
        oppositeTeam = "1";
    }

    for(int i = 1; i < 7 ; i++)        //team robots loop
    {
        string robotID = to_string(i);
        client.subscribe("robot" + myTeam + "." + robotID + "/motion/state");
        client.subscribe("robot" + myTeam + "." + robotID + "/power/powerConsumption");
        client.subscribe("robot" + myTeam + "." + robotID + "/power/batteryLevel");
        client.subscribe("robot" + myTeam + "." + robotID + "/pid/setpoint/set");
        client.subscribe("robot" + myTeam + "." + robotID + "/pid/parameters/set");

        for(int j = 1; j < 5; j++)    //motors loop
        {
            string motorNMBR = to_string(i);
            client.subscribe("robot" + myTeam + "." + robotID + "/motor" + motorNMBR + "/voltage");
            client.subscribe("robot" + myTeam + "." + robotID + "/motor" + motorNMBR + "/rpm");
            client.subscribe("robot" + myTeam + "." + robotID + "/motor" + motorNMBR + "/temperature");
            client.subscribe("robot" + myTeam + "." + robotID + "/motor" + motorNMBR + "/voltage/set");
            client.subscribe("robot" + myTeam + "." + robotID + "/motor" + motorNMBR + "/current/set");
        }

        client.subscribe("robot" + myTeam + "." + robotID + "/dribbler/voltage");
        client.subscribe("robot" + myTeam + "." + robotID + "/dribbler/rpm");
        client.subscribe("robot" + myTeam + "." + robotID + "/dribbler/temperature");
        client.subscribe("robot" + myTeam + "." + robotID + "/dribbler/voltage/set");
        client.subscribe("robot" + myTeam + "." + robotID + "/dribbler/current/set");
        
        client.subscribe("robot" + myTeam + "." + robotID + "/kicker/voltage");
        client.subscribe("robot" + myTeam + "." + robotID + "/kicker/chargeVoltage/set");
        client.subscribe("robot" + myTeam + "." + robotID + "/kicker/kick/cmd");
        client.subscribe("robot" + myTeam + "." + robotID + "/kicker/chip/cmd");
        
        client.subscribe("robot" + myTeam + "." + robotID + "/display/lcd/set");
        client.subscribe("robot" + myTeam + "." + robotID + "/display/leftEye/set");
        client.subscribe("robot" + myTeam + "." + robotID + "/display/rightEye/cmd");
    }

    for(int i = 1; i < 7; i++)   //opponent´s robots
    {
        string robotID = to_string(i);
        client.subscribe("robot" + oppositeTeam + "." + robotID + "/motion/state");
    }
}