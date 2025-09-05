#include "../Include/KeyBoard.h"
#include "../Include/Gdi.h"

void KeyBoard::RegisterListener(IEventListener* listener)
{
    std::shared_ptr<IEventListener> listenerPtr(listener);
    m_listeners.push_back(listenerPtr);
}

void KeyBoard::RemoveListener(IEventListener* listener)
{ 
    std::erase_if(m_listeners, [listener](const std::shared_ptr<IEventListener>& ptr) {
        return ptr.get() == listener;
    });
}

void KeyBoard::BroadcastEvent(const Event& event)
{
    for (auto& listener : m_listeners)
    {
        listener->OnKeyEvent(event);
    }
}

KeyBoard::KeyBoard()
{

}

void KeyBoard::Deal(KeyStatus status, WPARAM wparam, LPARAM lparam)
{ 
    switch (status)
    {
    case KeyStatus::PRESSED: OnPressed(wparam);
        break;
    case KeyStatus::RELEASED: OnReleased(wparam);
        break;
    case KeyStatus::CHAR: OnChar(wparam);
        break;
    }
}

bool KeyBoard::IsPressed(UINT keycode)
{
    return GetAsyncKeyState(keycode) & 0x8000;
}

void KeyBoard::OnPressed(UINT keycode)
{
    //TODO:
}

void KeyBoard::OnReleased(UINT keycode)
{

}

void KeyBoard::OnChar(UINT keycode)
{
    m_text += static_cast<char>(keycode);
    HWND hwnd = FindWindow(nullptr, "firstWindow");
    Gdi::DrawTextToWindow(hwnd, m_text);
}