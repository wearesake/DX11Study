#pragma once
#include "MyWindow.h"
#include "MyTimer.h"

class App {
public:
    App();
    int Go();

private:
    void DoFrame();

private:
    MyWindow wnd;
    MyTimer timer;
};

