#pragma once
#include "MyWindow.h"
#include "MyTimer.h"
#include "../../Manager/ImguiManager.h"
#include "Camera.h"

class App
{
public:
    App();
    int Go();
    ~App();

private:
    void DoFrame();

private:
    ImguiManager imgui;
    MyWindow wnd;
    MyTimer timer;
    std::vector<std::unique_ptr<class Box>> boxes;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    float speed_factor = 1.0f;
    Camera cam;
    static constexpr size_t nDrawables = 180;
};