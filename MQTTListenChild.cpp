#include "MQTTListenChild.h"

using namespace std;

void MQTTListenChild::onMessage(string topic, vector<char> payload)
{
    cout << topic << endl;
}