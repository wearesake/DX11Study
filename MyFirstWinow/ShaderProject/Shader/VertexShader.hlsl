//struct VSOut
//{
//    float3 color : Color;
//    float4 pos : SV_Position;
//};

//VSOut main(float2 pos : Position, float3 color : Color)
//{
//    VSOut vso;
//    vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
//    vso.color = color;
//    return vso;
//}

//cbuffer（常量缓冲区）
cbuffer CBuf
{
    //matrix 是一种 矩阵类型
    matrix transform;
};

float4 main(float3 pos : Position) : SV_Position
{
    //点与矩阵相乘，这里具体原理就要学习图形学了
    return mul(float4(pos, 1.0f), transform);
}
