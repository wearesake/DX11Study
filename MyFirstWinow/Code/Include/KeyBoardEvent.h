#pragma once
enum class KeyStatus {
    PRESSED,
    RELEASED,
    CHAR
};

struct Event {
    KeyStatus m_status;
    char m_key;
};

class IEventListener {
public:
    virtual void OnKeyEvent(const Event& event) = 0;
    virtual ~IEventListener() = default;
};

class IEventBroadcaster {
public:
    virtual ~IEventBroadcaster() = default;
    virtual void RegisterListener(IEventListener* listener) = 0;
    virtual void RemoveListener(IEventListener* listener) = 0;
    virtual void BroadcastEvent(const Event& event) = 0;
};

class KeyBoardEvent {
};

