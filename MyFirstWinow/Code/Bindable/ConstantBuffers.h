#pragma once
#include "Bindable.h"
#include "../../Macros/GraphicsThrowMacros.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update( Graphics& gfx,const C& consts )
	{
		INFOMAN( gfx );

		//Map()：映射 GPU 缓冲区到 CPU 地址空间；
		//memcpy()：把新的常量数据复制进去；
		//Unmap()：解除映射，让 GPU 能再次访问；
		//D3D11_MAP_WRITE_DISCARD：表示每次更新都丢弃旧内容，效率最高。
		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO( GetContext( gfx )->Map(
			pConstantBuffer.Get(),0u,
			D3D11_MAP_WRITE_DISCARD,0u,
			&msr
		) );
		memcpy( msr.pData,&consts,sizeof( consts ) );
		GetContext( gfx )->Unmap( pConstantBuffer.Get(),0u );
	}
	ConstantBuffer( Graphics& gfx,const C& consts )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof( consts );
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd,&csd,&pConstantBuffer ) );
	}
	ConstantBuffer( Graphics& gfx )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof( C );
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd,nullptr,&pConstantBuffer ) );
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override
	{
		GetContext( gfx )->VSSetConstantBuffers( 0u,1u,pConstantBuffer.GetAddressOf() );
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override
	{
		GetContext( gfx )->PSSetConstantBuffers( 0u,1u,pConstantBuffer.GetAddressOf() );
	}
};