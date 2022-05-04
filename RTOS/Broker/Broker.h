#include "../interface.h"
#include "./HTTPServer.h"
#include "./WebSocketServer.h"

class Broker : public IBroker
{
public:
    Broker() {
        
    }
    ~Broker() {
        
    }
    void run() {
        std::thread t1(http_server, std::ref(topics));
        std::thread t2(print_map, std::ref(topics));
        t1.join();
        t2.join();
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
    ThreadSafeMap<std::string, std::string> topics;
    ThreadSafeMap<std::string, std::string> subscribers;
};
