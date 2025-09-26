#pragma once
#include "Bindable.h"
#include "../../Macros/GraphicsThrowMacros.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
    void Update(Graphics &gfx, const C &consts)
    {
        INFOMAN(gfx)

        D3D11_MAPPED_SUBRESOURCE msr;
        GFX_THROW_INFO(
            GetDevice(gfx)->Map(
                m_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));

        memcpy( msr.pData, &consts, sizeof( consts ));
        GetContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0u );
    }
    ConstantBuffer( Graphics &gfx, const C &consts )
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC desc;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0u;
        desc.StructureByteStride = 0u;
        desc.ByteWidth = sizeof( consts );
        GFX_THROW_INFO(
            GetDevice(gfx)->CreateBuffer( &desc, nullptr, &m_pConstantBuffer));
        
    }
    ConstantBuffer( Graphics &gfx )
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC desc;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0u;
        desc.StructureByteStride = 0u;
        desc.ByteWidth = sizeof( C );
        GFX_THROW_INFO(
            GetDevice(gfx)->CreateBuffer( &desc, nullptr, &m_pConstantBuffer));
        
    }
protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::m_pConstantBuffer;
    using Bindable::GetContext;
public:
    using ConstantBuffer<C>::ConstantBuffer;
    void Bind(Graphics &gfx) noexcept override
    {
        GetContext(gfx)->VSSetConstantBuffers( 0u, 1u, m_pConstantBuffer.GetAddressOf());
    }
};

template<typename C>
class PiexlConstantBuffer : public ConstantBuffer<C>
{
    //C++ 编译器 不会自动查找依赖于模板参数的基类成员
    using ConstantBuffer<C>::m_pConstantBuffer;
    using Bindable::GetContext;
public:
    using ConstantBuffer<C>::ConstantBuffer;
    void Bind(Graphics &gfx) noexcept override
    {
        GetContext(gfx)->VSSetConstantBuffers( 0u, 1u, m_pConstantBuffer.GetAddressOf());
    }
};