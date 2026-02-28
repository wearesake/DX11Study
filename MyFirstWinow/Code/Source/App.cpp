#include "../Include/App.h"
#include "../Drawable/Drawable.h"
#include "../Drawable/Box.h"
#include "../Drawable/Pyramid.h"
#include "../Drawable/Melon.h"
#include "../Drawable/MyMath.h"
#include "../Drawable/Sheet.h"
#include "../Drawable/SkinnedBox.h"
#include "../../Manager/GDIPlusManager.h"
#include "../Imgui/imgui.h"
#include <memory>

GDIPlusManager gdipm;

App::App() : wnd(800, 600, "The Donkey Fart Box"), light( wnd.GetGraphics() )
{
    // std::mt19937 rng( std::random_device{}() );
    // std::uniform_real_distribution<float> adist( 0.0f,3.1415f * 2.0f ); //水平方向角度, 0~360° 
    // std::uniform_real_distribution<float> ddist( 0.0f,3.1415f * 2.0f ); //垂直方向角度
    // std::uniform_real_distribution<float> odist( 0.0f,3.1415f * 0.3f ); //方向偏移
    // std::uniform_real_distribution<float> rdist( 6.0f,20.0f ); // 表示半径或距离分布
    // std::uniform_real_distribution<float> bdist( 0.4f,3.0f ); //缩放
    // for( auto i = 0; i < 80; i++ )
    // {
    //     boxes.push_back( std::make_unique<Box>(
    //         wnd.GetGraphics(),rng,adist,
    //         ddist,odist,rdist,bdist
    //     ) );
    // }
    class Factory
    {
    public:
        Factory( Graphics& gfx )
            :
            gfx( gfx )
        {}
        std::unique_ptr<Drawable> operator()()
        {
            switch( typedist( rng ) )
            {
            case 0:
                return std::make_unique<Pyramid>(
                    gfx,rng,adist,ddist,
                    odist,rdist
                );
            case 1:
                return std::make_unique<Box>(
                    gfx,rng,adist,ddist,
                    odist,rdist,bdist
                );
            case 2:
                return std::make_unique<Melon>(
                    gfx,rng,adist,ddist,
                    odist,rdist,longdist,latdist
                );
            case 3:
                return std::make_unique<Sheet>(
                    gfx,rng,adist,ddist,
                    odist,rdist
                );
            case 4:
                return std::make_unique<SkinnedBox>(
                    gfx,rng,adist,ddist,
                    odist,rdist
                );
            default:
                assert( false && "bad drawable type in factory" );
                return {};
            }
        }
    private:
        Graphics& gfx;
        std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
        std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
        std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
        std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
        std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
        std::uniform_int_distribution<int> latdist{ 5,20 };
        std::uniform_int_distribution<int> longdist{ 10,40 };
        std::uniform_int_distribution<int> typedist{ 1,1 };
    };

    Factory f( wnd.GetGraphics() );
    drawables.reserve( nDrawables );
    std::generate_n( std::back_inserter( drawables ),nDrawables,f );

    wnd.GetGraphics().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,9.0f / 16.0f,0.5f,40.0f ) ); //投影矩阵, 宽高比-宽可见范围，高可见范围，两个形成宽高比
    //wnd.GetGraphics().SetCamera( DirectX::XMMatrixTranslation( 0.0f, 0.0f, 20.0f ) ); 把整个世界往 +Z 方向移动 20
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
    auto dt = timer.Mark() * speed_factor;
    
    wnd.GetGraphics().BeginFrame( 0.07f, 0.0f, 0.12f);
    wnd.GetGraphics().SetCamera( cam.GetMatrix() );
    light.Bind( wnd.GetGraphics() );
    
    for (auto &d : drawables)
    {
        d->Update( wnd.kb.IsPressed( VK_SPACE ) ? 0.0f : dt );
        d->Draw(wnd.GetGraphics() );
    }
    light.Draw( wnd.GetGraphics() );
    

    if ( ImGui::Begin("Simulation Speed") )
    {
        ImGui::SliderFloat( "Speed Factor", &speed_factor, 0.0f, 4.0f );
        ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
        ImGui::Text( "Status: %s", wnd.kb.IsPressed( VK_SPACE ) ? "PAUSED" : "RUNNING (hold spacebar to pause)" );
    }
    ImGui::End();

    cam.SpawnControlWindow();
    light.SpawnControlWindow();
    
    wnd.GetGraphics().EndFrame();
}

App::~App()
{
    
}