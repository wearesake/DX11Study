#pragma once
#include "KeyBoardEvent.h"
class KeyboardListener : public IEventListener{
public:
    void OnKeyEvent(const Event& event) override;
};

