#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "MyMath.h"
//?? 这个类的作用
class Cone
{
public:
    template<class V>
    static IndexedTriangleList<V> MakeTesselated( int longDiv )
    {
        assert(longDiv >= 3);

        const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
        const float longitudeAngle = 2.0f * PI / longDiv;

        // base vertices
        std::vector<V> vertices;
        for (int i = 0; i < longDiv; i++)
        {
            vertices.emplace_back(); // ?? 这里是为什么空插入
            auto v = DirectX::XMVector3Transform(
                base,
                DirectX::XMMatrixRotationZ(i * longitudeAngle)
            );
            DirectX::XMStoreFloat3(&vertices.back().pos, v);
        }

        //the center
        vertices.emplace_back();
        vertices.back().pos = {0.0f, 0.0f, -1.0f};
        const auto iCenter = (unsigned short)(vertices.size() - 1);
        //the tip: darkness:
        vertices.emplace_back();
        vertices.back().pos = {0.0f, 0.0f, 1.0f};
        const auto iTip = (unsigned short)(vertices.size() - 1);

        //base indices
        std::vector<unsigned short> indices;
        for ( unsigned short iLong = 0; iLong < longDiv; iLong++ )
        {
            indices.push_back( iCenter );
            indices.push_back( ( iLong + 1 ) % longDiv );
            indices.push_back( iLong );
        }

        //cone indices
        for ( unsigned short iLong = 0; iLong < longDiv; iLong++ )
        {
            indices.push_back( iLong );
            indices.push_back( ( iLong + 1 ) % longDiv );
            indices.push_back( iLong );
        }

        return { std::move( vertices ), std::move( indices ) };
    }

    template<class V>
    static IndexedTriangleList<V> Make()
    {
        return MakeTesselated<V>( 24 );
    }
    
};