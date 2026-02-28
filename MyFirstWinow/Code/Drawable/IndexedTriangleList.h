#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class IndexedTriangleList
{
public:
    IndexedTriangleList() = default;
    IndexedTriangleList( std::vector<T> verts_in, std::vector<unsigned short> indices_in )
        : vertices( std::move(verts_in) ), indices(std::move( indices_in )) //std::move 把一个对象转换为 右值引用（T&&），从而允许编译器调用移动构造或移动赋值。
    {
        assert( vertices.size() > 2 );
        assert( indices.size() % 3 == 0 );
    }
    void Transform( DirectX::FXMMATRIX matrix )
    {
        for ( auto &v : vertices )
        {
            const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3( &v.pos ); //DirectX::XMLoadFloat3 把内存中的 XMFLOAT3 加载到 SIMD 寄存器中
            //在寄存器中可以并行计算，速度比在内存中快速许多，
            DirectX::XMStoreFloat3(
                &v.pos,
                DirectX::XMVector3Transform( pos, matrix )  //相当于 pos（3D 点） × matrix（4×4矩阵） = 新的3D坐标
            ); //将新坐标的数据保存再&v.pos中
        }
    }

    // asserts face-independent vertices w/ normals cleared to zero
    void SetNomalsIndependentFlat() noexcept(!_DEBUG)
    {
        using namespace DirectX;
        assert( indices.size() % 3 == 0 && indices.size() > 0 );
        for ( size_t i = 0; i < indices.size(); i += 3 )
        {
            auto& v0 = vertices[ indices[i] ];
            auto& v1 = vertices[ indices[i + 1] ];
            auto& v2 = vertices[ indices[i + 2] ];
            const auto p0 = XMLoadFloat3( &v0.pos );
            const auto p1 = XMLoadFloat3( &v1.pos );
            const auto p2 = XMLoadFloat3( &v2.pos );

            const auto n = XMVector3Normalize( XMVector3Cross( ( p1 - p0 ), ( p2 - p0 ) ) );

            XMStoreFloat3( &v0.n, n );
            XMStoreFloat3( &v1.n, n );
            XMStoreFloat3( &v2.n, n );
        }
    }
public:
    std::vector<T> vertices;
    std::vector<unsigned short> indices;
};