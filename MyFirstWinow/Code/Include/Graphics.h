#pragma once
#include <d3d11.h>

#include "DxgiInfoManager.h"
#include "MyException.h"
#include "wrl.h"

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) )) throw Graphics::HrException(__LINE__,__FILE__,hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__,hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,hr, infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__,hr, infoManager.GetMessages())
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__,__FILE__,hr)
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__,hr)
#endif

class Graphics {
public:
    Graphics( HWND hwnd );
    Graphics( const Graphics& )  = delete;
    Graphics& operator = ( const Graphics& ) = delete;
    ~Graphics() = default;
    void EndFrame();
    void ClearBuffer(float red, float green, float blue) noexcept
    {
        const float color[] =  { red, green, blue, 1.0f };
        pDeviceContext->ClearRenderTargetView(pTarget.Get(), color );
    }
    void DrawTestTriangle();
    
    
    

public:
    class Exception : public MyException {
        //继承构造函数（C++11 引入的“继承构造函数” 特性），不需要Exception自己再写一个自己的构造函数，可以直接继承父类
        using MyException::MyException;
    };

    class HrException : public Exception {
    public:
        HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {} ) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;
    private:
        HRESULT hr;
        std::string info;
    };
    class DeviceRemovedException : public HrException {
        //设备移除之后的逻辑需要特殊处理，因此做一个特殊的HrException异常类
        using HrException::HrException;
    public:
        const char* GetType() const noexcept override;
    };

#ifndef NDEBUG
    DxgiInfoManager infoManager;
#endif
    
private:
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

