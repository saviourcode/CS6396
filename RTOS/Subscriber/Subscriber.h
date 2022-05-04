#include "interface.h"
#include <iostream>
class Subscriber : public ISubscriber
{  
public:
    Subscriber() {
    
    }
    virtual ~Subscriber() {
    
    }

    virtual void subscribeToBroker(IBroker * ptr, std::string topic)
    {
    
    
        m_topic = topic;
        ptr->addSubscriber(this, topic);
    }

    virtual void updateCallback(int newData)
    {
    
    
        std::cout << "new data for topic " << m_topic <<  " is " << m_data << "\n";
        m_data = newData;
    }

private:
    int m_data;
    std::string m_topic;
};