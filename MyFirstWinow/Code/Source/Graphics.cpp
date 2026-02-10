#include "../Include/Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include "../../Macros/GraphicsThrowMacros.h"

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
    
    GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
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
    GFX_THROW_INFO( pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer ) ); //得到 DXGI 交换链中的后缓冲区纹理对象
    GFX_THROW_INFO( pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget) ); //把绘制结果输出到这张后缓冲的纹理上

    //!!!2025/10/09 今天一下午出不来显示box, 寻找了4个小时的差异才发现是这里的代码少了。--需要消化看懂原因
    //深度测试（Depth Test）决定哪些像素应该显示在前面，哪些应该被挡在后面。 
    // create depth stensil state
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
    //配置深度测试行为的规则，也就是“判断谁在前、谁在后”的逻辑。
    GFX_THROW_INFO( pDevice->CreateDepthStencilState( &dsDesc,&pDSState ) );

    // bind depth state
    pDeviceContext->OMSetDepthStencilState( pDSState.Get(),1u );

    // create depth stensil texture
    // 创建深度缓冲区（Depth Buffer）
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = 800u;
    descDepth.Height = 600u;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO( pDevice->CreateTexture2D( &descDepth,nullptr,&pDepthStencil ) );

    // create view of depth stensil texture
    // 把刚才创建的“深度纹理”包装成 GPU 可以使用的“视图”，告诉管线如何访问这块内存。
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO( pDevice->CreateDepthStencilView(
        pDepthStencil.Get(),&descDSV,&pDSV
    ) );

    // bind depth stensil view to OM
    pDeviceContext->OMSetRenderTargets( 1u,pTarget.GetAddressOf(),pDSV.Get() );
	   
    // configure viewport
    // 配置视口（Viewport） 
    D3D11_VIEWPORT vp;
    vp.Width = 800.0f;
    vp.Height = 600.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    pDeviceContext->RSSetViewports( 1u,&vp );
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
}

void Graphics::DrawTestTriangle()
{
    namespace wrl = Microsoft::WRL;
    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        }pos;

        struct
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        }color;
    };
    
    const Vertex vertices[] ={
        {0.0f, 0.5f, 255,  0, 0, 0},
        {0.5f, -0.5f, 0,  255, 0, 0},
        {-0.5f, -0.5f, 0,  0, 255, 0},
    };
    
    //输入布局（Input Layout）的描述，它告诉 D3D11 顶点缓冲区里的数据该如何解释并传递给顶点着色器。
    //变量名 索引 数据格式 槽位 偏移 数据类型 实例化步长
    //变量名必须和顶点着色器输入变量的语义一致
    
    const D3D11_INPUT_ELEMENT_DESC layout[] = {
        
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    
    wrl::ComPtr<ID3DBlob>  pBlobVertex; //创建之前读入shader
    GFX_THROW_INFO( D3DReadFileToBlob(L"./ShaderProject/VertexShader.cso", pBlobVertex.GetAddressOf() ) );
    
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;    //ID3D11InputLayout布局类

    GFX_THROW_INFO(
        pDevice->CreateInputLayout( layout,(UINT)std::size( layout ), pBlobVertex->GetBufferPointer(), pBlobVertex->GetBufferSize(), &pInputLayout)
    );

    //（IA → VS → RS → PS → OM）
    /* -----------------------------1.IA---------------------------------------*/
    //设置三角形集 IA = Input Assembler 阶段 负责把顶点缓冲中的数据拼接成图元（点/线/三角形等）。
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //绑定布局
    
    pDeviceContext->IASetInputLayout( pInputLayout.Get() );
   
    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof( vertices );    //缓冲区的大小（以字节为单位）。
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.StructureByteStride = sizeof( Vertex );
    
    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = vertices;     //pSysMem指向初始化数据的指针。
    GFX_THROW_INFO
    (
        pDevice->CreateBuffer( &bufferDesc, &subresourceData, &pVertexBuffer )   //pVertexBuffer 会保存指向新建缓冲区的指针
    );
    const UINT stride = sizeof( Vertex );  //&stride → 指向 stride 数组的指针（告诉 GPU 每个顶点占多少字节）
    const UINT offset = 0u;  //&offset → 指向 offset 数组的指针（告诉 GPU 从缓冲区哪个字节开始读）
    //设置顶点缓冲区---从输入槽位0开始，绑定1个缓冲区 &stride和&offset它们都不是单个值，而是数组，每个元素对应一个缓冲区。
    pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    /* -----------------------------2.VS---------------------------------------*/
    //创建顶点着色器
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO( pDevice->CreateVertexShader(pBlobVertex->GetBufferPointer(), pBlobVertex->GetBufferSize(), nullptr, &pVertexShader ) );
    //绑定顶点着色器
    pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
    
    /* -----------------------------3.RS---------------------------------------*/
    //配置 viewport 设置渲染区域（裁剪空间范围）
    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pDeviceContext->RSSetViewports(1u, &vp);
    /* -----------------------------4.PS---------------------------------------*/
    wrl::ComPtr<ID3DBlob>  pBlobPixel;
    GFX_THROW_INFO( D3DReadFileToBlob( L"./ShaderProject/PixelShader.cso", pBlobPixel.GetAddressOf() ) );
    //创建像素着色器
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    GFX_THROW_INFO( pDevice->CreatePixelShader(pBlobPixel->GetBufferPointer(), pBlobPixel->GetBufferSize(), nullptr, &pPixelShader ) );
    //绑定像素着色器
    pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
    
    /* -----------------------------5.OM---------------------------------------*/
    //绑定render target   渲染管线的最后一环 把像素着色器的输出结果写入 Render Target
    pDeviceContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
    /* -----------------------------Draw---------------------------------------*/
    GFX_THROW_INFO_ONLY( pDeviceContext->Draw((UINT)std::size( vertices ), 0u) );
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
    const float color[] = { red, green, blue, 1.0f };
    pDeviceContext->ClearRenderTargetView( pTarget.Get(), color );
    pDeviceContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count) noexcept(!_DEBUG)
{
    GFX_THROW_INFO_ONLY( pDeviceContext->DrawIndexed(count, 0u, 0u) );
}

void Graphics::SetProjection( DirectX::FXMMATRIX proj ) noexcept
{
    m_proj = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
    return m_proj;
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
    //std::hex 把整数输出为16进制， std::uppercase，把16进制中的字母A-F大写， std::dec恢复10进制
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
    //return DXGetErrorString(hr);
    return ""; 
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    //DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}

Graphics::InfoException::InfoException( int line,const char * file,std::vector<std::string> infoMsgs ) noexcept
    :
    Exception( line,file )
{
    // join all info messages with newlines into single string
    for (int i = 0; i < infoMsgs.size(); i++)
    {
        info += infoMsgs[i];
        info.push_back( '\n' );
        if (i != infoMsgs.size() - 1)
        {
            info.push_back('\n');
        }
    }
   
}


const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
    return "My Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "My Graphics Exception [Device Removed] (DXGI_ERROR_REMOVED)";
}