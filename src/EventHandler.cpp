#include "EventHandler.hpp"
#include "Camera.hpp"

#include <algorithm>


void EventSignaller::attach(EventObserver* observer)
{
    m_Observers.push_back(observer);
}

void EventSignaller::detach(EventObserver* observer)
{
    auto pos = std::find(m_Observers.begin(), m_Observers.end(), observer);
    m_Observers.erase(pos);
}

void EventSignaller::notify(const Event* event)
{
    for (auto it = m_Observers.begin(); it != m_Observers.end(); it++)
    {
        (*it)->receiveEvent(event);
    }
}
