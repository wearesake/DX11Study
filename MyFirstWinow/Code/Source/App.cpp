#include "../Include/App.h"

App::App() :wnd("firstWindow")
{
    wnd.Begin("ssj", 800, 600);
}

int App::Go()
{
    while (true)
    {
        if (const auto ecode = MyWindow::ProcessMessages())
        {
            return *ecode;
        }
        DoFrame();
    }
}

void App::DoFrame()
{
    const float c = sin(timer.Peek()) / 2.0f + 0.5f;
    wnd.GetGraphics().ClearBuffer(c, c, 1.0f);
    wnd.GetGraphics().DrawTestTriangle();
    wnd.GetGraphics().EndFrame();
}