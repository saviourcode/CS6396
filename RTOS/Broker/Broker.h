#include "../interface.h"
#include <vector>

class Broker : public IBroker
{
public:
    Broker() {
        
    }
    ~Broker() {
    
    }
    virtual void addSubscriber(ISubscriber * ptr, std::string topic)
    {
        if (topic == "AA")
        {
            m_subscriberForAA.push_back(ptr);
        }
        else if (topic == "BB")
        {
            m_subscriberForBB.push_back(ptr);
        }
        
    }

    virtual void registerToPublisher(IPublisher * ptr)
    {
    
    
        ptr->registerBroker(this);
    }

    virtual void onPublish(std::string topic, int newData)
    {
    
    
        if (topic == "AA")
        {
    
    
            for (auto one : m_subscriberForAA)
            {
    
    
                one->updateCallback(newData);
            }
        }

        else if (topic == "BB")
        {
    
    
            for (auto one : m_subscriberForBB)
            {
                one->updateCallback(newData);
            }
        }
    }

private:
    IPublisher * m_pPublisher;
    std::vector<ISubscriber *> m_subscriberForAA;
    std::vector<ISubscriber *> m_subscriberForBB;
};
