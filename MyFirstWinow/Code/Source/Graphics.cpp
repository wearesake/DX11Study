#include "../Include/Graphics.h"
#include "../../Dxerr/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib");


Graphics::Graphics(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hwnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
#ifdef NOEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    HRESULT hr;
    
    GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
        nullptr, 
        D3D_DRIVER_TYPE_HARDWARE, 
        nullptr, 
        swapCreateFlags, 
        nullptr, 
        0,
        D3D11_SDK_VERSION,
        &sd, 
        &pSwapChain, 
        &pDevice, 
        nullptr, 
        &pDeviceContext
    ) );
    //智能指针，引用为零时自动释放
    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
    GFX_THROW_INFO( pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer ) );
    GFX_THROW_INFO( pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget) );
}

void Graphics::EndFrame()
{
    HRESULT hr;
#ifndef NDEBUG
    infoManager.Set();
#endif
    if (FAILED(hr = pSwapChain->Present(1u, 0u)))
    {
        if ( hr == DXGI_ERROR_DEVICE_REMOVED )
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
            throw GFX_EXCEPT( hr );
        }
    }
    pSwapChain->Present(1u, 0u);
}

void Graphics::DrawTestTriangle()
{
    namespace wrl = Microsoft::WRL;
    HRESULT hr;

    struct Vertex
    {
        float x,y;
    };

    const Vertex vertices[] ={
        {0.0f, 0.5f},
        {0.5f, -0.5f},
        {-0.5f, -0.5f},
    };
    //ComPtr的&重写了，先Reset在取地址
    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = sizeof( vertices );
    bufferDesc.StructureByteStride = sizeof( Vertex );
    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = vertices;
    GFX_THROW_INFO( pDevice->CreateBuffer( &bufferDesc, &subresourceData, &pVertexBuffer ) );
    const UINT stride = sizeof( Vertex );
    const UINT offset = 0u;
    pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    
    wrl::ComPtr<ID3DBlob>  pBlob;
    
    //创建像素着色器
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    GFX_THROW_INFO( D3DReadFileToBlob( L"./ShaderProject/PixelShader.cso", pBlob.GetAddressOf() ) );
    GFX_THROW_INFO( pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader ) );
    //绑定像素着色器
    pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    //创建顶点着色器
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO( D3DReadFileToBlob(L"./ShaderProject/VertexShader.cso", pBlob.GetAddressOf() ) );
    GFX_THROW_INFO( pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader ) );
    //绑定顶点着色器
    pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    //配置 vireport
    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pDeviceContext->RSSetViewports(1u, &vp);
    //输入布局
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    GFX_THROW_INFO( pDevice->CreateInputLayout(
        layout,(UINT)std::size( layout ),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));

    //绑定布局
    pDeviceContext->IASetInputLayout( pInputLayout.Get() );
    
    //绑定render target
    pDeviceContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
    //&和GetAddressOf()的区别：前者在使用时会释放掉原先数据，填充调用函数中的新的数据，后者只是取得该地址

    //设置三角形集
    pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    pDeviceContext->Draw((UINT)std::size( vertices ), 0u);
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs ) noexcept
    : Exception(line, file) , hr(hr) 
{
    for (int i = 0; i < infoMsgs.size(); i++)
    {
        info += infoMsgs[i];
        if (i != infoMsgs.size() - 1)
        {
            info.push_back('\n');
        }
    }
}

const char* Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;
    if (!info.empty() )
    {
        oss<< "\n[Error Info]\n" << GetErrorInfo() << std::endl <<  std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{ 
    return "My Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "My Graphics Exception [Device Removed] (DXGI_ERROR_REMOVED)";
}