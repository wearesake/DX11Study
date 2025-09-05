#include "../Include/KeyboardListener.h"
#include <format>
#include <windows.h>

void KeyboardListener::OnKeyEvent(const Event& event)
{ 
    if (event.m_status == KeyStatus::PRESSED)
    {
        auto text = std::format("Key {} pressed\n", event.m_key);
        MessageBox(nullptr, text.c_str(), "Keyboard event", MB_OK);
    }
}