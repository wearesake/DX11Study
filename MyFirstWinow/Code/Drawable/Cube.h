#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Cube
{
public:
    template<class V>
    static IndexedTriangleList<V> Make()
    {
        namespace dx = DirectX;
        
        constexpr float side = 1.0f / 2.0f;

        std::vector<dx::XMFLOAT3> vertices;
        //push_back 需要先构造对象再复制/移动到容器里
        //emplace_back 直接在容器内原地构造对象（避免临时对象）
        vertices.emplace_back( -side, -side, -side );
        vertices.emplace_back( side, -side, -side );
        vertices.emplace_back( -side, side, -side );
        vertices.emplace_back( side, side, -side );
        vertices.emplace_back( -side, -side, side );
        vertices.emplace_back( side, -side, side );
        vertices.emplace_back( -side, side, side );
        vertices.emplace_back( side, side, side );

        std::vector<V> verts( vertices.size() );
        for ( size_t i = 0; i < vertices.size(); i++ )
        {
            verts[i].pos = vertices[i];
        }
        return{
            std::move( verts ), {
                0,2,1, 2,3,1,
                1,3,5, 3,7,5,
                2,6,3, 3,6,7,
                4,5,7, 4,7,6,
                0,4,2, 2,4,6,
                0,1,4, 1,5,4
            }
        };
    }
};