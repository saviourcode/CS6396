#include "interface.h"

class Publisher : public IPublisher
{
public:
    Publisher() : m_dataForAA(0), m_dataForBB(0), 
        m_cntForAA(0), m_cntForBB(0),
        m_brokerPtr(nullptr)
    {
    
    }

    virtual ~Publisher() {
    
    }

    virtual void registerBroker(IBroker * pBroker)
    {
        m_brokerPtr = pBroker;
    }

    virtual void publishToBroker(std::string topic, int newData)
    {
        m_brokerPtr->onPublish(topic, newData);
    }

    virtual void update(void)
    {
        if ((++m_cntForAA) == 100000)
        {
            m_cntForAA = 0;
            m_dataForAA += 10;
            publishToBroker("AA", m_dataForAA);
        }

        if ((++m_cntForBB) == 200000)
        {
            m_cntForBB = 0;
            m_dataForBB += 20;

            publishToBroker("BB", m_dataForBB);
        }
    }

private:
    int m_dataForAA;
    int m_dataForBB;

    int m_cntForAA;
    int m_cntForBB;

    IBroker * m_brokerPtr;
};