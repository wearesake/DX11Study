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

        std::vector<V> vertices(8);
        //push_back 需要先构造对象再复制/移动到容器里
        //emplace_back 直接在容器内原地构造对象（避免临时对象）
        // vertices.emplace_back( -side, -side, -side );
        // vertices.emplace_back( side, -side, -side );
        // vertices.emplace_back( -side, side, -side );
        // vertices.emplace_back( side, side, -side );
        // vertices.emplace_back( -side, -side, side );
        // vertices.emplace_back( side, -side, side );
        // vertices.emplace_back( -side, side, side );
        // vertices.emplace_back( side, side, side );
        vertices[0].pos = { -side,-side,-side };
        vertices[1].pos = { side,-side,-side };
        vertices[2].pos = { -side,side,-side };
        vertices[3].pos = { side,side,-side };
        vertices[4].pos = { -side,-side,side };
        vertices[5].pos = { side,-side,side };
        vertices[6].pos = { -side,side,side };
        vertices[7].pos = { side,side,side };
        
        return{
            std::move( vertices ), {
                0,2,1, 2,3,1,
                1,3,5, 3,7,5,
                2,6,3, 3,6,7,
                4,5,7, 4,7,6,
                0,4,2, 2,4,6,
                0,1,4, 1,5,4
            }
        };
    }

    template<class V>
    static IndexedTriangleList<V> MakeSkinned()
    {
        constexpr float side = 1.0f / 2.0f;

        // 特殊的UV贴图
        // std::vector<V> vertices( 14 );
        //
        // vertices[0].pos = { -side,-side,-side };
        // vertices[0].tex = { 2.0f / 3.0f,0.0f / 4.0f };
        // vertices[1].pos = { side,-side,-side };
        // vertices[1].tex = { 1.0f / 3.0f,0.0f / 4.0f };
        // vertices[2].pos = { -side,side,-side };
        // vertices[2].tex = { 2.0f / 3.0f,1.0f / 4.0f };
        // vertices[3].pos = { side,side,-side };
        // vertices[3].tex = { 1.0f / 3.0f,1.0f / 4.0f };
        // vertices[4].pos = { -side,-side,side };
        // vertices[4].tex = { 2.0f / 3.0f,3.0f / 4.0f };
        // vertices[5].pos = { side,-side,side };
        // vertices[5].tex = { 1.0f / 3.0f,3.0f / 4.0f };
        // vertices[6].pos = { -side,side,side };
        // vertices[6].tex = { 2.0f / 3.0f,2.0f / 4.0f };
        // vertices[7].pos = { side,side,side };
        // vertices[7].tex = { 1.0f / 3.0f,2.0f / 4.0f };
        // vertices[8].pos = { -side,-side,-side };
        // vertices[8].tex = { 2.0f / 3.0f,4.0f / 4.0f };
        // vertices[9].pos = { side,-side,-side };
        // vertices[9].tex = { 1.0f / 3.0f,4.0f / 4.0f };
        // vertices[10].pos = { -side,-side,-side };
        // vertices[10].tex = { 3.0f / 3.0f,1.0f / 4.0f };
        // vertices[11].pos = { -side,-side,side };
        // vertices[11].tex = { 3.0f / 3.0f,2.0f / 4.0f };
        // vertices[12].pos = { side,-side,-side };
        // vertices[12].tex = { 0.0f / 3.0f,1.0f / 4.0f };
        // vertices[13].pos = { side,-side,side };
        // vertices[13].tex = { 0.0f / 3.0f,2.0f / 4.0f };
        //
        // return{
        //     std::move( vertices ),{
        //         0,2,1,   2,3,1,
        //         4,8,5,   5,8,9,
        //         2,6,3,   3,6,7,
        //         4,5,7,   4,7,6,
        //         2,10,11, 2,11,6,
        //         12,3,7,  12,7,13
        //     }
        // };

        std::vector<V> vertices(24);

        // ======================
        // Front 面 (Z+)
        // ======================
        vertices[0].pos = { -side,-side, side };
        vertices[0].tex = { 0.0f,1.0f };

        vertices[1].pos = {  side,-side, side };
        vertices[1].tex = { 1.0f,1.0f };

        vertices[2].pos = { -side, side, side };
        vertices[2].tex = { 0.0f,0.0f };

        vertices[3].pos = {  side, side, side };
        vertices[3].tex = { 1.0f,0.0f };

        // ======================
        // Back 面 (Z-)
        // ======================
        vertices[4].pos = {  side,-side,-side };
        vertices[4].tex = { 0.0f,1.0f };

        vertices[5].pos = { -side,-side,-side };
        vertices[5].tex = { 1.0f,1.0f };

        vertices[6].pos = {  side, side,-side };
        vertices[6].tex = { 0.0f,0.0f };

        vertices[7].pos = { -side, side,-side };
        vertices[7].tex = { 1.0f,0.0f };

        // ======================
        // Left 面 (X-)
        // ======================
        vertices[8].pos  = { -side,-side,-side };
        vertices[8].tex  = { 0.0f,1.0f };

        vertices[9].pos  = { -side,-side, side };
        vertices[9].tex  = { 1.0f,1.0f };

        vertices[10].pos = { -side, side,-side };
        vertices[10].tex = { 0.0f,0.0f };

        vertices[11].pos = { -side, side, side };
        vertices[11].tex = { 1.0f,0.0f };

        // ======================
        // Right 面 (X+)
        // ======================
        vertices[12].pos = { side,-side, side };
        vertices[12].tex = { 0.0f,1.0f };

        vertices[13].pos = { side,-side,-side };
        vertices[13].tex = { 1.0f,1.0f };

        vertices[14].pos = { side, side, side };
        vertices[14].tex = { 0.0f,0.0f };

        vertices[15].pos = { side, side,-side };
        vertices[15].tex = { 1.0f,0.0f };

        // ======================
        // Top 面 (Y+)
        // ======================
        vertices[16].pos = { -side, side, side };
        vertices[16].tex = { 0.0f,1.0f };

        vertices[17].pos = {  side, side, side };
        vertices[17].tex = { 1.0f,1.0f };

        vertices[18].pos = { -side, side,-side };
        vertices[18].tex = { 0.0f,0.0f };

        vertices[19].pos = {  side, side,-side };
        vertices[19].tex = { 1.0f,0.0f };

        // ======================
        // Bottom 面 (Y-)
        // ======================
        vertices[20].pos = { -side,-side,-side };
        vertices[20].tex = { 0.0f,1.0f };

        vertices[21].pos = {  side,-side,-side };
        vertices[21].tex = { 1.0f,1.0f };

        vertices[22].pos = { -side,-side, side };
        vertices[22].tex = { 0.0f,0.0f };

        vertices[23].pos = {  side,-side, side };
        vertices[23].tex = { 1.0f,0.0f };

        return {
            std::move(vertices),
            {
                0,2,1,  2,3,1,      // front
                4,6,5,  6,7,5,      // back
                8,10,9, 10,11,9,    // left
                12,14,13, 14,15,13, // right
                16,18,17, 18,19,17, // top
                20,22,21, 22,23,21  // bottom
            }
        };
    }
};