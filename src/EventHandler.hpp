#pragma once

#include <iostream>

enum class EventType {
    KeyboardEvent,
    MouseMoveEvent,
    MousePositionEvent
};

struct Event
{
public:
    Event(EventType type) { m_Type = type; }

    EventType getType() const { return m_Type; }
private:
    EventType m_Type;
};

struct KeyboardEvent : public Event
{
public:
    int key;
    int action;
    int scancode;
    int mods;

    KeyboardEvent() : Event(EventType::KeyboardEvent) {}
};

struct MouseMoveEvent : public Event
{
public:
    double xOffset;
    double yOffset;
    MouseMoveEvent() : Event(EventType::MouseMoveEvent) {}
};

struct MousePositionEvent : public Event
{
public:
    double xPosition;
    double yPosition;
    MousePositionEvent() : Event(EventType::MousePositionEvent) {}
};





class EventObserver
{
public:
    virtual ~EventObserver() {};

    virtual void receiveEvent(const Event* event)
    {
        switch (event->getType())
        {
            case EventType::KeyboardEvent:
                keyboardEvent(*reinterpret_cast<const KeyboardEvent*>(event));
                break;
            case EventType::MouseMoveEvent:
                mouseMoveEvent(*reinterpret_cast<const MouseMoveEvent*>(event));
                break;
            case EventType::MousePositionEvent:
                mousePositionEvent(*reinterpret_cast<const MousePositionEvent*>(event));
                break;
        }
    }
    virtual void keyboardEvent(const KeyboardEvent& event) {}
    virtual void mouseMoveEvent(const MouseMoveEvent& event) {}
    virtual void mousePositionEvent(const MousePositionEvent& event) {}
};

class EventSignaller
{
public:
    virtual ~EventSignaller() {};

    virtual void attach(EventObserver* observer) = 0;
    virtual void detach(EventObserver* observer) = 0;
};
