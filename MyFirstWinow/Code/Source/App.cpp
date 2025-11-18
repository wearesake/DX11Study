#include "../Include/App.h"
#include "../Drawable/Box.h"
#include <memory>

App::App() : wnd(800, 600, "The Donkey Fart Box")
{
    std::mt19937 rng( std::random_device{}() );
    std::uniform_real_distribution<float> adist( 0.0f,3.1415f * 2.0f ); //水平方向角度, 0~360° 
    std::uniform_real_distribution<float> ddist( 0.0f,3.1415f * 2.0f ); //垂直方向角度
    std::uniform_real_distribution<float> odist( 0.0f,3.1415f * 0.3f ); //方向偏移
    std::uniform_real_distribution<float> rdist( 6.0f,20.0f ); // 表示半径或距离分布
    for( auto i = 0; i < 10; i++ )
    {
        boxes.push_back( std::make_unique<Box>(
            wnd.GetGraphics(),rng,adist,
            ddist,odist,rdist
        ) );
    }
    wnd.GetGraphics().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) );
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
    auto dt = timer.Mark();
    wnd.GetGraphics().ClearBuffer( 0.07f, 0.0f, 0.12f);
    for (auto &b : boxes)
    {
        b->Update(dt);
        b->Draw(wnd.GetGraphics() );
    }
    wnd.GetGraphics().EndFrame();
}

App::~App()
{
    
}