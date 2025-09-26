#include "IndexBuffer.h"
#include "../../Macros/GraphicsThrowMacros.h"

IndexBuffer::IndexBuffer( Graphics & gfx, const std::vector<unsigned short> & indices )
 : m_indexCount( (UINT)indices.size() )
{
    INFOMAN(gfx);
    
     D3D11_BUFFER_DESC bufferDesc = {};
     bufferDesc.ByteWidth = sizeof( m_indexCount * sizeof(unsigned short));    //缓冲区的大小（以字节为单位）。
     bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
     bufferDesc.Usage = D3D11_USAGE_DEFAULT;
     bufferDesc.CPUAccessFlags = 0u;
     bufferDesc.MiscFlags = 0u;
     bufferDesc.StructureByteStride = sizeof( unsigned short );
        
     D3D11_SUBRESOURCE_DATA subresourceData = {};
     subresourceData.pSysMem = indices.data();     //pSysMem指向初始化数据的指针。
     GFX_THROW_INFO
     (
         GetDevice( gfx )->CreateBuffer( &bufferDesc, &subresourceData, &pIndexBuffer )   //pVertexBuffer 会保存指向新建缓冲区的指针
     );
}

void IndexBuffer::Bind( Graphics & gfx )
{
    GetContext(gfx)->IASetIndexBuffer( pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0 );
}

//后面 const → 修饰成员函数，使其成为“常量成员函数”
//--它的含义是：
//--在函数体内，this 指针会变成 IndexBuffer const* this。
//--你不能修改类的成员变量（除非它们被 mutable 修饰）。
//--你也不能调用其他的非 const 成员函数。
//前面 const → 修饰返回值类型（const UINT）。
UINT IndexBuffer::GetCount() const
{
    return m_indexCount;
}