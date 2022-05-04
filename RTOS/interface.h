#ifndef INTERFACE
#define INTERFACE

#include <string>

class IPublisher;
class ISubscriber;

class IBroker
{
public:
    virtual void addSubscriber(ISubscriber * ptr, std::string topic) = 0;
    virtual void registerToPublisher(IPublisher * ptr) = 0;
    virtual void onPublish(std::string topic, int newData) = 0;

};

class IPublisher
{
public:
    virtual void registerBroker(IBroker * pBroker) = 0;
    virtual void publishToBroker(std::string topic, int newData) = 0;
    virtual void update(void) = 0;
};

class ISubscriber
{
public:
    virtual void subscribeToBroker(IBroker * ptr, std::string topic) = 0;
    virtual void updateCallback(int newVal) = 0;
};

#endif