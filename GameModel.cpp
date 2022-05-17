#include "GameModel.h"

#define AT(x,z)  ((z)*900 + (x))

/////////////////          DESPUES VA A HABER Q BORRAR ESTA FUNCION
/////////////////          NO LA BORREN HASTA NO TENER LISTO EL UPDATE DE MOVIMIENTO XQ ESTO ANDA
void GameModel::testMovement()
{
    setPoint_t destination = team[0]->goToBall(arcoOpposite,{ball[0],ball[2]},1.05f);
    // MQTTMessage setpointMessage = {"robot" + teamID + "." + team[0]->robotID + "/pid/setpoint/set",
    //         getArrayFromSetPoint(destination)};
    // mqttClient->publish(setpointMessage.topic, setpointMessage.payload);
    setSetpoint(destination, "1");
    
    cout << "ESTO DEBERIA MOVER A UN ROBOT" << endl;
    // cout << setpointMessage.topic << endl;   //esto es para ver si esta bien el topic
    cout << ball[0] << "," << ball[2]<< endl;  //esto es para ver bien las coordenadas de la bocha
    cout << "rotacion: " << destination.rotation << endl;
}
//////////////////

/**
* @brief
*
*/
GameModel::GameModel(MQTTClient2 &mqttClient, string myTeam)
{
    this->mqttClient = &mqttClient;
    this->teamID = myTeam;
    this->oppTeamID = (myTeam=="1") ? "2" : "1";
    Vector2 arco1 = {4.5f,0.0f};
    Vector2 arco2 = {-4.5f,0.0f};
    arcoTeam = (myTeam == "1") ? arco1 : arco2;
    arcoOpposite = (myTeam == "1") ? arco2 : arco1;
    for(int i = 0; i<12; i++)
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
}

/**
* @brief
*
*/
void GameModel::onMessage(string topic, vector<char> payload)
{  
    if(!topic.compare("ball/motion/state"))  //compare returns 0 if are equal
    {
        memcpy(&ball,&payload,(sizeof(ball)/sizeof(ball[0])));
        //updateDeltaTime();  //HAY Q HACER ESTOS UPDATES
        //updateGameModel();  //ORIENTARLO SEGUN EL GAMESTATE (PARA LA PROX ENTREGA)
        while (!messagesToSend.empty())   //sends all messages appended to message vector
        {
            MQTTMessage actualMessage = messagesToSend.back();
            mqttClient->publish(actualMessage.topic, actualMessage.payload);
            messagesToSend.pop_back();
        }
    }
    else
    {
        assignMessagePayload(topic, payload);
    }
}

void GameModel::assignMessagePayload(string topic, vector<char> payload)
{
    deque<string> segmentedTopic;
    separateString(topic, segmentedTopic);

    if(segmentedTopic[0] == "edacup")  //game state messagge
    {
        if(segmentedTopic[1] == "preKickOff")
            gameState = PRE_KICKOFF;
        else if(segmentedTopic[1] == "kickOff")
            gameState = KICKOFF;
        else if(segmentedTopic[1] == "preFreeKick")
            gameState = PRE_FREEKICK;
        else if(segmentedTopic[1] == "freereeKick")
            gameState = FREEKICK;
        else if(segmentedTopic[1] == "prePenaltyKick")
            gameState = PRE_PENALTY;
        else if(segmentedTopic[1] == "penaltyKick")
            gameState = PENALTY;
        else if(segmentedTopic[1] == "pause")
            gameState = PAUSE;
        else if(segmentedTopic[1] == "continue")
            gameState = CONTINUE;
        else if(segmentedTopic[1] == "removeRobot")
            gameState = REMOVE_ROBOT;
        else if(segmentedTopic[1] == "addRobot")
            gameState = ADD_ROBOT;
        else
            cout << "FAILURE: INVALID GAMESTATE MESSAGE RECIVED" << endl;
    }
    else if(segmentedTopic[0].compare(5,1,teamID) == 0)  //the robot belongs to the team
    {
        char * ptrCharInSting = (char*) &segmentedTopic[0];
        int robotIndex = *(ptrCharInSting + 7) - '0';   //gets the number of robot of the team
        float payloadToFloat [12];
        memcpy(&payloadToFloat, &payload, 12);
        team[robotIndex - 1]->setPosition({payloadToFloat[0],payloadToFloat[1],payloadToFloat[2]});
        team[robotIndex - 1]->setSpeed({payloadToFloat[3],payloadToFloat[4],payloadToFloat[5]});
        team[robotIndex - 1]->setRotation({payloadToFloat[6],payloadToFloat[7],payloadToFloat[8]});
        team[robotIndex - 1]->setAngularSpeed({payloadToFloat[9],payloadToFloat[10],payloadToFloat[11]});
    }
    else if(segmentedTopic[0].compare(5,1,oppTeamID) == 0)  //the robot belongs to opposite team
    {
        char * ptrCharInSting = (char*) &segmentedTopic[0];
        int robotIndex = *(ptrCharInSting + 7) - '0';   //gets the number of robot of the oppTeam
        float payloadToFloat [12];
        memcpy(&payloadToFloat, &payload, 12);
        oppTeam[robotIndex - 1]->setPosition({payloadToFloat[0],payloadToFloat[1],payloadToFloat[2]});
        oppTeam[robotIndex - 1]->setSpeed({payloadToFloat[3],payloadToFloat[4],payloadToFloat[5]});
        oppTeam[robotIndex - 1]->setRotation({payloadToFloat[6],payloadToFloat[7],payloadToFloat[8]});
        oppTeam[robotIndex - 1]->setAngularSpeed({payloadToFloat[9],payloadToFloat[10],payloadToFloat[11]});
    }
    else   //error in the messagge recived or topic processing
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
        team[playerNumber]->start(to_string(playerNumber +1));
        cout << "player added" << endl;
    }
    cout << "GameModel START ended" << endl;
    // startHeatMap();
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
        setPoint_t destination = teamRobot->goToBall(arcoOpposite,{ball[0],ball[2]}, 1.05f);
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
    int coordX;
    int coordZ;
    
    for(auto i : team)
    {
        coordX = (int) 100 * ( 4.5F + i->getPosition().x);
        coordZ = (int) 100 * ( 3.0F + i->getPosition().z);

        heatMap[AT(coordX,coordZ)].type = 1;
    }

    coordX = (int)100 *(4.5F + ball[0]);
    coordZ = (int)100 *(3.0F + ball[2]);

    for(int contx = 0 ; contx < 5 ; contx++)
        for(int conty = 0 ; conty < 5 ; conty++)
            heatMap[AT(coordX,coordZ)].type = 2;
}

