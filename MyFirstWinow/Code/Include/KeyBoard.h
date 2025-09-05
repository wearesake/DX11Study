#pragma once
#include <vector>
#include <Windows.h>
#include "KeyBoardEvent.h"
#include "KeyboardListener.h"
#include <string>
#include <memory>

class KeyBoard: public IEventBroadcaster {
public: 
    void RegisterListener(IEventListener* listener) override;
    void RemoveListener(IEventListener* listener) override;
    void BroadcastEvent(const Event& event) override;

public:
    KeyBoard();
    ~KeyBoard() override = default;
    KeyBoard(const KeyBoard& other) = delete;
    KeyBoard(KeyBoard&& other) noexcept = delete;
    KeyBoard& operator=(const KeyBoard& other) = delete;
    KeyBoard& operator=(KeyBoard&& other) noexcept = delete;
public:
    void Deal(KeyStatus status, WPARAM wparam, LPARAM lparam);
    bool IsPressed(UINT keycode);
private:
    void OnPressed(UINT keycode);
    void OnReleased(UINT keycode);
    void OnChar(UINT keycode);

private:
    std::string m_text;
    KeyboardListener m_listener;
    std::vector<std::shared_ptr<IEventListener>> m_listeners;
};

