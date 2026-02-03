#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "MyMath.h"
//?? 这个类的作用
//Cone 圆锥
class Cone
{
public:
    template<class V>
    static IndexedTriangleList<V> MakeTesselated( int longDiv )
    {
        assert(longDiv >= 3);

        const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
        const float longitudeAngle = 2.0f * PI / longDiv; //DirectX里面使用的是弧度制，因此2Π为一圈；除以总数即为每面的角度

        // base vertices
        std::vector<V> vertices;
        for (int i = 0; i < longDiv; i++)
        {
            vertices.emplace_back(); // ?? 这里是为什么空插入 -- 为了“先创建一个 V 对象，再往里面填字段”
            //emplace_back() 接受无参构造, push_back()需传入值
            //因此emplace_back()妙用，可以先进行占位构造，再赋值
            auto v = DirectX::XMVector3Transform(
                base,
                DirectX::XMMatrixRotationZ(i * longitudeAngle) //把点绕z轴旋转i*Angle角度，得到一个新的点
                //正值 逆时针，负值，顺时针
            );
            //XMVector3Transform(向量，矩阵) 等价于 向量 × 矩阵
            DirectX::XMStoreFloat3(&vertices.back().pos, v);
        }

        //the center
        vertices.emplace_back();
        vertices.back().pos = {0.0f, 0.0f, -1.0f};//位置在 (0, 0, -1) —— 底面中心
        const auto iCenter = (unsigned short)(vertices.size() - 1); 
        //the tip: darkness:
        vertices.emplace_back();
        vertices.back().pos = {0.0f, 0.0f, 1.0f};//位置在 (0,0,1) —— 圆锥尖顶
        const auto iTip = (unsigned short)(vertices.size() - 1); 

        //base indices
        std::vector<unsigned short> indices;
        for ( unsigned short iLong = 0; iLong < longDiv; iLong++ )
        {
            indices.push_back( iCenter ); //iCenter为索引， indices保存的是vertices的索引
            indices.push_back( ( iLong + 1 ) % longDiv );
            indices.push_back( iLong );
        }

        //cone indices
        for ( unsigned short iLong = 0; iLong < longDiv; iLong++ )
        {
            indices.push_back( iLong );
            indices.push_back( ( iLong + 1 ) % longDiv );
            indices.push_back( iTip );
        }

        return { std::move( vertices ), std::move( indices ) };
    }

    template<class V>
    static IndexedTriangleList<V> Make()
    {
        return MakeTesselated<V>( 24 );
    }
    
};