void GameModel::setDisplay(string path, string robotID)
{
    Image displayImage = LoadImage(path.c_str());
    ImageFormat(&displayImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8); // a Robot10.png no le cambia el formato

	const int dataSize = 16 * 16 * 3;
	vector<char> payload(dataSize);
	memcpy(payload.data(), displayImage.data, dataSize);

	MQTTMessage setDisplayMessage = {robotID + "/display/lcd/set", payload};
	messagesToSend.push_back(setDisplayMessage);

    UnloadImage(displayImage);
}

void GameModel::voltageKickerChipper(string robotID)
{
    float voltage = 100.0;
    vector<char> payload = getData(voltage);
    MQTTMessage setKicker = {robotID + "/kicker/chargeVoltage/set", payload};
    messagesToSend.push_back(setKicker);
}

void GameModel::setDribbler(string robotID)
{
    float voltage = 100.0;
    vector<char> payload = getData(voltage);
    MQTTMessage setKicker = {robotID + "/dribbler/voltage/set", payload};
    messagesToSend.push_back(setKicker);
}

void GameModel::setKicker (string robotID)
{
    float potencia = 0.8;
    vector<char> payload = getData(potencia);
    MQTTMessage setKicker = {robotID + "/kicker/kick/cmd", payload};
    messagesToSend.push_back(setKicker);
}

void GameModel::setChipper(string robotID)
{
    float potencia = 0.8;
    vector<char> payload = getData(potencia);
    MQTTMessage setKicker = {robotID + "/kicker/chip/cmd", payload};
    messagesToSend.push_back(setKicker);
}

void GameModel::shootToGoal(Players* player)
{
    setPoint_t placeInCourt = player->kickBallLogic(arcoOpposite, {ball[0],ball[2]});
    setPoint_t kickValue = {100,100,100};
    if((placeInCourt.coord.x == kickValue.coord.x) &&
        (placeInCourt.coord.y == kickValue.coord.y) &&
        (placeInCourt.rotation == kickValue.rotation))   //comparacion de igualdad de setpoints
    {
        setKicker(player->robotID);
        voltageKickerChipper(player->robotID);   //este orden por el pop_back del vector
    }
    else     //mover hasta el setpoint indicado
    {
        vector<char> payload = getArrayFromSetPoint(placeInCourt);
        MQTTMessage setPosition = 
            {"robot" + teamID + "." + player->robotID + "/pid/setpoint/set", payload};
        messagesToSend.push_back(setPosition);
    }
}

bool GameModel::isBallStill(void)
{
    if((ball[3]==0) && (ball[4]==0) && (ball[5]==0)) 
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
    MQTTMessage setpointMessage = {"robot" + teamID + "." + robotID + "/pid/setpoint/set",
                                   getArrayFromSetPoint(setpoint)};
    messagesToSend.push_back(setpointMessage);
}