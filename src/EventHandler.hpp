#pragma once

#include <iostream>

enum class EventType {
    UpdateEvent,
    KeyboardEvent,
    MouseMoveEvent,
    MousePositionEvent,
    MouseEnterEvent
};

struct Event
{
public:
    Event(EventType type) { m_Type = type; }

    EventType getType() const { return m_Type; }
private:
    EventType m_Type;
};

struct UpdateEvent : public Event
{
public:
    float dt;
    UpdateEvent() : Event(EventType::UpdateEvent) {}
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

struct MouseEnterEvent : public Event
{
public:
    bool entered;
    MouseEnterEvent() : Event(EventType::MouseEnterEvent) {}
};




class EventObserver
{
public:
    virtual ~EventObserver() {};

    virtual void receiveEvent(const Event* event)
    {
        switch (event->getType())
        {
            case EventType::UpdateEvent:
                update(*reinterpret_cast<const UpdateEvent*>(event));
                break;
            case EventType::KeyboardEvent:
                keyboardEvent(*reinterpret_cast<const KeyboardEvent*>(event));
                break;
            case EventType::MouseMoveEvent:
                mouseMoveEvent(*reinterpret_cast<const MouseMoveEvent*>(event));
                break;
            case EventType::MousePositionEvent:
                mousePositionEvent(*reinterpret_cast<const MousePositionEvent*>(event));
                break;
            case EventType::MouseEnterEvent:
                mouseEnterEvent(*reinterpret_cast<const MouseEnterEvent*>(event));
                break;
        }
    }

    virtual void update(const UpdateEvent& event) {}
    virtual void keyboardEvent(const KeyboardEvent& event) {}
    virtual void mouseMoveEvent(const MouseMoveEvent& event) {}
    virtual void mousePositionEvent(const MousePositionEvent& event) {}
    virtual void mouseEnterEvent(const MouseEnterEvent& event) {}
};

class EventSignaller
{
public:
    virtual ~EventSignaller() {};

    virtual void attach(EventObserver* observer) = 0;
    virtual void detach(EventObserver* observer) = 0;
};
