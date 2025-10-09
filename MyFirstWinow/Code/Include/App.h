#pragma once
#include "MyWindow.h"
#include "MyTimer.h"

class App {
public:
    App();
    int Go();
    ~App();

private:
    void DoFrame();

private:
    MyWindow wnd;
    MyTimer timer;
    std::vector<std::unique_ptr<class Box>> boxes;
};

