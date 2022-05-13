#ifndef _MQTTLISTENCHILD_H
#define _MQTTLISTENCHILD_H

#include "MQTTClient2.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MQTTListenChild : public MQTTListener
{
public:
    void onMessage(string topic, vector<char> payload);
};

#endif   //_MQTTLISTENCHILD_H un experto programador en C, puede tener problemas en la logica. Un experto programador en c++ puede tener problemas entendiendo c++